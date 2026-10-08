/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xdlahqr.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xdlahqr.h"
#include "rt_nonfinite.h"
#include "xdlanv2.h"
#include "xzlarfg.h"
#include <math.h>

/* Function Declarations */
static int div_nzp_s32(int numerator, int denominator);

/* Function Definitions */
/*
 * Arguments    : int numerator
 *                int denominator
 * Return Type  : int
 */
static int div_nzp_s32(int numerator, int denominator)
{
  int quotient;
  unsigned int tempAbsQuotient;
  unsigned int u;
  if (numerator < 0) {
    tempAbsQuotient = ~(unsigned int)numerator + 1U;
  } else {
    tempAbsQuotient = (unsigned int)numerator;
  }
  if (denominator < 0) {
    u = ~(unsigned int)denominator + 1U;
  } else {
    u = (unsigned int)denominator;
  }
  tempAbsQuotient /= u;
  if ((numerator < 0) != (denominator < 0)) {
    quotient = -(int)tempAbsQuotient;
  } else {
    quotient = (int)tempAbsQuotient;
  }
  return quotient;
}

/*
 * Arguments    : int ihi
 *                double h_data[]
 *                const int h_size[2]
 *                double wr_data[]
 *                int *wr_size
 *                double wi_data[]
 *                int *wi_size
 * Return Type  : int
 */
int b_xdlahqr(int ihi, double h_data[], const int h_size[2], double wr_data[],
              int *wr_size, double wi_data[], int *wi_size)
{
  double d;
  double h12;
  double h22;
  double rt1r;
  double rt2r;
  double s;
  double tst;
  int b_i;
  int b_k;
  int c_k;
  int i;
  int info;
  int j;
  *wr_size = h_size[0];
  *wi_size = *wr_size;
  info = 0;
  i = ihi + 1;
  for (b_i = i; b_i <= *wr_size; b_i++) {
    wr_data[b_i - 1] = h_data[(b_i + h_size[0] * (b_i - 1)) - 1];
    wi_data[b_i - 1] = 0.0;
  }
  if (ihi == 1) {
    wr_data[0] = h_data[0];
    wi_data[0] = 0.0;
  } else {
    double smlnum;
    int kdefl;
    bool exitg1;
    for (j = 0; j <= ihi - 4; j++) {
      i = j + h_size[0] * j;
      h_data[i + 2] = 0.0;
      h_data[i + 3] = 0.0;
    }
    if (ihi - 2 >= 1) {
      h_data[(ihi + h_size[0] * (ihi - 3)) - 1] = 0.0;
    }
    smlnum = 2.2250738585072014E-308 * ((double)ihi / 2.2204460492503131E-16);
    kdefl = 0;
    b_i = ihi - 1;
    exitg1 = false;
    while ((!exitg1) && (b_i + 1 >= 1)) {
      int its;
      int l;
      int tst_tmp_tmp;
      bool converged;
      bool exitg2;
      l = 1;
      converged = false;
      its = 0;
      exitg2 = false;
      while ((!exitg2) && (its < 301)) {
        double aa;
        double tr;
        int k;
        bool exitg3;
        k = b_i;
        exitg3 = false;
        while ((!exitg3) && (k + 1 > l)) {
          i = k + h_size[0] * (k - 1);
          d = fabs(h_data[i]);
          if (d <= smlnum) {
            exitg3 = true;
          } else {
            tst_tmp_tmp = k + h_size[0] * k;
            h12 = fabs(h_data[tst_tmp_tmp]);
            aa = h_data[i - 1];
            tst = fabs(aa) + h12;
            if (tst == 0.0) {
              if (k - 1 >= 1) {
                tst = fabs(h_data[(k + h_size[0] * (k - 2)) - 1]);
              }
              if (k + 2 <= ihi) {
                tst += fabs(h_data[(k + h_size[0] * k) + 1]);
              }
            }
            if (d <= 2.2204460492503131E-16 * tst) {
              tr = fabs(h_data[tst_tmp_tmp - 1]);
              tst = fabs(aa - h_data[tst_tmp_tmp]);
              aa = fmax(h12, tst);
              tst = fmin(h12, tst);
              s = aa + tst;
              if (fmin(d, tr) * (fmax(d, tr) / s) <=
                  fmax(smlnum, 2.2204460492503131E-16 * (tst * (aa / s)))) {
                exitg3 = true;
              } else {
                k--;
              }
            } else {
              k--;
            }
          }
        }
        l = k + 1;
        if (k + 1 > 1) {
          h_data[k + h_size[0] * (k - 1)] = 0.0;
        }
        if (k + 1 >= b_i) {
          converged = true;
          exitg2 = true;
        } else {
          double v[3];
          int m;
          kdefl++;
          if (kdefl - div_nzp_s32(kdefl, 20) * 20 == 0) {
            s = fabs(h_data[b_i + h_size[0] * (b_i - 1)]) +
                fabs(h_data[(b_i + h_size[0] * (b_i - 2)) - 1]);
            tst = 0.75 * s + h_data[b_i + h_size[0] * b_i];
            h12 = -0.4375 * s;
            aa = s;
            h22 = tst;
          } else if (kdefl - div_nzp_s32(kdefl, 10) * 10 == 0) {
            tst_tmp_tmp = k + h_size[0] * k;
            s = fabs(h_data[tst_tmp_tmp + 1]) +
                fabs(h_data[(k + h_size[0] * (k + 1)) + 2]);
            tst = 0.75 * s + h_data[tst_tmp_tmp];
            h12 = -0.4375 * s;
            aa = s;
            h22 = tst;
          } else {
            tst_tmp_tmp = b_i + h_size[0] * (b_i - 1);
            tst = h_data[tst_tmp_tmp - 1];
            aa = h_data[tst_tmp_tmp];
            h12 = h_data[(b_i + h_size[0] * b_i) - 1];
            h22 = h_data[b_i + h_size[0] * b_i];
          }
          s = ((fabs(tst) + fabs(h12)) + fabs(aa)) + fabs(h22);
          if (s == 0.0) {
            rt1r = 0.0;
            h12 = 0.0;
            rt2r = 0.0;
            aa = 0.0;
          } else {
            tst /= s;
            aa /= s;
            h12 /= s;
            h22 /= s;
            tr = (tst + h22) / 2.0;
            tst = (tst - tr) * (h22 - tr) - h12 * aa;
            h12 = sqrt(fabs(tst));
            if (tst >= 0.0) {
              rt1r = tr * s;
              rt2r = rt1r;
              h12 *= s;
              aa = -h12;
            } else {
              rt1r = tr + h12;
              rt2r = tr - h12;
              if (fabs(rt1r - h22) <= fabs(rt2r - h22)) {
                rt1r *= s;
                rt2r = rt1r;
              } else {
                rt2r *= s;
                rt1r = rt2r;
              }
              h12 = 0.0;
              aa = 0.0;
            }
          }
          m = b_i - 1;
          exitg3 = false;
          while ((!exitg3) && (m >= k + 1)) {
            tst_tmp_tmp = m + h_size[0] * (m - 1);
            tst = h_data[tst_tmp_tmp];
            tr = h_data[tst_tmp_tmp - 1];
            h22 = tr - rt2r;
            s = (fabs(h22) + fabs(aa)) + fabs(tst);
            tst /= s;
            tst_tmp_tmp = m + h_size[0] * m;
            v[0] = (tst * h_data[tst_tmp_tmp - 1] + h22 * (h22 / s)) -
                   h12 * (aa / s);
            v[1] = tst * (((tr + h_data[tst_tmp_tmp]) - rt1r) - rt2r);
            v[2] = tst * h_data[tst_tmp_tmp + 1];
            s = (fabs(v[0]) + fabs(v[1])) + fabs(v[2]);
            v[0] /= s;
            v[1] /= s;
            v[2] /= s;
            if (m == k + 1) {
              exitg3 = true;
            } else {
              i = m + h_size[0] * (m - 2);
              if (fabs(h_data[i - 1]) * (fabs(v[1]) + fabs(v[2])) <=
                  2.2204460492503131E-16 * fabs(v[0]) *
                      ((fabs(h_data[i - 2]) + fabs(tr)) +
                       fabs(h_data[tst_tmp_tmp]))) {
                exitg3 = true;
              } else {
                m--;
              }
            }
          }
          for (b_k = m; b_k <= b_i; b_k++) {
            int nr;
            tst_tmp_tmp = (b_i - b_k) + 2;
            if (tst_tmp_tmp >= 3) {
              nr = 3;
            } else {
              nr = tst_tmp_tmp;
            }
            if (b_k > m) {
              tst_tmp_tmp = ((b_k - 2) * *wr_size + b_k) - 1;
              for (c_k = 0; c_k < nr; c_k++) {
                v[c_k] = h_data[tst_tmp_tmp + c_k];
              }
            }
            tst = v[0];
            tr = b_xzlarfg(nr, &tst, v);
            if (b_k > m) {
              h_data[(b_k + h_size[0] * (b_k - 2)) - 1] = tst;
              i = b_k + h_size[0] * (b_k - 2);
              h_data[i] = 0.0;
              if (b_k < b_i) {
                h_data[i + 1] = 0.0;
              }
            } else if (m > k + 1) {
              h_data[(b_k + h_size[0] * (b_k - 2)) - 1] *= 1.0 - tr;
            }
            d = v[1];
            tst = tr * v[1];
            if (nr == 3) {
              h22 = v[2];
              aa = tr * v[2];
              for (j = b_k; j <= b_i + 1; j++) {
                i = b_k + h_size[0] * (j - 1);
                rt2r = h_data[i - 1];
                rt1r = h_data[i];
                s = h_data[i + 1];
                h12 = (rt2r + d * rt1r) + h22 * s;
                rt2r -= h12 * tr;
                h_data[i - 1] = rt2r;
                rt1r -= h12 * tst;
                h_data[i] = rt1r;
                s -= h12 * aa;
                h_data[i + 1] = s;
              }
              if (b_k + 3 <= b_i + 1) {
                i = b_k;
              } else {
                i = b_i - 2;
              }
              for (j = k + 1; j <= i + 3; j++) {
                tst_tmp_tmp = (j + h_size[0] * (b_k - 1)) - 1;
                rt2r = h_data[tst_tmp_tmp];
                nr = (j + h_size[0] * b_k) - 1;
                rt1r = h_data[nr];
                c_k = (j + h_size[0] * (b_k + 1)) - 1;
                s = h_data[c_k];
                h12 = (rt2r + d * rt1r) + h22 * s;
                rt2r -= h12 * tr;
                h_data[tst_tmp_tmp] = rt2r;
                rt1r -= h12 * tst;
                h_data[nr] = rt1r;
                s -= h12 * aa;
                h_data[c_k] = s;
              }
            } else if (nr == 2) {
              for (j = b_k; j <= b_i + 1; j++) {
                i = b_k + h_size[0] * (j - 1);
                h22 = h_data[i - 1];
                rt2r = h_data[i];
                h12 = h22 + d * rt2r;
                h22 -= h12 * tr;
                h_data[i - 1] = h22;
                rt2r -= h12 * tst;
                h_data[i] = rt2r;
              }
              for (j = k + 1; j <= b_i + 1; j++) {
                i = (j + h_size[0] * (b_k - 1)) - 1;
                h22 = h_data[i];
                tst_tmp_tmp = (j + h_size[0] * b_k) - 1;
                rt2r = h_data[tst_tmp_tmp];
                h12 = h22 + d * rt2r;
                h22 -= h12 * tr;
                h_data[i] = h22;
                rt2r -= h12 * tst;
                h_data[tst_tmp_tmp] = rt2r;
              }
            }
          }
          its++;
        }
      }
      if (!converged) {
        info = b_i + 1;
        exitg1 = true;
      } else {
        if (l == b_i + 1) {
          wr_data[b_i] = h_data[b_i + h_size[0] * b_i];
          wi_data[b_i] = 0.0;
        } else if (l == b_i) {
          i = b_i + h_size[0] * b_i;
          d = h_data[i - 1];
          tst_tmp_tmp = b_i + h_size[0] * (b_i - 1);
          h22 = h_data[tst_tmp_tmp];
          rt2r = h_data[i];
          wr_data[b_i - 1] =
              xdlanv2(&h_data[(b_i + h_size[0] * (b_i - 1)) - 1], &d, &h22,
                      &rt2r, &wi_data[b_i - 1], &rt1r, &s, &tst, &h12);
          wr_data[b_i] = rt1r;
          wi_data[b_i] = s;
          h_data[i - 1] = d;
          h_data[tst_tmp_tmp] = h22;
          h_data[i] = rt2r;
        }
        kdefl = 0;
        b_i = l - 2;
      }
    }
    if ((info != 0) && (*wr_size > 2)) {
      for (j = 3; j <= *wr_size; j++) {
        for (b_i = j; b_i <= *wr_size; b_i++) {
          h_data[(b_i + h_size[0] * (j - 3)) - 1] = 0.0;
        }
      }
    }
  }
  return info;
}

/*
 * Arguments    : int ilo
 *                int ihi
 *                double h_data[]
 *                const int h_size[2]
 *                double wr_data[]
 *                int *wr_size
 *                double wi_data[]
 *                int *wi_size
 * Return Type  : int
 */
int xdlahqr(int ilo, int ihi, double h_data[], const int h_size[2],
            double wr_data[], int *wr_size, double wi_data[], int *wi_size)
{
  double d;
  double h12;
  double h22;
  double rt1r;
  double rt2r;
  double s;
  double tst;
  int b_i;
  int b_k;
  int c_k;
  int i;
  int info;
  int j;
  *wr_size = h_size[0];
  *wi_size = *wr_size;
  info = 0;
  i = (unsigned char)(ilo - 1);
  for (b_i = 0; b_i < i; b_i++) {
    wr_data[b_i] = h_data[b_i + h_size[0] * b_i];
    wi_data[b_i] = 0.0;
  }
  i = ihi + 1;
  for (b_i = i; b_i <= *wr_size; b_i++) {
    wr_data[b_i - 1] = h_data[(b_i + h_size[0] * (b_i - 1)) - 1];
    wi_data[b_i - 1] = 0.0;
  }
  if (ilo == ihi) {
    wr_data[ilo - 1] = h_data[(ilo + h_size[0] * (ilo - 1)) - 1];
    wi_data[ilo - 1] = 0.0;
  } else {
    double smlnum;
    int kdefl;
    int tst_tmp_tmp;
    bool exitg1;
    i = ihi - 3;
    for (j = ilo; j <= i; j++) {
      tst_tmp_tmp = j + h_size[0] * (j - 1);
      h_data[tst_tmp_tmp + 1] = 0.0;
      h_data[tst_tmp_tmp + 2] = 0.0;
    }
    if (ilo <= ihi - 2) {
      h_data[(ihi + h_size[0] * (ihi - 3)) - 1] = 0.0;
    }
    smlnum = 2.2250738585072014E-308 *
             ((double)((ihi - ilo) + 1) / 2.2204460492503131E-16);
    kdefl = 0;
    b_i = ihi - 1;
    exitg1 = false;
    while ((!exitg1) && (b_i + 1 >= ilo)) {
      int its;
      int l;
      bool converged;
      bool exitg2;
      l = ilo;
      converged = false;
      its = 0;
      exitg2 = false;
      while ((!exitg2) && (its < 301)) {
        double aa;
        double tr;
        int k;
        bool exitg3;
        k = b_i;
        exitg3 = false;
        while ((!exitg3) && (k + 1 > l)) {
          i = k + h_size[0] * (k - 1);
          d = fabs(h_data[i]);
          if (d <= smlnum) {
            exitg3 = true;
          } else {
            tst_tmp_tmp = k + h_size[0] * k;
            h12 = fabs(h_data[tst_tmp_tmp]);
            aa = h_data[i - 1];
            tst = fabs(aa) + h12;
            if (tst == 0.0) {
              if (k - 1 >= ilo) {
                tst = fabs(h_data[(k + h_size[0] * (k - 2)) - 1]);
              }
              if (k + 2 <= ihi) {
                tst += fabs(h_data[(k + h_size[0] * k) + 1]);
              }
            }
            if (d <= 2.2204460492503131E-16 * tst) {
              tr = fabs(h_data[tst_tmp_tmp - 1]);
              tst = fabs(aa - h_data[tst_tmp_tmp]);
              aa = fmax(h12, tst);
              tst = fmin(h12, tst);
              s = aa + tst;
              if (fmin(d, tr) * (fmax(d, tr) / s) <=
                  fmax(smlnum, 2.2204460492503131E-16 * (tst * (aa / s)))) {
                exitg3 = true;
              } else {
                k--;
              }
            } else {
              k--;
            }
          }
        }
        l = k + 1;
        if (k + 1 > ilo) {
          h_data[k + h_size[0] * (k - 1)] = 0.0;
        }
        if (k + 1 >= b_i) {
          converged = true;
          exitg2 = true;
        } else {
          double v[3];
          int m;
          kdefl++;
          if (kdefl - div_nzp_s32(kdefl, 20) * 20 == 0) {
            s = fabs(h_data[b_i + h_size[0] * (b_i - 1)]) +
                fabs(h_data[(b_i + h_size[0] * (b_i - 2)) - 1]);
            tst = 0.75 * s + h_data[b_i + h_size[0] * b_i];
            h12 = -0.4375 * s;
            aa = s;
            h22 = tst;
          } else if (kdefl - div_nzp_s32(kdefl, 10) * 10 == 0) {
            tst_tmp_tmp = k + h_size[0] * k;
            s = fabs(h_data[tst_tmp_tmp + 1]) +
                fabs(h_data[(k + h_size[0] * (k + 1)) + 2]);
            tst = 0.75 * s + h_data[tst_tmp_tmp];
            h12 = -0.4375 * s;
            aa = s;
            h22 = tst;
          } else {
            tst_tmp_tmp = b_i + h_size[0] * (b_i - 1);
            tst = h_data[tst_tmp_tmp - 1];
            aa = h_data[tst_tmp_tmp];
            h12 = h_data[(b_i + h_size[0] * b_i) - 1];
            h22 = h_data[b_i + h_size[0] * b_i];
          }
          s = ((fabs(tst) + fabs(h12)) + fabs(aa)) + fabs(h22);
          if (s == 0.0) {
            rt1r = 0.0;
            h12 = 0.0;
            rt2r = 0.0;
            aa = 0.0;
          } else {
            tst /= s;
            aa /= s;
            h12 /= s;
            h22 /= s;
            tr = (tst + h22) / 2.0;
            tst = (tst - tr) * (h22 - tr) - h12 * aa;
            h12 = sqrt(fabs(tst));
            if (tst >= 0.0) {
              rt1r = tr * s;
              rt2r = rt1r;
              h12 *= s;
              aa = -h12;
            } else {
              rt1r = tr + h12;
              rt2r = tr - h12;
              if (fabs(rt1r - h22) <= fabs(rt2r - h22)) {
                rt1r *= s;
                rt2r = rt1r;
              } else {
                rt2r *= s;
                rt1r = rt2r;
              }
              h12 = 0.0;
              aa = 0.0;
            }
          }
          m = b_i - 1;
          exitg3 = false;
          while ((!exitg3) && (m >= k + 1)) {
            tst_tmp_tmp = m + h_size[0] * (m - 1);
            tst = h_data[tst_tmp_tmp];
            tr = h_data[tst_tmp_tmp - 1];
            h22 = tr - rt2r;
            s = (fabs(h22) + fabs(aa)) + fabs(tst);
            tst /= s;
            tst_tmp_tmp = m + h_size[0] * m;
            v[0] = (tst * h_data[tst_tmp_tmp - 1] + h22 * (h22 / s)) -
                   h12 * (aa / s);
            v[1] = tst * (((tr + h_data[tst_tmp_tmp]) - rt1r) - rt2r);
            v[2] = tst * h_data[tst_tmp_tmp + 1];
            s = (fabs(v[0]) + fabs(v[1])) + fabs(v[2]);
            v[0] /= s;
            v[1] /= s;
            v[2] /= s;
            if (m == k + 1) {
              exitg3 = true;
            } else {
              i = m + h_size[0] * (m - 2);
              if (fabs(h_data[i - 1]) * (fabs(v[1]) + fabs(v[2])) <=
                  2.2204460492503131E-16 * fabs(v[0]) *
                      ((fabs(h_data[i - 2]) + fabs(tr)) +
                       fabs(h_data[tst_tmp_tmp]))) {
                exitg3 = true;
              } else {
                m--;
              }
            }
          }
          for (b_k = m; b_k <= b_i; b_k++) {
            int nr;
            tst_tmp_tmp = (b_i - b_k) + 2;
            if (tst_tmp_tmp >= 3) {
              nr = 3;
            } else {
              nr = tst_tmp_tmp;
            }
            if (b_k > m) {
              tst_tmp_tmp = ((b_k - 2) * *wr_size + b_k) - 1;
              for (c_k = 0; c_k < nr; c_k++) {
                v[c_k] = h_data[tst_tmp_tmp + c_k];
              }
            }
            tst = v[0];
            tr = b_xzlarfg(nr, &tst, v);
            if (b_k > m) {
              h_data[(b_k + h_size[0] * (b_k - 2)) - 1] = tst;
              i = b_k + h_size[0] * (b_k - 2);
              h_data[i] = 0.0;
              if (b_k < b_i) {
                h_data[i + 1] = 0.0;
              }
            } else if (m > k + 1) {
              h_data[(b_k + h_size[0] * (b_k - 2)) - 1] *= 1.0 - tr;
            }
            d = v[1];
            tst = tr * v[1];
            if (nr == 3) {
              h22 = v[2];
              aa = tr * v[2];
              for (j = b_k; j <= b_i + 1; j++) {
                i = b_k + h_size[0] * (j - 1);
                rt2r = h_data[i - 1];
                rt1r = h_data[i];
                s = h_data[i + 1];
                h12 = (rt2r + d * rt1r) + h22 * s;
                rt2r -= h12 * tr;
                h_data[i - 1] = rt2r;
                rt1r -= h12 * tst;
                h_data[i] = rt1r;
                s -= h12 * aa;
                h_data[i + 1] = s;
              }
              if (b_k + 3 <= b_i + 1) {
                i = b_k;
              } else {
                i = b_i - 2;
              }
              for (j = k + 1; j <= i + 3; j++) {
                tst_tmp_tmp = (j + h_size[0] * (b_k - 1)) - 1;
                rt2r = h_data[tst_tmp_tmp];
                nr = (j + h_size[0] * b_k) - 1;
                rt1r = h_data[nr];
                c_k = (j + h_size[0] * (b_k + 1)) - 1;
                s = h_data[c_k];
                h12 = (rt2r + d * rt1r) + h22 * s;
                rt2r -= h12 * tr;
                h_data[tst_tmp_tmp] = rt2r;
                rt1r -= h12 * tst;
                h_data[nr] = rt1r;
                s -= h12 * aa;
                h_data[c_k] = s;
              }
            } else if (nr == 2) {
              for (j = b_k; j <= b_i + 1; j++) {
                i = b_k + h_size[0] * (j - 1);
                h22 = h_data[i - 1];
                rt2r = h_data[i];
                h12 = h22 + d * rt2r;
                h22 -= h12 * tr;
                h_data[i - 1] = h22;
                rt2r -= h12 * tst;
                h_data[i] = rt2r;
              }
              for (j = k + 1; j <= b_i + 1; j++) {
                i = (j + h_size[0] * (b_k - 1)) - 1;
                h22 = h_data[i];
                tst_tmp_tmp = (j + h_size[0] * b_k) - 1;
                rt2r = h_data[tst_tmp_tmp];
                h12 = h22 + d * rt2r;
                h22 -= h12 * tr;
                h_data[i] = h22;
                rt2r -= h12 * tst;
                h_data[tst_tmp_tmp] = rt2r;
              }
            }
          }
          its++;
        }
      }
      if (!converged) {
        info = b_i + 1;
        exitg1 = true;
      } else {
        if (l == b_i + 1) {
          wr_data[b_i] = h_data[b_i + h_size[0] * b_i];
          wi_data[b_i] = 0.0;
        } else if (l == b_i) {
          i = b_i + h_size[0] * b_i;
          d = h_data[i - 1];
          tst_tmp_tmp = b_i + h_size[0] * (b_i - 1);
          h22 = h_data[tst_tmp_tmp];
          rt2r = h_data[i];
          wr_data[b_i - 1] =
              xdlanv2(&h_data[(b_i + h_size[0] * (b_i - 1)) - 1], &d, &h22,
                      &rt2r, &wi_data[b_i - 1], &rt1r, &s, &tst, &h12);
          wr_data[b_i] = rt1r;
          wi_data[b_i] = s;
          h_data[i - 1] = d;
          h_data[tst_tmp_tmp] = h22;
          h_data[i] = rt2r;
        }
        kdefl = 0;
        b_i = l - 2;
      }
    }
    if ((info != 0) && (*wr_size > 2)) {
      for (j = 3; j <= *wr_size; j++) {
        for (b_i = j; b_i <= *wr_size; b_i++) {
          h_data[(b_i + h_size[0] * (j - 3)) - 1] = 0.0;
        }
      }
    }
  }
  return info;
}

/*
 * File trailer for xdlahqr.c
 *
 * [EOF]
 */
