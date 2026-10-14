/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: svd.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "svd.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "svd1.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *A
 *                emxArray_real_T *U
 *                emxArray_real_T *S
 *                emxArray_real_T *V
 * Return Type  : void
 */
void svd(const emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *S,
         emxArray_real_T *V)
{
  emxArray_real_T *U1;
  emxArray_real_T *V1;
  emxArray_real_T *r;
  emxArray_real_T *s;
  const double *A_data;
  double *U_data;
  double *s_data;
  int i;
  int k;
  int nx;
  bool p;
  A_data = A->data;
  nx = A->size[0] * A->size[1];
  p = true;
  for (k = 0; k < nx; k++) {
    if ((!p) || (rtIsInf(A_data[k]) || rtIsNaN(A_data[k]))) {
      p = false;
    }
  }
  emxInit_real_T(&s, 1);
  if (p) {
    b_svd(A, U, s, V);
    s_data = s->data;
  } else {
    emxInit_real_T(&r, 2);
    i = r->size[0] * r->size[1];
    r->size[0] = A->size[0];
    r->size[1] = A->size[1];
    emxEnsureCapacity_real_T(r, i);
    U_data = r->data;
    nx = A->size[0] * A->size[1];
    for (i = 0; i < nx; i++) {
      U_data[i] = 0.0;
    }
    emxInit_real_T(&U1, 2);
    emxInit_real_T(&V1, 2);
    b_svd(r, U1, s, V1);
    emxFree_real_T(&r);
    i = U->size[0] * U->size[1];
    U->size[0] = U1->size[0];
    U->size[1] = U1->size[1];
    emxEnsureCapacity_real_T(U, i);
    U_data = U->data;
    nx = U1->size[0] * U1->size[1];
    emxFree_real_T(&U1);
    for (i = 0; i < nx; i++) {
      U_data[i] = rtNaN;
    }
    nx = s->size[0];
    i = s->size[0];
    s->size[0] = nx;
    emxEnsureCapacity_real_T(s, i);
    s_data = s->data;
    for (i = 0; i < nx; i++) {
      s_data[i] = rtNaN;
    }
    i = V->size[0] * V->size[1];
    V->size[0] = V1->size[0];
    V->size[1] = V1->size[1];
    emxEnsureCapacity_real_T(V, i);
    U_data = V->data;
    nx = V1->size[0] * V1->size[1];
    emxFree_real_T(&V1);
    for (i = 0; i < nx; i++) {
      U_data[i] = rtNaN;
    }
  }
  i = S->size[0] * S->size[1];
  S->size[0] = U->size[1];
  S->size[1] = V->size[1];
  emxEnsureCapacity_real_T(S, i);
  U_data = S->data;
  nx = U->size[1] * V->size[1];
  for (i = 0; i < nx; i++) {
    U_data[i] = 0.0;
  }
  i = s->size[0] - 1;
  for (k = 0; k <= i; k++) {
    U_data[k + S->size[0] * k] = s_data[k];
  }
  emxFree_real_T(&s);
}

/*
 * File trailer for svd.c
 *
 * [EOF]
 */
