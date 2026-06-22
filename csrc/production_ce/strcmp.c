/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: strcmp.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Jun-2026 20:56:24
 */

/* Include Files */
#include "strcmp.h"
#include "DesignWizardVn_App_GapPrimary_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const char a_data[]
 *                const int a_size[2]
 * Return Type  : bool
 */
bool b_strcmp(const char a_data[], const int a_size[2])
{
  static const char b_cv[7] = {'a', 'v', 'e', 'r', 'a', 'g', 'e'};
  bool b_bool;
  b_bool = false;
  if (a_size[1] == 7) {
    int kstr;
    kstr = 0;
    int exitg1;
    do {
      exitg1 = 0;
      if (kstr < 7) {
        if (cv[(unsigned char)a_data[kstr] & 127] != cv[(int)b_cv[kstr]]) {
          exitg1 = 1;
        } else {
          kstr++;
        }
      } else {
        b_bool = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  return b_bool;
}

/*
 * File trailer for strcmp.c
 *
 * [EOF]
 */
