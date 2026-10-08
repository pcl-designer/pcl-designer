/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xgeqp3.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xgeqp3.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "xzlarf.h"
#include "xzlarfg.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *A
 *                emxArray_real_T *tau
 *                emxArray_int32_T *jpvt
 * Return Type  : void
 */
void xgeqp3(emxArray_real_T *A, emxArray_real_T *tau, emxArray_int32_T *jpvt)
{
  emxArray_real_T *vn1;
  emxArray_real_T *vn2;
  emxArray_real_T *work;
  double smax;
  double *A_data;
  double *tau_data;
  double *vn1_data;
  double *vn2_data;
  double *work_data;
  int b_i;
  int i;
  int ix;
  int k;
  int m_tmp;
  int n_tmp;
  int temp_tmp;
  int u1;
  int *jpvt_data;
  A_data = A->data;
  m_tmp = A->size[0];
  n_tmp = A->size[1];
  ix = A->size[0];
  u1 = A->size[1];
  if (ix <= u1) {
    u1 = ix;
  }
  i = tau->size[0];
  tau->size[0] = u1;
  emxEnsureCapacity_real_T(tau, i);
  tau_data = tau->data;
  for (i = 0; i < u1; i++) {
    tau_data[i] = 0.0;
  }
  emxInit_real_T(&work, 1);
  emxInit_real_T(&vn1, 1);
  emxInit_real_T(&vn2, 1);
  if ((A->size[0] == 0) || (A->size[1] == 0) || (u1 < 1)) {
    i = jpvt->size[0] * jpvt->size[1];
    jpvt->size[0] = 1;
    jpvt->size[1] = n_tmp;
    emxEnsureCapacity_int32_T(jpvt, i);
    jpvt_data = jpvt->data;
    for (temp_tmp = 0; temp_tmp < n_tmp; temp_tmp++) {
      jpvt_data[temp_tmp] = temp_tmp + 1;
    }
  } else {
    double d;
    i = jpvt->size[0] * jpvt->size[1];
    jpvt->size[0] = 1;
    jpvt->size[1] = n_tmp;
    emxEnsureCapacity_int32_T(jpvt, i);
    jpvt_data = jpvt->data;
    i = work->size[0];
    work->size[0] = n_tmp;
    emxEnsureCapacity_real_T(work, i);
    work_data = work->data;
    i = vn1->size[0];
    vn1->size[0] = n_tmp;
    emxEnsureCapacity_real_T(vn1, i);
    vn1_data = vn1->data;
    i = vn2->size[0];
    vn2->size[0] = n_tmp;
    emxEnsureCapacity_real_T(vn2, i);
    vn2_data = vn2->data;
    for (k = 0; k < n_tmp; k++) {
      jpvt_data[k] = k + 1;
      work_data[k] = 0.0;
      d = xnrm2(m_tmp, A, k * m_tmp + 1);
      vn1_data[k] = d;
      vn2_data[k] = d;
    }
    for (b_i = 0; b_i < u1; b_i++) {
      double s;
      int ii;
      int ii_tmp;
      int ip1;
      int mmi;
      int nmi;
      int pvt;
      ip1 = b_i + 2;
      ii_tmp = b_i * m_tmp;
      ii = ii_tmp + b_i;
      nmi = n_tmp - b_i;
      mmi = m_tmp - b_i;
      if (nmi < 1) {
        ix = -1;
      } else {
        ix = 0;
        if (nmi > 1) {
          smax = fabs(vn1_data[b_i]);
          for (k = 2; k <= nmi; k++) {
            s = fabs(vn1_data[(b_i + k) - 1]);
            if (s > smax) {
              ix = k - 1;
              smax = s;
            }
          }
        }
      }
      pvt = b_i + ix;
      if (pvt + 1 != b_i + 1) {
        ix = pvt * m_tmp;
        for (k = 0; k < m_tmp; k++) {
          temp_tmp = ix + k;
          smax = A_data[temp_tmp];
          i = ii_tmp + k;
          A_data[temp_tmp] = A_data[i];
          A_data[i] = smax;
        }
        ix = jpvt_data[pvt];
        jpvt_data[pvt] = jpvt_data[b_i];
        jpvt_data[b_i] = ix;
        vn1_data[pvt] = vn1_data[b_i];
        vn2_data[pvt] = vn2_data[b_i];
      }
      if (b_i + 1 < m_tmp) {
        smax = A_data[ii];
        d = xzlarfg(mmi, &smax, A, ii + 2);
        A_data = A->data;
        tau_data[b_i] = d;
        A_data[ii] = smax;
      } else {
        d = 0.0;
        tau_data[b_i] = 0.0;
      }
      if (b_i + 1 < n_tmp) {
        smax = A_data[ii];
        A_data[ii] = 1.0;
        b_xzlarf(mmi, nmi - 1, ii + 1, d, A, (ii + m_tmp) + 1, m_tmp, work);
        A_data = A->data;
        A_data[ii] = smax;
      }
      for (temp_tmp = ip1; temp_tmp <= n_tmp; temp_tmp++) {
        ix = b_i + (temp_tmp - 1) * m_tmp;
        d = vn1_data[temp_tmp - 1];
        if (d != 0.0) {
          smax = fabs(A_data[ix]) / d;
          smax = 1.0 - smax * smax;
          if (smax < 0.0) {
            smax = 0.0;
          }
          s = d / vn2_data[temp_tmp - 1];
          s = smax * (s * s);
          if (s <= 1.4901161193847656E-8) {
            if (b_i + 1 < m_tmp) {
              d = xnrm2(mmi - 1, A, ix + 2);
              vn1_data[temp_tmp - 1] = d;
              vn2_data[temp_tmp - 1] = d;
            } else {
              vn1_data[temp_tmp - 1] = 0.0;
              vn2_data[temp_tmp - 1] = 0.0;
            }
          } else {
            vn1_data[temp_tmp - 1] = d * sqrt(smax);
          }
        }
      }
    }
  }
  emxFree_real_T(&vn2);
  emxFree_real_T(&vn1);
  emxFree_real_T(&work);
}

/*
 * File trailer for xgeqp3.c
 *
 * [EOF]
 */
