/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: DesignWizardVn_App_GapPrimary_initialize.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_initialize.h"
#include "DesignWizardVn_App_GapPrimary_data.h"
#include "eml_rand_mt19937ar_stateful.h"
#include "rt_nonfinite.h"
#include "omp.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void DesignWizardVn_App_GapPrimary_initialize(void)
{
  omp_init_nest_lock(&DesignWizardVn_App_GapPrimary_nestLockGlobal);
  c_eml_rand_mt19937ar_stateful_i();
  isInitialized_DesignWizardVn_App_GapPrimary = true;
}

/*
 * File trailer for DesignWizardVn_App_GapPrimary_initialize.c
 *
 * [EOF]
 */
