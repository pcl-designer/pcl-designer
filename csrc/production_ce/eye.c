/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: eye.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "eye.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : double varargin_1
 *                emxArray_real_T *b_I
 * Return Type  : void
 */
void eye(double varargin_1, emxArray_real_T *b_I)
{
  double t;
  double *I_data;
  int i;
  int loop_ub;
  int m_tmp;
  if (varargin_1 < 0.0) {
    t = 0.0;
  } else {
    t = varargin_1;
  }
  m_tmp = (int)t;
  i = b_I->size[0] * b_I->size[1];
  b_I->size[0] = (int)t;
  b_I->size[1] = (int)t;
  emxEnsureCapacity_real_T(b_I, i);
  I_data = b_I->data;
  loop_ub = (int)t * (int)t;
  for (i = 0; i < loop_ub; i++) {
    I_data[i] = 0.0;
  }
  if ((int)t > 0) {
    for (loop_ub = 0; loop_ub < m_tmp; loop_ub++) {
      I_data[loop_ub + b_I->size[0] * loop_ub] = 1.0;
    }
  }
}

/*
 * File trailer for eye.c
 *
 * [EOF]
 */
