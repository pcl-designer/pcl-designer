/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xzlascl.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xzlascl.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double cfrom
 *                double cto
 *                int m
 *                emxArray_real_T *A
 *                int iA0
 * Return Type  : void
 */
void b_xzlascl(double cfrom, double cto, int m, emxArray_real_T *A, int iA0)
{
  double cfromc;
  double ctoc;
  double *A_data;
  int i;
  bool notdone;
  A_data = A->data;
  cfromc = cfrom;
  ctoc = cto;
  notdone = true;
  while (notdone) {
    double cfrom1;
    double cto1;
    double mul;
    cfrom1 = cfromc * 2.0041683600089728E-292;
    cto1 = ctoc / 4.9896007738368E+291;
    if ((fabs(cfrom1) > fabs(ctoc)) && (ctoc != 0.0)) {
      mul = 2.0041683600089728E-292;
      cfromc = cfrom1;
    } else if (fabs(cto1) > fabs(cfromc)) {
      mul = 4.9896007738368E+291;
      ctoc = cto1;
    } else {
      mul = ctoc / cfromc;
      notdone = false;
    }
    for (i = 0; i < m; i++) {
      int b_i;
      b_i = (iA0 + i) - 1;
      A_data[b_i] *= mul;
    }
  }
}

/*
 * Arguments    : double cfrom
 *                double cto
 *                int m
 *                int n
 *                emxArray_real_T *A
 *                int lda
 * Return Type  : void
 */
void xzlascl(double cfrom, double cto, int m, int n, emxArray_real_T *A,
             int lda)
{
  double cfromc;
  double ctoc;
  double *A_data;
  int i;
  int j;
  bool notdone;
  A_data = A->data;
  cfromc = cfrom;
  ctoc = cto;
  notdone = true;
  while (notdone) {
    double cfrom1;
    double cto1;
    double mul;
    cfrom1 = cfromc * 2.0041683600089728E-292;
    cto1 = ctoc / 4.9896007738368E+291;
    if ((fabs(cfrom1) > fabs(ctoc)) && (ctoc != 0.0)) {
      mul = 2.0041683600089728E-292;
      cfromc = cfrom1;
    } else if (fabs(cto1) > fabs(cfromc)) {
      mul = 4.9896007738368E+291;
      ctoc = cto1;
    } else {
      mul = ctoc / cfromc;
      notdone = false;
    }
    for (j = 0; j < n; j++) {
      int offset;
      offset = j * lda - 1;
      for (i = 0; i < m; i++) {
        int b_i;
        b_i = (offset + i) + 1;
        A_data[b_i] *= mul;
      }
    }
  }
}

/*
 * File trailer for xzlascl.c
 *
 * [EOF]
 */
