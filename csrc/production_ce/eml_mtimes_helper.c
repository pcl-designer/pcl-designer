/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_mtimes_helper.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Jun-2026 20:56:24
 */

/* Include Files */
#include "eml_mtimes_helper.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *in1
 * Return Type  : void
 */
void binary_expand_op_4(emxArray_real_T *in1)
{
  emxArray_real_T *r;
  double *in1_data;
  double *r1;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0_tmp;
  int stride_0_1_tmp;
  in1_data = in1->data;
  emxInit_real_T(&r, 2);
  if (in1->size[1] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = in1->size[1];
  }
  i = r->size[0] * r->size[1];
  r->size[0] = loop_ub;
  if (in1->size[0] == 1) {
    b_loop_ub = in1->size[1];
  } else {
    b_loop_ub = in1->size[0];
  }
  r->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(r, i);
  r1 = r->data;
  stride_0_0_tmp = (in1->size[0] != 1);
  stride_0_1_tmp = (in1->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      r1[i1 + r->size[0] * i] =
          0.5 * (in1_data[i1 * stride_0_0_tmp + in1->size[0] * aux_0_1] +
                 in1_data[aux_1_1 + in1->size[0] * (i1 * stride_0_1_tmp)]);
    }
    aux_1_1 += stride_0_0_tmp;
    aux_0_1 += stride_0_1_tmp;
  }
  i = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = r1[i1 + r->size[0] * i];
    }
  }
  emxFree_real_T(&r);
}

/*
 * File trailer for eml_mtimes_helper.c
 *
 * [EOF]
 */
