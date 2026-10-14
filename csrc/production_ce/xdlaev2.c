/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xdlaev2.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xdlaev2.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double a
 *                double b
 *                double c
 *                double *rt2
 * Return Type  : double
 */
double xdlaev2(double a, double b, double c, double *rt2)
{
  double ab;
  double acmn;
  double acmx;
  double adf;
  double rt1;
  double sm;
  sm = a + c;
  adf = fabs(a - c);
  ab = fabs(b + b);
  if (fabs(a) > fabs(c)) {
    acmx = a;
    acmn = c;
  } else {
    acmx = c;
    acmn = a;
  }
  if (adf > ab) {
    double b_a;
    b_a = ab / adf;
    adf *= sqrt(b_a * b_a + 1.0);
  } else if (adf < ab) {
    double b_a;
    b_a = adf / ab;
    adf = ab * sqrt(b_a * b_a + 1.0);
  } else {
    adf = ab * 1.4142135623730951;
  }
  if (sm < 0.0) {
    rt1 = 0.5 * (sm - adf);
    *rt2 = acmx / rt1 * acmn - b / rt1 * b;
  } else if (sm > 0.0) {
    rt1 = 0.5 * (sm + adf);
    *rt2 = acmx / rt1 * acmn - b / rt1 * b;
  } else {
    rt1 = 0.5 * adf;
    *rt2 = -0.5 * adf;
  }
  return rt1;
}

/*
 * File trailer for xdlaev2.c
 *
 * [EOF]
 */
