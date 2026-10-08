/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xzlarf.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

#ifndef XZLARF_H
#define XZLARF_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_xzlarf(int m, int n, int iv0, double tau, emxArray_real_T *C, int ic0,
              int ldc, emxArray_real_T *work);

void xzlarf(int m, int n, int iv0, double tau, double C_data[], int ic0,
            int ldc, double work_data[]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for xzlarf.h
 *
 * [EOF]
 */
