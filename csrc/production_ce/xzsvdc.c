/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xzsvdc.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xzsvdc.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xaxpy.h"
#include "xnrm2.h"
#include "xrotg.h"
#include "xzlangeM.h"
#include "xzlascl.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *A
 *                emxArray_real_T *U
 *                emxArray_real_T *S
 *                emxArray_real_T *V
 * Return Type  : void
 */
void xzsvdc(emxArray_real_T *A, emxArray_real_T *U, emxArray_real_T *S,
            emxArray_real_T *V)
{
  emxArray_real_T *e;
  emxArray_real_T *s;
  emxArray_real_T *work;
  double anrm;
  double cscale;
  double nrm;
  double r;
  double sm;
  double *A_data;
  double *U_data;
  double *V_data;
  double *e_data;
  double *s_data;
  double *work_data;
  int i;
  int ii;
  int jj;
  int k;
  int minnp;
  int n_tmp;
  int nrt;
  int ns;
  int p_tmp;
  int q;
  int qjj;
  bool doscale;
  A_data = A->data;
  n_tmp = A->size[0];
  p_tmp = A->size[1];
  if (A->size[0] + 1 <= A->size[1]) {
    ns = A->size[0];
  } else {
    ns = A->size[1] - 1;
  }
  qjj = A->size[0];
  minnp = A->size[1];
  if (qjj <= minnp) {
    minnp = qjj;
  }
  emxInit_real_T(&s, 1);
  i = s->size[0];
  s->size[0] = ns + 1;
  emxEnsureCapacity_real_T(s, i);
  s_data = s->data;
  for (i = 0; i <= ns; i++) {
    s_data[i] = 0.0;
  }
  emxInit_real_T(&e, 1);
  i = e->size[0];
  e->size[0] = A->size[1];
  emxEnsureCapacity_real_T(e, i);
  e_data = e->data;
  for (i = 0; i < p_tmp; i++) {
    e_data[i] = 0.0;
  }
  emxInit_real_T(&work, 1);
  i = work->size[0];
  work->size[0] = A->size[0];
  emxEnsureCapacity_real_T(work, i);
  work_data = work->data;
  for (i = 0; i < n_tmp; i++) {
    work_data[i] = 0.0;
  }
  i = U->size[0] * U->size[1];
  U->size[0] = A->size[0];
  U->size[1] = A->size[0];
  emxEnsureCapacity_real_T(U, i);
  U_data = U->data;
  ns = A->size[0] * A->size[0];
  for (i = 0; i < ns; i++) {
    U_data[i] = 0.0;
  }
  i = V->size[0] * V->size[1];
  V->size[0] = A->size[1];
  V->size[1] = A->size[1];
  emxEnsureCapacity_real_T(V, i);
  V_data = V->data;
  ns = A->size[1] * A->size[1];
  for (i = 0; i < ns; i++) {
    V_data[i] = 0.0;
  }
  doscale = false;
  cscale = 0.0;
  anrm = xzlangeM(A);
  if ((A->size[0] == 0) || (A->size[1] == 0)) {
    for (ii = 0; ii < n_tmp; ii++) {
      U_data[ii + U->size[0] * ii] = 1.0;
    }
    for (ii = 0; ii < p_tmp; ii++) {
      V_data[ii + V->size[0] * ii] = 1.0;
    }
  } else {
    double snorm;
    int m;
    int nct;
    int nctp1;
    int nmq;
    int qp1;
    int qq;
    bool guard1;
    cscale = anrm;
    guard1 = false;
    if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
      doscale = true;
      cscale = 6.7178761075670888E-139;
      guard1 = true;
    } else if (anrm > 1.4885657073574029E+138) {
      doscale = true;
      cscale = 1.4885657073574029E+138;
      guard1 = true;
    }
    if (guard1) {
      xzlascl(anrm, cscale, A->size[0], A->size[1], A, A->size[0]);
      A_data = A->data;
    }
    if (A->size[1] >= 2) {
      qjj = A->size[1] - 2;
    } else {
      qjj = 0;
    }
    nrt = A->size[0];
    if (qjj <= nrt) {
      nrt = qjj;
    }
    if (A->size[0] >= 1) {
      qjj = A->size[0] - 1;
    } else {
      qjj = 0;
    }
    nct = A->size[1];
    if (qjj <= nct) {
      nct = qjj;
    }
    nctp1 = nct + 1;
    if (nct >= nrt) {
      i = nct;
    } else {
      i = nrt;
    }
    for (q = 0; q < i; q++) {
      bool apply_transform;
      qp1 = q + 2;
      qq = (q + n_tmp * q) + 1;
      nmq = (n_tmp - q) - 1;
      apply_transform = false;
      if (q + 1 <= nct) {
        nrm = xnrm2(nmq + 1, A, qq);
        if (nrm > 0.0) {
          apply_transform = true;
          if (A_data[qq - 1] < 0.0) {
            nrm = -nrm;
          }
          s_data[q] = nrm;
          if (fabs(nrm) >= 1.0020841800044864E-292) {
            nrm = 1.0 / nrm;
            ns = qq + nmq;
            for (k = qq; k <= ns; k++) {
              A_data[k - 1] *= nrm;
            }
          } else {
            ns = qq + nmq;
            for (k = qq; k <= ns; k++) {
              A_data[k - 1] /= s_data[q];
            }
          }
          A_data[qq - 1]++;
          s_data[q] = -s_data[q];
        } else {
          s_data[q] = 0.0;
        }
      }
      for (jj = qp1; jj <= p_tmp; jj++) {
        qjj = q + n_tmp * (jj - 1);
        if (apply_transform) {
          nrm = 0.0;
          if (nmq + 1 >= 1) {
            for (k = 0; k <= nmq; k++) {
              nrm += A_data[(qq + k) - 1] * A_data[qjj + k];
            }
          }
          nrm = -(nrm / A_data[q + A->size[0] * q]);
          b_xaxpy(nmq + 1, nrm, qq, A, qjj + 1);
          A_data = A->data;
        }
        e_data[jj - 1] = A_data[qjj];
      }
      if (q + 1 <= nct) {
        for (ii = q + 1; ii <= n_tmp; ii++) {
          U_data[(ii + U->size[0] * q) - 1] = A_data[(ii + A->size[0] * q) - 1];
        }
      }
      if (q + 1 <= nrt) {
        qq = p_tmp - q;
        nrm = xnrm2(qq - 1, e, q + 2);
        if (nrm == 0.0) {
          e_data[q] = 0.0;
        } else {
          if (e_data[q + 1] < 0.0) {
            e_data[q] = -nrm;
          } else {
            e_data[q] = nrm;
          }
          nrm = e_data[q];
          if (fabs(e_data[q]) >= 1.0020841800044864E-292) {
            nrm = 1.0 / e_data[q];
            ns = q + qq;
            for (k = qp1; k <= ns; k++) {
              e_data[k - 1] *= nrm;
            }
          } else {
            ns = q + qq;
            for (k = qp1; k <= ns; k++) {
              e_data[k - 1] /= nrm;
            }
          }
          e_data[q + 1]++;
          e_data[q] = -e_data[q];
          if (q + 2 <= n_tmp) {
            for (ii = qp1; ii <= n_tmp; ii++) {
              work_data[ii - 1] = 0.0;
            }
            for (jj = qp1; jj <= p_tmp; jj++) {
              xaxpy(nmq, e_data[jj - 1], A, (q + n_tmp * (jj - 1)) + 2, work,
                    q + 2);
              work_data = work->data;
            }
            for (jj = qp1; jj <= p_tmp; jj++) {
              xaxpy(nmq, -e_data[jj - 1] / e_data[q + 1], work, q + 2, A,
                    (q + n_tmp * (jj - 1)) + 2);
              A_data = A->data;
            }
          }
        }
        for (ii = qp1; ii <= p_tmp; ii++) {
          V_data[(ii + V->size[0] * q) - 1] = e_data[ii - 1];
        }
      }
    }
    if (A->size[1] <= A->size[0] + 1) {
      m = A->size[1] - 1;
    } else {
      m = A->size[0];
    }
    if (nct < A->size[1]) {
      s_data[nct] = A_data[nct + A->size[0] * nct];
    }
    if (A->size[0] < m + 1) {
      s_data[m] = 0.0;
    }
    if (nrt + 1 < m + 1) {
      e_data[nrt] = A_data[nrt + A->size[0] * m];
    }
    e_data[m] = 0.0;
    if (nct + 1 <= A->size[0]) {
      for (jj = nctp1; jj <= n_tmp; jj++) {
        for (ii = 0; ii < n_tmp; ii++) {
          U_data[ii + U->size[0] * (jj - 1)] = 0.0;
        }
        U_data[(jj + U->size[0] * (jj - 1)) - 1] = 1.0;
      }
    }
    for (q = nct; q >= 1; q--) {
      qp1 = q + 1;
      ns = n_tmp - q;
      qq = (q + n_tmp * (q - 1)) - 1;
      if (s_data[q - 1] != 0.0) {
        for (jj = qp1; jj <= n_tmp; jj++) {
          qjj = q + n_tmp * (jj - 1);
          nrm = 0.0;
          if (ns + 1 >= 1) {
            for (k = 0; k <= ns; k++) {
              nrm += U_data[qq + k] * U_data[(qjj + k) - 1];
            }
          }
          nrm = -(nrm / U_data[qq]);
          b_xaxpy(ns + 1, nrm, qq + 1, U, qjj);
          U_data = U->data;
        }
        for (ii = q; ii <= n_tmp; ii++) {
          U_data[(ii + U->size[0] * (q - 1)) - 1] =
              -U_data[(ii + U->size[0] * (q - 1)) - 1];
        }
        U_data[qq]++;
        for (ii = 0; ii <= q - 2; ii++) {
          U_data[ii + U->size[0] * (q - 1)] = 0.0;
        }
      } else {
        for (ii = 0; ii < n_tmp; ii++) {
          U_data[ii + U->size[0] * (q - 1)] = 0.0;
        }
        U_data[qq] = 1.0;
      }
    }
    for (q = p_tmp; q >= 1; q--) {
      if ((q <= nrt) && (e_data[q - 1] != 0.0)) {
        qp1 = q + 1;
        qq = p_tmp - q;
        ns = q + p_tmp * (q - 1);
        for (jj = qp1; jj <= p_tmp; jj++) {
          qjj = q + p_tmp * (jj - 1);
          nrm = 0.0;
          if (qq >= 1) {
            for (k = 0; k < qq; k++) {
              nrm += V_data[ns + k] * V_data[qjj + k];
            }
          }
          nrm = -(nrm / V_data[ns]);
          b_xaxpy(qq, nrm, ns + 1, V, qjj + 1);
          V_data = V->data;
        }
      }
      for (ii = 0; ii < p_tmp; ii++) {
        V_data[ii + V->size[0] * (q - 1)] = 0.0;
      }
      V_data[(q + V->size[0] * (q - 1)) - 1] = 1.0;
    }
    nct = m;
    nctp1 = 0;
    snorm = 0.0;
    for (q = 0; q <= m; q++) {
      if (s_data[q] != 0.0) {
        nrm = fabs(s_data[q]);
        r = s_data[q] / nrm;
        s_data[q] = nrm;
        if (q + 1 < m + 1) {
          e_data[q] /= r;
        }
        if (q + 1 <= n_tmp) {
          ns = n_tmp * q;
          i = ns + n_tmp;
          for (k = ns + 1; k <= i; k++) {
            U_data[k - 1] *= r;
          }
        }
      }
      if ((q + 1 < m + 1) && (e_data[q] != 0.0)) {
        nrm = fabs(e_data[q]);
        r = nrm / e_data[q];
        e_data[q] = nrm;
        s_data[q + 1] *= r;
        ns = p_tmp * (q + 1);
        i = ns + p_tmp;
        for (k = ns + 1; k <= i; k++) {
          V_data[k - 1] *= r;
        }
      }
      snorm = fmax(snorm, fmax(fabs(s_data[q]), fabs(e_data[q])));
    }
    while ((m + 1 > 0) && (nctp1 < 75)) {
      bool exitg1;
      ii = m;
      exitg1 = false;
      while (!(exitg1 || (ii == 0))) {
        nrm = fabs(e_data[ii - 1]);
        if ((nrm <= 2.2204460492503131E-16 *
                        (fabs(s_data[ii - 1]) + fabs(s_data[ii]))) ||
            (nrm <= 1.0020841800044864E-292) ||
            ((nctp1 > 20) && (nrm <= 2.2204460492503131E-16 * snorm))) {
          e_data[ii - 1] = 0.0;
          exitg1 = true;
        } else {
          ii--;
        }
      }
      if (ii == m) {
        ns = 4;
      } else {
        qjj = m + 1;
        ns = m + 1;
        exitg1 = false;
        while ((!exitg1) && (ns >= ii)) {
          qjj = ns;
          if (ns == ii) {
            exitg1 = true;
          } else {
            nrm = 0.0;
            if (ns < m + 1) {
              nrm = fabs(e_data[ns - 1]);
            }
            if (ns > ii + 1) {
              nrm += fabs(e_data[ns - 2]);
            }
            r = fabs(s_data[ns - 1]);
            if ((r <= 2.2204460492503131E-16 * nrm) ||
                (r <= 1.0020841800044864E-292)) {
              s_data[ns - 1] = 0.0;
              exitg1 = true;
            } else {
              ns--;
            }
          }
        }
        if (qjj == ii) {
          ns = 3;
        } else if (qjj == m + 1) {
          ns = 1;
        } else {
          ns = 2;
          ii = qjj;
        }
      }
      switch (ns) {
      case 1: {
        r = e_data[m - 1];
        e_data[m - 1] = 0.0;
        for (k = m; k >= ii + 1; k--) {
          double sqds;
          sqds = xrotg(&s_data[k - 1], &r, &sm);
          if (k > ii + 1) {
            double b;
            b = e_data[k - 2];
            r = -sm * b;
            e_data[k - 2] = b * sqds;
          }
          if (p_tmp >= 1) {
            qq = p_tmp * (k - 1);
            nmq = p_tmp * m;
            for (nrt = 0; nrt < p_tmp; nrt++) {
              double scale;
              double temp_tmp;
              ns = nmq + nrt;
              scale = V_data[ns];
              qjj = qq + nrt;
              temp_tmp = V_data[qjj];
              V_data[ns] = sqds * scale - sm * temp_tmp;
              V_data[qjj] = sqds * temp_tmp + sm * scale;
            }
          }
        }
      } break;
      case 2: {
        r = e_data[ii - 1];
        e_data[ii - 1] = 0.0;
        for (k = ii + 1; k <= m + 1; k++) {
          double b;
          double sqds;
          sqds = xrotg(&s_data[k - 1], &r, &sm);
          b = e_data[k - 1];
          r = -sm * b;
          e_data[k - 1] = b * sqds;
          if (n_tmp >= 1) {
            qq = n_tmp * (k - 1);
            nmq = n_tmp * (ii - 1);
            for (nrt = 0; nrt < n_tmp; nrt++) {
              double scale;
              double temp_tmp;
              ns = nmq + nrt;
              scale = U_data[ns];
              qjj = qq + nrt;
              temp_tmp = U_data[qjj];
              U_data[ns] = sqds * scale - sm * temp_tmp;
              U_data[qjj] = sqds * temp_tmp + sm * scale;
            }
          }
        }
      } break;
      case 3: {
        double b;
        double scale;
        double sqds;
        nrm = s_data[m - 1];
        r = e_data[m - 1];
        scale = fmax(fmax(fmax(fmax(fabs(s_data[m]), fabs(nrm)), fabs(r)),
                          fabs(s_data[ii])),
                     fabs(e_data[ii]));
        sm = s_data[m] / scale;
        nrm /= scale;
        r /= scale;
        sqds = s_data[ii] / scale;
        b = ((nrm + sm) * (nrm - sm) + r * r) / 2.0;
        nrm = sm * r;
        nrm *= nrm;
        if ((b != 0.0) || (nrm != 0.0)) {
          r = sqrt(b * b + nrm);
          if (b < 0.0) {
            r = -r;
          }
          r = nrm / (b + r);
        } else {
          r = 0.0;
        }
        r += (sqds + sm) * (sqds - sm);
        nrm = sqds * (e_data[ii] / scale);
        for (k = ii + 1; k <= m; k++) {
          double temp_tmp;
          sqds = xrotg(&r, &nrm, &sm);
          if (k > ii + 1) {
            e_data[k - 2] = r;
          }
          nrm = e_data[k - 1];
          b = s_data[k - 1];
          e_data[k - 1] = sqds * nrm - sm * b;
          r = sm * s_data[k];
          s_data[k] *= sqds;
          if (p_tmp >= 1) {
            qq = p_tmp * (k - 1);
            nmq = p_tmp * k;
            for (nrt = 0; nrt < p_tmp; nrt++) {
              ns = nmq + nrt;
              scale = V_data[ns];
              qjj = qq + nrt;
              temp_tmp = V_data[qjj];
              V_data[ns] = sqds * scale - sm * temp_tmp;
              V_data[qjj] = sqds * temp_tmp + sm * scale;
            }
          }
          s_data[k - 1] = sqds * b + sm * nrm;
          sqds = xrotg(&s_data[k - 1], &r, &sm);
          b = e_data[k - 1];
          r = sqds * b + sm * s_data[k];
          s_data[k] = -sm * b + sqds * s_data[k];
          nrm = sm * e_data[k];
          e_data[k] *= sqds;
          if ((k < n_tmp) && (n_tmp >= 1)) {
            qq = n_tmp * (k - 1);
            nmq = n_tmp * k;
            for (nrt = 0; nrt < n_tmp; nrt++) {
              ns = nmq + nrt;
              scale = U_data[ns];
              qjj = qq + nrt;
              temp_tmp = U_data[qjj];
              U_data[ns] = sqds * scale - sm * temp_tmp;
              U_data[qjj] = sqds * temp_tmp + sm * scale;
            }
          }
        }
        e_data[m - 1] = r;
        nctp1++;
      } break;
      default:
        if (s_data[ii] < 0.0) {
          s_data[ii] = -s_data[ii];
          ns = p_tmp * ii;
          i = ns + p_tmp;
          for (k = ns + 1; k <= i; k++) {
            V_data[k - 1] = -V_data[k - 1];
          }
        }
        qp1 = ii + 1;
        while ((ii + 1 < nct + 1) && (s_data[ii] < s_data[qp1])) {
          nrm = s_data[ii];
          s_data[ii] = s_data[qp1];
          s_data[qp1] = nrm;
          if (ii + 1 < p_tmp) {
            qq = p_tmp * ii;
            nmq = p_tmp * (ii + 1);
            for (k = 0; k < p_tmp; k++) {
              ns = qq + k;
              nrm = V_data[ns];
              i = nmq + k;
              V_data[ns] = V_data[i];
              V_data[i] = nrm;
            }
          }
          if (ii + 1 < n_tmp) {
            qq = n_tmp * ii;
            nmq = n_tmp * (ii + 1);
            for (k = 0; k < n_tmp; k++) {
              ns = qq + k;
              nrm = U_data[ns];
              i = nmq + k;
              U_data[ns] = U_data[i];
              U_data[i] = nrm;
            }
          }
          ii = qp1;
          qp1++;
        }
        nctp1 = 0;
        m--;
        break;
      }
    }
  }
  emxFree_real_T(&work);
  emxFree_real_T(&e);
  i = S->size[0];
  S->size[0] = minnp;
  emxEnsureCapacity_real_T(S, i);
  A_data = S->data;
  for (k = 0; k < minnp; k++) {
    A_data[k] = s_data[k];
  }
  emxFree_real_T(&s);
  if (doscale) {
    b_xzlascl(cscale, anrm, minnp, S, 1);
  }
}

/*
 * File trailer for xzsvdc.c
 *
 * [EOF]
 */
