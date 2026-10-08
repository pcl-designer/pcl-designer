/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: DesignWizardVn_App_GapPrimary_emxutil.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

#ifndef DESIGNWIZARDVN_APP_GAPPRIMARY_EMXUTIL_H
#define DESIGNWIZARDVN_APP_GAPPRIMARY_EMXUTIL_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray,
                                        int oldNumel);

extern void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_int8_T(emxArray_int8_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_uint32_T(emxArray_uint32_T *emxArray,
                                       int oldNumel);

extern void emxFreeStruct_cell_wrap_0(cell_wrap_0 *pStruct);

extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);

extern void emxFree_cell_wrap_0(emxArray_cell_wrap_0 **pEmxArray);

extern void emxFree_cell_wrap_0_1x20(emxArray_cell_wrap_0_1x20 *pEmxArray);

extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);

extern void emxFree_int8_T(emxArray_int8_T **pEmxArray);

extern void emxFree_real_T(emxArray_real_T **pEmxArray);

extern void emxFree_uint32_T(emxArray_uint32_T **pEmxArray);

extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray);

extern void emxInit_cell_wrap_0(emxArray_cell_wrap_0 **pEmxArray,
                                int numDimensions);

extern void emxInit_cell_wrap_0_1x20(emxArray_cell_wrap_0_1x20 *pEmxArray);

extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);

extern void emxInit_int8_T(emxArray_int8_T **pEmxArray);

extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

extern void emxInit_uint32_T(emxArray_uint32_T **pEmxArray, int numDimensions);

extern void emxReserve_real_T(emxArray_real_T *emxArray);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for DesignWizardVn_App_GapPrimary_emxutil.h
 *
 * [EOF]
 */
