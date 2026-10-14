/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xzlangeM.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xzlangeM.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double xzlangeM(const emxArray_real_T *x)
{
  const double *x_data;
  double y;
  bool b;
  bool b1;
  x_data = x->data;
  y = 0.0;
  b = (x->size[0] == 0);
  b1 = (x->size[1] == 0);
  if ((!b) && (!b1)) {
    int k;
    bool exitg1;
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k <= x->size[0] * x->size[1] - 1)) {
      double absxk;
      absxk = fabs(x_data[k]);
      if (rtIsNaN(absxk)) {
        y = rtNaN;
        exitg1 = true;
      } else {
        if (absxk > y) {
          y = absxk;
        }
        k++;
      }
    }
  }
  return y;
}

/*
 * File trailer for xzlangeM.c
 *
 * [EOF]
 */
