# -*- mode: python ; coding: utf-8 -*-
"""
PyInstaller spec for PCL Designer.

Bundles the per-host production_ce binary AND its runtime dependencies
(libomp.dylib / libgomp.so.1 / MinGW DLLs) so the resulting executable
is fully portable across machines of the same OS + arch.

Run from the repo root after build_binary.sh has populated binaries/:

    bash scripts/build_binary.sh
    pyinstaller packaging/pcl_designer.spec
"""

import os
import platform
import sys
from pathlib import Path

spec_dir = Path(os.path.dirname(os.path.abspath(SPEC)))  # noqa: F821
repo_root = spec_dir.parent
bin_dir = repo_root / "binaries"

# --- Pick per-host binary + its runtime deps -----------------------------
plat = platform.system()
mach = platform.machine().lower()

if plat == "Darwin":
    arch = "arm64" if mach in ("arm64", "aarch64") else "x86_64"
    bin_src_name = f"production_ce_macos_{arch}"
    bundled_name = "production_ce"
    runtime_libs = ["libomp.dylib"]
elif plat == "Linux":
    bin_src_name = "production_ce_linux_x86_64"
    bundled_name = "production_ce"
    runtime_libs = ["libgomp.so.1"]
elif plat == "Windows":
    bin_src_name = "production_ce_windows_x86_64.exe"
    bundled_name = "production_ce.exe"
    runtime_libs = ["libgomp-1.dll", "libgcc_s_seh-1.dll", "libwinpthread-1.dll"]
else:
    raise SystemExit(f"Unsupported build platform: {plat}")

binary_src = bin_dir / bin_src_name
if not binary_src.exists():
    raise SystemExit(
        f"Pre-built binary not found at {binary_src}. "
        f"Run `bash scripts/build_binary.sh` first."
    )

# Each entry in `binaries` is (source_path, dest_dir_in_bundle).
# We also include any runtime lib that was bundled alongside in binaries/.
extra_binaries = []
for libname in runtime_libs:
    libpath = bin_dir / libname
    if libpath.exists():
        extra_binaries.append((str(libpath), "binaries"))
    else:
        print(f"WARNING: runtime lib {libname} not found in {bin_dir}; "
              f"binary may fail on machines without it pre-installed.")

# --- Analysis -----------------------------------------------------------
block_cipher = None

a = Analysis(  # noqa: F821
    [str(repo_root / "pcl_designer" / "__main__.py")],
    pathex=[str(repo_root)],
    binaries=[(str(binary_src), "binaries")] + extra_binaries,
    datas=[
        (str(repo_root / "pcl_designer" / "templates"), "pcl_designer/templates"),
        (str(repo_root / "pcl_designer" / "static"), "pcl_designer/static"),
    ],
    hiddenimports=["waitress"],
    hookspath=[],
    runtime_hooks=[],
    excludes=["matplotlib", "numpy", "scipy", "pandas", "tkinter", "PIL"],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

# Rename the bundled production_ce binary so runner.binary_path()
# matches it via the "production_ce[.exe]" candidate. The runtime libs
# keep their original names (they're located by the OS loader, not by
# our code).
a.binaries = [
    (
        ("binaries/" + bundled_name) if name.endswith(bin_src_name) else name,
        src,
        kind,
    )
    for name, src, kind in a.binaries
]

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)  # noqa: F821

exe = EXE(  # noqa: F821
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name="PCLDesigner",
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=False,
    console=True,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)
