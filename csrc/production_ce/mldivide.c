/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: mldivide.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "mldivide.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "qrsolve.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "xgetrf.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double A_data[]
 *                const int A_size[2]
 *                double B_data[]
 *                int *B_size
 * Return Type  : void
 */
void b_mldivide(const double A_data[], const int A_size[2], double B_data[],
                int *B_size)
{
  emxArray_int32_T *jpvt;
  emxArray_real_T *A;
  emxArray_real_T *tau;
  double b_B_data[10];
  double *b_A_data;
  double *tau_data;
  int b_i;
  int i;
  int j;
  int *jpvt_data;
  emxInit_real_T(&A, 2);
  emxInit_int32_T(&jpvt, 2);
  if (A_size[0] == A_size[1]) {
    double wj;
    int LDA;
    int m;
    int n;
    m = A_size[0];
    n = A_size[1];
    if (m <= n) {
      n = m;
    }
    m = *B_size;
    if (m <= n) {
      n = m;
    }
    i = A->size[0] * A->size[1];
    A->size[0] = A_size[0];
    A->size[1] = A_size[1];
    emxEnsureCapacity_real_T(A, i);
    b_A_data = A->data;
    m = A_size[0] * A_size[1];
    for (i = 0; i < m; i++) {
      b_A_data[i] = A_data[i];
    }
    xgetrf(n, n, A, A_size[0], jpvt);
    jpvt_data = jpvt->data;
    b_A_data = A->data;
    LDA = A->size[0];
    i = (unsigned char)(n - 1);
    for (b_i = 0; b_i < i; b_i++) {
      m = jpvt_data[b_i];
      if (m != b_i + 1) {
        wj = B_data[b_i];
        B_data[b_i] = B_data[m - 1];
        B_data[m - 1] = wj;
      }
    }
    for (j = 0; j < n; j++) {
      m = LDA * j;
      if (B_data[j] != 0.0) {
        i = j + 2;
        for (b_i = i; b_i <= n; b_i++) {
          B_data[b_i - 1] -= B_data[j] * b_A_data[(b_i + m) - 1];
        }
      }
    }
    for (j = n; j >= 1; j--) {
      m = LDA * (j - 1);
      wj = B_data[j - 1];
      if (wj != 0.0) {
        wj /= b_A_data[(j + m) - 1];
        B_data[j - 1] = wj;
        for (b_i = 0; b_i <= j - 2; b_i++) {
          B_data[b_i] -= B_data[j - 1] * b_A_data[b_i + m];
        }
      }
    }
  } else {
    int LDA;
    int m;
    int n;
    i = A->size[0] * A->size[1];
    A->size[0] = A_size[0];
    A->size[1] = A_size[1];
    emxEnsureCapacity_real_T(A, i);
    b_A_data = A->data;
    m = A_size[0] * A_size[1];
    for (i = 0; i < m; i++) {
      b_A_data[i] = A_data[i];
    }
    emxInit_real_T(&tau, 1);
    xgeqp3(A, tau, jpvt);
    jpvt_data = jpvt->data;
    tau_data = tau->data;
    b_A_data = A->data;
    n = rankFromQR(A);
    m = *B_size;
    if (m - 1 >= 0) {
      memcpy(&b_B_data[0], &B_data[0], (unsigned int)m * sizeof(double));
    }
    m = A->size[1];
    *B_size = A->size[1];
    if (m - 1 >= 0) {
      memset(&B_data[0], 0, (unsigned int)m * sizeof(double));
    }
    m = A->size[0];
    LDA = A->size[1];
    if (m <= LDA) {
      LDA = m;
    }
    m = A->size[0];
    for (j = 0; j < LDA; j++) {
      if (tau_data[j] != 0.0) {
        double wj;
        wj = b_B_data[j];
        i = j + 2;
        for (b_i = i; b_i <= m; b_i++) {
          wj += b_A_data[(b_i + A->size[0] * j) - 1] * b_B_data[b_i - 1];
        }
        wj *= tau_data[j];
        if (wj != 0.0) {
          b_B_data[j] -= wj;
          for (b_i = i; b_i <= m; b_i++) {
            b_B_data[b_i - 1] -= b_A_data[(b_i + A->size[0] * j) - 1] * wj;
          }
        }
      }
    }
    emxFree_real_T(&tau);
    i = (unsigned char)n;
    for (b_i = 0; b_i < i; b_i++) {
      B_data[jpvt_data[b_i] - 1] = b_B_data[b_i];
    }
    for (j = n; j >= 1; j--) {
      i = jpvt_data[j - 1];
      B_data[i - 1] /= b_A_data[(j + A->size[0] * (j - 1)) - 1];
      m = (unsigned char)(j - 1);
      for (b_i = 0; b_i < m; b_i++) {
        B_data[jpvt_data[b_i] - 1] -=
            B_data[i - 1] * b_A_data[b_i + A->size[0] * (j - 1)];
      }
    }
  }
  emxFree_int32_T(&jpvt);
  emxFree_real_T(&A);
}

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
  int *jpvt_data;
  B_data = B->data;
  A_data = A->data;
  emxInit_real_T(&b_A, 2);
  emxInit_real_T(&tau, 1);
  emxInit_int32_T(&jpvt, 2);
  emxInit_real_T(&b_B, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0) ||
      ((B->size[0] == 0) || (B->size[1] == 0))) {
    int m;
    i = Y->size[0] * Y->size[1];
    Y->size[0] = A->size[1];
    Y->size[1] = B->size[1];
    emxEnsureCapacity_real_T(Y, i);
    Y_data = Y->data;
    m = A->size[1] * B->size[1];
    for (i = 0; i < m; i++) {
      Y_data[i] = 0.0;
    }
  } else if (A->size[0] == A->size[1]) {
    double wj;
    int i2;
    int kAcol;
    int m;
    int mn;
    int n;
    int nrhs;
    i = B->size[0];
    i1 = Y->size[0] * Y->size[1];
    Y->size[0] = B->size[0];
    Y->size[1] = B->size[1];
    emxEnsureCapacity_real_T(Y, i1);
    Y_data = Y->data;
    m = B->size[0] * B->size[1];
    for (i1 = 0; i1 < m; i1++) {
      Y_data[i1] = B_data[i1];
    }
    m = A->size[0];
    n = A->size[1];
    if (m <= n) {
      n = m;
    }
    m = B->size[0];
    if (m <= n) {
      n = m;
    }
    nrhs = B->size[1] - 1;
    i1 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i1);
    b_A_data = b_A->data;
    m = A->size[0] * A->size[1];
    for (i1 = 0; i1 < m; i1++) {
      b_A_data[i1] = A_data[i1];
    }
    xgetrf(n, n, b_A, A->size[0], jpvt);
    jpvt_data = jpvt->data;
    b_A_data = b_A->data;
    m = b_A->size[0];
    for (b_i = 0; b_i <= n - 2; b_i++) {
      i1 = jpvt_data[b_i];
      if (i1 != b_i + 1) {
        for (j = 0; j <= nrhs; j++) {
          wj = Y_data[b_i + Y->size[0] * j];
          Y_data[b_i + Y->size[0] * j] = Y_data[(i1 + Y->size[0] * j) - 1];
          Y_data[(i1 + Y->size[0] * j) - 1] = wj;
        }
      }
    }
    for (j = 0; j <= nrhs; j++) {
      mn = i * j;
      for (k = 0; k < n; k++) {
        kAcol = m * k;
        i1 = k + mn;
        if (Y_data[i1] != 0.0) {
          i2 = k + 2;
          for (b_i = i2; b_i <= n; b_i++) {
            int i3;
            i3 = (b_i + mn) - 1;
            Y_data[i3] -= Y_data[i1] * b_A_data[(b_i + kAcol) - 1];
          }
        }
      }
    }
    for (j = 0; j <= nrhs; j++) {
      mn = i * j - 1;
      for (k = n; k >= 1; k--) {
        kAcol = m * (k - 1) - 1;
        i1 = k + mn;
        wj = Y_data[i1];
        if (wj != 0.0) {
          Y_data[i1] = wj / b_A_data[k + kAcol];
          for (b_i = 0; b_i <= k - 2; b_i++) {
            i2 = (b_i + mn) + 1;
            Y_data[i2] -= Y_data[i1] * b_A_data[(b_i + kAcol) + 1];
          }
        }
      }
    }
  } else {
    int kAcol;
    int m;
    int mn;
    i = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i);
    b_A_data = b_A->data;
    m = A->size[0] * A->size[1];
    for (i = 0; i < m; i++) {
      b_A_data[i] = A_data[i];
    }
    xgeqp3(b_A, tau, jpvt);
    jpvt_data = jpvt->data;
    tau_data = tau->data;
    b_A_data = b_A->data;
    kAcol = rankFromQR(b_A);
    i = b_B->size[0] * b_B->size[1];
    b_B->size[0] = B->size[0];
    i1 = B->size[1];
    b_B->size[1] = B->size[1];
    emxEnsureCapacity_real_T(b_B, i);
    b_B_data = b_B->data;
    m = B->size[0] * B->size[1];
    for (i = 0; i < m; i++) {
      b_B_data[i] = B_data[i];
    }
    i = Y->size[0] * Y->size[1];
    Y->size[0] = b_A->size[1];
    Y->size[1] = B->size[1];
    emxEnsureCapacity_real_T(Y, i);
    Y_data = Y->data;
    m = b_A->size[1] * B->size[1];
    for (i = 0; i < m; i++) {
      Y_data[i] = 0.0;
    }
    m = b_A->size[0];
    mn = b_A->size[1];
    if (m <= mn) {
      mn = m;
    }
    for (j = 0; j < mn; j++) {
      m = b_A->size[0];
      if (tau_data[j] != 0.0) {
        i = j + 2;
        for (k = 0; k < i1; k++) {
          double wj;
          wj = b_B_data[j + b_B->size[0] * k];
          for (b_i = i; b_i <= m; b_i++) {
            wj += b_A_data[(b_i + b_A->size[0] * j) - 1] *
                  b_B_data[(b_i + b_B->size[0] * k) - 1];
          }
          wj *= tau_data[j];
          if (wj != 0.0) {
            b_B_data[j + b_B->size[0] * k] -= wj;
            for (b_i = i; b_i <= m; b_i++) {
              b_B_data[(b_i + b_B->size[0] * k) - 1] -=
                  b_A_data[(b_i + b_A->size[0] * j) - 1] * wj;
            }
          }
        }
      }
    }
    for (k = 0; k < i1; k++) {
      for (b_i = 0; b_i < kAcol; b_i++) {
        Y_data[(jpvt_data[b_i] + Y->size[0] * k) - 1] =
            b_B_data[b_i + b_B->size[0] * k];
      }
      for (j = kAcol; j >= 1; j--) {
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
