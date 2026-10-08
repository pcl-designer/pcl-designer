/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xgetrf.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xgetrf.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : int m
 *                int n
 *                emxArray_real_T *A
 *                int lda
 *                emxArray_int32_T *ipiv
 * Return Type  : int
 */
int xgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T *ipiv)
{
  double *A_data;
  int b_n;
  int i;
  int ijA;
  int info;
  int j;
  int k;
  int yk;
  int *ipiv_data;
  A_data = A->data;
  if (m <= n) {
    yk = m;
  } else {
    yk = n;
  }
  if (yk < 1) {
    b_n = 0;
  } else {
    b_n = yk;
  }
  i = ipiv->size[0] * ipiv->size[1];
  ipiv->size[0] = 1;
  ipiv->size[1] = b_n;
  emxEnsureCapacity_int32_T(ipiv, i);
  ipiv_data = ipiv->data;
  if (b_n > 0) {
    ipiv_data[0] = 1;
    yk = 1;
    for (k = 2; k <= b_n; k++) {
      yk++;
      ipiv_data[k - 1] = yk;
    }
  }
  info = 0;
  if ((m >= 1) && (n >= 1)) {
    int u0;
    u0 = m - 1;
    if (u0 > n) {
      u0 = n;
    }
    for (j = 0; j < u0; j++) {
      double smax;
      int b_tmp;
      int jA;
      int mmj;
      int temp_tmp;
      mmj = m - j;
      b_tmp = j * (lda + 1);
      b_n = b_tmp + 2;
      if (mmj < 1) {
        yk = -1;
      } else {
        yk = 0;
        if (mmj > 1) {
          smax = fabs(A_data[b_tmp]);
          for (k = 2; k <= mmj; k++) {
            double s;
            s = fabs(A_data[(b_tmp + k) - 1]);
            if (s > smax) {
              yk = k - 1;
              smax = s;
            }
          }
        }
      }
      if (A_data[b_tmp + yk] != 0.0) {
        if (yk != 0) {
          jA = j + yk;
          ipiv_data[j] = jA + 1;
          for (k = 0; k < n; k++) {
            yk = k * lda;
            temp_tmp = j + yk;
            smax = A_data[temp_tmp];
            i = jA + yk;
            A_data[temp_tmp] = A_data[i];
            A_data[i] = smax;
          }
        }
        i = b_tmp + mmj;
        for (yk = b_n; yk <= i; yk++) {
          A_data[yk - 1] /= A_data[b_tmp];
        }
      } else {
        info = j + 1;
      }
      b_n = n - j;
      yk = b_tmp + lda;
      jA = yk;
      for (k = 0; k <= b_n - 2; k++) {
        smax = A_data[yk + k * lda];
        if (smax != 0.0) {
          i = jA + 2;
          temp_tmp = mmj + jA;
          for (ijA = i; ijA <= temp_tmp; ijA++) {
            A_data[ijA - 1] += A_data[((b_tmp + ijA) - jA) - 1] * -smax;
          }
        }
        jA += lda;
      }
    }
    if ((info == 0) && (m <= n) &&
        (!(A_data[(m + A->size[0] * (m - 1)) - 1] != 0.0))) {
      info = m;
    }
  }
  return info;
}

/*
 * File trailer for xgetrf.c
 *
 * [EOF]
 */
