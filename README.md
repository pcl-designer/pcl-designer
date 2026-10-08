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
| `config.txt` | Key=value | `m`, `K`, `seed`, `copula_type` (0 = Frank, 1 = Clayton), `sigma2_fixed`, `lambda_fixed`, `evalMethod`, `crit_mode`, `num_starts`; v0.2.9: `quadrature` (`axial` default, or `gjs`), `gjs_radii` (default 2), `gjs_rotations` (default 1), `write_restarts` (0 default; 1 writes `restart_design_NN.csv` and `restart_diagnostics.csv`) |
| `start_design.csv` (optional) | N × #factors | Warm start (v0.2.8): factor levels of every run, whole-plot factors first, rows grouped by whole plot. When present it replaces the random starting design of restart 1. |

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

- **v0.2.9** --- A second prior-integration rule. The criterion averages log det over prior nodes; until now that was the 2p-node axial rule, which moves one parameter sqrt(p) prior standard deviations at a time and is sound for narrow priors. The new option `quadrature=gjs` (web UI: *Advanced -> Prior integration*) uses the spherical-radial rule of Gotwalt, Jones and Steinberg (2009, *Technometrics* 51:88-95): a center point plus `gjs_radii` radii (default 2, exact for quintics), each a randomly rotated extended simplex (`gjs_rotations` rotations per radius, default 1), 1 + 2(p+1)(p+2) nodes in all. Rotations come from a self-contained generator, so the restart sequence is unchanged. Because the simplex vertex weights are negative for p > 7, a design whose information is singular at any node is rejected under this rule. With the default `quadrature=axial` the engine is unchanged: designs and criteria are identical to v0.2.8. New `write_restarts=1` writes each restart's final design (`restart_design_NN.csv`) and criterion (`restart_diagnostics.csv`). The engine log is now line-buffered and states the rule and node count.
- **v0.2.8** --- A PCL design can now be refined under the exact GLMM. After a PCL search, the result panel offers **Refine with exact GLMM**, which runs one exact-GLMM coordinate-exchange restart started from the PCL design and keeps every exchange that improves the exact criterion; the refined and PCL designs can be viewed and downloaded side by side. The panel reports the exact GLMM's K^n outcomes per whole plot and warns when a refinement is likely to be slow. In the two-whole-plot full-quadratic check of the accompanying paper, refinement raised the median exact-GLMM relative efficiency of the PCL design from 0.983 to 0.995, and the PCL search plus refinement took about a quarter of the time of a full 15-restart exact search. In the paper's case study, the PCL design was already a local optimum of the exact criterion and refinement changed no runs, so refinement does not always improve a design. The engine supports this through an optional `start_design.csv` input (also accepted as `start_design` by `/api/optimize`), which replaces the random starting design of restart 1. Without that file the engine is unchanged, and designs and criteria are identical to v0.2.7. `scripts/build_binary.sh` now re-signs the macOS binary and `libomp.dylib` ad hoc after rewriting their load paths, which Apple silicon otherwise refuses to launch.
- **v0.2.7** --- The PCL surrogate's copula family is now selectable in the web interface. Alongside the Frank copula used in the paper, the Clayton copula (lower-tail dependence) can be chosen for sensitivity analysis; it was already implemented in the search engine and reachable through `copula_type=1` in `config.txt`, and is now exposed in the form, labeled as not evaluated in the paper. A new panel under the λ field converts a GLMM whole-plot variance σ² to λ by the Kendall's-τ calibration of Section 5.1 of the paper, for either family, and the λ field reports the Kendall's τ the current value implies. The conversion is also available as `python -m pcl_designer.dependence`. Validation now accepts only `copula_type` 0 or 1 (previously any nonzero value silently selected Clayton) and caps Clayton λ at 30, the largest value for which the Clayton generator cannot overflow. This release also fixes a display bug in which inputs meant to be hidden for the selected evaluator (σ² under the PCL surrogate, λ under the exact GLMM) remained visible. The search engine is unchanged, and Frank designs are identical to v0.2.6.
- **v0.2.6** --- The web interface now accepts a custom prior. The Advanced panel exposes a prior-mean vector and a prior-covariance field --- enter a single scalar *s* for *s*&middot;I_p, *p* variances for a diagonal, or *p* rows of *p* values for a full matrix --- both in the gap coordinates (alpha_1, logDelta_1, ..., beta_1, ...), with a live readout of the required dimension *p* = (K-1) + #terms. Leaving the fields blank preserves the previous defaults (mu = 0, Sigma = 0.25 I_p). The search engine and the `config.txt` interface are unchanged; this only surfaces prior inputs the backend already accepted via the `/api/optimize` payload.
- **v0.2.5** --- Both evaluators now compute the per-block score by central finite differences in the effective coordinates (the sub-plot linear predictors and the cutpoints) and map it to the full parameter vector by the chain rule, rather than differencing all *p* parameters. Each block's log-likelihood depends on the parameter only through min(*p*, *n*+*K*-1) combinations for the exact GLMM evaluator and min(*p*, *K*+1) for the PCL surrogate, so the derivative cost falls at high parameter dimension while the computed information is algebraically identical. Optimal designs and reported D-criteria are unchanged from v0.2.4 for every evaluator (verified bit-identical design selection and matching criteria for `glmm_exact`, `copula_pcl`, and `copula_pcl_godambe`); the 1/(n-1) PCL block weighting (v0.2.2) and the configurable restart count (v0.2.3) are retained. Single-core speedups at p=8 are roughly 2x for the exact evaluator and the plain PCL criterion; the Godambe PCL path is unchanged in cost. The search binaries are rebuilt from the regenerated source. The v0.2.2 PCL block weighting and the v0.2.3 restart count, previously maintained as post-generation edits to the generated C, are now expressed directly in the MATLAB source, so the committed C is a clean regeneration with no hand-applied patches.
- **v0.2.4** --- The web app's Advanced panel now exposes the coordinate-exchange restart count (`num_starts`, default 15, maximum 1024), which v0.2.3 introduced at the `config.txt` level. No changes to the search binaries.
- **v0.2.3** --- The number of coordinate-exchange restarts is now configurable: add `num_starts=<k>` to `config.txt` (default 15, maximum 1024; the value is echoed in the run banner). Search behavior is otherwise unchanged, and for a given seed the first 15 restarts of a larger run reproduce a default run exactly, so `num_starts=15` (or omitting the key) is bit-identical to v0.2.2. Larger restart budgets are recommended at high parameter dimension, where the criterion landscape carries many local optima.
- **v0.2.2** --- The PCL evaluators (`copula_pcl`, `copula_pcl_godambe`) now weight each block's pairwise accumulation by 1/(n_i - 1), the standard composite-likelihood weighting for unequal cluster sizes (Varin, Reid & Firth, 2011, *Statistica Sinica* 21:5-42). Each observation enters n_i - 1 sub-plot pairs, so the weighting restores a common per-observation counting rate across blocks of unequal size. For **balanced** designs the factor is a design-independent constant: optimal designs are identical to v0.2.1 and the reported D-criterion shifts by exactly p*ln(n-1). For **unbalanced** designs, v0.2.2 selections supersede v0.2.1, which over-weighted large blocks.
- **v0.2.1** --- Initial public release.
