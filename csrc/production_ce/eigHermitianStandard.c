/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: eigHermitianStandard.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "eigHermitianStandard.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xdsterf.h"
#include "xzlarfg.h"
#include "xzlascl.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double A_data[]
 *                const int A_size[2]
 *                creal_T V_data[]
 * Return Type  : int
 */
int eigHermitianStandard(const double A_data[], const int A_size[2],
                         creal_T V_data[])
{
  emxArray_real_T *A;
  emxArray_real_T *y;
  double W_data[9];
  double tmp_data[9];
  double e_data[8];
  double *b_A_data;
  double *y_data;
  int V_size;
  int b_i;
  int b_taui_tmp;
  int i;
  int ii;
  int jj;
  int loop_ub;
  int loop_ub_tmp;
  emxInit_real_T(&A, 2);
  loop_ub = A_size[0];
  i = A->size[0] * A->size[1];
  A->size[0] = A_size[0];
  A->size[1] = A_size[1];
  emxEnsureCapacity_real_T(A, i);
  b_A_data = A->data;
  loop_ub_tmp = A_size[0] * A_size[1];
  for (i = 0; i < loop_ub_tmp; i++) {
    b_A_data[i] = A_data[i];
  }
  V_size = A_size[0];
  emxInit_real_T(&y, 1);
  if (A_size[0] == 1) {
    W_data[0] = A_data[0];
  } else {
    double absx;
    double anrm;
    int iv;
    bool exitg2;
    anrm = 0.0;
    iv = 0;
    exitg2 = false;
    while ((!exitg2) && (iv <= A_size[0] - 1)) {
      int exitg1;
      b_i = 0;
      do {
        exitg1 = 0;
        if (b_i <= iv) {
          absx = fabs(A_data[b_i + A_size[0] * iv]);
          if (rtIsNaN(absx)) {
            anrm = rtNaN;
            exitg1 = 1;
          } else {
            if (absx > anrm) {
              anrm = absx;
            }
            b_i++;
          }
        } else {
          iv++;
          exitg1 = 2;
        }
      } while (exitg1 == 0);
      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
    if (rtIsInf(anrm) || rtIsNaN(anrm)) {
      V_size = A_size[0];
      for (i = 0; i < loop_ub; i++) {
        W_data[i] = rtNaN;
      }
    } else {
      int e_size;
      int n_tmp;
      bool iscale;
      iscale = false;
      if ((anrm > 0.0) && (anrm < 1.0010415475915505E-146)) {
        iscale = true;
        anrm = 1.0010415475915505E-146 / anrm;
        i = A->size[0] * A->size[1];
        A->size[0] = A_size[0];
        A->size[1] = A_size[1];
        emxEnsureCapacity_real_T(A, i);
        b_A_data = A->data;
        for (i = 0; i < loop_ub_tmp; i++) {
          b_A_data[i] = A_data[i];
        }
        xzlascl(1.0, anrm, A_size[0], A_size[0], A, A_size[0]);
        b_A_data = A->data;
      }
      n_tmp = A->size[0] - 1;
      V_size = A->size[0];
      e_size = A->size[0] - 1;
      i = y->size[0];
      y->size[0] = A->size[0] - 1;
      emxEnsureCapacity_real_T(y, i);
      y_data = y->data;
      i = (unsigned char)(A->size[0] - 1);
      for (b_i = 0; b_i < i; b_i++) {
        double taui;
        int taui_tmp;
        e_data[b_i] = b_A_data[(b_i + A->size[0] * b_i) + 1];
        taui_tmp = n_tmp - b_i;
        b_taui_tmp = b_i * (n_tmp + 1);
        iv = b_i + 3;
        loop_ub_tmp = n_tmp + 1;
        if (iv <= loop_ub_tmp) {
          loop_ub_tmp = iv;
        }
        taui = xzlarfg(taui_tmp, &e_data[b_i], A, b_taui_tmp + loop_ub_tmp);
        b_A_data = A->data;
        if (taui != 0.0) {
          double d;
          double temp1;
          int i1;
          b_A_data[(b_i + A->size[0] * b_i) + 1] = 1.0;
          for (loop_ub_tmp = b_i + 1; loop_ub_tmp <= n_tmp; loop_ub_tmp++) {
            y_data[loop_ub_tmp - 1] = 0.0;
          }
          for (jj = 0; jj < taui_tmp; jj++) {
            loop_ub_tmp = b_i + jj;
            temp1 = taui * b_A_data[(loop_ub_tmp + A->size[0] * b_i) + 1];
            absx = 0.0;
            y_data[loop_ub_tmp] +=
                temp1 *
                b_A_data[(loop_ub_tmp + A->size[0] * (loop_ub_tmp + 1)) + 1];
            i1 = jj + 2;
            for (ii = i1; ii <= taui_tmp; ii++) {
              iv = b_i + ii;
              d = b_A_data[iv + A->size[0] * (loop_ub_tmp + 1)];
              y_data[iv - 1] += temp1 * d;
              absx += d * b_A_data[iv + A->size[0] * b_i];
            }
            y_data[loop_ub_tmp] += taui * absx;
          }
          iv = b_taui_tmp + b_i;
          absx = 0.0;
          if (taui_tmp >= 1) {
            for (b_taui_tmp = 0; b_taui_tmp < taui_tmp; b_taui_tmp++) {
              absx +=
                  y_data[b_i + b_taui_tmp] * b_A_data[(iv + b_taui_tmp) + 1];
            }
          }
          absx *= -0.5 * taui;
          if ((taui_tmp >= 1) && (!(absx == 0.0))) {
            i1 = taui_tmp - 1;
            for (b_taui_tmp = 0; b_taui_tmp <= i1; b_taui_tmp++) {
              loop_ub_tmp = b_i + b_taui_tmp;
              y_data[loop_ub_tmp] += absx * b_A_data[(iv + b_taui_tmp) + 1];
            }
          }
          for (jj = 0; jj < taui_tmp; jj++) {
            loop_ub_tmp = b_i + jj;
            temp1 = b_A_data[(loop_ub_tmp + A->size[0] * b_i) + 1];
            absx = y_data[loop_ub_tmp];
            d = absx * temp1;
            b_A_data[(loop_ub_tmp + A->size[0] * (loop_ub_tmp + 1)) + 1] =
                (b_A_data[(loop_ub_tmp + A->size[0] * (loop_ub_tmp + 1)) + 1] -
                 d) -
                d;
            i1 = jj + 2;
            for (ii = i1; ii <= taui_tmp; ii++) {
              iv = b_i + ii;
              b_A_data[iv + A->size[0] * (loop_ub_tmp + 1)] =
                  (b_A_data[iv + A->size[0] * (loop_ub_tmp + 1)] -
                   y_data[iv - 1] * temp1) -
                  b_A_data[iv + A->size[0] * b_i] * absx;
            }
          }
        }
        b_A_data[(b_i + A->size[0] * b_i) + 1] = e_data[b_i];
        W_data[b_i] = b_A_data[b_i + A->size[0] * b_i];
        y_data[b_i] = taui;
      }
      W_data[A->size[0] - 1] =
          b_A_data[(A->size[0] + A->size[0] * (A->size[0] - 1)) - 1];
      iv = e_size - 1;
      if (iv >= 0) {
        memcpy(&tmp_data[0], &e_data[0],
               (unsigned int)(iv + 1) * sizeof(double));
      }
      iv = xdsterf(W_data, &V_size, tmp_data, e_size);
      if (iv != 0) {
        for (i = 0; i < V_size; i++) {
          W_data[i] = rtNaN;
        }
      } else if (iscale) {
        absx = 1.0 / anrm;
        for (b_taui_tmp = 0; b_taui_tmp < loop_ub; b_taui_tmp++) {
          W_data[b_taui_tmp] *= absx;
        }
      }
    }
  }
  emxFree_real_T(&y);
  emxFree_real_T(&A);
  for (i = 0; i < V_size; i++) {
    V_data[i].re = W_data[i];
    V_data[i].im = 0.0;
  }
  return V_size;
}

/*
 * File trailer for eigHermitianStandard.c
 *
 * [EOF]
 */
