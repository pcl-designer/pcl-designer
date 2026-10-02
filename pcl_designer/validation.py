"""
Input validation for the /api/optimize endpoint.

Returns a clean dict ready for runner.write_inputs(), or raises ValidationError
with a human-readable message that the frontend can display verbatim.
"""

from __future__ import annotations

import random
from typing import Any


class ValidationError(Exception):
    """Raised when the incoming JSON payload fails validation."""


# Copula families accepted by the optimizer's copula_type key.
COPULA_FAMILIES = {0: "Frank", 1: "Clayton"}

# The Clayton generator raises cumulative probabilities to the power -lambda,
# which overflows for very small probabilities once lambda is large. lambda = 30
# already corresponds to Kendall's tau = 0.9375, far beyond the within-block
# dependence of practical split-plot experiments.
CLAYTON_LAMBDA_MAX = 30.0


def _as_int(value: Any, name: str, minimum: int | None = None) -> int:
    try:
        iv = int(value)
    except (TypeError, ValueError):
        raise ValidationError(f"{name} must be an integer (got: {value!r}).")
    if minimum is not None and iv < minimum:
        raise ValidationError(f"{name} must be at least {minimum} (got: {iv}).")
    return iv


def _as_float(value: Any, name: str, minimum: float | None = None) -> float:
    try:
        fv = float(value)
    except (TypeError, ValueError):
        raise ValidationError(f"{name} must be a number (got: {value!r}).")
    if minimum is not None and fv <= minimum:
        raise ValidationError(f"{name} must be greater than {minimum} (got: {fv}).")
    return fv


def _as_level_set(value: Any, name: str) -> list[float]:
    if not isinstance(value, list):
        raise ValidationError(f"{name} must be a list of numbers (got: {type(value).__name__}).")
    if len(value) < 2:
        raise ValidationError(f"{name} must have at least 2 levels (got: {len(value)}).")
    out = []
    for i, v in enumerate(value):
        try:
            out.append(float(v))
        except (TypeError, ValueError):
            raise ValidationError(f"{name}[{i}] must be a number (got: {v!r}).")
    return out


def _identity_matrix(p: int, scale: float = 1.0) -> list[list[float]]:
    return [[scale if i == j else 0.0 for j in range(p)] for i in range(p)]


# --------------------------------------------------------------------------
# Term-label helpers (single source of truth for column headers in the UI)
# --------------------------------------------------------------------------

def factor_label(idx1: int, num_wp_factors: int, num_sp_factors: int) -> str:
    """
    Map a 1-indexed factor number to its display label.

    The form lists WP factors first, then SP factors, so factor 1 is WP1,
    factor (num_wp + 1) is SP1, etc. If idx1 is out of range, falls back
    to `F{idx1}` so the label is at least unambiguous.
    """
    if idx1 < 1:
        return f"F{idx1}"
    if idx1 <= num_wp_factors:
        return f"WP{idx1}"
    if idx1 <= num_wp_factors + num_sp_factors:
        return f"SP{idx1 - num_wp_factors}"
    return f"F{idx1}"


def model_term_labels(
    model_terms: list[list[int]],
    num_wp_factors: int,
    num_sp_factors: int,
) -> list[str]:
    """
    Compute human-readable labels for every model term, in order.

    Main-effect terms (single factor) → `WP1`, `SP2`, etc.
    Interaction terms (>1 factors) → factors joined by `·`, e.g. `WP1·SP1`.
    Higher-order interactions extend naturally: `WP1·SP1·SP2`.
    """
    labels: list[str] = []
    for term in model_terms:
        parts = [factor_label(i, num_wp_factors, num_sp_factors) for i in term]
        labels.append("·".join(parts))
    return labels


# --------------------------------------------------------------------------
# Public entry point
# --------------------------------------------------------------------------

def validate_payload(payload: dict[str, Any] | None) -> dict[str, Any]:
    """
    Validate and normalize the JSON request body.

    Required keys: m, n, K, wp_levels, sp_levels, model_terms.
    Optional keys with defaults: sigma2_fixed, lambda_fixed, seed,
                                  copula_type, eval_method, crit_mode,
                                  prior_mean, prior_cov, timeout_sec, num_starts.
    """
    if not isinstance(payload, dict):
        raise ValidationError("Request body must be a JSON object.")

    # --- Core dimensions ---
    m = _as_int(payload.get("m"), "m (number of whole plots)", minimum=2)
    K = _as_int(payload.get("K"), "K (ordinal categories)", minimum=2)

    n_raw = payload.get("n")
    if isinstance(n_raw, list):
        if len(n_raw) != m:
            raise ValidationError(
                f"n (block sizes vector) must have length {m} (the value of m); got {len(n_raw)}."
            )
        n_sizes = [_as_int(v, f"n[{i}]", minimum=2) for i, v in enumerate(n_raw)]
    else:
        n_scalar = _as_int(n_raw, "n (block size)", minimum=2)
        n_sizes = [n_scalar] * m

    # --- Factors ---
    wp_levels_raw = payload.get("wp_levels", [])
    sp_levels_raw = payload.get("sp_levels", [])
    if not isinstance(wp_levels_raw, list):
        raise ValidationError("wp_levels must be a list of level-set lists.")
    if not isinstance(sp_levels_raw, list):
        raise ValidationError("sp_levels must be a list of level-set lists.")
    if not wp_levels_raw and not sp_levels_raw:
        raise ValidationError("At least one factor (whole-plot or sub-plot) must be defined.")

    wp_levels = [_as_level_set(ls, f"wp_levels[{i}]") for i, ls in enumerate(wp_levels_raw)]
    sp_levels = [_as_level_set(ls, f"sp_levels[{i}]") for i, ls in enumerate(sp_levels_raw)]
    num_factors = len(wp_levels) + len(sp_levels)

    # --- Model terms ---
    model_terms_raw = payload.get("model_terms")
    if not isinstance(model_terms_raw, list) or not model_terms_raw:
        raise ValidationError("model_terms must be a non-empty list of term lists.")
    model_terms: list[list[int]] = []
    for i, term in enumerate(model_terms_raw):
        if not isinstance(term, list) or not term:
            raise ValidationError(
                f"model_terms[{i}] must be a non-empty list of factor indices (got: {term!r})."
            )
        normalized = []
        for j, idx in enumerate(term):
            iv = _as_int(idx, f"model_terms[{i}][{j}]", minimum=1)
            if iv > num_factors:
                raise ValidationError(
                    f"model_terms[{i}][{j}] = {iv} references factor index outside "
                    f"the declared {num_factors} factors ({len(wp_levels)} WP + {len(sp_levels)} SP)."
                )
            normalized.append(iv)
        model_terms.append(normalized)

    # --- Scalar parameters (with paper defaults) ---
    sigma2_fixed = _as_float(payload.get("sigma2_fixed", 2.5), "sigma2_fixed", minimum=0.0)
    lambda_fixed = _as_float(payload.get("lambda_fixed", 3.0), "lambda_fixed", minimum=0.0)
    seed = _as_int(payload.get("seed", random.randint(1, 2**31 - 1)), "seed", minimum=1)
    copula_type = _as_int(payload.get("copula_type", 0), "copula_type", minimum=0)
    if copula_type not in COPULA_FAMILIES:
        raise ValidationError(
            "copula_type must be 0 (Frank) or 1 (Clayton) "
            f"(got: {copula_type})."
        )

    # --- Method strings (whitelist) ---
    eval_method = str(payload.get("eval_method", "copula_pcl"))
    if eval_method not in {"copula_pcl", "glmm_exact", "glmm_approx"}:
        raise ValidationError(
            f"eval_method must be one of: copula_pcl, glmm_exact, glmm_approx (got: {eval_method!r})."
        )
    if (
        eval_method == "copula_pcl"
        and copula_type == 1
        and lambda_fixed > CLAYTON_LAMBDA_MAX
    ):
        raise ValidationError(
            f"lambda_fixed must be at most {CLAYTON_LAMBDA_MAX:g} for the Clayton copula "
            f"(got: {lambda_fixed:g}). Larger values imply Kendall's tau above "
            f"{CLAYTON_LAMBDA_MAX / (CLAYTON_LAMBDA_MAX + 2):.3f} and overflow the "
            "Clayton generator at small cumulative probabilities."
        )
    crit_mode = str(payload.get("crit_mode", "average"))
    if crit_mode not in {"average", "minimax"}:
        raise ValidationError(
            f"crit_mode must be one of: average, minimax (got: {crit_mode!r})."
        )

    # --- Prior (defaults to baseline used in the paper) ---
    # Dimension: (K - 1) cutpoint params + one slope per model term.
    p_dim = (K - 1) + len(model_terms)

    prior_mean_raw = payload.get("prior_mean")
    if prior_mean_raw is None:
        prior_mean = [0.0] * p_dim
    else:
        if not isinstance(prior_mean_raw, list) or len(prior_mean_raw) != p_dim:
            raise ValidationError(
                f"prior_mean must be a list of length {p_dim} = (K-1) + len(model_terms) "
                f"(got length {len(prior_mean_raw) if isinstance(prior_mean_raw, list) else 'N/A'})."
            )
        prior_mean = [_as_float(v, f"prior_mean[{i}]") for i, v in enumerate(prior_mean_raw)]

    prior_cov_raw = payload.get("prior_cov")
    if prior_cov_raw is None:
        prior_cov = _identity_matrix(p_dim, scale=0.25)
    else:
        if (
            not isinstance(prior_cov_raw, list)
            or len(prior_cov_raw) != p_dim
            or any(not isinstance(r, list) or len(r) != p_dim for r in prior_cov_raw)
        ):
            raise ValidationError(
                f"prior_cov must be a {p_dim}x{p_dim} matrix (nested lists)."
            )
        prior_cov = [
            [_as_float(v, f"prior_cov[{i}][{j}]") for j, v in enumerate(row)]
            for i, row in enumerate(prior_cov_raw)
        ]

    timeout_sec = _as_float(payload.get("timeout_sec", 600.0), "timeout_sec", minimum=0.0)

    num_starts = _as_int(payload.get("num_starts", 15), "num_starts (CE restarts)", minimum=1)
    if num_starts > 1024:
        raise ValidationError("num_starts must be at most 1024.")

    term_labels = model_term_labels(model_terms, len(wp_levels), len(sp_levels))

    # Indices (0-based, into the binary's output columns) of terms that
    # are main effects — i.e., terms involving exactly one factor. These
    # are the columns a practitioner needs to actually run the experiment;
    # the remaining columns (interactions) are derivable products.
    main_effect_term_indices = [
        i for i, term in enumerate(model_terms) if len(term) == 1
    ]

    return {
        "m": m,
        "K": K,
        "n_sizes": n_sizes,
        "wp_levels": wp_levels,
        "sp_levels": sp_levels,
        "model_terms": model_terms,
        "term_labels": term_labels,
        "main_effect_term_indices": main_effect_term_indices,
        "sigma2_fixed": sigma2_fixed,
        "lambda_fixed": lambda_fixed,
        "seed": seed,
        "copula_type": copula_type,
        "copula_family": COPULA_FAMILIES[copula_type],
        "eval_method": eval_method,
        "crit_mode": crit_mode,
        "prior_mean": prior_mean,
        "prior_cov": prior_cov,
        "timeout_sec": timeout_sec,
        "num_starts": num_starts,
        "num_factors": num_factors,
        "num_wp_factors": len(wp_levels),
        "num_sp_factors": len(sp_levels),
        "p_dim": p_dim,
    }
