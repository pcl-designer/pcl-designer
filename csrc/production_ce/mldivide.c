/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: mldivide.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 03-Oct-2026 17:22:39
 */

/* Include Files */
#include "mldivide.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *A
 *                const emxArray_real_T *B
 *                emxArray_real_T *Y
 * Return Type  : void
 */
void mldivide(const emxArray_real_T *A, const emxArray_real_T *B,
              emxArray_real_T *Y)
{
  emxArray_int32_T *jpvt;
  emxArray_real_T *b_A;
  emxArray_real_T *b_B;
  emxArray_real_T *tau;
  const double *A_data;
  const double *B_data;
  double *Y_data;
  double *b_A_data;
  double *b_B_data;
  double *tau_data;
  int b_i;
  int i;
  int i1;
  int j;
  int k;
  int rankA;
  int *jpvt_data;
  B_data = B->data;
  A_data = A->data;
  emxInit_real_T(&b_A, 2);
  emxInit_real_T(&tau, 1);
  emxInit_int32_T(&jpvt, 2);
  emxInit_real_T(&b_B, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0) ||
      ((B->size[0] == 0) || (B->size[1] == 0))) {
    int minmn;
    i = Y->size[0] * Y->size[1];
    Y->size[0] = A->size[1];
    Y->size[1] = B->size[1];
    emxEnsureCapacity_real_T(Y, i);
    Y_data = Y->data;
    minmn = A->size[1] * B->size[1];
    for (i = 0; i < minmn; i++) {
      Y_data[i] = 0.0;
    }
  } else if (A->size[0] == A->size[1]) {
    double tol;
    int LDA_tmp;
    int jp1j;
    int maxmn;
    int minmn;
    int n;
    int nrhs;
    int u0;
    i = B->size[0];
    i1 = Y->size[0] * Y->size[1];
    Y->size[0] = B->size[0];
    Y->size[1] = B->size[1];
    emxEnsureCapacity_real_T(Y, i1);
    Y_data = Y->data;
    minmn = B->size[0] * B->size[1];
    for (i1 = 0; i1 < minmn; i1++) {
      Y_data[i1] = B_data[i1];
    }
    u0 = A->size[0];
    n = A->size[1];
    if (u0 <= n) {
      n = u0;
    }
    u0 = B->size[0];
    if (u0 <= n) {
      n = u0;
    }
    nrhs = B->size[1] - 1;
    LDA_tmp = A->size[0];
    i1 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i1);
    b_A_data = b_A->data;
    minmn = A->size[0] * A->size[1];
    for (i1 = 0; i1 < minmn; i1++) {
      b_A_data[i1] = A_data[i1];
    }
    i1 = jpvt->size[0] * jpvt->size[1];
    jpvt->size[0] = 1;
    jpvt->size[1] = n;
    emxEnsureCapacity_int32_T(jpvt, i1);
    jpvt_data = jpvt->data;
    jpvt_data[0] = 1;
    minmn = 1;
    for (k = 2; k <= n; k++) {
      minmn++;
      jpvt_data[k - 1] = minmn;
    }
    u0 = n - 1;
    if (u0 > n) {
      u0 = n;
    }
    for (j = 0; j < u0; j++) {
      int b_tmp;
      int mmj_tmp;
      mmj_tmp = n - j;
      b_tmp = j * (LDA_tmp + 1);
      jp1j = b_tmp + 2;
      if (mmj_tmp < 1) {
        minmn = -1;
      } else {
        minmn = 0;
        if (mmj_tmp > 1) {
          tol = fabs(b_A_data[b_tmp]);
          for (k = 2; k <= mmj_tmp; k++) {
            double s;
            s = fabs(b_A_data[(b_tmp + k) - 1]);
            if (s > tol) {
              minmn = k - 1;
              tol = s;
            }
          }
        }
      }
      if (b_A_data[b_tmp + minmn] != 0.0) {
        if (minmn != 0) {
          maxmn = j + minmn;
          jpvt_data[j] = maxmn + 1;
          for (k = 0; k < n; k++) {
            minmn = k * LDA_tmp;
            rankA = j + minmn;
            tol = b_A_data[rankA];
            i1 = maxmn + minmn;
            b_A_data[rankA] = b_A_data[i1];
            b_A_data[i1] = tol;
          }
        }
        i1 = b_tmp + mmj_tmp;
        for (b_i = jp1j; b_i <= i1; b_i++) {
          b_A_data[b_i - 1] /= b_A_data[b_tmp];
        }
      }
      minmn = b_tmp + LDA_tmp;
      maxmn = minmn;
      for (rankA = 0; rankA <= mmj_tmp - 2; rankA++) {
        tol = b_A_data[minmn + rankA * LDA_tmp];
        if (tol != 0.0) {
          i1 = maxmn + 2;
          jp1j = mmj_tmp + maxmn;
          for (b_i = i1; b_i <= jp1j; b_i++) {
            b_A_data[b_i - 1] += b_A_data[((b_tmp + b_i) - maxmn) - 1] * -tol;
          }
        }
        maxmn += LDA_tmp;
      }
    }
    for (b_i = 0; b_i <= n - 2; b_i++) {
      i1 = jpvt_data[b_i];
      if (i1 != b_i + 1) {
        for (j = 0; j <= nrhs; j++) {
          tol = Y_data[b_i + Y->size[0] * j];
          Y_data[b_i + Y->size[0] * j] = Y_data[(i1 + Y->size[0] * j) - 1];
          Y_data[(i1 + Y->size[0] * j) - 1] = tol;
        }
      }
    }
    for (j = 0; j <= nrhs; j++) {
      minmn = i * j;
      for (k = 0; k < n; k++) {
        maxmn = LDA_tmp * k;
        i1 = k + minmn;
        if (Y_data[i1] != 0.0) {
          jp1j = k + 2;
          for (b_i = jp1j; b_i <= n; b_i++) {
            rankA = (b_i + minmn) - 1;
            Y_data[rankA] -= Y_data[i1] * b_A_data[(b_i + maxmn) - 1];
          }
        }
      }
    }
    for (j = 0; j <= nrhs; j++) {
      minmn = i * j - 1;
      for (k = n; k >= 1; k--) {
        maxmn = LDA_tmp * (k - 1) - 1;
        i1 = k + minmn;
        tol = Y_data[i1];
        if (tol != 0.0) {
          Y_data[i1] = tol / b_A_data[k + maxmn];
          for (b_i = 0; b_i <= k - 2; b_i++) {
            jp1j = (b_i + minmn) + 1;
            Y_data[jp1j] -= Y_data[i1] * b_A_data[(b_i + maxmn) + 1];
          }
        }
      }
    }
  } else {
    double tol;
    int maxmn;
    int minmn;
    int u0;
    i = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i);
    b_A_data = b_A->data;
    minmn = A->size[0] * A->size[1];
    for (i = 0; i < minmn; i++) {
      b_A_data[i] = A_data[i];
    }
    xgeqp3(b_A, tau, jpvt);
    jpvt_data = jpvt->data;
    tau_data = tau->data;
    b_A_data = b_A->data;
    rankA = 0;
    if (b_A->size[0] < b_A->size[1]) {
      minmn = b_A->size[0];
      maxmn = b_A->size[1];
    } else {
      minmn = b_A->size[1];
      maxmn = b_A->size[0];
    }
    if (minmn > 0) {
      tol =
          fmin(1.4901161193847656E-8, 2.2204460492503131E-15 * (double)maxmn) *
          fabs(b_A_data[0]);
      while ((rankA < minmn) &&
             (!(fabs(b_A_data[rankA + b_A->size[0] * rankA]) <= tol))) {
        rankA++;
      }
    }
    i = b_B->size[0] * b_B->size[1];
    b_B->size[0] = B->size[0];
    i1 = B->size[1];
    b_B->size[1] = B->size[1];
    emxEnsureCapacity_real_T(b_B, i);
    b_B_data = b_B->data;
    minmn = B->size[0] * B->size[1];
    for (i = 0; i < minmn; i++) {
      b_B_data[i] = B_data[i];
    }
    i = Y->size[0] * Y->size[1];
    Y->size[0] = b_A->size[1];
    Y->size[1] = B->size[1];
    emxEnsureCapacity_real_T(Y, i);
    Y_data = Y->data;
    minmn = b_A->size[1] * B->size[1];
    for (i = 0; i < minmn; i++) {
      Y_data[i] = 0.0;
    }
    u0 = b_A->size[0];
    maxmn = b_A->size[1];
    if (u0 <= maxmn) {
      maxmn = u0;
    }
    for (j = 0; j < maxmn; j++) {
      minmn = b_A->size[0];
      if (tau_data[j] != 0.0) {
        i = j + 2;
        for (k = 0; k < i1; k++) {
          tol = b_B_data[j + b_B->size[0] * k];
          for (b_i = i; b_i <= minmn; b_i++) {
            tol += b_A_data[(b_i + b_A->size[0] * j) - 1] *
                   b_B_data[(b_i + b_B->size[0] * k) - 1];
          }
          tol *= tau_data[j];
          if (tol != 0.0) {
            b_B_data[j + b_B->size[0] * k] -= tol;
            for (b_i = i; b_i <= minmn; b_i++) {
              b_B_data[(b_i + b_B->size[0] * k) - 1] -=
                  b_A_data[(b_i + b_A->size[0] * j) - 1] * tol;
            }
          }
        }
      }
    }
    for (k = 0; k < i1; k++) {
      for (b_i = 0; b_i < rankA; b_i++) {
        Y_data[(jpvt_data[b_i] + Y->size[0] * k) - 1] =
            b_B_data[b_i + b_B->size[0] * k];
      }
      for (j = rankA; j >= 1; j--) {
        i = jpvt_data[j - 1];
        Y_data[(i + Y->size[0] * k) - 1] /=
            b_A_data[(j + b_A->size[0] * (j - 1)) - 1];
        for (b_i = 0; b_i <= j - 2; b_i++) {
          Y_data[(jpvt_data[b_i] + Y->size[0] * k) - 1] -=
              Y_data[(i + Y->size[0] * k) - 1] *
              b_A_data[b_i + b_A->size[0] * (j - 1)];
        }
      }
    }
  }
  emxFree_real_T(&b_B);
  emxFree_int32_T(&jpvt);
  emxFree_real_T(&tau);
  emxFree_real_T(&b_A);
}

/*
 * File trailer for mldivide.c
 *
 * [EOF]
 */
