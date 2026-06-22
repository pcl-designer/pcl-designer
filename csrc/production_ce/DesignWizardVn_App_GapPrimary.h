/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: DesignWizardVn_App_GapPrimary.h
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Jun-2026 20:56:24
 */

#ifndef DESIGNWIZARDVN_APP_GAPPRIMARY_H
#define DESIGNWIZARDVN_APP_GAPPRIMARY_H

/* Include Files */
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void DesignWizardVn_App_GapPrimary(
    double m, emxArray_real_T *n, double K, const emxArray_real_T *priorMean,
    const emxArray_real_T *priorCov, double numPoints,
    const char priorMethod_data[], const int priorMethod_size[2],
    const cell_wrap_0 wp_levelSets_data[], const int wp_levelSets_size[2],
    const cell_wrap_0 sp_levelSets_data[], const int sp_levelSets_size[2],
    const emxArray_cell_wrap_0 *modelTerms, char evalMethod_data[],
    int evalMethod_size[2], char crit_mode_data[], int crit_mode_size[2],
    double copulaType, double sigma2_fixed_data[], int sigma2_fixed_size[2],
    const double lambda_fixed_data[], int lambda_fixed_size[2], double seed,
    emxArray_real_T *optimalX, double *optimalCrit);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for DesignWizardVn_App_GapPrimary.h
 *
 * [EOF]
 */
