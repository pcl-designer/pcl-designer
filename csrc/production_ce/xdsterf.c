/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xdsterf.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xdsterf.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_rtwutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "insertionsort.h"
#include "rt_nonfinite.h"
#include "xdlaev2.h"
#include "xzlascl.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double d_data[]
 *                int *d_size
 *                double e_data[]
 *                int e_size
 * Return Type  : int
 */
int xdsterf(double d_data[], int *d_size, double e_data[], int e_size)
{
  emxArray_real_T *r;
  double d;
  double *r1;
  int b_i;
  int i;
  int info;
  int n;
  info = 0;
  n = *d_size;
  emxInit_real_T(&r, 1);
  if (n > 1) {
    int jtot;
    int l1;
    int nmaxit;
    nmaxit = n * 30;
    jtot = 0;
    l1 = 1;
    int exitg1;
    do {
      exitg1 = 0;
      if (l1 > n) {
        if (*d_size > 1) {
          signed char stack_idx_1;
          stack_idx_1 = (signed char)*d_size;
          if (stack_idx_1 - 1 > 0) {
            insertionsort(d_data, stack_idx_1);
          }
        }
        exitg1 = 1;
      } else {
        int l;
        int lend;
        int lendsv_tmp;
        int lsv;
        int m;
        bool exitg2;
        if (l1 > 1) {
          e_data[l1 - 2] = 0.0;
        }
        m = l1;
        exitg2 = false;
        while ((!exitg2) && (m < n)) {
          if (fabs(e_data[m - 1]) <= sqrt(fabs(d_data[m - 1])) *
                                         sqrt(fabs(d_data[m])) *
                                         2.2204460492503131E-16) {
            e_data[m - 1] = 0.0;
            exitg2 = true;
          } else {
            m++;
          }
        }
        l = l1;
        lsv = l1;
        lend = m;
        lendsv_tmp = m + 1;
        l1 = m + 1;
        if (m != l) {
          double anorm;
          double b_anorm;
          int anorm_tmp;
          int n_tmp;
          n_tmp = m - l;
          if (n_tmp + 1 <= 0) {
            anorm = 0.0;
          } else {
            anorm = fabs(d_data[(l + n_tmp) - 1]);
            i = -1;
            exitg2 = false;
            while ((!exitg2) && (i + 1 <= n_tmp - 1)) {
              anorm_tmp = l + i;
              b_anorm = fabs(d_data[anorm_tmp]);
              if (rtIsNaN(b_anorm)) {
                anorm = rtNaN;
                exitg2 = true;
              } else {
                if (b_anorm > anorm) {
                  anorm = b_anorm;
                }
                b_anorm = fabs(e_data[anorm_tmp]);
                if (rtIsNaN(b_anorm)) {
                  anorm = rtNaN;
                  exitg2 = true;
                } else {
                  if (b_anorm > anorm) {
                    anorm = b_anorm;
                  }
                  i++;
                }
              }
            }
          }
          if (!(anorm == 0.0)) {
            int iscale;
            iscale = 0;
            if (anorm > 2.2346346549904327E+153) {
              iscale = 1;
              anorm_tmp = *d_size;
              b_i = r->size[0];
              r->size[0] = anorm_tmp;
              emxEnsureCapacity_real_T(r, b_i);
              r1 = r->data;
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                r1[b_i] = d_data[b_i];
              }
              b_xzlascl(anorm, 2.2346346549904327E+153, n_tmp + 1, r, l);
              r1 = r->data;
              anorm_tmp = r->size[0];
              *d_size = r->size[0];
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                d_data[b_i] = r1[b_i];
              }
              b_i = r->size[0];
              r->size[0] = e_size;
              emxEnsureCapacity_real_T(r, b_i);
              r1 = r->data;
              for (b_i = 0; b_i < e_size; b_i++) {
                r1[b_i] = e_data[b_i];
              }
              b_xzlascl(anorm, 2.2346346549904327E+153, n_tmp, r, l);
              r1 = r->data;
              anorm_tmp = r->size[0];
              e_size = r->size[0];
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                e_data[b_i] = r1[b_i];
              }
            } else if (anorm < 3.02546243347603E-123) {
              iscale = 2;
              anorm_tmp = *d_size;
              b_i = r->size[0];
              r->size[0] = anorm_tmp;
              emxEnsureCapacity_real_T(r, b_i);
              r1 = r->data;
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                r1[b_i] = d_data[b_i];
              }
              b_xzlascl(anorm, 3.02546243347603E-123, n_tmp + 1, r, l);
              r1 = r->data;
              anorm_tmp = r->size[0];
              *d_size = r->size[0];
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                d_data[b_i] = r1[b_i];
              }
              b_i = r->size[0];
              r->size[0] = e_size;
              emxEnsureCapacity_real_T(r, b_i);
              r1 = r->data;
              for (b_i = 0; b_i < e_size; b_i++) {
                r1[b_i] = e_data[b_i];
              }
              b_xzlascl(anorm, 3.02546243347603E-123, n_tmp, r, l);
              r1 = r->data;
              anorm_tmp = r->size[0];
              e_size = r->size[0];
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                e_data[b_i] = r1[b_i];
              }
            }
            b_i = m - 1;
            for (i = l; i <= b_i; i++) {
              b_anorm = e_data[i - 1];
              e_data[i - 1] = b_anorm * b_anorm;
            }
            if (fabs(d_data[m - 1]) < fabs(d_data[l - 1])) {
              lend = lsv;
              l = m;
            }
            if (lend >= l) {
              int exitg4;
              do {
                exitg4 = 0;
                if (l != lend) {
                  m = l;
                  while ((m < lend) &&
                         (!(fabs(e_data[m - 1]) <= 4.9303806576313238E-32 *
                                                       fabs(d_data[m - 1]) *
                                                       fabs(d_data[m])))) {
                    m++;
                  }
                } else {
                  m = lend;
                }
                if (m < lend) {
                  e_data[m - 1] = 0.0;
                }
                if (m == l) {
                  l++;
                  if (l > lend) {
                    exitg4 = 1;
                  }
                } else if (m == l + 1) {
                  d_data[l - 1] = xdlaev2(d_data[l - 1], sqrt(e_data[l - 1]),
                                          d_data[l], &d);
                  d_data[l] = d;
                  e_data[l - 1] = 0.0;
                  l += 2;
                  if (l > lend) {
                    exitg4 = 1;
                  }
                } else if (jtot == nmaxit) {
                  exitg4 = 1;
                } else {
                  double b_gamma;
                  double b_r;
                  double c;
                  double s;
                  double sigma;
                  double x;
                  jtot++;
                  b_anorm = sqrt(e_data[l - 1]);
                  b_r = d_data[l - 1];
                  sigma = (d_data[l] - b_r) / (2.0 * b_anorm);
                  x = rt_hypotd_snf(sigma, 1.0);
                  if (!(sigma >= 0.0)) {
                    x = -x;
                  }
                  sigma = b_r - b_anorm / (sigma + x);
                  c = 1.0;
                  s = 0.0;
                  b_gamma = d_data[m - 1] - sigma;
                  b_anorm = b_gamma * b_gamma;
                  b_i = m - 1;
                  for (i = b_i; i >= l; i--) {
                    double oldc;
                    d = e_data[i - 1];
                    b_r = b_anorm + d;
                    if (i != m - 1) {
                      e_data[i] = s * b_r;
                    }
                    oldc = c;
                    c = b_anorm / b_r;
                    s = d / b_r;
                    b_anorm = b_gamma;
                    x = d_data[i - 1];
                    b_gamma = c * (x - sigma) - s * b_gamma;
                    d_data[i] = b_anorm + (x - b_gamma);
                    if (c != 0.0) {
                      b_anorm = b_gamma * b_gamma / c;
                    } else {
                      b_anorm = oldc * d;
                    }
                  }
                  e_data[l - 1] = s * b_anorm;
                  d_data[l - 1] = sigma + b_gamma;
                }
              } while (exitg4 == 0);
            } else {
              int exitg3;
              do {
                exitg3 = 0;
                m = l;
                while ((m > lend) &&
                       (!(fabs(e_data[m - 2]) <= 4.9303806576313238E-32 *
                                                     fabs(d_data[m - 1]) *
                                                     fabs(d_data[m - 2])))) {
                  m--;
                }
                if (m > lend) {
                  e_data[m - 2] = 0.0;
                }
                if (m == l) {
                  l--;
                  if (l < lend) {
                    exitg3 = 1;
                  }
                } else if (m == l - 1) {
                  d_data[l - 1] = xdlaev2(d_data[l - 1], sqrt(e_data[l - 2]),
                                          d_data[l - 2], &d);
                  d_data[l - 2] = d;
                  e_data[l - 2] = 0.0;
                  l -= 2;
                  if (l < lend) {
                    exitg3 = 1;
                  }
                } else if (jtot == nmaxit) {
                  exitg3 = 1;
                } else {
                  double b_gamma;
                  double b_r;
                  double c;
                  double s;
                  double sigma;
                  double x;
                  jtot++;
                  b_anorm = sqrt(e_data[l - 2]);
                  b_r = d_data[l - 1];
                  sigma = (d_data[l - 2] - b_r) / (2.0 * b_anorm);
                  x = rt_hypotd_snf(sigma, 1.0);
                  if (!(sigma >= 0.0)) {
                    x = -x;
                  }
                  sigma = b_r - b_anorm / (sigma + x);
                  c = 1.0;
                  s = 0.0;
                  b_gamma = d_data[m - 1] - sigma;
                  b_anorm = b_gamma * b_gamma;
                  b_i = l - 1;
                  for (i = m; i <= b_i; i++) {
                    double oldc;
                    d = e_data[i - 1];
                    b_r = b_anorm + d;
                    if (i != m) {
                      e_data[i - 2] = s * b_r;
                    }
                    oldc = c;
                    c = b_anorm / b_r;
                    s = d / b_r;
                    b_anorm = b_gamma;
                    b_gamma = c * (d_data[i] - sigma) - s * b_gamma;
                    d_data[i - 1] = b_anorm + (d_data[i] - b_gamma);
                    if (c != 0.0) {
                      b_anorm = b_gamma * b_gamma / c;
                    } else {
                      b_anorm = oldc * d;
                    }
                  }
                  e_data[l - 2] = s * b_anorm;
                  d_data[l - 1] = sigma + b_gamma;
                }
              } while (exitg3 == 0);
            }
            if (iscale == 1) {
              anorm_tmp = *d_size;
              b_i = r->size[0];
              r->size[0] = anorm_tmp;
              emxEnsureCapacity_real_T(r, b_i);
              r1 = r->data;
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                r1[b_i] = d_data[b_i];
              }
              b_xzlascl(2.2346346549904327E+153, anorm, lendsv_tmp - lsv, r,
                        lsv);
              r1 = r->data;
              anorm_tmp = r->size[0];
              *d_size = r->size[0];
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                d_data[b_i] = r1[b_i];
              }
            } else if (iscale == 2) {
              anorm_tmp = *d_size;
              b_i = r->size[0];
              r->size[0] = anorm_tmp;
              emxEnsureCapacity_real_T(r, b_i);
              r1 = r->data;
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                r1[b_i] = d_data[b_i];
              }
              b_xzlascl(3.02546243347603E-123, anorm, lendsv_tmp - lsv, r, lsv);
              r1 = r->data;
              anorm_tmp = r->size[0];
              *d_size = r->size[0];
              for (b_i = 0; b_i < anorm_tmp; b_i++) {
                d_data[b_i] = r1[b_i];
              }
            }
            if (jtot >= nmaxit) {
              for (i = 0; i <= n - 2; i++) {
                if (e_data[i] != 0.0) {
                  info++;
                }
              }
              exitg1 = 1;
            }
          }
        }
      }
    } while (exitg1 == 0);
  }
  emxFree_real_T(&r);
  return info;
}

/*
 * File trailer for xdsterf.c
 *
 * [EOF]
 */
