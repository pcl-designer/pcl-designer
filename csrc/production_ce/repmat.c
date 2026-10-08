/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: repmat.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "repmat.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *a
 *                double varargin_1
 *                emxArray_real_T *b
 * Return Type  : void
 */
void repmat(const emxArray_real_T *a, double varargin_1, emxArray_real_T *b)
{
  const double *a_data;
  double *b_data;
  int i;
  int i1;
  int ibmat;
  int itilerow;
  int jcol;
  a_data = a->data;
  i = (int)varargin_1;
  ibmat = b->size[0] * b->size[1];
  b->size[0] = (int)varargin_1;
  i1 = a->size[1];
  b->size[1] = a->size[1];
  emxEnsureCapacity_real_T(b, ibmat);
  b_data = b->data;
  for (jcol = 0; jcol < i1; jcol++) {
    ibmat = jcol * (int)varargin_1;
    for (itilerow = 0; itilerow < i; itilerow++) {
      b_data[ibmat + itilerow] = a_data[jcol];
    }
  }
}

/*
 * File trailer for repmat.c
 *
 * [EOF]
 */
