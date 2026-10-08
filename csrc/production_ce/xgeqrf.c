/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: xgeqrf.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "xgeqrf.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rt_nonfinite.h"
#include "xzlarf.h"
#include "xzlarfg.h"

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *A
 *                emxArray_real_T *tau
 * Return Type  : void
 */
void xgeqrf(emxArray_real_T *A, emxArray_real_T *tau)
{
  emxArray_real_T *work;
  double atmp;
  double *A_data;
  double *tau_data;
  double *work_data;
  int i;
  int ii;
  int m_tmp;
  int n_tmp;
  int u1;
  A_data = A->data;
  m_tmp = A->size[0];
  n_tmp = A->size[1];
  ii = A->size[0];
  u1 = A->size[1];
  if (ii <= u1) {
    u1 = ii;
  }
  ii = tau->size[0];
  tau->size[0] = u1;
  emxEnsureCapacity_real_T(tau, ii);
  tau_data = tau->data;
  for (ii = 0; ii < u1; ii++) {
    tau_data[ii] = 0.0;
  }
  emxInit_real_T(&work, 1);
  if ((A->size[0] != 0) && (A->size[1] != 0) && (u1 >= 1)) {
    ii = work->size[0];
    work->size[0] = n_tmp;
    emxEnsureCapacity_real_T(work, ii);
    work_data = work->data;
    for (ii = 0; ii < n_tmp; ii++) {
      work_data[ii] = 0.0;
    }
    for (i = 0; i < u1; i++) {
      double d;
      int mmi;
      ii = i * m_tmp + i;
      mmi = m_tmp - i;
      if (i + 1 < m_tmp) {
        atmp = A_data[ii];
        d = xzlarfg(mmi, &atmp, A, ii + 2);
        A_data = A->data;
        tau_data[i] = d;
        A_data[ii] = atmp;
      } else {
        d = 0.0;
        tau_data[i] = 0.0;
      }
      if (i + 1 < n_tmp) {
        atmp = A_data[ii];
        A_data[ii] = 1.0;
        b_xzlarf(mmi, (n_tmp - i) - 1, ii + 1, d, A, (ii + m_tmp) + 1, m_tmp,
                 work);
        A_data = A->data;
        A_data[ii] = atmp;
      }
    }
  }
  emxFree_real_T(&work);
}

/*
 * File trailer for xgeqrf.c
 *
 * [EOF]
 */
