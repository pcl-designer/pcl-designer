"""
Whole-plot variance to copula dependence conversion.

Implements the calibration recipe of Section 5.1 of the accompanying paper.
Given a GLMM whole-plot variance sigma^2, the value a GLMM user typically has
in hand, it returns the copula dependence parameter lambda used by the PCL
surrogate, for either supported copula family.

Recipe
  1. Latent-scale intra-block correlation under the logistic GLMM
     (Hedeker and Gibbons, 1994), rho = sigma^2 / (sigma^2 + pi^2/3).
  2. Kendall's tau under the elliptical relation (Joe, 2014),
     tau = (2/pi) * asin(rho).
  3. The copula parameter whose Kendall's tau equals tau (Joe, 2014).
       Frank    tau(lambda) = 1 - (4/lambda) * (1 - D1(lambda)), with D1 the
                first Debye function, inverted numerically (tau is strictly
                increasing in lambda, so the root is unique).
       Clayton  tau(lambda) = lambda / (lambda + 2), so lambda = 2 tau / (1 - tau).

The Frank inversion mirrors the stand-alone C tool sigma2lambda.c (composite
64-point Gauss-Legendre quadrature for D1 and bisection for the root), and the
browser UI uses a line-for-line JavaScript port (static/app.js). The three
implementations agree to well below display precision.

Usage from the command line
    python -m pcl_designer.dependence 2.5
    python -m pcl_designer.dependence --family clayton 0.5 1 2.5 5
"""

from __future__ import annotations

import math

FRANK = 0
CLAYTON = 1
FAMILY_NAMES = {FRANK: "Frank", CLAYTON: "Clayton"}

# 64-point Gauss-Legendre nodes and weights on [-1, 1] (positive half).
_GL_X = (
    0.0243502926634244, 0.0729931217877990, 0.1214628192961206,
    0.1696444204239928, 0.2174236437400071, 0.2646871622087674,
    0.3113228719902110, 0.3572201583376681, 0.4022701579639916,
    0.4463660172534641, 0.4894031457070530, 0.5312794640198946,
    0.5718956462026340, 0.6111553551723933, 0.6489654712546573,
    0.6852363130542333, 0.7198818501716109, 0.7528199072605319,
    0.7839723589433414, 0.8132653151227975, 0.8406292962525803,
    0.8659993981540928, 0.8893154459951141, 0.9105221370785028,
    0.9295691721319396, 0.9464113748584028, 0.9610087996520538,
    0.9733268277899110, 0.9833362538846260, 0.9910133714767443,
    0.9963401167719553, 0.9993050417357722,
)
_GL_W = (
    0.0486909570091397, 0.0485754674415034, 0.0483447622348030,
    0.0479993885964583, 0.0475401657148303, 0.0469681828162100,
    0.0462847965813144, 0.0454916279274181, 0.0445905581637566,
    0.0435837245293235, 0.0424735151236536, 0.0412625632426235,
    0.0399537411327203, 0.0385501531786156, 0.0370551285402400,
    0.0354722132568824, 0.0338051618371416, 0.0320579283548516,
    0.0302346570724025, 0.0283396726142595, 0.0263774697150547,
    0.0243527025687109, 0.0222701738083833, 0.0201348231535302,
    0.0179517157756973, 0.0157260304760247, 0.0134630478967186,
    0.0111681394601311, 0.0088467598263639, 0.0065044579689784,
    0.0041470332605625, 0.0017832807216964,
)


def _debye_integrand(t: float) -> float:
    if t < 1e-12:
        return 1.0 - t / 2.0
    return t / math.expm1(t)


def debye1(x: float) -> float:
    """First Debye function D1(x) = (1/x) * integral_0^x t/(e^t - 1) dt, x > 0."""
    npanel = max(1, math.ceil(x))
    h = x / npanel
    total = 0.0
    for p in range(npanel):
        c = p * h + h / 2.0
        s = h / 2.0
        for xi, wi in zip(_GL_X, _GL_W):
            total += wi * s * (_debye_integrand(c - s * xi) + _debye_integrand(c + s * xi))
    return total / x


def frank_tau(lam: float) -> float:
    """Kendall's tau of the Frank copula with parameter lam > 0."""
    if lam <= 0.0:
        return 0.0
    return 1.0 - (4.0 / lam) * (1.0 - debye1(lam))


def clayton_tau(lam: float) -> float:
    """Kendall's tau of the Clayton copula with parameter lam > 0."""
    if lam <= 0.0:
        return 0.0
    return lam / (lam + 2.0)


def copula_tau(lam: float, family: int = FRANK) -> float:
    """Kendall's tau implied by parameter lam for the given family."""
    return clayton_tau(lam) if family == CLAYTON else frank_tau(lam)


def _invert_frank_tau(tau: float) -> float:
    lo, hi = 1e-10, 1.0
    while frank_tau(hi) < tau:
        hi *= 2.0
        if hi > 1e6:
            raise ValueError(f"Kendall's tau {tau:.6f} is outside the invertible range.")
    for _ in range(200):
        mid = 0.5 * (lo + hi)
        if frank_tau(mid) < tau:
            lo = mid
        else:
            hi = mid
        if hi - lo < 1e-13 * (1.0 + hi):
            break
    return 0.5 * (lo + hi)


def tau_to_lambda(tau: float, family: int = FRANK) -> float:
    """Copula parameter whose Kendall's tau equals tau, for 0 <= tau < 1."""
    if not 0.0 <= tau < 1.0:
        raise ValueError(f"Kendall's tau must lie in [0, 1) (got {tau}).")
    if tau == 0.0:
        return 0.0
    if family == CLAYTON:
        return 2.0 * tau / (1.0 - tau)
    return _invert_frank_tau(tau)


def sigma2_to_lambda(sigma2: float, family: int = FRANK) -> dict:
    """
    Convert a GLMM whole-plot variance to the copula parameter.

    Returns a dict with keys sigma2, rho, tau, lambda and family. sigma2 = 0
    returns lambda = 0 (independence), which the optimizer itself does not
    accept, since it requires lambda > 0.
    """
    if not (isinstance(sigma2, (int, float)) and math.isfinite(sigma2) and sigma2 >= 0.0):
        raise ValueError(f"sigma2 must be a finite nonnegative number (got {sigma2!r}).")
    if family not in FAMILY_NAMES:
        raise ValueError(f"family must be {FRANK} (Frank) or {CLAYTON} (Clayton) (got {family!r}).")
    rho = sigma2 / (sigma2 + math.pi ** 2 / 3.0)
    tau = (2.0 / math.pi) * math.asin(rho)
    lam = tau_to_lambda(tau, family)
    return {"sigma2": float(sigma2), "rho": rho, "tau": tau, "lambda": lam,
            "family": FAMILY_NAMES[family]}


def _main(argv: list[str] | None = None) -> int:
    import argparse

    ap = argparse.ArgumentParser(
        prog="python -m pcl_designer.dependence",
        description="Convert GLMM whole-plot variance(s) sigma^2 to the copula parameter lambda.",
    )
    ap.add_argument("sigma2", type=float, nargs="+", help="whole-plot variance(s), >= 0")
    ap.add_argument("--family", choices=["frank", "clayton"], default="frank")
    args = ap.parse_args(argv)
    fam = CLAYTON if args.family == "clayton" else FRANK
    print(f"{'sigma2':>12} {'rho_latent':>12} {'kendall_tau':>12} {'lambda':>12}   ({FAMILY_NAMES[fam]})")
    for s2 in args.sigma2:
        r = sigma2_to_lambda(s2, fam)
        print(f"{r['sigma2']:12.6f} {r['rho']:12.6f} {r['tau']:12.6f} {r['lambda']:12.6f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(_main())
