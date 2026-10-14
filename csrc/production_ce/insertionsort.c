/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: insertionsort.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "insertionsort.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : double x_data[]
 *                int xend
 * Return Type  : void
 */
void insertionsort(double x_data[], int xend)
{
  int k;
  for (k = 2; k <= xend; k++) {
    double xc;
    int idx;
    bool exitg1;
    xc = x_data[k - 1];
    idx = k - 1;
    exitg1 = false;
    while ((!exitg1) && (idx >= 1)) {
      double d;
      d = x_data[idx - 1];
      if (xc < d) {
        x_data[idx] = d;
        idx--;
      } else {
        exitg1 = true;
      }
    }
    x_data[idx] = xc;
  }
}

/*
 * File trailer for insertionsort.c
 *
 * [EOF]
 */
