/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xaxpy.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xaxpy.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : int n
 *                double a
 *                int ix0
 *                emxArray_real_T *y
 *                int iy0
 * Return Type  : void
 */
void b_xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0)
{
  double *y_data;
  int k;
  y_data = y->data;
  if ((n >= 1) && (!(a == 0.0))) {
    int i;
    i = n - 1;
    for (k = 0; k <= i; k++) {
      int i1;
      i1 = (iy0 + k) - 1;
      y_data[i1] += a * y_data[(ix0 + k) - 1];
    }
  }
}

/*
 * Arguments    : int n
 *                double a
 *                const emxArray_real_T *x
 *                int ix0
 *                emxArray_real_T *y
 *                int iy0
 * Return Type  : void
 */
void xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
           emxArray_real_T *y, int iy0)
{
  const double *x_data;
  double *y_data;
  int k;
  y_data = y->data;
  x_data = x->data;
  if ((n >= 1) && (!(a == 0.0))) {
    int i;
    i = n - 1;
    for (k = 0; k <= i; k++) {
      int i1;
      i1 = (iy0 + k) - 1;
      y_data[i1] += a * x_data[(ix0 + k) - 1];
    }
  }
}

/*
 * File trailer for xaxpy.c
 *
 * [EOF]
 */
