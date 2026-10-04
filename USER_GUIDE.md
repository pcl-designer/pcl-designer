# PCL Designer — User Guide

A desktop tool for generating optimal D-designs for ordinal split-plot experiments via the pairwise composite likelihood (PCL) surrogate. No MATLAB required. Nothing leaves your computer.

This guide walks you through downloading, running, and using PCL Designer on macOS, Windows, and Linux.

---

## 1. Download

Pre-built binaries for your platform are bundled with this anonymized
source archive when supplied. Otherwise, build from source using
`scripts/build_binary.sh` as described in `README.md`. The expected
filenames for each platform are:

| Your computer | Download |
|---|---|
| Mac (Apple Silicon: M1, M2, M3, M4) | `PCLDesigner-macos-arm64` |
| Mac (Intel) | *Not included — see "Intel Mac" note below* |
| Windows (64-bit) | `PCLDesigner-windows-x86_64.exe` |
| Linux (64-bit) | `PCLDesigner-linux-x86_64` |

Each binary is roughly 80–150 MB. It is a single self-contained executable: no installer wizard, no Python, no MATLAB, no other downloads.

> **Intel Mac:** No Intel binary is supplied in the standard build. To produce one, follow the developer README to build from source on an Intel Mac.

---

## 2. Launch

### 2A. macOS

After the download finishes:

1. Open **Terminal** (Cmd+Space, type "Terminal", press Enter).
2. Paste these three commands:

   ```bash
   cd ~/Downloads
   chmod +x PCLDesigner-macos-arm64
   xattr -dr com.apple.quarantine PCLDesigner-macos-arm64
   ./PCLDesigner-macos-arm64
   ```

3. A banner appears in Terminal:

   ```
   ============================================================
     PCL Designer v0.1.x
     Open in your browser: http://localhost:8765
     Close this window (or Ctrl+C) to quit.
   ============================================================
   ```

4. Your default browser opens automatically to a form. (If it doesn't, manually visit `http://localhost:8765`.)

**Prefer not to use Terminal?** Double-click `PCLDesigner-macos-arm64` in Finder. macOS will say "developer cannot be verified" — click OK, then **right-click** the file → **Open** → click **Open** in the confirmation dialog. macOS will remember your decision; future double-clicks launch silently.

### 2B. Windows

1. Open **File Explorer** and navigate to your **Downloads** folder.
2. Double-click `PCLDesigner-windows-x86_64.exe`.
3. Windows shows: "Microsoft Defender SmartScreen prevented an unrecognized app from starting."
4. Click the small blue **More info** link at the top of the popup.
5. A **Run anyway** button appears at the bottom. Click it.
6. A black console window opens; your default browser opens to `http://localhost:8765`.

Future launches are silent — Windows remembers you've trusted the app.

### 2C. Linux

1. Open a terminal in your **Downloads** folder.
2. Run:

   ```bash
   chmod +x PCLDesigner-linux-x86_64
   ./PCLDesigner-linux-x86_64
   ```

3. The banner appears; your default browser opens to `http://localhost:8765`.

---

## 3. Use

The browser form has five sections.

### Whole plot structure
- **Number of whole plots (m):** how many hard-to-change blocks.
- **Sub-plots per whole plot (n):** how many easy-to-change runs within each block. Enter either a single integer (e.g. `4`) for equal-size blocks, or a comma-separated list of `m` integers (e.g. `4, 4, 6, 6`) for unequal-size blocks.

### Response model
- **Ordinal categories (K):** the number of response categories your outcome takes.
- **Whole-plot variance (σ²):** prior estimate of the between-block variance, used by the exact GLMM evaluator. Defaults to 2.5 (the paper's baseline).
- **Copula family:** the copula the PCL surrogate uses to couple sub-plot pairs. **Frank** (the default) has symmetric dependence and is the family evaluated in the paper. **Clayton** has lower-tail dependence, so responses within a whole plot are most strongly associated when they fall jointly in the low categories; it is provided for sensitivity analysis and was not evaluated in the paper.
- **Copula dependence (λ):** strength of within-block dependence for the PCL surrogate. Defaults to 3.0, the paper's working value for the Frank copula. The field reports the Kendall's τ that the current λ implies under the selected family, which makes values comparable across families (for example, λ = 3 means τ ≈ 0.31 under Frank but τ = 0.60 under Clayton). Clayton λ is limited to at most 30 (τ = 0.94).
- **Set λ from a whole-plot variance σ²:** if you think in terms of the GLMM whole-plot variance rather than λ, open this panel, enter σ², and click **Use this λ**. The conversion is the calibration of Section 5.1 of the paper. The latent intra-block correlation is ρ = σ²/(σ² + π²/3), its Kendall's τ is (2/π) arcsin ρ, and λ is the parameter of the selected copula with that τ. At the paper's baseline σ² = 2.5 this gives Frank λ = 2.741 (which the paper rounds to 3.0) and Clayton λ = 0.794. The same conversion is available from the command line as `python -m pcl_designer.dependence 2.5` (add `--family clayton` for Clayton).
- **Random seed:** leave blank for a fresh random seed each run, or enter a specific integer for reproducibility.

### Factors
Add each whole-plot and sub-plot factor with its level set. Type levels as comma-separated values:
- Two-level factor: `-1, 1`
- Three-level: `-1, 0, 1`

### Model terms
Each line is one term in the model, written as **1-indexed factor numbers** separated by spaces:
- `1` — main effect of factor 1
- `1 2` — interaction between factors 1 and 2
- `1 2 3` — three-way interaction

The factor numbering: whole-plot factors first (in the order you added them), then sub-plot factors. So with 1 WP factor and 2 SP factors, factor 1 = WP, factors 2 and 3 = SP.

Use the **Main effects only** or **Main + 2FI** quick-fill buttons to populate the textarea with a common pattern, then edit as needed.

### Advanced (optional)
Surrogate evaluator, criterion mode, restart count, timeout, and the prior. (The copula family now sits next to λ under *Response model*.) The defaults match the paper's recommended configuration. The restart count (`num_starts`, default 15, maximum 1024) sets how many independent coordinate-exchange restarts the search runs before returning the best design; larger budgets (e.g. 100) are recommended at high parameter dimension, where the criterion landscape carries many local optima.

The **prior** fields let you replace the default prior over the model parameters. Enter a prior-mean vector and a prior-covariance (a single scalar *s* for *s*&middot;I_p, *p* variances for a diagonal, or *p* rows of *p* values for a full matrix), both in the gap coordinates (alpha_1, logDelta_1, ..., beta_1, ...); the panel shows the required dimension *p* = (K-1) + #terms. Leave them blank to use the defaults (mu = 0, Sigma = 0.25 I_p).

### Click "Generate optimal design"

The CE search runs for a few seconds to a few minutes depending on problem size. When it finishes, the page shows:

- **D-criterion** value, qualified by the evaluator that produced it (e.g., "D-criterion (PCL surrogate)"). When the PCL surrogate is the evaluator — the default — the number is the log-determinant of the *surrogate* information matrix, which approximates the true exact-GLMM D-criterion. They agree closely in practice but are formally distinct.
- **Wall-clock** time
- **Design size** (rows × columns)
- **Seed used** (so you can reproduce the run)
- **The design matrix** as a scrollable table
- **Download buttons**

### Refine with exact GLMM (optional)

After a PCL search, the result panel offers **Refine with exact GLMM**. It runs one coordinate-exchange restart under the exact GLMM, starting from the PCL design, and keeps every exchange that improves the exact criterion. Because it starts close to a good design, it usually needs only a couple of passes and recovers most of any gap between the PCL design and the exact-GLMM optimum, at a fraction of the cost of a full exact search.

- **Whole-plot variance σ²:** the exact GLMM needs σ² rather than λ. The field is pre-filled from the σ² → λ converter.
- **Timeout:** the refinement has its own timeout, 3600 s by default.
- **Cost:** the exact GLMM sums over K^n outcomes per whole plot, so its cost grows exponentially in the whole-plot size. The panel shows K^n for your design and warns when it exceeds 10,000; at that size the PCL design alone may be the practical choice.
- **Viewing the result:** a summary reports how many runs changed and the refined design's exact-GLMM D-criterion. Switch between **Refined design** and **PCL design** to view or download either.

Refinement needs a main-effect term for every factor, so the factor levels of each run can be read from the design; otherwise the button is disabled.

### Run Sheet vs Model Matrix

The result section gives you two views of the design and two corresponding CSV downloads. Both are derived from the same underlying optimization — pick whichever fits the next thing you're going to do.

**Run Sheet (default view, "Download Run Sheet (CSV)").**
One column per factor. Each cell is the level to set in that run. This is what you take to the lab or production floor to actually run the experiment. Columns are `Run | Block | WP1 | SP1 | SP2 | ...`.

**Model Matrix (toggle on "Show interaction columns", "Download Model Matrix (CSV)").**
The full expanded design matrix — one column per model term, with interaction columns being the products of their constituent factors. This is the `X` matrix you'd plug directly into a regression (e.g. into `ordinal::clmm` in R). Columns are `Run | Block | WP1 | SP1 | SP2 | WP1·SP1 | WP1·SP2 | ...`.

The two CSVs contain the same factor settings — only the interaction columns are present/absent. If you don't have any interactions in your model (main effects only), the toggle won't appear and the two views are identical.

---

## 4. Quit

Close the console window (the one showing the "PCL Designer running at..." banner), or press Ctrl+C inside it. The browser tab can stay open but stops working once the server stops.

---

## 5. Troubleshooting

**Problem: Double-clicking on Mac does nothing.**
Open Terminal and run the `chmod +x` and `xattr` commands in section 2A. The browser-downloaded file likely lacks execute permission.

**Problem: macOS says "App is damaged and can't be opened."**
This is Gatekeeper being aggressive. The `xattr -dr com.apple.quarantine ...` command in section 2A removes the quarantine flag and resolves it.

**Problem: Windows SmartScreen blocks the download entirely.**
Right-click the downloaded file in File Explorer → **Properties** → check **Unblock** at the bottom → click **OK**. Then double-click again.

**Problem: Browser doesn't open automatically.**
Manually visit `http://localhost:8765`. If the page doesn't load, check the console window — it prints the actual port on the banner line (it may have picked a different one if 8765 was occupied).

**Problem: "Generate" runs forever.**
The CE search scales rapidly with block size `n` and category count `K`. Try smaller values first to confirm the app works (e.g., `m=4, n=2, K=3`, main effects only — runs in a few seconds). The default 600-second timeout will fire if the run actually wedges.

**Problem: Empty design matrix in the result.**
Almost always a validation issue the server caught. Check the **Error** panel at the top of the page — the message will tell you exactly what's wrong with your input.

---

## 6. What this tool produces

The output is a *D-optimal design under the pairwise composite likelihood (PCL) surrogate*, robust over the prior distribution you supplied. It's the same algorithm and output described in:

> [Author(s) anonymized for peer review] (under review). *Robust D-Optimal Designs for Ordinal Split-Plot Experiments via a Pairwise Composite Likelihood Surrogate.*

Use the design matrix to run your experiment: each row is one run, the **Block** column tells you which whole plot it belongs to, and the factor columns give the levels to set. After running the experiment and observing your ordinal response, fit a cumulative-logit GLMM (e.g., via `ordinal::clmm` in R) using the design's expanded model matrix as `X`.

---

## 7. Privacy and safety

PCL Designer runs entirely on your local machine. It does not:
- Send any data over the network
- Phone home or report telemetry
- Read or write files outside its own working directory
- Persist your inputs between sessions

You can run it on an air-gapped machine. The browser UI is just a local web page rendered by your browser — `http://localhost:8765` is your own computer talking to itself.

---

## 8. Help, bugs, feature requests

Author contact details are withheld during peer review. Issue
tracking and support channels will be made available with the
public release that accompanies publication of the methods paper.

---

## 9. Version

This guide covers PCL Designer v0.2.x.
