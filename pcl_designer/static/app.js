// PCL Designer — frontend logic (vanilla JS, no build step)

const WP = "wp";
const SP = "sp";
let factorCount = { wp: 0, sp: 0 };
let lastResult = null;
let lastEcho = null;

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

  document.getElementById("design-form").addEventListener("submit", onSubmit);
  document.getElementById("download-runsheet").addEventListener("click", downloadRunSheet);
  document.getElementById("download-model-matrix").addEventListener("click", downloadModelMatrix);
  document.getElementById("download-json").addEventListener("click", downloadJSON);
  document.getElementById("show-interactions").addEventListener("change", rerenderDesignTable);
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
}

function readModelTerms() {
  const text = document.getElementById("model-terms").value.trim();
  if (!text) return [];
  return text.split(/\n+/).map((line) => {
    return line.trim().split(/[\s,]+/).filter((x) => x.length > 0).map(Number);
  });
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
      renderResults(data.result, data.echo, wallSec);
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
  const dCrit = result.d_criterion;
  document.getElementById("d-criterion").textContent =
    dCrit === null ? "—" : dCrit.toFixed(6);
  // Qualify the D-criterion label with the evaluator that actually
  // produced it. When eval_method=copula_pcl, the number is the log-det
  // of the surrogate information matrix — NOT the exact-GLMM D-criterion.
  // The two agree closely under the paper's operating regime but are
  // formally distinct quantities; being explicit prevents confusion.
  document.getElementById("d-criterion-label").textContent =
    `D-criterion (${evaluatorDisplayName(echo && echo.eval_method)})`;
  document.getElementById("elapsed").textContent = `${result.elapsed_sec.toFixed(1)} s`;
  document.getElementById("dimensions").textContent = `${result.n_rows} × ${result.n_cols}`;
  document.getElementById("seed-used").textContent = echo.seed;
  document.getElementById("raw-stdout").textContent = result.raw_stdout || "(no stdout)";

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
