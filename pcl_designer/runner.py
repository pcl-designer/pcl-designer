"""
Bridge to the production_ce binary.

Python port of matlab/helpers/call_gap_primary_standalone.m. Same protocol:
    1. Write priorMean.csv, priorCov.csv, wp_levels.csv, sp_levels.csv,
       modelTerms.csv, n_sizes.csv, config.txt into a tempdir.
    2. Spawn the production_ce binary with the tempdir as cwd.
    3. Parse OptimalDesign_Output.csv and extract D-criterion from stdout.

Returns a dict suitable for JSON serialization back to the browser.
"""

from __future__ import annotations

import csv
import os
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path
from typing import Any


# --------------------------------------------------------------------------
# Binary location
# --------------------------------------------------------------------------

def binary_path() -> Path:
    """
    Return the path to the bundled production_ce binary for this platform.

    When running under PyInstaller, binaries are unpacked to sys._MEIPASS.
    In source/dev mode, we look in the repo's binaries/ directory.
    """
    if getattr(sys, "frozen", False):
        # PyInstaller bundle: binaries land in <bundle>/binaries/
        base = Path(sys._MEIPASS) / "binaries"
    else:
        # Dev mode: binaries/ at the repo root, sibling to pcl_designer/
        base = Path(__file__).resolve().parent.parent / "binaries"

    suffix = ".exe" if sys.platform.startswith("win") else ""

    # PyInstaller spec strips the platform suffix; source tree keeps it.
    candidates = [
        base / f"production_ce{suffix}",
        base / f"production_ce_macos_arm64",
        base / f"production_ce_macos_x86_64",
        base / f"production_ce_linux_x86_64",
        base / f"production_ce_windows_x86_64.exe",
    ]
    for c in candidates:
        if c.exists() and os.access(c, os.X_OK):
            return c
    raise RuntimeError(
        f"production_ce binary not found. Searched: {[str(c) for c in candidates]}"
    )


# --------------------------------------------------------------------------
# CSV / config writers
# --------------------------------------------------------------------------

def _write_row_vector(path: Path, values: list[float]) -> None:
    with path.open("w", newline="") as f:
        csv.writer(f).writerow([f"{v:.17g}" for v in values])


def _write_matrix(path: Path, rows: list[list[float | None]]) -> None:
    """Write a 2-D matrix; None entries become empty cells (NaN-padding)."""
    with path.open("w", newline="") as f:
        w = csv.writer(f)
        for row in rows:
            w.writerow(["" if v is None else f"{v:.17g}" for v in row])


def _pad_rows(rows: list[list[float]], fill: float | None) -> list[list[float | None]]:
    """Right-pad each row to the max width with `fill`."""
    if not rows:
        return rows
    width = max(len(r) for r in rows)
    return [list(r) + [fill] * (width - len(r)) for r in rows]


def write_inputs(params: dict[str, Any], work_dir: Path) -> None:
    """
    Write the seven input artifacts the C binary reads, plus the optional
    start_design.csv warm start.

    `params` is the validated dict from validation.validate_payload().
    """
    work_dir.mkdir(parents=True, exist_ok=True)

    # priorMean.csv (1 x p)
    _write_row_vector(work_dir / "priorMean.csv", params["prior_mean"])

    # priorCov.csv (p x p)
    _write_matrix(work_dir / "priorCov.csv", params["prior_cov"])

    # wp_levels.csv (one row per WP factor, NaN-padded)
    _write_matrix(
        work_dir / "wp_levels.csv",
        _pad_rows(params["wp_levels"], fill=None),
    )

    # sp_levels.csv (one row per SP factor, NaN-padded)
    _write_matrix(
        work_dir / "sp_levels.csv",
        _pad_rows(params["sp_levels"], fill=None),
    )

    # modelTerms.csv (one row per term, zero-padded)
    _write_matrix(
        work_dir / "modelTerms.csv",
        _pad_rows(params["model_terms"], fill=0),
    )

    # n_sizes.csv (1 x m)
    _write_row_vector(work_dir / "n_sizes.csv", params["n_sizes"])

    # start_design.csv (optional warm start, v0.2.8): the binary uses it as
    # the starting design of restart 1 when the file is present.
    if params.get("start_design"):
        _write_matrix(work_dir / "start_design.csv", params["start_design"])

    # config.txt
    cfg = work_dir / "config.txt"
    with cfg.open("w") as f:
        f.write(f"m={params['m']}\n")
        f.write(f"K={params['K']}\n")
        f.write(f"seed={params['seed']}\n")
        f.write(f"copula_type={params['copula_type']}\n")
        f.write(f"sigma2_fixed={params['sigma2_fixed']}\n")
        f.write(f"lambda_fixed={params['lambda_fixed']}\n")
        f.write(f"evalMethod={params['eval_method']}\n")
        f.write(f"crit_mode={params['crit_mode']}\n")
        f.write(f"num_starts={params['num_starts']}\n")
        # v0.2.9: prior integration rule (binary default is axial)
        f.write(f"quadrature={params.get('quadrature', 'axial')}\n")
        if params.get("quadrature", "axial") == "gjs":
            f.write(f"gjs_radii={params.get('gjs_radii', 2)}\n")
            f.write(f"gjs_rotations={params.get('gjs_rotations', 1)}\n")


# --------------------------------------------------------------------------
# Binary invocation
# --------------------------------------------------------------------------

_DCRIT_RE = re.compile(r"D-Criterion:\s*([\-+]?\d+(?:\.\d*)?(?:[eE][\-+]?\d+)?)")


def run_binary(work_dir: Path, timeout_sec: float = 600.0) -> tuple[str, float]:
    """
    Run the binary with cwd=work_dir, return (stdout, elapsed_sec).
    Raises RuntimeError on non-zero exit or timeout.
    """
    binary = binary_path()
    output_csv = work_dir / "OptimalDesign_Output.csv"
    if output_csv.exists():
        output_csv.unlink()

    t0 = time.perf_counter()
    try:
        completed = subprocess.run(
            [str(binary)],
            cwd=str(work_dir),
            capture_output=True,
            text=True,
            timeout=timeout_sec,
        )
    except subprocess.TimeoutExpired:
        raise RuntimeError(
            f"production_ce timed out after {timeout_sec:.0f}s. Try reducing m, n, K, "
            f"or the number of model terms."
        )
    elapsed = time.perf_counter() - t0

    if completed.returncode != 0:
        tail = (completed.stderr or completed.stdout or "").strip().splitlines()[-20:]
        raise RuntimeError(
            f"production_ce returned non-zero exit code ({completed.returncode}):\n"
            + "\n".join(tail)
        )
    return completed.stdout, elapsed


# --------------------------------------------------------------------------
# Output parsing
# --------------------------------------------------------------------------

def parse_outputs(work_dir: Path, stdout: str) -> dict[str, Any]:
    """Parse the design matrix CSV and pull the D-criterion from stdout."""
    output_csv = work_dir / "OptimalDesign_Output.csv"
    if not output_csv.exists():
        raise RuntimeError(
            "production_ce completed but did not produce OptimalDesign_Output.csv. "
            "Check the binary output for clues."
        )

    design = []
    with output_csv.open("r") as f:
        for row in csv.reader(f):
            # Skip blank lines / non-numeric leading rows defensively
            if not row or not row[0].strip():
                continue
            try:
                design.append([float(x) for x in row])
            except ValueError:
                # Treat as a header line; skip
                continue

    m = _DCRIT_RE.search(stdout)
    d_criterion = float(m.group(1)) if m else None

    return {
        "design_matrix": design,
        "d_criterion": d_criterion,
        "n_rows": len(design),
        "n_cols": len(design[0]) if design else 0,
        "raw_stdout": stdout,
    }


# --------------------------------------------------------------------------
# Public entry point
# --------------------------------------------------------------------------

def run_optimization(params: dict[str, Any], timeout_sec: float = 600.0) -> dict[str, Any]:
    """
    End-to-end: write inputs, spawn binary, parse outputs. Cleans up tempdir
    on success; leaves it in place on failure so the user can inspect.
    """
    work_dir = Path(tempfile.mkdtemp(prefix="pcl_designer_"))
    cleanup = True
    try:
        write_inputs(params, work_dir)
        stdout, elapsed = run_binary(work_dir, timeout_sec=timeout_sec)
        result = parse_outputs(work_dir, stdout)
        result["elapsed_sec"] = elapsed
        result["binary_path"] = str(binary_path())
        return result
    except Exception:
        cleanup = False
        raise
    finally:
        if cleanup:
            # Best-effort cleanup; ignore if files are locked on Windows
            try:
                for p in work_dir.iterdir():
                    p.unlink(missing_ok=True)
                work_dir.rmdir()
            except OSError:
                pass
