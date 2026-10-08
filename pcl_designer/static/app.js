// PCL Designer — frontend logic (vanilla JS, no build step)

const WP = "wp";
const SP = "sp";
let factorCount = { wp: 0, sp: 0 };
let lastResult = null;
let lastEcho = null;
// v0.2.8 exact-GLMM refinement state. lastPayload is the request that
// produced the current PCL result; the refine request reuses it so later
// edits to the form cannot desynchronize the two runs.
let lastPayload = null;
let pclResult = null, pclEcho = null;
let refinedResult = null, refinedEcho = null;

// --------------------------- Init ---------------------------------------

window.addEventListener("DOMContentLoaded", () => {
  // Seed the form with one WP and two SP factors, all 2-level (-1, 1).
  addFactor(WP, "-1, 1");
  addFactor(SP, "-1, 1");
  addFactor(SP, "-1, 1");

  // Default model terms: main effects of factors 1, 2, 3 plus their 2FIs.
  fillTerms("main2fi");

  // Hide σ² and λ inputs that aren't relevant to the selected evaluator.
  // - copula_pcl  → λ used, σ² ignored
  // - glmm_exact  → σ² used, λ ignored
  // - glmm_approx → neither used
  // Values still submit (the backend always needs *some* number), but the
  // user is no longer asked for inputs that don't affect the design.
  document.getElementById("eval-method").addEventListener("change", updateEvaluatorFields);
  updateEvaluatorFields();

  // Copula family and the sigma^2 -> lambda converter (PCL surrogate only).
  document.getElementById("copula-type").addEventListener("change", updateDependenceInfo);
  document.getElementById("lambda").addEventListener("input", updateDependenceInfo);
  document.getElementById("conv-sigma2").addEventListener("input", updateDependenceInfo);
  document.getElementById("conv-apply").addEventListener("click", applyConvertedLambda);
  updateDependenceInfo();

  document.getElementById("K").addEventListener("input", updatePriorDim);
  document.getElementById("model-terms").addEventListener("input", updatePriorDim);
  updatePriorDim();

  document.getElementById("design-form").addEventListener("submit", onSubmit);
  document.getElementById("download-runsheet").addEventListener("click", downloadRunSheet);
  document.getElementById("download-model-matrix").addEventListener("click", downloadModelMatrix);
  document.getElementById("download-json").addEventListener("click", downloadJSON);
  document.getElementById("show-interactions").addEventListener("change", rerenderDesignTable);
  document.getElementById("refine-btn").addEventListener("click", onRefine);
  document.querySelectorAll('input[name="design-view"]').forEach((el) =>
    el.addEventListener("change", onDesignViewChange));
  document.getElementById("rerun-new-seed").addEventListener("click", () => {
    document.getElementById("seed").value = "";
    onSubmit(new Event("submit"));
  });
});

// --------------------------- Evaluator-dependent fields -----------------

function updateEvaluatorFields() {
  // Show σ² only for glmm_exact and λ only for copula_pcl. Hidden fields
  // still carry their default values into the payload so the backend (which
  // always reads sigma2_fixed and lambda_fixed from config.txt) doesn't see
  // missing keys, but the user is no longer shown irrelevant inputs.
  const method = document.getElementById("eval-method").value;
  const sigmaField = document.getElementById("sigma2-field");
  const lambdaField = document.getElementById("lambda-field");
  sigmaField.hidden = method !== "glmm_exact";
  lambdaField.hidden = method !== "copula_pcl";
  document.getElementById("copula-family-field").hidden = method !== "copula_pcl";
  document.getElementById("converter-field").hidden = method !== "copula_pcl";
}

// --------------------------- Copula dependence --------------------------
//
// Line-for-line port of pcl_designer/dependence.py (itself a port of the
// stand-alone sigma2lambda.c tool). Implements the Section 5.1 calibration
// sigma^2 -> rho -> Kendall's tau -> lambda for the Frank and Clayton copulas.

const COPULA_FRANK = 0;
const COPULA_CLAYTON = 1;
const COPULA_NAMES = { 0: "Frank", 1: "Clayton" };
const COPULA_NOTES = {
  0: "Symmetric dependence in both tails. This is the family evaluated in the paper.",
  1: "Lower-tail dependence, so responses within a whole plot are most strongly " +
     "associated when they fall jointly in the low categories. Not evaluated in the paper.",
};

// 64-point Gauss-Legendre nodes and weights on [-1, 1] (positive half).
const GL_X = [
  0.0243502926634244, 0.072993121787799, 0.1214628192961206, 0.1696444204239928,
  0.2174236437400071, 0.2646871622087674, 0.311322871990211, 0.3572201583376681,
  0.4022701579639916, 0.4463660172534641, 0.489403145707053, 0.5312794640198946,
  0.571895646202634, 0.6111553551723933, 0.6489654712546573, 0.6852363130542333,
  0.7198818501716109, 0.7528199072605319, 0.7839723589433414, 0.8132653151227975,
  0.8406292962525803, 0.8659993981540928, 0.8893154459951141, 0.9105221370785028,
  0.9295691721319396, 0.9464113748584028, 0.9610087996520538, 0.973326827789911,
  0.983336253884626, 0.9910133714767443, 0.9963401167719553, 0.9993050417357722
];
const GL_W = [
  0.0486909570091397, 0.0485754674415034, 0.048344762234803, 0.0479993885964583,
  0.0475401657148303, 0.04696818281621, 0.0462847965813144, 0.0454916279274181,
  0.0445905581637566, 0.0435837245293235, 0.0424735151236536, 0.0412625632426235,
  0.0399537411327203, 0.0385501531786156, 0.03705512854024, 0.0354722132568824,
  0.0338051618371416, 0.0320579283548516, 0.0302346570724025, 0.0283396726142595,
  0.0263774697150547, 0.0243527025687109, 0.0222701738083833, 0.0201348231535302,
  0.0179517157756973, 0.0157260304760247, 0.0134630478967186, 0.0111681394601311,
  0.0088467598263639, 0.0065044579689784, 0.0041470332605625, 0.0017832807216964
];

function debyeIntegrand(t) {
  return t < 1e-12 ? 1.0 - t / 2.0 : t / Math.expm1(t);
}

function debye1(x) {
  const npanel = Math.max(1, Math.ceil(x));
  const h = x / npanel;
  let total = 0.0;
  for (let p = 0; p < npanel; p++) {
    const c = p * h + h / 2.0;
    const s = h / 2.0;
    for (let i = 0; i < 32; i++) {
      total += GL_W[i] * s * (debyeIntegrand(c - s * GL_X[i]) + debyeIntegrand(c + s * GL_X[i]));
    }
  }
  return total / x;
}

function frankTau(lam) {
  return lam <= 0 ? 0.0 : 1.0 - (4.0 / lam) * (1.0 - debye1(lam));
}

function claytonTau(lam) {
  return lam <= 0 ? 0.0 : lam / (lam + 2.0);
}

function copulaTau(lam, family) {
  return family === COPULA_CLAYTON ? claytonTau(lam) : frankTau(lam);
}

function tauToLambda(tau, family) {
  if (!(tau >= 0 && tau < 1)) return NaN;
  if (tau === 0) return 0.0;
  if (family === COPULA_CLAYTON) return (2.0 * tau) / (1.0 - tau);
  let lo = 1e-10, hi = 1.0;
  while (frankTau(hi) < tau) {
    hi *= 2.0;
    if (hi > 1e6) return NaN;
  }
  for (let it = 0; it < 200; it++) {
    const mid = 0.5 * (lo + hi);
    if (frankTau(mid) < tau) lo = mid; else hi = mid;
    if (hi - lo < 1e-13 * (1.0 + hi)) break;
  }
  return 0.5 * (lo + hi);
}

function sigma2ToLambda(sigma2, family) {
  const rho = sigma2 / (sigma2 + (Math.PI * Math.PI) / 3.0);
  const tau = (2.0 / Math.PI) * Math.asin(rho);
  return { rho, tau, lambda: tauToLambda(tau, family) };
}

function selectedCopulaFamily() {
  return parseInt(document.getElementById("copula-type").value, 10) === COPULA_CLAYTON
    ? COPULA_CLAYTON : COPULA_FRANK;
}

let convertedLambda = NaN;

function updateDependenceInfo() {
  const family = selectedCopulaFamily();
  const name = COPULA_NAMES[family];
  document.getElementById("copula-family-note").textContent = COPULA_NOTES[family];

  const lam = parseFloat(document.getElementById("lambda").value);
  document.getElementById("lambda-tau").textContent =
    Number.isFinite(lam) && lam > 0
      ? `Under the ${name} copula, \u03bb = ${lam} corresponds to Kendall's \u03c4 = ${copulaTau(lam, family).toFixed(3)}.`
      : "Must be greater than 0.";

  const s2 = parseFloat(document.getElementById("conv-sigma2").value);
  const out = document.getElementById("conv-result");
  const btn = document.getElementById("conv-apply");
  if (!(Number.isFinite(s2) && s2 > 0)) {
    convertedLambda = NaN;
    out.textContent = "Enter a whole-plot variance greater than 0.";
    btn.disabled = true;
    return;
  }
  const r = sigma2ToLambda(s2, family);
  convertedLambda = r.lambda;
  out.textContent =
    `\u03c1 = ${r.rho.toFixed(4)}, \u03c4 = ${r.tau.toFixed(4)}, ` +
    `so ${name} \u03bb = ${r.lambda.toFixed(4)}`;
  btn.disabled = !Number.isFinite(r.lambda);
}

function applyConvertedLambda() {
  if (!Number.isFinite(convertedLambda)) return;
  document.getElementById("lambda").value = convertedLambda.toFixed(4);
  updateDependenceInfo();
}

// --------------------------- Factor rows --------------------------------

function addFactor(kind, defaultLevels = "-1, 1") {
  factorCount[kind]++;
  const container = document.getElementById(`${kind}-factors`);
  const row = document.createElement("div");
  row.className = "factor-row";
  const label = `${kind === WP ? "WP" : "SP"}${factorCount[kind]}`;
  row.innerHTML = `
    <label>
      <span>${label} levels (comma-separated)</span>
      <input type="text" data-kind="${kind}" value="${defaultLevels}" required>
    </label>
    <button type="button" class="remove" aria-label="Remove ${label}">Remove</button>
  `;
  row.querySelector(".remove").addEventListener("click", () => {
    row.remove();
    relabelFactors(kind);
  });
  container.appendChild(row);
}

function relabelFactors(kind) {
  const container = document.getElementById(`${kind}-factors`);
  factorCount[kind] = 0;
  container.querySelectorAll(".factor-row").forEach((row) => {
    factorCount[kind]++;
    const label = `${kind === WP ? "WP" : "SP"}${factorCount[kind]}`;
    row.querySelector("label > span").textContent = `${label} levels (comma-separated)`;
    row.querySelector(".remove").setAttribute("aria-label", `Remove ${label}`);
  });
}

function readFactorLevels(kind) {
  const container = document.getElementById(`${kind}-factors`);
  const rows = [...container.querySelectorAll("input[data-kind]")];
  return rows.map((inp) => parseLevelString(inp.value));
}

function parseLevelString(s) {
  return s.split(/[,\s]+/).filter((x) => x.length > 0).map(Number);
}

function parseBlockSizes(s) {
  // The n field accepts either a single integer (equal blocks) or a
  // comma-separated list (unequal blocks, one entry per whole plot).
  // Returns a scalar if there's only one value, an array otherwise.
  // The backend handles both shapes via validation.validate_payload().
  const parts = s.split(/[,\s]+/).filter((x) => x.length > 0).map((x) => parseInt(x, 10));
  if (parts.length === 0 || parts.some(Number.isNaN)) {
    return NaN;          // upstream will catch this via the form's pattern attribute
  }
  return parts.length === 1 ? parts[0] : parts;
}

// --------------------------- Model terms helpers -------------------------

function fillTerms(kind) {
  const numFactors = factorCount.wp + factorCount.sp;
  if (numFactors === 0) return;
  const ta = document.getElementById("model-terms");
  const lines = [];
  for (let i = 1; i <= numFactors; i++) lines.push(`${i}`);
  if (kind === "main2fi") {
    for (let i = 1; i <= numFactors; i++) {
      for (let j = i + 1; j <= numFactors; j++) {
        lines.push(`${i} ${j}`);
      }
    }
  }
  ta.value = lines.join("\n");
  updatePriorDim();
}

function readModelTerms() {
  const text = document.getElementById("model-terms").value.trim();
  if (!text) return [];
  return text.split(/\n+/).map((line) => {
    return line.trim().split(/[\s,]+/).filter((x) => x.length > 0).map(Number);
  });
}

// --------------------------- Prior parsing ------------------------------

function parseNumList(str) {
  return str.split(/[\s,]+/).filter((x) => x.length > 0).map(Number);
}

// Required prior dimension p = (K - 1) + number of model terms.
function priorDim() {
  const K = parseInt(document.getElementById("K").value, 10);
  const terms = readModelTerms();
  if (!Number.isFinite(K) || K < 2 || terms.length === 0) return null;
  return (K - 1) + terms.length;
}

function updatePriorDim() {
  const el = document.getElementById("prior-dim");
  if (!el) return;
  const p = priorDim();
  el.textContent = p === null ? "\u2014" : String(p);
}

function identityScaled(p, s) {
  return Array.from({ length: p }, (_, i) =>
    Array.from({ length: p }, (_, j) => (i === j ? s : 0)));
}

function diagFromVariances(vars) {
  const p = vars.length;
  return Array.from({ length: p }, (_, i) =>
    Array.from({ length: p }, (_, j) => (i === j ? vars[i] : 0)));
}

// Reads the optional prior mean/covariance fields and validates them against
// p. Returns a partial payload { prior_mean?, prior_cov? }; omitted keys fall
// back to the backend defaults (mu = 0, Sigma = 0.25 I_p). Throws on a size or
// numeric error so collectPayload surfaces it.
function readPrior(p) {
  const out = {};
  const meanRaw = document.getElementById("prior-mean").value.trim();
  const covRaw = document.getElementById("prior-cov").value.trim();
  if (!meanRaw && !covRaw) return out;
  if (p === null) {
    throw new Error("Set K and at least one model term before entering a prior.");
  }
  if (meanRaw) {
    const mean = parseNumList(meanRaw);
    if (mean.some((v) => !Number.isFinite(v))) {
      throw new Error("Prior mean has a non-numeric entry.");
    }
    if (mean.length !== p) {
      throw new Error(`Prior mean must have length p = ${p} (got ${mean.length}).`);
    }
    out.prior_mean = mean;
  }
  if (covRaw) {
    const rows = covRaw.split(/\n+/).map(parseNumList).filter((r) => r.length > 0);
    const flat = rows.flat();
    if (flat.some((v) => !Number.isFinite(v))) {
      throw new Error("Prior covariance has a non-numeric entry.");
    }
    let cov;
    if (flat.length === 1) {
      cov = identityScaled(p, flat[0]);
    } else if (rows.length === 1 && rows[0].length === p) {
      cov = diagFromVariances(rows[0]);
    } else if (rows.length === p && rows.every((r) => r.length === p)) {
      cov = rows;
    } else {
      throw new Error(
        `Prior covariance must be one scalar, ${p} variances, or a ${p}\u00d7${p} matrix.`);
    }
    out.prior_cov = cov;
  }
  return out;
}

// --------------------------- Submit -------------------------------------

async function onSubmit(ev) {
  ev.preventDefault();
  hideError();
  hideResults();

  const payload = collectPayload();
  if (!payload) return;

  const submitBtn = document.getElementById("submit-btn");
  const statusEl = document.getElementById("status");
  submitBtn.disabled = true;
  statusEl.textContent = "Running CE search…";

  const t0 = performance.now();
  try {
    const resp = await fetch("/api/optimize", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    const data = await resp.json();
    const wallSec = (performance.now() - t0) / 1000;

    if (!data.ok) {
      showError(data.error || `Server returned HTTP ${resp.status}`);
    } else {
      lastResult = data.result;
      lastEcho = data.echo;
      lastPayload = payload;
      pclResult = data.result;
      pclEcho = data.echo;
      refinedResult = null;
      refinedEcho = null;
      renderResults(data.result, data.echo, wallSec);
      setupRefinePanel();
    }
  } catch (e) {
    showError(`Network or parse error: ${e.message}`);
  } finally {
    submitBtn.disabled = false;
    statusEl.textContent = "";
  }
}

function collectPayload() {
  try {
    const m = parseInt(document.getElementById("m").value, 10);
    const n = parseBlockSizes(document.getElementById("n").value);
    const K = parseInt(document.getElementById("K").value, 10);
    const sigma2 = parseFloat(document.getElementById("sigma2").value);
    const lambda = parseFloat(document.getElementById("lambda").value);
    const seedRaw = document.getElementById("seed").value;
    const seed = seedRaw === "" ? undefined : parseInt(seedRaw, 10);
    const evalMethod = document.getElementById("eval-method").value;
    const critMode = document.getElementById("crit-mode").value;
    const copulaType = parseInt(document.getElementById("copula-type").value, 10);
    const timeoutSec = parseFloat(document.getElementById("timeout-sec").value);

    const wpLevels = readFactorLevels(WP);
    const spLevels = readFactorLevels(SP);
    const modelTerms = readModelTerms();

    if (modelTerms.length === 0) {
      showError("Add at least one model term.");
      return null;
    }

    const prior = readPrior((K - 1) + modelTerms.length);

    const payload = {
      m, n, K,
      sigma2_fixed: sigma2,
      lambda_fixed: lambda,
      copula_type: copulaType,
      eval_method: evalMethod,
      crit_mode: critMode,
      wp_levels: wpLevels,
      sp_levels: spLevels,
      model_terms: modelTerms,
      timeout_sec: timeoutSec,
      num_starts: parseInt(document.getElementById("num-starts").value, 10) || 15,
      quadrature: (document.getElementById("quadrature") || {}).value || "axial",
      ...prior,
    };
    if (seed !== undefined && !Number.isNaN(seed)) payload.seed = seed;
    return payload;
  } catch (e) {
    showError(`Form read error: ${e.message}`);
    return null;
  }
}

// --------------------------- Render -------------------------------------

function renderResults(result, echo, wallSec) {
  renderMetrics(result, echo);

  // Reset interactions toggle to default (main effects only) on each new run.
  document.getElementById("show-interactions").checked = false;
  // Hide the toggle entirely if there's nothing to toggle (no interactions in model).
  const meIdx = (echo && echo.main_effect_term_indices) || [];
  const hasInteractions = result.n_cols > meIdx.length;
  document.getElementById("view-toggle-wrap").style.display =
    hasInteractions ? "" : "none";

  rerenderDesignTable();

  document.getElementById("results").hidden = false;
  document.getElementById("results").scrollIntoView({ behavior: "smooth", block: "start" });
}

function renderMetrics(result, echo) {
  const dCrit = result.d_criterion;
  document.getElementById("d-criterion").textContent =
    dCrit === null ? "—" : dCrit.toFixed(6);
  // Qualify the D-criterion label with the evaluator that actually
  // produced it. When eval_method=copula_pcl, the number is the log-det
  // of the surrogate information matrix — NOT the exact-GLMM D-criterion.
  // The two agree closely under the paper's operating regime but are
  // formally distinct quantities; being explicit prevents confusion.
  document.getElementById("d-criterion-label").textContent =
    `D-criterion (${evaluatorDisplayName(echo && echo.eval_method)}` +
    (echo && echo.eval_method === "copula_pcl" && echo.copula_family
      ? `, ${echo.copula_family} copula)` : ")");
  document.getElementById("elapsed").textContent = `${result.elapsed_sec.toFixed(1)} s`;
  document.getElementById("dimensions").textContent = `${result.n_rows} × ${result.n_cols}`;
  document.getElementById("seed-used").textContent = echo.seed;
  document.getElementById("raw-stdout").textContent = result.raw_stdout || "(no stdout)";
}

// --------------------------- Exact-GLMM refinement (v0.2.8) -------------

function factorColumnIndices(payload) {
  // For each factor (in order: whole-plot factors, then sub-plot factors),
  // the output column of its main-effect term. Returns null if some factor
  // has no main-effect term, in which case its levels cannot be read back
  // from the model matrix and the refinement is unavailable.
  const nFactors = payload.wp_levels.length + payload.sp_levels.length;
  const cols = [];
  for (let f = 1; f <= nFactors; f++) {
    const idx = payload.model_terms.findIndex((t) => t.length === 1 && t[0] === f);
    if (idx < 0) return null;
    cols.push(idx);
  }
  return cols;
}

function setupRefinePanel() {
  const panel = document.getElementById("refine-panel");
  document.getElementById("refine-result").hidden = true;
  document.getElementById("refine-status").textContent = "";
  if (!pclEcho || pclEcho.eval_method !== "copula_pcl" || !lastPayload) {
    panel.hidden = true;
    return;
  }
  const btn = document.getElementById("refine-btn");
  const costEl = document.getElementById("refine-cost");
  const cols = factorColumnIndices(lastPayload);
  if (!cols) {
    btn.disabled = true;
    costEl.textContent = "Refinement needs a main-effect term for every factor, " +
      "so that the factor levels of each run can be read from the design.";
    costEl.classList.add("refine-warn");
    panel.hidden = false;
    return;
  }
  btn.disabled = false;
  const convS2 = parseFloat(document.getElementById("conv-sigma2").value);
  if (!Number.isNaN(convS2)) document.getElementById("refine-sigma2").value = convS2;

  // The exact GLMM sums over K^n outcomes per whole plot, so its cost grows
  // exponentially in the whole-plot size. Flag configurations where even a
  // single exact restart is likely to be slow.
  const nMax = Math.max(...pclEcho.n_sizes);
  const outcomes = Math.pow(pclEcho.K, nMax);
  const big = outcomes > 10000;
  costEl.textContent =
    `The exact GLMM sums over K^n = ${pclEcho.K}^${nMax} = ` +
    `${outcomes.toLocaleString()} outcomes per whole plot. ` +
    (big
      ? "At this size even one exact restart may take hours or not finish " +
        "within the timeout; the PCL design alone may be the practical choice."
      : "At this size a refinement typically takes minutes.");
  costEl.classList.toggle("refine-warn", big);
  panel.hidden = false;
}

async function onRefine() {
  if (!pclResult || !lastPayload) return;
  hideError();
  const cols = factorColumnIndices(lastPayload);
  if (!cols) return;
  const startDesign = pclResult.design_matrix.map((row) => cols.map((c) => row[c]));
  const sigma2 = parseFloat(document.getElementById("refine-sigma2").value);
  const timeoutSec = parseFloat(document.getElementById("refine-timeout").value) || 3600;
  if (Number.isNaN(sigma2) || sigma2 <= 0) {
    showError("Enter a positive whole-plot variance for the exact GLMM.");
    return;
  }
  const payload = {
    ...lastPayload,
    eval_method: "glmm_exact",
    sigma2_fixed: sigma2,
    num_starts: 1,
    seed: pclEcho.seed,
    timeout_sec: timeoutSec,
    start_design: startDesign,
  };

  const btn = document.getElementById("refine-btn");
  const statusEl = document.getElementById("refine-status");
  btn.disabled = true;
  statusEl.textContent = "Running exact-GLMM refinement…";
  try {
    const resp = await fetch("/api/optimize", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload),
    });
    const data = await resp.json();
    if (!data.ok) {
      showError(data.error || `Server returned HTTP ${resp.status}`);
      return;
    }
    refinedResult = data.result;
    refinedEcho = data.echo;
    const refinedLevels = refinedResult.design_matrix.map((row) => cols.map((c) => row[c]));
    const changed = refinedLevels.filter((row, i) =>
      row.some((v, j) => Math.abs(v - startDesign[i][j]) > 1e-9)).length;
    const crit = refinedResult.d_criterion;
    document.getElementById("refine-summary").textContent =
      `Refinement changed ${changed} of ${startDesign.length} runs in ` +
      `${refinedResult.elapsed_sec.toFixed(1)} s (σ² = ${sigma2}). ` +
      `Exact-GLMM D-criterion of the refined design: ` +
      `${crit === null ? "—" : crit.toFixed(6)}.` +
      (changed === 0
        ? " No exchange improved the exact criterion, so the PCL design is already a local optimum of it."
        : "");
    document.getElementById("refine-result").hidden = false;
    document.querySelector('input[name="design-view"][value="refined"]').checked = true;
    onDesignViewChange();
    statusEl.textContent = "";
  } catch (e) {
    showError(`Network or parse error: ${e.message}`);
  } finally {
    btn.disabled = false;
    if (statusEl.textContent.startsWith("Running")) statusEl.textContent = "";
  }
}

function onDesignViewChange() {
  const view = document.querySelector('input[name="design-view"]:checked').value;
  if (view === "refined" && refinedResult) {
    lastResult = refinedResult;
    lastEcho = refinedEcho;
  } else {
    lastResult = pclResult;
    lastEcho = pclEcho;
  }
  renderMetrics(lastResult, lastEcho);
  rerenderDesignTable();
}

function rerenderDesignTable() {
  // Re-draw the design table based on the current show-interactions
  // toggle state. Called both on initial render and whenever the user
  // flips the checkbox. Uses lastResult / lastEcho cached at submit time.
  if (!lastResult) return;
  const result = lastResult;
  const echo = lastEcho || {};
  const showInteractions = document.getElementById("show-interactions").checked;

  const table = document.getElementById("design-table");
  const thead = table.querySelector("thead");
  const tbody = table.querySelector("tbody");
  thead.innerHTML = "";
  tbody.innerHTML = "";

  if (result.n_rows === 0) {
    tbody.innerHTML = "<tr><td>(empty output)</td></tr>";
    return;
  }

  const visibleCols = visibleColumnIndices(result.n_cols, echo, showInteractions);
  const labels = columnLabels(result.n_cols, echo).filter((_, i) => visibleCols.includes(i));
  const nSizes = (echo && echo.n_sizes) || [];
  const blockForRow = buildBlockLookup(nSizes, result.n_rows);

  const headerRow = document.createElement("tr");
  headerRow.innerHTML = `<th>Run</th><th>Block</th>` +
    labels.map((label) => `<th>${label}</th>`).join("");
  thead.appendChild(headerRow);

  result.design_matrix.forEach((row, rowIdx) => {
    const tr = document.createElement("tr");
    const block = blockForRow[rowIdx];
    const visibleCells = visibleCols.map((ci) => fmtNum(row[ci]));
    tr.innerHTML =
      `<td>${rowIdx + 1}</td>` +
      `<td class="wp-block-col">${block}</td>` +
      visibleCells.map((v) => `<td>${v}</td>`).join("");
    tbody.appendChild(tr);
  });
}

function visibleColumnIndices(ncols, echo, showInteractions) {
  // Return the 0-based column indices to display in the table.
  // - Run-sheet view (default): only main-effect term indices.
  // - Full view (interactions checkbox on): all columns.
  if (showInteractions) {
    return Array.from({ length: ncols }, (_, i) => i);
  }
  const meIdx = (echo && echo.main_effect_term_indices) || [];
  // Defensive fallback: if the server didn't echo main_effect indices
  // (older API), show everything so the user still gets the data.
  if (meIdx.length === 0) {
    return Array.from({ length: ncols }, (_, i) => i);
  }
  return meIdx.filter((i) => i < ncols);
}

function columnLabels(ncols, echo) {
  const echoLabels = (echo && echo.term_labels) || [];
  const labels = echoLabels.slice(0, ncols);
  while (labels.length < ncols) {
    labels.push(`X${labels.length + 1}`);
  }
  return labels;
}

function buildBlockLookup(nSizes, totalRows) {
  // Return an array `block[rowIdx]` giving the 1-indexed block number
  // for each row. Supports both equal-block (n_sizes = [n, n, n, ...])
  // and unequal-block configurations via cumulative-sum lookup.
  const block = new Array(totalRows);
  let blockIdx = 1;
  let remaining = nSizes.length > 0 ? nSizes[0] : totalRows;
  let nextBlockBoundary = nSizes.length > 0 ? nSizes[0] : totalRows;
  for (let r = 0; r < totalRows; r++) {
    if (r >= nextBlockBoundary && blockIdx < nSizes.length) {
      blockIdx++;
      nextBlockBoundary += nSizes[blockIdx - 1];
    }
    block[r] = blockIdx;
  }
  return block;
}

function fmtNum(v) {
  if (v === null || v === undefined || Number.isNaN(v)) return "";
  if (Number.isInteger(v)) return v.toString();
  return Math.abs(v) < 1e-6 ? "0" : v.toFixed(4);
}

function evaluatorDisplayName(evalMethod) {
  // Map the API's eval_method strings to human-readable labels for the UI.
  // The PCL surrogate is explicitly named as such because the number it
  // produces is the log-det of a surrogate information matrix, not of
  // the exact-GLMM Fisher information — formally distinct quantities
  // that happen to agree closely in the paper's operating regime.
  switch (evalMethod) {
    case "copula_pcl":  return "PCL surrogate";
    case "glmm_exact":  return "exact GLMM";
    case "glmm_approx": return "independence GLM";
    default:            return evalMethod || "unknown evaluator";
  }
}

// --------------------------- Downloads ----------------------------------

function downloadRunSheet() {
  // Main-effect columns only (plus Run + Block context columns).
  // This is what the experimenter actually uses to run the experiment.
  if (!lastResult) return;
  const echo = lastEcho || {};
  const meIdx = (echo.main_effect_term_indices && echo.main_effect_term_indices.length > 0)
    ? echo.main_effect_term_indices
    : Array.from({ length: lastResult.n_cols }, (_, i) => i); // fallback: full
  const labels = columnLabels(lastResult.n_cols, echo);
  const visibleLabels = meIdx.map((i) => labels[i]);
  const nSizes = echo.n_sizes || [];
  const blockForRow = buildBlockLookup(nSizes, lastResult.n_rows);

  const header = ["Run", "Block", ...visibleLabels].join(",");
  const rows = lastResult.design_matrix.map((row, rowIdx) => {
    const visible = meIdx.map((ci) => row[ci]);
    return [rowIdx + 1, blockForRow[rowIdx], ...visible].join(",");
  });
  triggerDownload("run_sheet.csv", [header, ...rows].join("\n"), "text/csv");
}

function downloadModelMatrix() {
  // Full expanded model matrix (every term, including interactions).
  // This is the X matrix to plug into a regression.
  if (!lastResult) return;
  const echo = lastEcho || {};
  const labels = columnLabels(lastResult.n_cols, echo);
  const nSizes = echo.n_sizes || [];
  const blockForRow = buildBlockLookup(nSizes, lastResult.n_rows);

  const header = ["Run", "Block", ...labels].join(",");
  const rows = lastResult.design_matrix.map((row, rowIdx) =>
    [rowIdx + 1, blockForRow[rowIdx], ...row].join(",")
  );
  triggerDownload("model_matrix.csv", [header, ...rows].join("\n"), "text/csv");
}

function downloadJSON() {
  if (!lastResult) return;
  const payload = {
    d_criterion: lastResult.d_criterion,
    elapsed_sec: lastResult.elapsed_sec,
    n_rows: lastResult.n_rows,
    n_cols: lastResult.n_cols,
    design_matrix: lastResult.design_matrix,
    echo: lastEcho,
    raw_stdout: lastResult.raw_stdout,
  };
  triggerDownload("pcl_design.json", JSON.stringify(payload, null, 2), "application/json");
}

function triggerDownload(filename, content, mime) {
  const blob = new Blob([content], { type: mime });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = filename;
  document.body.appendChild(a);
  a.click();
  document.body.removeChild(a);
  URL.revokeObjectURL(url);
}

// --------------------------- Errors / state -----------------------------

function showError(msg) {
  document.getElementById("error-message").textContent = msg;
  document.getElementById("error").hidden = false;
  document.getElementById("error").scrollIntoView({ behavior: "smooth", block: "start" });
}
function hideError() {
  document.getElementById("error").hidden = true;
}
function hideResults() {
  document.getElementById("results").hidden = true;
}
