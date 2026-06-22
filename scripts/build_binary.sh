#!/usr/bin/env bash
# =============================================================================
# build_binary.sh — Build production_ce and bundle its runtime dependencies
#                   into binaries/ so the result is fully portable across
#                   machines of the same OS+arch.
#
# Usage:
#   ./scripts/build_binary.sh
#
# Side effects (per-platform):
#   macOS  → binaries/production_ce_macos_<arch>
#            binaries/libomp.dylib              (copied from Homebrew)
#            install_name_tool rewrites the binary's libomp reference to
#            @executable_path/libomp.dylib so it loads the bundled copy.
#
#   Linux  → binaries/production_ce_linux_x86_64
#            binaries/libgomp.so.1              (copied from system gcc)
#            patchelf sets the rpath to $ORIGIN so the bundled libgomp is
#            preferred over the system one.
#
#   Windows (MSYS2/MINGW64) → binaries/production_ce_windows_x86_64.exe
#                             binaries/libgomp-1.dll
#                             binaries/libgcc_s_seh-1.dll
#                             binaries/libwinpthread-1.dll
#
# The result on every platform: drop the contents of binaries/ on any
# same-OS / same-arch machine and the binary runs with zero external
# dependencies beyond the OS itself.
# =============================================================================

set -euo pipefail

REPO_ROOT="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )/.." &> /dev/null && pwd )"
CSRC_DIR="$REPO_ROOT/csrc/production_ce"
BIN_DIR="$REPO_ROOT/binaries"
mkdir -p "$BIN_DIR"

UNAME_S="$(uname -s)"
UNAME_M="$(uname -m)"

echo "=== build_binary.sh ==="
echo "  Host:        $UNAME_S $UNAME_M"
echo "  Source:      $CSRC_DIR"
echo "  Output dir:  $BIN_DIR"
echo ""

# -----------------------------------------------------------------------------
# Step 1: build
# -----------------------------------------------------------------------------
echo "--- Compiling production_ce ---"
( cd "$CSRC_DIR" && make clean && make )

# The Makefile produces an executable named "DesignWizardVn_App_GapPrimary"
# (or .exe on MSYS2). Locate it.
BUILT="$CSRC_DIR/DesignWizardVn_App_GapPrimary"
[ -f "$CSRC_DIR/DesignWizardVn_App_GapPrimary.exe" ] && BUILT="$CSRC_DIR/DesignWizardVn_App_GapPrimary.exe"
if [ ! -x "$BUILT" ]; then
    echo "ERROR: expected build output not found at $BUILT" >&2
    exit 1
fi

# -----------------------------------------------------------------------------
# Step 2: place + bundle runtime libs + fix dynamic-loader paths
# -----------------------------------------------------------------------------
case "$UNAME_S" in

    Darwin)
        case "$UNAME_M" in
            arm64|aarch64) DEST="$BIN_DIR/production_ce_macos_arm64" ;;
            x86_64)        DEST="$BIN_DIR/production_ce_macos_x86_64" ;;
            *) echo "ERROR: unrecognized macOS arch $UNAME_M" >&2; exit 1 ;;
        esac
        cp "$BUILT" "$DEST"
        chmod +x "$DEST"

        echo "--- Bundling libomp.dylib alongside binary ---"
        # Resolve the actual libomp.dylib the binary links to. otool -L
        # prints the install names; we grep for libomp and follow it.
        LIBOMP_REF="$(otool -L "$DEST" | awk '/libomp/{print $1; exit}')"
        if [ -z "$LIBOMP_REF" ]; then
            echo "ERROR: binary does not reference libomp; expected dynamic link" >&2
            exit 1
        fi
        # If LIBOMP_REF is already @-prefixed, find the real file via Homebrew.
        if [[ "$LIBOMP_REF" == @* ]]; then
            LIBOMP_PATH="$(brew --prefix libomp 2>/dev/null)/lib/libomp.dylib"
            [ -f "$LIBOMP_PATH" ] || LIBOMP_PATH="/opt/homebrew/opt/libomp/lib/libomp.dylib"
            [ -f "$LIBOMP_PATH" ] || LIBOMP_PATH="/usr/local/opt/libomp/lib/libomp.dylib"
        else
            LIBOMP_PATH="$LIBOMP_REF"
        fi
        if [ ! -f "$LIBOMP_PATH" ]; then
            echo "ERROR: libomp.dylib not found at $LIBOMP_PATH" >&2
            exit 1
        fi
        cp "$LIBOMP_PATH" "$BIN_DIR/libomp.dylib"
        chmod +w "$BIN_DIR/libomp.dylib"

        echo "--- Rewriting dynamic-loader paths ---"
        # Make the binary look for libomp.dylib next to itself.
        install_name_tool -change "$LIBOMP_REF" "@executable_path/libomp.dylib" "$DEST"
        # Make libomp.dylib's own install name match its bundled location.
        install_name_tool -id "@executable_path/libomp.dylib" "$BIN_DIR/libomp.dylib"

        echo "--- Verifying portability ---"
        echo "Binary install names:"
        otool -L "$DEST" | sed 's/^/    /'
        ;;

    Linux)
        DEST="$BIN_DIR/production_ce_linux_x86_64"
        cp "$BUILT" "$DEST"
        chmod +x "$DEST"

        echo "--- Bundling libgomp.so.1 alongside binary ---"
        # libgomp ships with gcc; locate it via gcc itself.
        LIBGOMP_PATH="$(gcc --print-file-name=libgomp.so.1)"
        if [ ! -f "$LIBGOMP_PATH" ]; then
            # Some distros put the .so.1 symlink elsewhere; check ld
            LIBGOMP_PATH="$(ldconfig -p | awk '/libgomp\.so\.1/{print $NF; exit}')"
        fi
        if [ ! -f "$LIBGOMP_PATH" ]; then
            echo "ERROR: libgomp.so.1 not found via gcc or ldconfig" >&2
            exit 1
        fi
        cp -L "$LIBGOMP_PATH" "$BIN_DIR/libgomp.so.1"
        chmod +w "$BIN_DIR/libgomp.so.1"

        echo "--- Setting rpath to \$ORIGIN ---"
        if command -v patchelf >/dev/null 2>&1; then
            patchelf --set-rpath '$ORIGIN' "$DEST"
        else
            echo "WARNING: patchelf not installed; rpath fix skipped." >&2
            echo "         Install with: sudo apt-get install -y patchelf" >&2
        fi

        echo "--- Verifying portability ---"
        echo "Binary dynamic deps:"
        ldd "$DEST" 2>&1 | sed 's/^/    /'
        ;;

    MINGW*|MSYS*|CYGWIN*)
        DEST="$BIN_DIR/production_ce_windows_x86_64.exe"
        cp "$BUILT" "$DEST"

        echo "--- Bundling MinGW runtime DLLs alongside binary ---"
        # MinGW64 standard install: /mingw64/bin/
        MINGW_BIN="/mingw64/bin"
        for dll in libgomp-1.dll libgcc_s_seh-1.dll libwinpthread-1.dll; do
            if [ -f "$MINGW_BIN/$dll" ]; then
                cp "$MINGW_BIN/$dll" "$BIN_DIR/$dll"
                echo "    copied $dll"
            else
                echo "WARNING: $MINGW_BIN/$dll not found" >&2
            fi
        done

        echo "--- Verifying portability ---"
        echo "Binary dependencies:"
        if command -v objdump >/dev/null 2>&1; then
            objdump -p "$DEST" | grep "DLL Name" | sed 's/^/    /'
        else
            echo "    (objdump not installed; skipping dep listing)"
        fi
        ;;

    *)
        echo "ERROR: unrecognized OS: $UNAME_S" >&2
        exit 1
        ;;
esac

echo ""
echo "=== Done ==="
echo "Contents of $BIN_DIR:"
ls -la "$BIN_DIR" | tail -n +2
