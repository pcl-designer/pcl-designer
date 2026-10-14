/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: norm.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "norm.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *x
 * Return Type  : double
 */
double b_norm(const emxArray_real_T *x)
{
  const double *x_data;
  double y;
  int k;
  x_data = x->data;
  if (x->size[1] == 0) {
    y = 0.0;
  } else {
    y = 0.0;
    if (x->size[1] == 1) {
      y = fabs(x_data[0]);
    } else {
      double scale;
      int kend;
      scale = 3.3121686421112381E-170;
      kend = x->size[1];
      for (k = 0; k < kend; k++) {
        double absxk;
        absxk = fabs(x_data[k]);
        if (absxk > scale) {
          double t;
          t = scale / absxk;
          y = y * t * t + 1.0;
          scale = absxk;
        } else {
          double t;
          t = absxk / scale;
          y += t * t;
        }
      }
      y = scale * sqrt(y);
    }
  }
  return y;
}

/*
 * File trailer for norm.c
 *
 * [EOF]
 */
