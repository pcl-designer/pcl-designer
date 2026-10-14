/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: svd1.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "svd1.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xzsvdc.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *A
 *                emxArray_real_T *U
 *                emxArray_real_T *s
 *                emxArray_real_T *V
 * Return Type  : void
 */
void b_svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *s,
           emxArray_real_T *V)
{
  emxArray_real_T *b_A;
  const double *A_data;
  double *U_data;
  int i;
  int i1;
  int loop_ub;
  A_data = A->data;
  if ((A->size[0] == 0) || (A->size[1] == 0)) {
    int m_tmp;
    m_tmp = A->size[1];
    i = A->size[0];
    i1 = U->size[0] * U->size[1];
    U->size[0] = A->size[0];
    U->size[1] = A->size[0];
    emxEnsureCapacity_real_T(U, i1);
    U_data = U->data;
    loop_ub = A->size[0] * A->size[0];
    for (i1 = 0; i1 < loop_ub; i1++) {
      U_data[i1] = 0.0;
    }
    for (loop_ub = 0; loop_ub < i; loop_ub++) {
      U_data[loop_ub + U->size[0] * loop_ub] = 1.0;
    }
    i = V->size[0] * V->size[1];
    V->size[0] = A->size[1];
    V->size[1] = A->size[1];
    emxEnsureCapacity_real_T(V, i);
    U_data = V->data;
    loop_ub = A->size[1] * A->size[1];
    for (i = 0; i < loop_ub; i++) {
      U_data[i] = 0.0;
    }
    if (A->size[1] > 0) {
      for (loop_ub = 0; loop_ub < m_tmp; loop_ub++) {
        U_data[loop_ub + V->size[0] * loop_ub] = 1.0;
      }
    }
    s->size[0] = 0;
  } else {
    emxInit_real_T(&b_A, 2);
    i = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = A->size[1];
    emxEnsureCapacity_real_T(b_A, i);
    U_data = b_A->data;
    loop_ub = A->size[0] * A->size[1] - 1;
    for (i = 0; i <= loop_ub; i++) {
      U_data[i] = A_data[i];
    }
    xzsvdc(b_A, U, s, V);
    emxFree_real_T(&b_A);
  }
}

/*
 * File trailer for svd1.c
 *
 * [EOF]
 */
