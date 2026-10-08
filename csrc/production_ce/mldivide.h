/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: mldivide.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

#ifndef MLDIVIDE_H
#define MLDIVIDE_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_mldivide(const double A_data[], const int A_size[2], double B_data[],
                int *B_size);

void mldivide(const emxArray_real_T *A, const emxArray_real_T *B,
              emxArray_real_T *Y);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for mldivide.h
 *
 * [EOF]
 */
