/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xorgqr.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xorgqr.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xzlarf.h"

/* Function Definitions */
/*
 * Arguments    : int m
 *                int n
 *                int k
 *                emxArray_real_T *A
 *                int lda
 *                const emxArray_real_T *tau
 * Return Type  : void
 */
void xorgqr(int m, int n, int k, emxArray_real_T *A, int lda,
            const emxArray_real_T *tau)
{
  emxArray_real_T *work;
  const double *tau_data;
  double *A_data;
  double *work_data;
  int b_i;
  int i;
  int j;
  tau_data = tau->data;
  A_data = A->data;
  if (n >= 1) {
    int ia;
    int iaii;
    int itau;
    i = n - 1;
    for (j = k; j <= i; j++) {
      ia = j * lda;
      iaii = m - 1;
      for (b_i = 0; b_i <= iaii; b_i++) {
        A_data[ia + b_i] = 0.0;
      }
      A_data[ia + j] = 1.0;
    }
    itau = k - 1;
    emxInit_real_T(&work, 1);
    ia = A->size[1];
    i = work->size[0];
    work->size[0] = ia;
    emxEnsureCapacity_real_T(work, i);
    work_data = work->data;
    for (i = 0; i < ia; i++) {
      work_data[i] = 0.0;
    }
    for (b_i = k; b_i >= 1; b_i--) {
      iaii = b_i + (b_i - 1) * lda;
      if (b_i < n) {
        A_data[iaii - 1] = 1.0;
        b_xzlarf((m - b_i) + 1, n - b_i, iaii, tau_data[itau], A, iaii + lda,
                 lda, work);
        A_data = A->data;
      }
      if (b_i < m) {
        ia = iaii + 1;
        i = (iaii + m) - b_i;
        for (j = ia; j <= i; j++) {
          A_data[j - 1] *= -tau_data[itau];
        }
      }
      A_data[iaii - 1] = 1.0 - tau_data[itau];
      for (j = 0; j <= b_i - 2; j++) {
        A_data[(iaii - j) - 2] = 0.0;
      }
      itau--;
    }
    emxFree_real_T(&work);
  }
}

/*
 * File trailer for xorgqr.c
 *
 * [EOF]
 */
