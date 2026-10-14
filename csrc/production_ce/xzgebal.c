/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xzgebal.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xzgebal.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double A_data[]
 *                int A_size[2]
 *                int *ihi
 *                double scale_data[]
 *                int *scale_size
 * Return Type  : int
 */
int xzgebal(double A_data[], int A_size[2], int *ihi, double scale_data[],
            int *scale_size)
{
  emxArray_real_T b_A_data;
  emxArray_real_T *x;
  double scale;
  double *x_data;
  int b_k;
  int b_loop_ub;
  int exitg5;
  int i;
  int ilo;
  int ix;
  int ix0_tmp;
  int iy;
  int k;
  int kend;
  int loop_ub;
  int n_tmp;
  bool converged;
  bool notdone;
  *scale_size = A_size[0];
  for (i = 0; i < *scale_size; i++) {
    scale_data[i] = 1.0;
  }
  k = 0;
  *ihi = *scale_size;
  notdone = true;
  emxInit_real_T(&x, 2);
  do {
    exitg5 = 0;
    if (notdone) {
      int exitg4;
      notdone = false;
      ix0_tmp = *ihi;
      do {
        exitg4 = 0;
        if (ix0_tmp > 0) {
          bool exitg6;
          converged = false;
          ix = 0;
          exitg6 = false;
          while ((!exitg6) && (ix <= (unsigned char)*ihi - 1)) {
            if ((ix + 1 == ix0_tmp) ||
                (!(A_data[(ix0_tmp + A_size[0] * ix) - 1] != 0.0))) {
              ix++;
            } else {
              converged = true;
              exitg6 = true;
            }
          }
          if (converged) {
            ix0_tmp--;
          } else {
            scale_data[*ihi - 1] = ix0_tmp;
            if (ix0_tmp != *ihi) {
              ix = (ix0_tmp - 1) * *scale_size;
              iy = (*ihi - 1) * *scale_size;
              i = x->size[0] * x->size[1];
              x->size[0] = *scale_size;
              loop_ub = A_size[1];
              x->size[1] = loop_ub;
              emxEnsureCapacity_real_T(x, i);
              x_data = x->data;
              b_loop_ub = A_size[0] * A_size[1];
              for (i = 0; i < b_loop_ub; i++) {
                x_data[i] = A_data[i];
              }
              for (b_k = 0; b_k < *ihi; b_k++) {
                b_loop_ub = ix + b_k;
                scale = x_data[b_loop_ub];
                i = iy + b_k;
                x_data[b_loop_ub] = x_data[i];
                x_data[i] = scale;
              }
              for (b_k = 0; b_k < *scale_size; b_k++) {
                b_loop_ub = b_k * *scale_size;
                n_tmp = (ix0_tmp + b_loop_ub) - 1;
                scale = x_data[n_tmp];
                i = (*ihi + b_loop_ub) - 1;
                x_data[n_tmp] = x_data[i];
                x_data[i] = scale;
              }
              A_size[0] = *scale_size;
              for (i = 0; i < loop_ub; i++) {
                for (kend = 0; kend < *scale_size; kend++) {
                  A_data[kend + A_size[0] * i] = x_data[kend + x->size[0] * i];
                }
              }
            }
            exitg4 = 1;
          }
        } else {
          exitg4 = 2;
        }
      } while (exitg4 == 0);
      if (exitg4 == 1) {
        if (*ihi == 1) {
          ilo = 1;
          *ihi = 1;
          exitg5 = 1;
        } else {
          (*ihi)--;
          notdone = true;
        }
      }
    } else {
      notdone = true;
      while (notdone) {
        bool exitg6;
        notdone = false;
        ix0_tmp = k;
        exitg6 = false;
        while ((!exitg6) && (ix0_tmp + 1 <= *ihi)) {
          bool exitg7;
          converged = false;
          ix = k;
          exitg7 = false;
          while ((!exitg7) && (ix + 1 <= *ihi)) {
            if ((ix + 1 == ix0_tmp + 1) ||
                (!(A_data[ix + A_size[0] * ix0_tmp] != 0.0))) {
              ix++;
            } else {
              converged = true;
              exitg7 = true;
            }
          }
          if (converged) {
            ix0_tmp++;
          } else {
            scale_data[k] = ix0_tmp + 1;
            if (ix0_tmp + 1 != k + 1) {
              ix = ix0_tmp * *scale_size;
              kend = k * *scale_size;
              i = x->size[0] * x->size[1];
              x->size[0] = *scale_size;
              loop_ub = A_size[1];
              x->size[1] = loop_ub;
              emxEnsureCapacity_real_T(x, i);
              x_data = x->data;
              b_loop_ub = A_size[0] * A_size[1];
              for (i = 0; i < b_loop_ub; i++) {
                x_data[i] = A_data[i];
              }
              for (b_k = 0; b_k < *ihi; b_k++) {
                b_loop_ub = ix + b_k;
                scale = x_data[b_loop_ub];
                i = kend + b_k;
                x_data[b_loop_ub] = x_data[i];
                x_data[i] = scale;
              }
              ix = kend + ix0_tmp;
              iy = kend + k;
              kend = *scale_size - k;
              for (b_k = 0; b_k < kend; b_k++) {
                b_loop_ub = b_k * *scale_size;
                n_tmp = ix + b_loop_ub;
                scale = x_data[n_tmp];
                i = iy + b_loop_ub;
                x_data[n_tmp] = x_data[i];
                x_data[i] = scale;
              }
              A_size[0] = *scale_size;
              for (i = 0; i < loop_ub; i++) {
                for (kend = 0; kend < *scale_size; kend++) {
                  A_data[kend + A_size[0] * i] = x_data[kend + x->size[0] * i];
                }
              }
            }
            k++;
            notdone = true;
            exitg6 = true;
          }
        }
      }
      ilo = k + 1;
      converged = false;
      exitg5 = 2;
    }
  } while (exitg5 == 0);
  if (exitg5 != 1) {
    bool exitg3;
    exitg3 = false;
    while ((!exitg3) && (!converged)) {
      int exitg2;
      converged = true;
      ix = k;
      do {
        exitg2 = 0;
        if (ix + 1 <= *ihi) {
          double absxk;
          double c;
          double ca;
          double r;
          double t;
          kend = *ihi - k;
          iy = ix * *scale_size;
          b_A_data.data = &A_data[0];
          b_A_data.size = &A_size[0];
          b_A_data.allocatedSize = -1;
          b_A_data.numDimensions = 2;
          b_A_data.canFreeData = false;
          c = xnrm2(kend, &b_A_data, (iy + k) + 1);
          ix0_tmp = k * *scale_size + ix;
          r = 0.0;
          if (kend >= 1) {
            if (kend == 1) {
              r = fabs(A_data[ix0_tmp]);
            } else {
              scale = 3.3121686421112381E-170;
              kend = (ix0_tmp + (kend - 1) * *scale_size) + 1;
              for (b_k = ix0_tmp + 1;
                   *scale_size < 0 ? b_k >= kend : b_k <= kend;
                   b_k += *scale_size) {
                absxk = fabs(A_data[b_k - 1]);
                if (absxk > scale) {
                  t = scale / absxk;
                  r = r * t * t + 1.0;
                  scale = absxk;
                } else {
                  t = absxk / scale;
                  r += t * t;
                }
              }
              r = scale * sqrt(r);
            }
          }
          if (*ihi < 1) {
            kend = 0;
          } else {
            kend = 1;
            if (*ihi > 1) {
              scale = fabs(A_data[iy]);
              for (b_k = 2; b_k <= *ihi; b_k++) {
                t = fabs(A_data[(iy + b_k) - 1]);
                if (t > scale) {
                  kend = b_k;
                  scale = t;
                }
              }
            }
          }
          ca = fabs(A_data[(kend + A_size[0] * ix) - 1]);
          n_tmp = *scale_size - k;
          if (n_tmp < 1) {
            kend = 0;
          } else {
            kend = 1;
            if (n_tmp > 1) {
              scale = fabs(A_data[ix0_tmp]);
              for (b_k = 2; b_k <= n_tmp; b_k++) {
                t = fabs(A_data[ix0_tmp + (b_k - 1) * *scale_size]);
                if (t > scale) {
                  kend = b_k;
                  scale = t;
                }
              }
            }
          }
          scale = fabs(A_data[ix + A_size[0] * ((kend + k) - 1)]);
          if ((c == 0.0) || (r == 0.0)) {
            ix++;
          } else {
            double f;
            int exitg1;
            absxk = r / 2.0;
            f = 1.0;
            t = c + r;
            do {
              exitg1 = 0;
              if ((c < absxk) &&
                  (fmax(f, fmax(c, ca)) < 4.9896007738368E+291) &&
                  (fmin(r, fmin(absxk, scale)) > 2.0041683600089728E-292)) {
                if (rtIsNaN(((((c + f) + ca) + r) + absxk) + scale)) {
                  exitg1 = 1;
                } else {
                  f *= 2.0;
                  c *= 2.0;
                  ca *= 2.0;
                  r /= 2.0;
                  absxk /= 2.0;
                  scale /= 2.0;
                }
              } else {
                absxk = c / 2.0;
                while ((absxk >= r) &&
                       (fmax(r, scale) < 4.9896007738368E+291) &&
                       (fmin(fmin(f, c), fmin(absxk, ca)) >
                        2.0041683600089728E-292)) {
                  f /= 2.0;
                  c /= 2.0;
                  absxk /= 2.0;
                  ca /= 2.0;
                  r *= 2.0;
                  scale *= 2.0;
                }
                if ((!(c + r >= 0.95 * t)) &&
                    ((!(f < 1.0)) || (!(scale_data[ix] < 1.0)) ||
                     (!(f * scale_data[ix] <= 1.0020841800044864E-292))) &&
                    ((!(f > 1.0)) || (!(scale_data[ix] > 1.0)) ||
                     (!(scale_data[ix] >= 9.9792015476736E+291 / f)))) {
                  scale = 1.0 / f;
                  scale_data[ix] *= f;
                  i = x->size[0] * x->size[1];
                  x->size[0] = *scale_size;
                  loop_ub = A_size[1];
                  x->size[1] = loop_ub;
                  emxEnsureCapacity_real_T(x, i);
                  x_data = x->data;
                  b_loop_ub = A_size[0] * A_size[1];
                  for (i = 0; i < b_loop_ub; i++) {
                    x_data[i] = A_data[i];
                  }
                  kend = ix0_tmp + 1;
                  if (*scale_size >= 1) {
                    i = (ix0_tmp + *scale_size * (n_tmp - 1)) + 1;
                    for (b_k = kend; *scale_size < 0 ? b_k >= i : b_k <= i;
                         b_k += *scale_size) {
                      x_data[b_k - 1] *= scale;
                    }
                  }
                  i = iy + *ihi;
                  for (b_k = iy + 1; b_k <= i; b_k++) {
                    x_data[b_k - 1] *= f;
                  }
                  A_size[0] = *scale_size;
                  for (i = 0; i < loop_ub; i++) {
                    for (kend = 0; kend < *scale_size; kend++) {
                      A_data[kend + A_size[0] * i] =
                          x_data[kend + x->size[0] * i];
                    }
                  }
                  converged = false;
                }
                exitg1 = 2;
              }
            } while (exitg1 == 0);
            if (exitg1 == 1) {
              exitg2 = 2;
            } else {
              ix++;
            }
          }
        } else {
          exitg2 = 1;
        }
      } while (exitg2 == 0);
      if (exitg2 != 1) {
        exitg3 = true;
      }
    }
  }
  emxFree_real_T(&x);
  return ilo;
}

/*
 * File trailer for xzgebal.c
 *
 * [EOF]
 */
