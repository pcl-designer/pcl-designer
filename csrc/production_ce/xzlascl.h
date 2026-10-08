/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xzlascl.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

#ifndef XZLASCL_H
#define XZLASCL_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_xzlascl(double cfrom, double cto, int m, emxArray_real_T *A, int iA0);

void xzlascl(double cfrom, double cto, int m, int n, emxArray_real_T *A,
             int lda);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for xzlascl.h
 *
 * [EOF]
 */
