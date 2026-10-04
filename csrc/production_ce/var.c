/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: var.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 03-Oct-2026 17:22:39
 */

/* Include Files */
#include "var.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "blockedSummation.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double var(const emxArray_real_T *x)
{
  const double *x_data;
  double y;
  int k;
  int n;
  x_data = x->data;
  n = x->size[0];
  if (x->size[0] == 0) {
    y = rtNaN;
  } else if (x->size[0] == 1) {
    if ((!rtIsInf(x_data[0])) && (!rtIsNaN(x_data[0]))) {
      y = 0.0;
    } else {
      y = rtNaN;
    }
  } else {
    double xbar;
    xbar = blockedSummation(x, x->size[0]) / (double)x->size[0];
    y = 0.0;
    for (k = 0; k < n; k++) {
      double t;
      t = x_data[k] - xbar;
      y += t * t;
    }
    y /= (double)x->size[0] - 1.0;
  }
  return y;
}

/*
 * File trailer for var.c
 *
 * [EOF]
 */
