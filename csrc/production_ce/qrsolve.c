/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: qrsolve.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "qrsolve.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *A
 * Return Type  : int
 */
int rankFromQR(const emxArray_real_T *A)
{
  const double *A_data;
  int maxmn;
  int minmn;
  int r;
  A_data = A->data;
  r = 0;
  if (A->size[0] < A->size[1]) {
    minmn = A->size[0];
    maxmn = A->size[1];
  } else {
    minmn = A->size[1];
    maxmn = A->size[0];
  }
  if (minmn > 0) {
    double tol;
    tol = fmin(1.4901161193847656E-8, 2.2204460492503131E-15 * (double)maxmn) *
          fabs(A_data[0]);
    while ((r < minmn) && (!(fabs(A_data[r + A->size[0] * r]) <= tol))) {
      r++;
    }
  }
  return r;
}

/*
 * File trailer for qrsolve.c
 *
 * [EOF]
 */
