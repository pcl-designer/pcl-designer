/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: eig.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "eig.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "eigHermitianStandard.h"
#include "rt_nonfinite.h"
#include "xdlahqr.h"
#include "xzgebal.h"
#include "xzlangeM.h"
#include "xzlarf.h"
#include "xzlarfg.h"
#include "xzlascl.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double A_data[]
 *                const int A_size[2]
 *                creal_T V_data[]
 * Return Type  : int
 */
int eig(const double A_data[], const int A_size[2], creal_T V_data[])
{
  emxArray_real_T b_A_data;
  emxArray_real_T *a;
  emxArray_real_T *work;
  double wi_data[9];
  double work_data[9];
  double tau_data[8];
  double alpha1;
  double *a_data;
  double *b_work_data;
  int V_size;
  int b_i;
  int i;
  int i1;
  int ihi;
  int k;
  int nx_tmp;
  bool scalea;
  nx_tmp = A_size[0] * A_size[1];
  scalea = true;
  for (k = 0; k < nx_tmp; k++) {
    if ((!scalea) || (rtIsInf(A_data[k]) || rtIsNaN(A_data[k]))) {
      scalea = false;
    }
  }
  if (!scalea) {
    int loop_ub;
    loop_ub = A_size[0];
    V_size = A_size[0];
    for (i = 0; i < loop_ub; i++) {
      V_data[i].re = rtNaN;
      V_data[i].im = 0.0;
    }
  } else {
    int exitg1;
    bool exitg2;
    scalea = (A_size[0] == A_size[1]);
    if (scalea) {
      k = 0;
      exitg2 = false;
      while ((!exitg2) && (k <= A_size[1] - 1)) {
        b_i = 0;
        do {
          exitg1 = 0;
          if (b_i <= k) {
            if (!(A_data[b_i + A_size[0] * k] == A_data[k + A_size[0] * b_i])) {
              scalea = false;
              exitg1 = 1;
            } else {
              b_i++;
            }
          } else {
            k++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);
        if (exitg1 == 1) {
          exitg2 = true;
        }
      }
    }
    if (scalea) {
      V_size = eigHermitianStandard(A_data, A_size, V_data);
    } else {
      scalea = (A_size[0] == A_size[1]);
      if (scalea) {
        k = 0;
        exitg2 = false;
        while ((!exitg2) && (k <= A_size[1] - 1)) {
          b_i = 0;
          do {
            exitg1 = 0;
            if (b_i <= k) {
              if (!(A_data[b_i + A_size[0] * k] ==
                    -A_data[k + A_size[0] * b_i])) {
                scalea = false;
                exitg1 = 1;
              } else {
                b_i++;
              }
            } else {
              k++;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      }
      emxInit_real_T(&a, 2);
      emxInit_real_T(&work, 1);
      if (scalea) {
        int loop_ub;
        loop_ub = A_size[0];
        i = a->size[0] * a->size[1];
        a->size[0] = A_size[0];
        a->size[1] = A_size[1];
        emxEnsureCapacity_real_T(a, i);
        a_data = a->data;
        for (i = 0; i < nx_tmp; i++) {
          a_data[i] = A_data[i];
        }
        k = A_size[0] - 1;
        if (A_size[0] > 1) {
          if (loop_ub <= k) {
            memset(&tau_data[loop_ub + -1], 0,
                   (unsigned int)((k - loop_ub) + 1) * sizeof(double));
          }
          V_size = A_size[0];
          memset(&work_data[0], 0, (unsigned int)loop_ub * sizeof(double));
          for (b_i = 0; b_i <= loop_ub - 2; b_i++) {
            double d;
            int in;
            nx_tmp = b_i * loop_ub;
            in = (b_i + 1) * loop_ub;
            alpha1 = a_data[(b_i + a->size[0] * b_i) + 1];
            i = (loop_ub - b_i) - 1;
            k = b_i + 3;
            if (k > loop_ub) {
              k = loop_ub;
            }
            d = xzlarfg(i, &alpha1, a, k + nx_tmp);
            a_data = a->data;
            tau_data[b_i] = d;
            a_data[(b_i + a->size[0] * b_i) + 1] = 1.0;
            i1 = (b_i + nx_tmp) + 2;
            emxReserve_real_T(a);
            xzlarf(loop_ub, i, i1, d, (double *)a->data, in + 1, loop_ub,
                   work_data);
            k = work->size[0];
            work->size[0] = V_size;
            emxEnsureCapacity_real_T(work, k);
            b_work_data = work->data;
            for (k = 0; k < V_size; k++) {
              b_work_data[k] = work_data[k];
            }
            b_xzlarf(i, i, i1, tau_data[b_i], a, (b_i + in) + 2, loop_ub, work);
            b_work_data = work->data;
            a_data = a->data;
            nx_tmp = work->size[0];
            V_size = work->size[0];
            for (i = 0; i < nx_tmp; i++) {
              work_data[i] = b_work_data[i];
            }
            a_data[(b_i + a->size[0] * b_i) + 1] = alpha1;
          }
        }
        emxReserve_real_T(a);
        k = b_xdlahqr(A_size[0], (double *)a->data, a->size, work_data, &V_size,
                      wi_data, &nx_tmp);
        V_size = A_size[0];
        i = (unsigned char)k;
        for (b_i = 0; b_i < i; b_i++) {
          V_data[b_i].re = rtNaN;
          V_data[b_i].im = 0.0;
        }
        i = k + 1;
        for (b_i = i; b_i <= loop_ub; b_i++) {
          V_data[b_i - 1].re = 0.0;
          V_data[b_i - 1].im = wi_data[b_i - 1];
        }
      } else {
        double anrm;
        int loop_ub;
        loop_ub = A_size[0];
        i = a->size[0] * a->size[1];
        a->size[0] = A_size[0];
        a->size[1] = A_size[1];
        emxEnsureCapacity_real_T(a, i);
        a_data = a->data;
        for (i = 0; i < nx_tmp; i++) {
          a_data[i] = A_data[i];
        }
        b_A_data.data = (double *)&A_data[0];
        b_A_data.size = (int *)&A_size[0];
        b_A_data.allocatedSize = -1;
        b_A_data.numDimensions = 2;
        b_A_data.canFreeData = false;
        anrm = xzlangeM(&b_A_data);
        if (rtIsInf(anrm) || rtIsNaN(anrm)) {
          V_size = A_size[0];
          for (i = 0; i < loop_ub; i++) {
            V_data[i].re = rtNaN;
            V_data[i].im = 0.0;
          }
        } else {
          double cscale;
          int ilo;
          int n_tmp;
          cscale = anrm;
          scalea = false;
          if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
            scalea = true;
            cscale = 6.7178761075670888E-139;
            i = a->size[0] * a->size[1];
            a->size[0] = A_size[0];
            a->size[1] = A_size[1];
            emxEnsureCapacity_real_T(a, i);
            a_data = a->data;
            for (i = 0; i < nx_tmp; i++) {
              a_data[i] = A_data[i];
            }
            xzlascl(anrm, 6.7178761075670888E-139, A_size[0], A_size[0], a,
                    A_size[0]);
          }
          emxReserve_real_T(a);
          a_data = a->data;
          ilo = xzgebal((double *)a->data, a->size, &ihi, work_data, &V_size);
          n_tmp = a->size[0];
          k = a->size[0] - 1;
          if ((ihi - ilo) + 1 > 1) {
            i = (unsigned char)(ilo - 1);
            if (i - 1 >= 0) {
              memset(&tau_data[0], 0, (unsigned int)i * sizeof(double));
            }
            if (ihi <= k) {
              memset(&tau_data[ihi + -1], 0,
                     (unsigned int)((k - ihi) + 1) * sizeof(double));
            }
            V_size = a->size[0];
            if (n_tmp - 1 >= 0) {
              memset(&work_data[0], 0, (unsigned int)n_tmp * sizeof(double));
            }
            i = ihi - 1;
            for (b_i = ilo; b_i <= i; b_i++) {
              double d;
              int in;
              nx_tmp = (b_i - 1) * n_tmp;
              in = b_i * n_tmp + 1;
              alpha1 = a_data[b_i + a->size[0] * (b_i - 1)];
              i1 = ihi - b_i;
              k = b_i + 2;
              if (k > n_tmp) {
                k = n_tmp;
              }
              d = xzlarfg(i1, &alpha1, a, k + nx_tmp);
              a_data = a->data;
              tau_data[b_i - 1] = d;
              a_data[b_i + a->size[0] * (b_i - 1)] = 1.0;
              k = (b_i + nx_tmp) + 1;
              emxReserve_real_T(a);
              xzlarf(ihi, i1, k, d, (double *)a->data, in, n_tmp, work_data);
              nx_tmp = work->size[0];
              work->size[0] = V_size;
              emxEnsureCapacity_real_T(work, nx_tmp);
              b_work_data = work->data;
              for (nx_tmp = 0; nx_tmp < V_size; nx_tmp++) {
                b_work_data[nx_tmp] = work_data[nx_tmp];
              }
              b_xzlarf(i1, n_tmp - b_i, k, tau_data[b_i - 1], a, b_i + in,
                       n_tmp, work);
              b_work_data = work->data;
              a_data = a->data;
              loop_ub = work->size[0];
              V_size = work->size[0];
              for (i1 = 0; i1 < loop_ub; i1++) {
                work_data[i1] = b_work_data[i1];
              }
              a_data[b_i + a->size[0] * (b_i - 1)] = alpha1;
            }
          }
          emxReserve_real_T(a);
          k = xdlahqr(ilo, ihi, (double *)a->data, a->size, work_data, &V_size,
                      wi_data, &nx_tmp);
          if (scalea) {
            i = work->size[0];
            work->size[0] = V_size;
            emxEnsureCapacity_real_T(work, i);
            b_work_data = work->data;
            for (i = 0; i < V_size; i++) {
              b_work_data[i] = work_data[i];
            }
            i = A_size[0] - k;
            b_xzlascl(cscale, anrm, i, work, k + 1);
            b_work_data = work->data;
            loop_ub = work->size[0];
            V_size = work->size[0];
            for (i1 = 0; i1 < loop_ub; i1++) {
              work_data[i1] = b_work_data[i1];
            }
            i1 = work->size[0];
            work->size[0] = nx_tmp;
            emxEnsureCapacity_real_T(work, i1);
            b_work_data = work->data;
            for (i1 = 0; i1 < nx_tmp; i1++) {
              b_work_data[i1] = wi_data[i1];
            }
            b_xzlascl(cscale, anrm, i, work, k + 1);
            b_work_data = work->data;
            nx_tmp = work->size[0];
            for (i = 0; i < nx_tmp; i++) {
              wi_data[i] = b_work_data[i];
            }
            if (k != 0) {
              i = work->size[0];
              work->size[0] = loop_ub;
              emxEnsureCapacity_real_T(work, i);
              b_work_data = work->data;
              for (i = 0; i < loop_ub; i++) {
                b_work_data[i] = work_data[i];
              }
              b_xzlascl(cscale, anrm, ilo - 1, work, 1);
              b_work_data = work->data;
              loop_ub = work->size[0];
              V_size = work->size[0];
              for (i = 0; i < loop_ub; i++) {
                work_data[i] = b_work_data[i];
              }
              i = work->size[0];
              work->size[0] = nx_tmp;
              emxEnsureCapacity_real_T(work, i);
              b_work_data = work->data;
              for (i = 0; i < nx_tmp; i++) {
                b_work_data[i] = wi_data[i];
              }
              b_xzlascl(cscale, anrm, ilo - 1, work, 1);
              b_work_data = work->data;
              loop_ub = work->size[0];
              for (i = 0; i < loop_ub; i++) {
                wi_data[i] = b_work_data[i];
              }
            }
          }
          if (k != 0) {
            for (b_i = ilo; b_i <= k; b_i++) {
              work_data[b_i - 1] = rtNaN;
              wi_data[b_i - 1] = 0.0;
            }
          }
          for (i = 0; i < V_size; i++) {
            V_data[i].re = work_data[i];
            V_data[i].im = wi_data[i];
          }
        }
      }
      emxFree_real_T(&work);
      emxFree_real_T(&a);
    }
  }
  return V_size;
}

/*
 * File trailer for eig.c
 *
 * [EOF]
 */
