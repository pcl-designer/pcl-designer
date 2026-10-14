# PCL Designer

Browser-based desktop application for generating optimal D-designs for
ordinal split-plot experiments using the pairwise composite likelihood (PCL)
surrogate from the accompanying methods manuscript (under review).

- **No MATLAB required.** Wraps the production CE binary in a local web UI.
- **Nothing leaves your machine.** All computation is local; no network calls.
- **Cross-platform.** Single-file executable for macOS (arm64, Intel), Linux, Windows.

---

## For practitioners (download and use)

1. Download the installer for your platform from
   [Releases](../../releases/latest):
   - macOS Apple Silicon (M1/M2/M3/M4): `PCLDesigner-macos-arm64`
   - Linux x86_64: `PCLDesigner-linux-x86_64`
   - Windows x86_64: `PCLDesigner-windows-x86_64.exe`

   *Intel Mac builds are not included by default* (the GitHub-hosted
   Intel runners are unreliable). If you need one, clone this repo on an
   Intel Mac and run `bash scripts/build_binary.sh` to produce a binary
   locally.
2. Double-click to launch. A console window opens, and your default browser
   opens to a form at `http://localhost:8765`.
3. Fill in the design parameters (block size, factors, model terms, etc.) and
   click **Generate optimal design**.
4. The design matrix appears in the browser. Download as CSV or JSON.
5. Close the console window to quit the app.

### Unsigned-binary warnings

PCL Designer is currently unsigned. On first launch you'll see:

- **macOS:** "App can't be opened because Apple cannot check it for malicious
  software." Right-click the app → **Open** → confirm in the dialog. Only
  needed the first time.
- **Windows:** "Windows protected your PC." Click **More info** → **Run anyway.**
- **Linux:** No prompt; you may need to `chmod +x PCLDesigner-linux-x86_64`.

---

## For developers

### Run from source

```bash
# (Clone or unzip the supplied source archive)
cd pcl-designer

# Place the per-platform production_ce binary in binaries/
# (e.g., build it from the methods paper's reproducibility archive)

python -m venv .venv
source .venv/bin/activate   # macOS / Linux
# .venv\Scripts\activate    # Windows
pip install -e ".[dev]"
python -m pcl_designer
```

The browser will open to `http://localhost:8765` automatically.

### Run the tests

```bash
pytest
```

Tests cover input validation, CSV writing, output parsing, and Flask routes.
They do not invoke the C binary itself.

### Package a standalone executable

```bash
pyinstaller packaging/pcl_designer.spec
# Output appears at dist/PCLDesigner (or PCLDesigner.exe on Windows)
```

The PyInstaller spec auto-picks the correct `production_ce` binary from
`binaries/` based on the build host. Build on the OS you want to target;
or push a tag (`git tag v0.1.0 && git push --tags`) to trigger the
GitHub Actions matrix build for all four platforms.

### Repo layout

```
pcl-designer/
├── pyproject.toml
├── pcl_designer/
│   ├── __init__.py
│   ├── __main__.py            ← launcher: starts Waitress, opens browser
│   ├── app.py                 ← Flask routes
│   ├── runner.py              ← writes CSVs, spawns binary, parses output
│   ├── validation.py          ← input validation with friendly errors
│   ├── templates/index.html
│   └── static/{style.css,app.js}
├── csrc/production_ce/        ← vendored C source for production_ce
├── scripts/build_binary.sh    ← builds C source + bundles runtime libs
├── binaries/                  ← populated by build_binary.sh (gitignored)
├── packaging/pcl_designer.spec
├── tests/{test_validation,test_runner,test_app}.py
└── .github/workflows/release.yml
```

### What the binary expects

`runner.py` mirrors `call_gap_primary_standalone.m` from the methods
paper's reproducibility archive. It writes seven artifacts into a temporary directory, plus an optional eighth:

| File | Shape | Notes |
|---|---|---|
| `priorMean.csv` | 1 × p | Defaults to zeros |
| `priorCov.csv` | p × p | Defaults to 0.25 · I |
| `wp_levels.csv` | #WP × max_levels | NaN-padded |
| `sp_levels.csv` | #SP × max_levels | NaN-padded |
| `modelTerms.csv` | #terms × max_term_len | Zero-padded |
| `n_sizes.csv` | 1 × m | Block sizes per whole plot |
| `config.txt` | Key=value | `m`, `K`, `seed`, `copula_type` (0 = Frank, 1 = Clayton), `sigma2_fixed`, `lambda_fixed`, `evalMethod`, `crit_mode`, `num_starts`, `quadrature` (`axial` default, or `gjs`), `gjs_radii` (default 2), `gjs_rotations` (default 1), `write_restarts` (0 default; 1 writes `restart_design_NN.csv` and `restart_diagnostics.csv`) |
| `start_design.csv` (optional) | N × #factors | Warm start: factor levels of every run, whole-plot factors first, rows grouped by whole plot. When present it replaces the random starting design of restart 1. |

Then spawns `production_ce` with the tempdir as cwd, parses
`OptimalDesign_Output.csv` and the `D-Criterion:` line from stdout.

### Building the binaries

The C source for `production_ce` lives in `csrc/production_ce/` (vendored
from the methods paper's reproducibility scaffold). To build for your current
host and stage the result in `binaries/`:

```bash
bash scripts/build_binary.sh
```

This does three things:

1. Runs `make` in `csrc/production_ce/`.
2. Copies the resulting executable into `binaries/` with the
   platform-suffixed name PyInstaller expects
   (`production_ce_macos_arm64`, `production_ce_linux_x86_64`,
   `production_ce_windows_x86_64.exe`, etc.).
3. **Bundles the dynamic runtime dependencies alongside the binary** and
   rewrites the binary's dynamic-loader paths so it loads them from its
   own directory rather than the build host's absolute paths:

| Platform | Runtime libs bundled | Loader fix |
|---|---|---|
| macOS | `libomp.dylib` | `install_name_tool -change @executable_path/...` |
| Linux | `libgomp.so.1` | `patchelf --set-rpath '$ORIGIN'` |
| Windows (MINGW64) | `libgomp-1.dll`, `libgcc_s_seh-1.dll`, `libwinpthread-1.dll` | (Windows loads DLLs from the .exe's directory by default) |

After `build_binary.sh` runs, the contents of `binaries/` are portable to
any same-OS / same-arch machine — no Homebrew, no MSYS2, no apt-installed
gcc required on the user's side. The PyInstaller spec picks up the
binary *and* its bundled runtime libs and packages them all into the
final standalone executable.

The CI workflow (`.github/workflows/release.yml`) runs `build_binary.sh`
on each platform automatically; you only need to invoke it manually when
testing locally before pushing a release tag.

---

## Citing

If PCL Designer is part of work you publish, please cite the underlying
methods paper:

> [Author(s) anonymized for peer review] (under review). *Robust
> D-Optimal Designs for Ordinal Split-Plot Experiments via a
> Pairwise Composite Likelihood Surrogate.*

## License

MIT — see `LICENSE`.

## Changelog

- **v1.0.0** --- First public release, the version cited in the accompanying paper.
  - Coordinate-exchange search for split-plot designs with an ordinal response under the PCL copula surrogate (`copula_pcl`; Frank or Clayton copula) or the exact GLMM (`glmm_exact`), with the exact copula, the Godambe form of the PCL criterion and the independence GLM available for diagnostics.
  - Fisher scores by central finite differences in the effective coordinates (the sub-plot linear predictors and the cutpoints), mapped to the full parameter vector by the chain rule: min(*p*, *n*+*K*-1) coordinates for the exact GLMM and min(*p*, *K*+1) for the PCL surrogate.
  - PCL block weighting 1/(n_i - 1), the standard composite-likelihood weighting for unequal cluster sizes (Varin, Reid & Firth, 2011, *Statistica Sinica* 21:5-42). For balanced designs it shifts the reported criterion by the constant *p* ln(*n*-1) and does not change design selection.
  - Prior integration by the 2p-node axial rule (`quadrature=axial`, the default) or by the spherical-radial rule of Gotwalt, Jones and Steinberg (2009, *Technometrics* 51:88-95; `quadrature=gjs`, 1 + 2(p+1)(p+2) nodes), recommended when the prior on the coefficients is wide. Under the latter, a design whose information is singular at any node is rejected, because the simplex vertex weights are negative for p > 7.
  - Configurable restart count (`num_starts`, default 15, maximum 1024); for a given seed the first 15 restarts of a larger run reproduce a default run. `write_restarts=1` saves each restart's final design and criterion.
  - Optional warm start (`start_design.csv`), and in the web interface refinement of a PCL design by one exact-GLMM restart started from it.
  - Web interface with custom priors in the gap coordinates, copula-family selection, and conversion of a GLMM whole-plot variance to the copula parameter by the Kendall's-tau calibration of the paper (also `python -m pcl_designer.dependence`).
