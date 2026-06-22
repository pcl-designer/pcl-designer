/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: DesignWizardVn_App_GapPrimary_terminate.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Jun-2026 20:56:24
 */

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_terminate.h"
#include "DesignWizardVn_App_GapPrimary_data.h"
#include "rt_nonfinite.h"
#include "omp.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void DesignWizardVn_App_GapPrimary_terminate(void)
{
  omp_destroy_nest_lock(&DesignWizardVn_App_GapPrimary_nestLockGlobal);
  isInitialized_DesignWizardVn_App_GapPrimary = false;
}

/*
 * File trailer for DesignWizardVn_App_GapPrimary_terminate.c
 *
 * [EOF]
 */
