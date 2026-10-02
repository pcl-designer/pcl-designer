/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xgeqp3.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Oct-2026 20:44:50
 */

/* Include Files */
#include "xgeqp3.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = rtNaN;
  } else {
    y = a * 1.4142135623730951;
  }
  return y;
}

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
  double *A_data;
  double *tau_data;
  double *vn1_data;
  double *vn2_data;
  double *work_data;
  int b_i;
  int i;
  int ia;
  int ix;
  int jA;
  int knt;
  int m_tmp;
  int n_tmp;
  int nmi;
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
    for (nmi = 0; nmi < n_tmp; nmi++) {
      jpvt_data[nmi] = nmi + 1;
    }
  } else {
    double smax;
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
    for (jA = 0; jA < n_tmp; jA++) {
      jpvt_data[jA] = jA + 1;
      work_data[jA] = 0.0;
      smax = xnrm2(m_tmp, A, jA * m_tmp + 1);
      vn1_data[jA] = smax;
      vn2_data[jA] = smax;
    }
    for (b_i = 0; b_i < u1; b_i++) {
      double s;
      double temp2;
      int ii;
      int ip1;
      int lastc;
      int mmi;
      int pvt;
      ip1 = b_i + 2;
      lastc = b_i * m_tmp;
      ii = lastc + b_i;
      nmi = n_tmp - b_i;
      mmi = m_tmp - b_i;
      if (nmi < 1) {
        ix = -1;
      } else {
        ix = 0;
        if (nmi > 1) {
          smax = fabs(vn1_data[b_i]);
          for (jA = 2; jA <= nmi; jA++) {
            s = fabs(vn1_data[(b_i + jA) - 1]);
            if (s > smax) {
              ix = jA - 1;
              smax = s;
            }
          }
        }
      }
      pvt = b_i + ix;
      if (pvt + 1 != b_i + 1) {
        ix = pvt * m_tmp;
        for (jA = 0; jA < m_tmp; jA++) {
          knt = ix + jA;
          smax = A_data[knt];
          i = lastc + jA;
          A_data[knt] = A_data[i];
          A_data[i] = smax;
        }
        ix = jpvt_data[pvt];
        jpvt_data[pvt] = jpvt_data[b_i];
        jpvt_data[b_i] = ix;
        vn1_data[pvt] = vn1_data[b_i];
        vn2_data[pvt] = vn2_data[b_i];
      }
      if (b_i + 1 < m_tmp) {
        temp2 = A_data[ii];
        ix = ii + 2;
        tau_data[b_i] = 0.0;
        if (mmi > 0) {
          smax = xnrm2(mmi - 1, A, ii + 2);
          if (smax != 0.0) {
            s = rt_hypotd_snf(A_data[ii], smax);
            if (A_data[ii] >= 0.0) {
              s = -s;
            }
            if (fabs(s) < 1.0020841800044864E-292) {
              knt = 0;
              i = ii + mmi;
              do {
                knt++;
                for (jA = ix; jA <= i; jA++) {
                  A_data[jA - 1] *= 9.9792015476736E+291;
                }
                s *= 9.9792015476736E+291;
                temp2 *= 9.9792015476736E+291;
              } while ((fabs(s) < 1.0020841800044864E-292) && (knt < 20));
              s = rt_hypotd_snf(temp2, xnrm2(mmi - 1, A, ii + 2));
              if (temp2 >= 0.0) {
                s = -s;
              }
              tau_data[b_i] = (s - temp2) / s;
              smax = 1.0 / (temp2 - s);
              for (jA = ix; jA <= i; jA++) {
                A_data[jA - 1] *= smax;
              }
              for (jA = 0; jA < knt; jA++) {
                s *= 1.0020841800044864E-292;
              }
              temp2 = s;
            } else {
              tau_data[b_i] = (s - A_data[ii]) / s;
              smax = 1.0 / (A_data[ii] - s);
              i = ii + mmi;
              for (jA = ix; jA <= i; jA++) {
                A_data[jA - 1] *= smax;
              }
              temp2 = s;
            }
          }
        }
        A_data[ii] = temp2;
      } else {
        tau_data[b_i] = 0.0;
      }
      if (b_i + 1 < n_tmp) {
        temp2 = A_data[ii];
        A_data[ii] = 1.0;
        jA = (ii + m_tmp) + 1;
        if (tau_data[b_i] != 0.0) {
          bool exitg2;
          pvt = mmi - 1;
          ix = (ii + mmi) - 1;
          while ((pvt + 1 > 0) && (A_data[ix] == 0.0)) {
            pvt--;
            ix--;
          }
          lastc = nmi - 2;
          exitg2 = false;
          while ((!exitg2) && (lastc + 1 > 0)) {
            int exitg1;
            ix = jA + lastc * m_tmp;
            ia = ix;
            do {
              exitg1 = 0;
              if (ia <= ix + pvt) {
                if (A_data[ia - 1] != 0.0) {
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
          pvt = -1;
          lastc = -1;
        }
        if (pvt + 1 > 0) {
          if (lastc + 1 != 0) {
            for (ix = 0; ix <= lastc; ix++) {
              work_data[ix] = 0.0;
            }
            ix = 0;
            i = jA + m_tmp * lastc;
            for (nmi = jA; m_tmp < 0 ? nmi >= i : nmi <= i; nmi += m_tmp) {
              smax = 0.0;
              knt = nmi + pvt;
              for (ia = nmi; ia <= knt; ia++) {
                smax += A_data[ia - 1] * A_data[(ii + ia) - nmi];
              }
              work_data[ix] += smax;
              ix++;
            }
          }
          if (!(-tau_data[b_i] == 0.0)) {
            for (nmi = 0; nmi <= lastc; nmi++) {
              if (work_data[nmi] != 0.0) {
                smax = work_data[nmi] * -tau_data[b_i];
                i = pvt + jA;
                for (knt = jA; knt <= i; knt++) {
                  A_data[knt - 1] += A_data[(ii + knt) - jA] * smax;
                }
              }
              jA += m_tmp;
            }
          }
        }
        A_data[ii] = temp2;
      }
      for (nmi = ip1; nmi <= n_tmp; nmi++) {
        ix = b_i + (nmi - 1) * m_tmp;
        smax = vn1_data[nmi - 1];
        if (smax != 0.0) {
          s = fabs(A_data[ix]) / smax;
          s = 1.0 - s * s;
          if (s < 0.0) {
            s = 0.0;
          }
          temp2 = smax / vn2_data[nmi - 1];
          temp2 = s * (temp2 * temp2);
          if (temp2 <= 1.4901161193847656E-8) {
            if (b_i + 1 < m_tmp) {
              smax = xnrm2(mmi - 1, A, ix + 2);
              vn1_data[nmi - 1] = smax;
              vn2_data[nmi - 1] = smax;
            } else {
              vn1_data[nmi - 1] = 0.0;
              vn2_data[nmi - 1] = 0.0;
            }
          } else {
            vn1_data[nmi - 1] = smax * sqrt(s);
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
