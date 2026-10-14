"""Tests for dependence.py, the Section 5.1 sigma^2 -> lambda calibration."""

import math

import pytest

from pcl_designer.dependence import (
    CLAYTON,
    FRANK,
    clayton_tau,
    debye1,
    frank_tau,
    sigma2_to_lambda,
    tau_to_lambda,
)


def test_paper_anchor_frank():
    # Section 5.1: sigma^2 = 2.5 gives rho = 0.4318, tau = 0.2842, lambda = 2.7411.
    r = sigma2_to_lambda(2.5, FRANK)
    assert r["rho"] == pytest.approx(0.431789, abs=1e-6)
    assert r["tau"] == pytest.approx(0.284235, abs=1e-6)
    assert r["lambda"] == pytest.approx(2.741078, abs=1e-6)


def test_paper_anchor_clayton():
    r = sigma2_to_lambda(2.5, CLAYTON)
    assert r["lambda"] == pytest.approx(2 * r["tau"] / (1 - r["tau"]), rel=1e-12)
    assert r["lambda"] == pytest.approx(0.794212, abs=1e-6)


@pytest.mark.parametrize("family, tau_fn", [(FRANK, frank_tau), (CLAYTON, clayton_tau)])
def test_round_trip(family, tau_fn):
    for tau in [0.01, 0.1, 0.25, 0.5, 0.75, 0.9]:
        assert tau_fn(tau_to_lambda(tau, family)) == pytest.approx(tau, abs=1e-10)


def test_debye1_known_values():
    # D1(x) -> 1 - x/4 as x -> 0, and D1(x) -> pi^2 / (6 x) as x -> infinity.
    assert debye1(1e-4) == pytest.approx(1 - 1e-4 / 4, abs=1e-9)
    assert debye1(60.0) == pytest.approx(math.pi ** 2 / 360.0, rel=1e-9)
    # Reference value D1(1) = 0.777504634112248...
    assert debye1(1.0) == pytest.approx(0.7775046341122482, abs=1e-12)


def test_monotone_in_sigma2():
    for fam in (FRANK, CLAYTON):
        lams = [sigma2_to_lambda(s, fam)["lambda"] for s in (0.1, 0.5, 1, 2.5, 5, 10, 50)]
        assert all(a < b for a, b in zip(lams, lams[1:]))


def test_zero_variance_is_independence():
    assert sigma2_to_lambda(0.0)["lambda"] == 0.0


@pytest.mark.parametrize("bad", [-1.0, float("nan"), float("inf"), "2.5"])
def test_rejects_bad_sigma2(bad):
    with pytest.raises(ValueError):
        sigma2_to_lambda(bad)


def test_rejects_unknown_family():
    with pytest.raises(ValueError):
        sigma2_to_lambda(2.5, family=2)
