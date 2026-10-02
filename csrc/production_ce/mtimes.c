/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: mtimes.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Oct-2026 20:44:50
 */

/* Include Files */
#include "mtimes.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *A
 *                const emxArray_real_T *B
 *                emxArray_real_T *C
 * Return Type  : void
 */
void b_mtimes(const emxArray_real_T *A, const emxArray_real_T *B,
              emxArray_real_T *C)
{
  const double *A_data;
  const double *B_data;
  double *C_data;
  int aoffset;
  int i;
  int inner;
  int k;
  int mc;
  B_data = B->data;
  A_data = A->data;
  mc = A->size[0] - 1;
  inner = A->size[1];
  aoffset = C->size[0];
  C->size[0] = A->size[0];
  emxEnsureCapacity_real_T(C, aoffset);
  C_data = C->data;
  for (i = 0; i <= mc; i++) {
    C_data[i] = 0.0;
  }
  for (k = 0; k < inner; k++) {
    aoffset = k * A->size[0];
    for (i = 0; i <= mc; i++) {
      C_data[i] += A_data[aoffset + i] * B_data[k];
    }
  }
}

/*
 * Arguments    : const emxArray_real_T *A
 *                const emxArray_real_T *B
 *                emxArray_real_T *C
 * Return Type  : void
 */
void mtimes(const emxArray_real_T *A, const emxArray_real_T *B,
            emxArray_real_T *C)
{
  const double *A_data;
  const double *B_data;
  double *C_data;
  int b_i;
  int i;
  int inner;
  int j;
  int k;
  int mc_tmp;
  int nc_tmp;
  B_data = B->data;
  A_data = A->data;
  mc_tmp = A->size[0];
  inner = A->size[1];
  nc_tmp = B->size[1];
  i = C->size[0] * C->size[1];
  C->size[0] = A->size[0];
  C->size[1] = B->size[1];
  emxEnsureCapacity_real_T(C, i);
  C_data = C->data;
  for (j = 0; j < nc_tmp; j++) {
    int boffset;
    int coffset;
    coffset = j * mc_tmp;
    boffset = j * B->size[0];
    for (b_i = 0; b_i < mc_tmp; b_i++) {
      C_data[coffset + b_i] = 0.0;
    }
    for (k = 0; k < inner; k++) {
      double bkj;
      int aoffset;
      aoffset = k * A->size[0];
      bkj = B_data[boffset + k];
      for (b_i = 0; b_i < mc_tmp; b_i++) {
        i = coffset + b_i;
        C_data[i] += A_data[aoffset + b_i] * bkj;
      }
    }
  }
}

/*
 * File trailer for mtimes.c
 *
 * [EOF]
 */
