/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: sortIdx.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 03-Oct-2026 17:22:39
 */

#ifndef SORTIDX_H
#define SORTIDX_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int n,
                 int preSortLevel, emxArray_int32_T *iwork,
                 emxArray_real_T *xwork);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for sortIdx.h
 *
 * [EOF]
 */
