/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xzlarf.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xzlarf.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xgerc.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : int m
 *                int n
 *                int iv0
 *                double tau
 *                emxArray_real_T *C
 *                int ic0
 *                int ldc
 *                emxArray_real_T *work
 * Return Type  : void
 */
void b_xzlarf(int m, int n, int iv0, double tau, emxArray_real_T *C, int ic0,
              int ldc, emxArray_real_T *work)
{
  double *C_data;
  double *work_data;
  int i;
  int ia;
  int iac;
  int iy;
  int lastc;
  int lastv;
  work_data = work->data;
  C_data = C->data;
  if (tau != 0.0) {
    bool exitg2;
    lastv = m;
    i = iv0 + m;
    while ((lastv > 0) && (C_data[i - 2] == 0.0)) {
      lastv--;
      i--;
    }
    lastc = n;
    exitg2 = false;
    while ((!exitg2) && (lastc > 0)) {
      int exitg1;
      i = ic0 + (lastc - 1) * ldc;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (C_data[ia - 1] != 0.0) {
            exitg1 = 1;
          } else {
            ia++;
          }
        } else {
          lastc--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);
      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    lastc = 0;
  }
  if (lastv > 0) {
    if (lastc != 0) {
      for (iy = 0; iy < lastc; iy++) {
        work_data[iy] = 0.0;
      }
      iy = 0;
      i = ic0 + ldc * (lastc - 1);
      for (iac = ic0; ldc < 0 ? iac >= i : iac <= i; iac += ldc) {
        double c;
        int b_i;
        c = 0.0;
        b_i = (iac + lastv) - 1;
        for (ia = iac; ia <= b_i; ia++) {
          c += C_data[ia - 1] * C_data[((iv0 + ia) - iac) - 1];
        }
        work_data[iy] += c;
        iy++;
      }
    }
    xgerc(lastv, lastc, -tau, iv0, work, C, ic0, ldc);
  }
}

/*
 * Arguments    : int m
 *                int n
 *                int iv0
 *                double tau
 *                double C_data[]
 *                int ic0
 *                int ldc
 *                double work_data[]
 * Return Type  : void
 */
void xzlarf(int m, int n, int iv0, double tau, double C_data[], int ic0,
            int ldc, double work_data[])
{
  int i;
  int ia;
  int iac;
  int lastc;
  int lastv;
  int rowright;
  if (tau != 0.0) {
    bool exitg2;
    lastv = n;
    i = iv0 + n;
    while ((lastv > 0) && (C_data[i - 2] == 0.0)) {
      lastv--;
      i--;
    }
    lastc = m;
    exitg2 = false;
    while ((!exitg2) && (lastc > 0)) {
      int exitg1;
      i = (ic0 + lastc) - 1;
      rowright = i + (lastv - 1) * ldc;
      do {
        exitg1 = 0;
        if (((ldc > 0) && (i <= rowright)) || ((ldc < 0) && (i >= rowright))) {
          if (C_data[i - 1] != 0.0) {
            exitg1 = 1;
          } else {
            i += ldc;
          }
        } else {
          lastc--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);
      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    lastc = 0;
  }
  if (lastv > 0) {
    int b_i;
    int i1;
    if (lastc != 0) {
      b_i = (unsigned char)lastc;
      memset(&work_data[0], 0, (unsigned int)b_i * sizeof(double));
      i = iv0;
      b_i = ic0 + ldc * (lastv - 1);
      for (iac = ic0; ldc < 0 ? iac >= b_i : iac <= b_i; iac += ldc) {
        i1 = (iac + lastc) - 1;
        for (ia = iac; ia <= i1; ia++) {
          rowright = ia - iac;
          work_data[rowright] += C_data[ia - 1] * C_data[i - 1];
        }
        i++;
      }
    }
    if (!(-tau == 0.0)) {
      i = ic0;
      b_i = (unsigned char)lastv;
      for (rowright = 0; rowright < b_i; rowright++) {
        double temp;
        temp = C_data[(iv0 + rowright) - 1];
        if (temp != 0.0) {
          temp *= -tau;
          i1 = lastc + i;
          for (iac = i; iac < i1; iac++) {
            C_data[iac - 1] += work_data[iac - i] * temp;
          }
        }
        i += ldc;
      }
    }
  }
}

/*
 * File trailer for xzlarf.c
 *
 * [EOF]
 */
