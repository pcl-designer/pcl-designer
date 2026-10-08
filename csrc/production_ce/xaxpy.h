/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xaxpy.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

#ifndef XAXPY_H
#define XAXPY_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_xaxpy(int n, double a, int ix0, emxArray_real_T *y, int iy0);

void xaxpy(int n, double a, const emxArray_real_T *x, int ix0,
           emxArray_real_T *y, int iy0);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for xaxpy.h
 *
 * [EOF]
 */
