/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xdlahqr.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

#ifndef XDLAHQR_H
#define XDLAHQR_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
int b_xdlahqr(int ihi, double h_data[], const int h_size[2], double wr_data[],
              int *wr_size, double wi_data[], int *wi_size);

int xdlahqr(int ilo, int ihi, double h_data[], const int h_size[2],
            double wr_data[], int *wr_size, double wi_data[], int *wi_size);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for xdlahqr.h
 *
 * [EOF]
 */
