/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: chol.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 03-Oct-2026 17:22:39
 */

/* Include Files */
#include "chol.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xpotrf.h"

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *A
 * Return Type  : void
 */
void chol(emxArray_real_T *A)
{
  double *A_data;
  int i;
  int j;
  int jmax;
  int n;
  jmax = A->size[0];
  n = A->size[1];
  if (jmax <= n) {
    n = jmax;
  }
  if (n != 0) {
    jmax = xpotrf(n, A, A->size[0]);
    A_data = A->data;
    if (jmax == 0) {
      jmax = n;
    } else {
      jmax--;
    }
    for (j = 0; j <= jmax - 2; j++) {
      n = j + 2;
      for (i = n; i <= jmax; i++) {
        A_data[(i + A->size[0] * j) - 1] = 0.0;
      }
    }
  }
}

/*
 * File trailer for chol.c
 *
 * [EOF]
 */
