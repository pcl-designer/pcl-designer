/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: DesignWizardVn_App_GapPrimary_emxAPI.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Oct-2026 20:44:50
 */

#ifndef DESIGNWIZARDVN_APP_GAPPRIMARY_EMXAPI_H
#define DESIGNWIZARDVN_APP_GAPPRIMARY_EMXAPI_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern emxArray_cell_wrap_0 *emxCreateND_cell_wrap_0(int numDimensions,
                                                     const int *size);

extern emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size);

extern emxArray_cell_wrap_0 *emxCreateWrapperND_cell_wrap_0(cell_wrap_0 *data,
                                                            int numDimensions,
                                                            const int *size);

extern emxArray_real_T *
emxCreateWrapperND_real_T(double *data, int numDimensions, const int *size);

extern emxArray_cell_wrap_0 *emxCreateWrapper_cell_wrap_0(cell_wrap_0 *data,
                                                          int rows, int cols);

extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows,
                                                int cols);

extern emxArray_cell_wrap_0 *emxCreate_cell_wrap_0(int rows, int cols);

extern emxArray_real_T *emxCreate_real_T(int rows, int cols);

extern void emxDestroyArray_cell_wrap_0(emxArray_cell_wrap_0 *emxArray);

extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);

extern void
emxDestroy_b_emxArray_cell_wrap_0_1x20(emxArray_cell_wrap_0_1x20 emxArray);

extern void emxInitArray_cell_wrap_0(emxArray_cell_wrap_0 **pEmxArray,
                                     int numDimensions);

extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

extern void
emxInit_b_emxArray_cell_wrap_0_1x20(emxArray_cell_wrap_0_1x20 *pEmxArray);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for DesignWizardVn_App_GapPrimary_emxAPI.h
 *
 * [EOF]
 */
