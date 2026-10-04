/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: rng.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 03-Oct-2026 17:22:39
 */

/* Include Files */
#include "rng.h"
#include "DesignWizardVn_App_GapPrimary_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : double varargin_1
 * Return Type  : void
 */
void rng(double varargin_1)
{
  int mti;
  unsigned int r;
  if (varargin_1 < 4.294967296E+9) {
    if (varargin_1 >= 0.0) {
      r = (unsigned int)varargin_1;
    } else {
      r = 0U;
    }
  } else if (varargin_1 >= 4.294967296E+9) {
    r = MAX_uint32_T;
  } else {
    r = 0U;
  }
  if (r == 0U) {
    r = 5489U;
  }
  state[0] = r;
  for (mti = 0; mti < 623; mti++) {
    r = ((r ^ r >> 30U) * 1812433253U + (unsigned int)mti) + 1U;
    state[mti + 1] = r;
  }
  state[624] = 624U;
}

/*
 * File trailer for rng.c
 *
 * [EOF]
 */
