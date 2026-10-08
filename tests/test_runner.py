"""Tests for runner.py — CSV writing and output parsing.

These tests do not exercise the C binary itself (which requires a real build
+ libomp on macOS). They cover the I/O layer that surrounds it.
"""

import csv
from pathlib import Path

import pytest

from pcl_designer.runner import (
    write_inputs,
    parse_outputs,
    _DCRIT_RE,
    _pad_rows,
)
from pcl_designer.validation import validate_payload


def _baseline_payload():
    return {
        "m": 10,
        "n": 4,
        "K": 4,
        "wp_levels": [[-1, 1]],
        "sp_levels": [[-1, 1], [-1, 1]],
        "model_terms": [[1], [2], [3], [1, 2], [1, 3], [2, 3]],
        "seed": 8675309,
    }


def test_write_inputs_creates_all_seven_files(tmp_path: Path):
    params = validate_payload(_baseline_payload())
    write_inputs(params, tmp_path)

    expected = [
        "priorMean.csv", "priorCov.csv",
        "wp_levels.csv", "sp_levels.csv",
        "modelTerms.csv", "n_sizes.csv",
        "config.txt",
    ]
    for name in expected:
        assert (tmp_path / name).exists(), f"{name} missing"


def test_n_sizes_csv_is_row_vector(tmp_path: Path):
    params = validate_payload(_baseline_payload())
    write_inputs(params, tmp_path)
    with (tmp_path / "n_sizes.csv").open() as f:
        rows = list(csv.reader(f))
    assert len(rows) == 1
    assert len(rows[0]) == 10
    assert all(int(float(v)) == 4 for v in rows[0])


def test_priorcov_default_is_diagonal(tmp_path: Path):
    params = validate_payload(_baseline_payload())
    write_inputs(params, tmp_path)
    with (tmp_path / "priorCov.csv").open() as f:
        rows = [[float(v) for v in r] for r in csv.reader(f)]
    p = params["p_dim"]
    assert len(rows) == p and len(rows[0]) == p
    for i in range(p):
        for j in range(p):
            expected = 0.25 if i == j else 0.0
            assert rows[i][j] == expected


def test_modelterms_zero_padded_to_uniform_width(tmp_path: Path):
    # Mix of 1-term and 2-term entries should pad the 1-term rows with a trailing 0.
    payload = _baseline_payload() | {"model_terms": [[1], [2, 3]]}
    params = validate_payload(payload)
    write_inputs(params, tmp_path)
    with (tmp_path / "modelTerms.csv").open() as f:
        rows = list(csv.reader(f))
    assert len(rows) == 2
    assert all(len(r) == 2 for r in rows)
    assert int(float(rows[0][1])) == 0  # padding
    assert int(float(rows[1][0])) == 2
    assert int(float(rows[1][1])) == 3


def test_factor_levels_nan_padded(tmp_path: Path):
    # One 2-level WP factor, one 3-level WP factor: shorter row gets empty cell.
    payload = _baseline_payload() | {
        "wp_levels": [[-1, 1], [-1, 0, 1]],
        "sp_levels": [[-1, 1]],
        "model_terms": [[1], [2], [3]],
    }
    params = validate_payload(payload)
    write_inputs(params, tmp_path)
    with (tmp_path / "wp_levels.csv").open() as f:
        rows = list(csv.reader(f))
    assert len(rows) == 2
    assert len(rows[0]) == 3
    assert rows[0][2] == ""  # NaN-padded as empty cell (matches MATLAB writematrix NaN)


def test_config_txt_has_expected_keys(tmp_path: Path):
    params = validate_payload(_baseline_payload())
    write_inputs(params, tmp_path)
    text = (tmp_path / "config.txt").read_text()
    for key in ["m=", "K=", "seed=", "copula_type=",
                "sigma2_fixed=", "lambda_fixed=",
                "evalMethod=", "crit_mode="]:
        assert key in text, f"config.txt missing key {key}"
    assert "evalMethod=copula_pcl" in text  # paper default


def test_dcrit_regex_extracts_value():
    sample = "Complete! Time: 28.5 seconds | D-Criterion: 13.072351\n"
    m = _DCRIT_RE.search(sample)
    assert m is not None
    assert float(m.group(1)) == pytest.approx(13.072351)


def test_dcrit_regex_handles_scientific():
    sample = "...D-Criterion: -1.23e+02 ..."
    m = _DCRIT_RE.search(sample)
    assert m is not None
    assert float(m.group(1)) == pytest.approx(-123.0)


def test_parse_outputs_round_trip(tmp_path: Path):
    # Write a fake OptimalDesign_Output.csv and matching stdout.
    csv_path = tmp_path / "OptimalDesign_Output.csv"
    csv_path.write_text("1,-1,1,-1\n1,1,-1,1\n2,-1,-1,1\n2,1,1,-1\n")
    stdout = "Banner...\nD-Criterion: 9.987654\n"
    parsed = parse_outputs(tmp_path, stdout)
    assert parsed["n_rows"] == 4
    assert parsed["n_cols"] == 4
    assert parsed["d_criterion"] == pytest.approx(9.987654)
    assert parsed["design_matrix"][0] == [1.0, -1.0, 1.0, -1.0]


def test_pad_rows_zero_pad():
    out = _pad_rows([[1], [2, 3]], fill=0)
    assert out == [[1, 0], [2, 3]]


def test_pad_rows_none_pad():
    out = _pad_rows([[1.0], [2.0, 3.0]], fill=None)
    assert out == [[1.0, None], [2.0, 3.0]]


def test_start_design_csv_written_only_when_given(tmp_path: Path):
    params = validate_payload(_baseline_payload())
    write_inputs(params, tmp_path)
    assert not (tmp_path / "start_design.csv").exists()

    rows = []
    for b in range(10):
        for r in range(4):
            rows.append([1.0 if b % 2 else -1.0, -1.0 if r % 2 == 0 else 1.0, 1.0])
    params = validate_payload(_baseline_payload() | {"start_design": rows})
    out = tmp_path / "warm"
    write_inputs(params, out)
    with (out / "start_design.csv").open() as f:
        written = [[float(v) for v in r] for r in csv.reader(f)]
    assert written == rows


def test_config_txt_writes_quadrature(tmp_path: Path):
    payload = _baseline_payload()
    payload["quadrature"] = "gjs"
    params = validate_payload(payload)
    write_inputs(params, tmp_path)
    text = (tmp_path / "config.txt").read_text()
    assert "quadrature=gjs" in text
    assert "gjs_radii=2" in text and "gjs_rotations=1" in text
