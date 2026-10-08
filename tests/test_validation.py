"""Smoke tests for validation.py — bounds, types, sensible defaults."""

import pytest

from pcl_designer.validation import (
    factor_label,
    model_term_labels,
    validate_payload,
    ValidationError,
)


def _baseline_payload():
    """Minimal valid payload mirroring the paper's baseline configuration."""
    return {
        "m": 10,
        "n": 4,
        "K": 4,
        "wp_levels": [[-1, 1]],
        "sp_levels": [[-1, 1], [-1, 1]],
        "model_terms": [[1], [2], [3], [1, 2], [1, 3], [2, 3]],
    }


def test_baseline_payload_validates():
    p = validate_payload(_baseline_payload())
    assert p["m"] == 10
    assert p["K"] == 4
    assert p["n_sizes"] == [4] * 10
    assert p["num_factors"] == 3
    # Defaults
    assert p["sigma2_fixed"] == 2.5
    assert p["lambda_fixed"] == 3.0
    assert p["eval_method"] == "copula_pcl"
    assert p["crit_mode"] == "average"
    assert p["copula_type"] == 0
    # Prior auto-sized: (K-1) + len(model_terms) = 3 + 6 = 9
    assert p["p_dim"] == 9
    assert len(p["prior_mean"]) == 9
    assert len(p["prior_cov"]) == 9
    assert len(p["prior_cov"][0]) == 9
    # Default prior is mu=0, Sigma=0.25*I
    assert all(v == 0.0 for v in p["prior_mean"])
    assert p["prior_cov"][0][0] == 0.25
    assert p["prior_cov"][0][1] == 0.0


def test_m_must_be_at_least_2():
    payload = _baseline_payload() | {"m": 1}
    with pytest.raises(ValidationError, match="at least 2"):
        validate_payload(payload)


def test_n_vector_must_match_m_length():
    payload = _baseline_payload() | {"n": [4, 4, 4]}
    with pytest.raises(ValidationError, match="length 10"):
        validate_payload(payload)


def test_unequal_block_sizes_via_vector():
    payload = _baseline_payload() | {"m": 3, "n": [4, 5, 6]}
    p = validate_payload(payload)
    assert p["n_sizes"] == [4, 5, 6]


def test_unequal_block_sizes_with_practical_values():
    """Realistic case: 4 whole plots with two big and two small blocks."""
    payload = _baseline_payload() | {"m": 4, "n": [4, 4, 6, 6]}
    p = validate_payload(payload)
    assert p["n_sizes"] == [4, 4, 6, 6]
    # Total runs in the design = sum(n_sizes), not m*n_scalar.
    assert sum(p["n_sizes"]) == 20


def test_n_vector_length_mismatch_message_is_clear():
    """The error should name both expected and actual lengths."""
    payload = _baseline_payload() | {"m": 5, "n": [4, 4, 4]}
    with pytest.raises(ValidationError) as exc:
        validate_payload(payload)
    msg = str(exc.value)
    assert "length 5" in msg
    assert "3" in msg  # the actual length, somewhere in the message


def test_at_least_one_factor_required():
    payload = _baseline_payload() | {"wp_levels": [], "sp_levels": []}
    with pytest.raises(ValidationError, match="At least one factor"):
        validate_payload(payload)


def test_factor_levels_must_have_at_least_2():
    payload = _baseline_payload() | {"sp_levels": [[1]]}
    with pytest.raises(ValidationError, match="at least 2 levels"):
        validate_payload(payload)


def test_model_term_factor_index_out_of_range():
    payload = _baseline_payload() | {"model_terms": [[5]]}
    with pytest.raises(ValidationError, match="outside the declared"):
        validate_payload(payload)


def test_invalid_eval_method_rejected():
    payload = _baseline_payload() | {"eval_method": "made_up"}
    with pytest.raises(ValidationError, match="eval_method must be"):
        validate_payload(payload)


def test_custom_seed_round_trips():
    payload = _baseline_payload() | {"seed": 8675309}
    p = validate_payload(payload)
    assert p["seed"] == 8675309


def test_empty_payload_rejected():
    with pytest.raises(ValidationError):
        validate_payload(None)
    with pytest.raises(ValidationError):
        validate_payload([])  # not a dict


# --------------------------------------------------------------------------
# Term-label helpers
# --------------------------------------------------------------------------

class TestFactorLabel:
    def test_first_wp_factor(self):
        assert factor_label(1, num_wp_factors=1, num_sp_factors=2) == "WP1"

    def test_second_wp_factor(self):
        assert factor_label(2, num_wp_factors=2, num_sp_factors=1) == "WP2"

    def test_first_sp_factor_when_one_wp(self):
        assert factor_label(2, num_wp_factors=1, num_sp_factors=2) == "SP1"

    def test_first_sp_factor_when_no_wp(self):
        assert factor_label(1, num_wp_factors=0, num_sp_factors=3) == "SP1"

    def test_second_sp_factor(self):
        assert factor_label(3, num_wp_factors=1, num_sp_factors=2) == "SP2"

    def test_only_wp_factors(self):
        # No SP factors → factor 3 is WP3 (not falling through).
        assert factor_label(3, num_wp_factors=3, num_sp_factors=0) == "WP3"

    def test_out_of_range_index(self):
        # Defensive fallback: index past the declared factor count.
        assert factor_label(99, num_wp_factors=1, num_sp_factors=2) == "F99"

    def test_zero_or_negative_index(self):
        # Defensive fallback: invalid 1-indexed value.
        assert factor_label(0, num_wp_factors=1, num_sp_factors=2) == "F0"
        assert factor_label(-1, num_wp_factors=1, num_sp_factors=2) == "F-1"


class TestModelTermLabels:
    def test_baseline_me_plus_2fi(self):
        """3 factors (1 WP + 2 SP), main effects plus all 2FIs."""
        terms = [[1], [2], [3], [1, 2], [1, 3], [2, 3]]
        labels = model_term_labels(terms, num_wp_factors=1, num_sp_factors=2)
        assert labels == [
            "WP1", "SP1", "SP2",
            "WP1·SP1", "WP1·SP2", "SP1·SP2",
        ]

    def test_main_effects_only(self):
        terms = [[1], [2], [3]]
        labels = model_term_labels(terms, num_wp_factors=1, num_sp_factors=2)
        assert labels == ["WP1", "SP1", "SP2"]

    def test_three_way_interaction(self):
        terms = [[1, 2, 3]]
        labels = model_term_labels(terms, num_wp_factors=1, num_sp_factors=2)
        assert labels == ["WP1·SP1·SP2"]

    def test_empty_terms_list(self):
        assert model_term_labels([], num_wp_factors=1, num_sp_factors=2) == []

    def test_all_wp_no_sp(self):
        terms = [[1], [2], [1, 2]]
        labels = model_term_labels(terms, num_wp_factors=2, num_sp_factors=0)
        assert labels == ["WP1", "WP2", "WP1·WP2"]

    def test_all_sp_no_wp(self):
        terms = [[1], [2], [1, 2]]
        labels = model_term_labels(terms, num_wp_factors=0, num_sp_factors=2)
        assert labels == ["SP1", "SP2", "SP1·SP2"]

    def test_term_order_is_preserved(self):
        """The output order must match the input order — column k ↔ term k."""
        terms = [[2, 3], [1], [1, 2]]
        labels = model_term_labels(terms, num_wp_factors=1, num_sp_factors=2)
        assert labels == ["SP1·SP2", "WP1", "WP1·SP1"]

    def test_validate_payload_includes_term_labels(self):
        """End-to-end: validate_payload echoes term_labels in the right shape."""
        payload = {
            "m": 10, "n": 4, "K": 4,
            "wp_levels": [[-1, 1]],
            "sp_levels": [[-1, 1], [-1, 1]],
            "model_terms": [[1], [2], [3], [1, 2], [1, 3], [2, 3]],
        }
        result = validate_payload(payload)
        assert result["term_labels"] == [
            "WP1", "SP1", "SP2",
            "WP1·SP1", "WP1·SP2", "SP1·SP2",
        ]
        assert result["num_wp_factors"] == 1
        assert result["num_sp_factors"] == 2


class TestMainEffectTermIndices:
    """The Run Sheet view in the UI shows only main-effect columns.
    The backend identifies them by term length == 1 and echoes the
    indices to the frontend."""

    def _base(self, terms):
        return {
            "m": 10, "n": 4, "K": 4,
            "wp_levels": [[-1, 1]],
            "sp_levels": [[-1, 1], [-1, 1]],
            "model_terms": terms,
        }

    def test_main_effects_only(self):
        result = validate_payload(self._base([[1], [2], [3]]))
        assert result["main_effect_term_indices"] == [0, 1, 2]

    def test_main_plus_2fi(self):
        """Standard ME+2FI: indices 0..2 are mains, 3..5 are interactions."""
        terms = [[1], [2], [3], [1, 2], [1, 3], [2, 3]]
        result = validate_payload(self._base(terms))
        assert result["main_effect_term_indices"] == [0, 1, 2]

    def test_interactions_only(self):
        """Edge case: no main effects in the model at all."""
        terms = [[1, 2], [1, 3], [2, 3]]
        result = validate_payload(self._base(terms))
        assert result["main_effect_term_indices"] == []

    def test_main_effects_interleaved(self):
        """Mains and interactions arbitrarily interleaved — order preserved."""
        terms = [[1, 2], [1], [2, 3], [2], [1, 2, 3], [3]]
        result = validate_payload(self._base(terms))
        # Mains are at positions 1, 3, 5
        assert result["main_effect_term_indices"] == [1, 3, 5]

    def test_three_way_not_a_main_effect(self):
        """Three-way interactions have length 3, not 1, so they're NOT mains."""
        terms = [[1], [1, 2, 3]]
        result = validate_payload(self._base(terms))
        assert result["main_effect_term_indices"] == [0]


def test_clayton_copula_accepted():
    payload = _baseline_payload()
    payload["copula_type"] = 1
    payload["lambda_fixed"] = 0.79
    p = validate_payload(payload)
    assert p["copula_type"] == 1
    assert p["copula_family"] == "Clayton"


def test_default_copula_family_is_frank():
    assert validate_payload(_baseline_payload())["copula_family"] == "Frank"


def test_unknown_copula_type_rejected():
    payload = _baseline_payload()
    payload["copula_type"] = 2
    with pytest.raises(ValidationError, match="copula_type"):
        validate_payload(payload)


def test_clayton_lambda_upper_bound():
    payload = _baseline_payload()
    payload["copula_type"] = 1
    payload["lambda_fixed"] = 30.0
    validate_payload(payload)  # boundary is allowed
    payload["lambda_fixed"] = 31.0
    with pytest.raises(ValidationError, match="Clayton"):
        validate_payload(payload)


def test_clayton_bound_ignored_when_lambda_unused():
    # The exact GLMM never reads lambda, so the Clayton bound must not block it.
    payload = _baseline_payload()
    payload["copula_type"] = 1
    payload["lambda_fixed"] = 100.0
    payload["eval_method"] = "glmm_exact"
    validate_payload(payload)


def test_lambda_must_be_positive():
    payload = _baseline_payload()
    payload["lambda_fixed"] = 0.0
    with pytest.raises(ValidationError, match="lambda_fixed"):
        validate_payload(payload)


# --------------------------------------------------------------------------
# Warm start (v0.2.8)
# --------------------------------------------------------------------------

def _start_design(m=10, n=4):
    """A valid start design for the baseline payload: 1 WP + 2 SP factors."""
    rows = []
    for b in range(m):
        wp = -1.0 if b % 2 == 0 else 1.0
        for r in range(n):
            rows.append([wp, -1.0 if r % 2 == 0 else 1.0, -1.0 if r < 2 else 1.0])
    return rows


def test_start_design_defaults_to_none():
    assert validate_payload(_baseline_payload())["start_design"] is None


def test_valid_start_design_is_accepted():
    p = validate_payload(_baseline_payload() | {"start_design": _start_design()})
    assert len(p["start_design"]) == 40
    assert p["start_design"][0] == [-1.0, -1.0, -1.0]


def test_start_design_wrong_row_count_rejected():
    with pytest.raises(ValidationError, match="one row per run"):
        validate_payload(_baseline_payload() | {"start_design": _start_design()[:39]})


def test_start_design_wrong_column_count_rejected():
    sd = [row[:2] for row in _start_design()]
    with pytest.raises(ValidationError, match="one level per factor"):
        validate_payload(_baseline_payload() | {"start_design": sd})


def test_start_design_level_not_in_level_set_rejected():
    sd = _start_design()
    sd[5][2] = 0.0  # factor 3 levels are {-1, 1}
    with pytest.raises(ValidationError, match="is not a level"):
        validate_payload(_baseline_payload() | {"start_design": sd})


def test_start_design_wp_level_must_be_constant_within_whole_plot():
    sd = _start_design()
    sd[1][0] = -sd[1][0]  # change the WP level of one run in whole plot 1
    with pytest.raises(ValidationError, match="constant within"):
        validate_payload(_baseline_payload() | {"start_design": sd})


# --- v0.2.9: prior integration rule -------------------------------------

def test_quadrature_defaults_to_axial():
    p = validate_payload(_baseline_payload())
    assert p["quadrature"] == "axial"
    assert p["quad_nodes"] == 2 * p["p_dim"]


def test_gjs_quadrature_node_count():
    payload = _baseline_payload()
    payload["quadrature"] = "GJS"
    p = validate_payload(payload)
    assert p["quadrature"] == "gjs"
    assert p["gjs_radii"] == 2 and p["gjs_rotations"] == 1
    # p = 9: 1 + 2 * (p+1)(p+2) = 221
    assert p["quad_nodes"] == 221


def test_unknown_quadrature_rejected():
    payload = _baseline_payload()
    payload["quadrature"] = "sparse"
    with pytest.raises(ValidationError):
        validate_payload(payload)


def test_gjs_radii_bounds():
    payload = _baseline_payload()
    payload["quadrature"] = "gjs"
    payload["gjs_radii"] = 10
    with pytest.raises(ValidationError):
        validate_payload(payload)
