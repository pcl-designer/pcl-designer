/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: DesignWizardVn_App_GapPrimary.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 01-Jun-2026 20:56:24
 */

/* Include Files */
#include "DesignWizardVn_App_GapPrimary.h"
#include "DesignWizardVn_App_GapPrimary_data.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_initialize.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "blockedSummation.h"
#include "eml_mtimes_helper.h"
#include "minOrMax.h"
#include "mldivide.h"
#include "mod.h"
#include "mtimes.h"
#include "rand.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "strcmp.h"
#include "var.h"
#include "xpotrf.h"
#include "omp.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* v0.2.3: configurable restart count (default 15; set from config.txt by main) */
int pcl_num_starts = 15;

/* Variable Definitions */
static const double dv[16] = {
    -4.6887389393058179, -3.8694479048601225,  -3.176999161979956,
    -2.5462021578474809, -1.9517879909162539,  -1.3802585391988806,
    -0.8229514491446559, -0.27348104613815238, 0.27348104613815238,
    0.8229514491446559,  1.3802585391988806,   1.9517879909162539,
    2.5462021578474809,  3.176999161979956,    3.8694479048601225,
    4.6887389393058179};

static const double dv1[16] = {
    1.4978147231618198E-10, 1.3094732162868036E-7, 1.5300032162487357E-5,
    0.00052598492657390838, 0.0072669376011847324, 0.047284752354014067,
    0.15833837275094967,    0.28656852123801213,   0.28656852123801213,
    0.15833837275094967,    0.047284752354014067,  0.0072669376011847324,
    0.00052598492657390838, 1.5300032162487357E-5, 1.3094732162868036E-7,
    1.4978147231618198E-10};

/* Function Declarations */
static double GLMM_ordinal_marginalPMF_cg(const emxArray_real_T *y,
                                          const emxArray_real_T *Xwp,
                                          const emxArray_real_T *beta,
                                          const emxArray_real_T *alpha,
                                          double sigma2, int K);

static void b_computeFisher_Copula(const emxArray_real_T *theta,
                                   const emxArray_real_T *Xwp, int K, double q,
                                   double type,
                                   const double lambda_fixed_data[],
                                   const int lambda_fixed_size[2],
                                   emxArray_real_T *b_I);

static double b_evaluate_copula_pmf(const emxArray_real_T *y,
                                    const emxArray_real_T *Xwp,
                                    const emxArray_real_T *th, int K, double q,
                                    double type,
                                    const double lambda_fixed_data[],
                                    const int lambda_fixed_size[2]);

static void binary_expand_op(emxArray_real_T *in1, int in2,
                             const emxArray_real_T *in3);

static void binary_expand_op_1(emxArray_real_T *in1, int in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4);

static void binary_expand_op_10(emxArray_real_T *in1, double in2,
                                const emxArray_real_T *in3);

static void binary_expand_op_2(emxArray_real_T *in1, const emxArray_real_T *in2,
                               const emxArray_real_T *in3);

static void binary_expand_op_3(emxArray_real_T *in1, double in2,
                               const emxArray_real_T *in3);

static void binary_expand_op_5(emxArray_real_T *in1, const emxArray_real_T *in2,
                               double in3, const emxArray_real_T *in4,
                               const emxArray_real_T *in5);

static void binary_expand_op_6(emxArray_real_T *in1, const emxArray_real_T *in2,
                               int in3, const emxArray_real_T *in4);

static void binary_expand_op_7(emxArray_real_T *in1, double in2,
                               const emxArray_real_T *in3);

static void
build_model_row(const double wp_lev_data[], const int wp_lev_size[2],
                const double sp_lev_data[], const int sp_lev_size[2],
                const emxArray_cell_wrap_0 *modelTerms, emxArray_real_T *xrow);

static double calculate_penalty(const emxArray_real_T *idx_wp,
                                const emxArray_real_T *idx_sp_full, double m,
                                const emxArray_real_T *n, double N,
                                const emxArray_real_T *wp_combos,
                                const emxArray_real_T *sp_combos, double w,
                                double s);

static double computeCriterion_otf(
    const double idx_data[], double m, const emxArray_real_T *n, double N,
    const emxArray_real_T *weights, double p, const char crit_mode_data[],
    const int crit_mode_size[2], const emxArray_real_T *wp_combos,
    const emxArray_real_T *sp_combos, const emxArray_real_T *thetaGrid,
    int K_int, double q, const double sigma2_fixed_data[],
    const int sigma2_fixed_size[2], const double lambda_fixed_data[],
    const int lambda_fixed_size[2], double copulaType, bool is_glmm_approx,
    bool is_glmm_exact, bool is_copula_pcl, bool is_copula_pcl_godambe,
    bool is_indep_glm, const emxArray_cell_wrap_0 *modelTerms);

static void computeFisher_Copula(const emxArray_real_T *theta,
                                 const emxArray_real_T *Xwp, int K, double q,
                                 double type, const double lambda_fixed_data[],
                                 const int lambda_fixed_size[2],
                                 emxArray_real_T *b_I);

static void computeFisher_GLMM_Exact(const emxArray_real_T *theta,
                                     const emxArray_real_T *Xwp, int K,
                                     double q, const double sigma2_fixed_data[],
                                     const int sigma2_fixed_size[2],
                                     emxArray_real_T *b_I);

static int coordinate_exchange_otf(
    double m, const emxArray_real_T *n, double N,
    const emxArray_real_T *weights, double p, const char crit_mode_data[],
    const int crit_mode_size[2], double nvars, const emxArray_real_T *lb,
    const emxArray_real_T *ub, const emxArray_real_T *wp_combos,
    const emxArray_real_T *sp_combos, const emxArray_real_T *thetaGrid,
    int K_int, double q, const double sigma2_fixed_data[],
    const int sigma2_fixed_size[2], const double lambda_fixed_data[],
    const int lambda_fixed_size[2], double copulaType, bool is_glmm_approx,
    bool is_glmm_exact, bool is_copula_pcl, bool is_copula_pcl_godambe,
    bool is_indep_glm, const emxArray_cell_wrap_0 *modelTerms,
    double best_idx_data[], double *best_crit);

static double evaluate_copula_pmf(const double y[2], const emxArray_real_T *Xwp,
                                  const emxArray_real_T *th, int K, double q,
                                  double type, const double lambda_fixed_data[],
                                  const int lambda_fixed_size[2]);

static double generate_combos(const cell_wrap_0 levelSets_data[],
                              const int levelSets_size[2],
                              emxArray_real_T *combos);

static void plus(emxArray_real_T *in1, const emxArray_real_T *in2);

static double rt_powd_snf(double u0, double u1);

static double rt_roundd_snf(double u);

static void univariateOrdinalFisher(double linpred,
                                    const emxArray_real_T *alpha, int K,
                                    const emxArray_real_T *xrow,
                                    emxArray_real_T *I1);

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *y
 *                const emxArray_real_T *Xwp
 *                const emxArray_real_T *beta
 *                const emxArray_real_T *alpha
 *                double sigma2
 *                int K
 * Return Type  : double
 */
static double GLMM_ordinal_marginalPMF_cg(const emxArray_real_T *y,
                                          const emxArray_real_T *Xwp,
                                          const emxArray_real_T *beta,
                                          const emxArray_real_T *alpha,
                                          double sigma2, int K)
{
  emxArray_real_T *eta;
  const double *Xwp_data;
  const double *alpha_data;
  const double *beta_data;
  const double *y_data;
  double pr;
  double *eta_data;
  int aoffset;
  int b_i;
  int i;
  int inner;
  int k;
  int kk;
  int mc;
  alpha_data = alpha->data;
  beta_data = beta->data;
  Xwp_data = Xwp->data;
  y_data = y->data;
  pr = 0.0;
  mc = Xwp->size[0] - 1;
  inner = Xwp->size[1];
  i = y->size[1];
  emxInit_real_T(&eta, 1);
  for (kk = 0; kk < 16; kk++) {
    double delta;
    double pr_cond;
    int loop_ub;
    delta = sqrt(2.0 * sigma2) * dv[kk];
    loop_ub = Xwp->size[0];
    aoffset = eta->size[0];
    eta->size[0] = Xwp->size[0];
    emxEnsureCapacity_real_T(eta, aoffset);
    eta_data = eta->data;
    for (b_i = 0; b_i <= mc; b_i++) {
      eta_data[b_i] = 0.0;
    }
    for (k = 0; k < inner; k++) {
      aoffset = k * Xwp->size[0];
      for (b_i = 0; b_i <= mc; b_i++) {
        eta_data[b_i] += Xwp_data[aoffset + b_i] * beta_data[k];
      }
    }
    for (aoffset = 0; aoffset < loop_ub; aoffset++) {
      eta_data[aoffset] += delta;
    }
    pr_cond = 1.0;
    for (b_i = 0; b_i < i; b_i++) {
      double p1;
      delta = y_data[b_i];
      if (delta < 0.0) {
        p1 = 0.0;
      } else if (delta >= (double)K - 1.0) {
        p1 = 1.0;
      } else {
        p1 = 1.0 /
             (exp(-(alpha_data[(int)(delta + 1.0) - 1] + eta_data[b_i])) + 1.0);
      }
      if (delta - 1.0 < 0.0) {
        delta = 0.0;
      } else if (delta - 1.0 >= (double)K - 1.0) {
        delta = 1.0;
      } else {
        delta = 1.0 / (exp(-(alpha_data[(int)((delta - 1.0) + 1.0) - 1] +
                             eta_data[b_i])) +
                       1.0);
      }
      pr_cond *= fmax(p1 - delta, 2.2204460492503131E-16);
    }
    pr += dv1[kk] * pr_cond;
  }
  emxFree_real_T(&eta);
  return fmax(pr, 2.2204460492503131E-16);
}

/*
 * Arguments    : const emxArray_real_T *theta
 *                const emxArray_real_T *Xwp
 *                int K
 *                double q
 *                double type
 *                const double lambda_fixed_data[]
 *                const int lambda_fixed_size[2]
 *                emxArray_real_T *b_I
 * Return Type  : void
 */
static void b_computeFisher_Copula(const emxArray_real_T *theta,
                                   const emxArray_real_T *Xwp, int K, double q,
                                   double type,
                                   const double lambda_fixed_data[],
                                   const int lambda_fixed_size[2],
                                   emxArray_real_T *b_I)
{
  emxArray_real_T *Hess;
  emxArray_real_T *X_pair;
  emxArray_real_T *x_mm;
  emxArray_real_T *x_mp;
  emxArray_real_T *x_pm;
  emxArray_real_T *x_pp;
  emxArray_uint32_T *Y_pairs;
  const double *Xwp_data;
  const double *theta_data;
  double n_outcomes_tmp;
  double *I_data;
  double *X_pair_data;
  double *x_mm_data;
  double *x_mp_data;
  double *x_pm_data;
  double *x_pp_data;
  int b_i;
  int b_y1;
  int i;
  int i1;
  int i2;
  int j;
  int k;
  int loop_ub;
  int loop_ub_tmp;
  unsigned int row;
  int y2;
  unsigned int *Y_pairs_data;
  Xwp_data = Xwp->data;
  theta_data = theta->data;
  loop_ub = theta->size[0];
  i = b_I->size[0] * b_I->size[1];
  b_I->size[0] = theta->size[0];
  b_I->size[1] = theta->size[0];
  emxEnsureCapacity_real_T(b_I, i);
  I_data = b_I->data;
  loop_ub_tmp = theta->size[0] * theta->size[0];
  for (i = 0; i < loop_ub_tmp; i++) {
    I_data[i] = 0.0;
  }
  n_outcomes_tmp = (double)K * (double)K;
  emxInit_uint32_T(&Y_pairs, 2);
  i = (int)n_outcomes_tmp;
  i1 = Y_pairs->size[0] * Y_pairs->size[1];
  Y_pairs->size[0] = (int)n_outcomes_tmp;
  Y_pairs->size[1] = 2;
  emxEnsureCapacity_uint32_T(Y_pairs, i1);
  Y_pairs_data = Y_pairs->data;
  loop_ub_tmp = (int)n_outcomes_tmp << 1;
  for (i1 = 0; i1 < loop_ub_tmp; i1++) {
    Y_pairs_data[i1] = 0U;
  }
  row = 1U;
  for (b_y1 = 0; b_y1 < K; b_y1++) {
    for (y2 = 0; y2 < K; y2++) {
      loop_ub_tmp = (int)(row + (unsigned int)y2) - 1;
      Y_pairs_data[loop_ub_tmp] = (unsigned int)b_y1;
      Y_pairs_data[loop_ub_tmp + Y_pairs->size[0]] = (unsigned int)y2;
    }
    if (K - 1 >= 0) {
      row += (unsigned int)K;
    }
  }
  i1 = Xwp->size[0];
  emxInit_real_T(&X_pair, 2);
  emxInit_real_T(&Hess, 2);
  emxInit_real_T(&x_pp, 1);
  emxInit_real_T(&x_pm, 1);
  emxInit_real_T(&x_mp, 1);
  emxInit_real_T(&x_mm, 1);
  for (b_i = 0; b_i <= i1 - 2; b_i++) {
    b_y1 = Xwp->size[0] - b_i;
    for (j = 0; j <= b_y1 - 2; j++) {
      y2 = b_i + j;
      i2 = X_pair->size[0] * X_pair->size[1];
      X_pair->size[0] = 2;
      loop_ub_tmp = Xwp->size[1];
      X_pair->size[1] = Xwp->size[1];
      emxEnsureCapacity_real_T(X_pair, i2);
      X_pair_data = X_pair->data;
      for (i2 = 0; i2 < loop_ub_tmp; i2++) {
        X_pair_data[2 * i2] = Xwp_data[b_i + Xwp->size[0] * i2];
        X_pair_data[2 * i2 + 1] = Xwp_data[(y2 + Xwp->size[0] * i2) + 1];
      }
      for (k = 0; k < i; k++) {
        double y_pair[2];
        double f0;
        y_pair[0] = Y_pairs_data[k];
        y_pair[1] = Y_pairs_data[k + Y_pairs->size[0]];
        n_outcomes_tmp =
            evaluate_copula_pmf(y_pair, X_pair, theta, K, q, type,
                                lambda_fixed_data, lambda_fixed_size);
        i2 = Hess->size[0] * Hess->size[1];
        Hess->size[0] = loop_ub;
        Hess->size[1] = loop_ub;
        emxEnsureCapacity_real_T(Hess, i2);
        X_pair_data = Hess->data;
        f0 = log(n_outcomes_tmp);
        for (y2 = 0; y2 < loop_ub; y2++) {
          for (loop_ub_tmp = 0; loop_ub_tmp < loop_ub; loop_ub_tmp++) {
            if (y2 == loop_ub_tmp) {
              double f1;
              i2 = x_pp->size[0];
              x_pp->size[0] = loop_ub;
              emxEnsureCapacity_real_T(x_pp, i2);
              x_pp_data = x_pp->data;
              for (i2 = 0; i2 < loop_ub; i2++) {
                x_pp_data[i2] = theta_data[i2];
              }
              x_pp_data[y2] = theta_data[y2] + 0.0001;
              f1 = log(evaluate_copula_pmf(y_pair, X_pair, x_pp, K, q, type,
                                           lambda_fixed_data,
                                           lambda_fixed_size));
              i2 = x_pp->size[0];
              x_pp->size[0] = loop_ub;
              emxEnsureCapacity_real_T(x_pp, i2);
              x_pp_data = x_pp->data;
              for (i2 = 0; i2 < loop_ub; i2++) {
                x_pp_data[i2] = theta_data[i2];
              }
              x_pp_data[y2] = theta_data[y2] - 0.0001;
              X_pair_data[y2 + Hess->size[0] * loop_ub_tmp] =
                  ((f1 - 2.0 * f0) +
                   log(evaluate_copula_pmf(y_pair, X_pair, x_pp, K, q, type,
                                           lambda_fixed_data,
                                           lambda_fixed_size))) /
                  1.0E-8;
            } else {
              i2 = x_pp->size[0];
              x_pp->size[0] = loop_ub;
              emxEnsureCapacity_real_T(x_pp, i2);
              x_pp_data = x_pp->data;
              i2 = x_pm->size[0];
              x_pm->size[0] = loop_ub;
              emxEnsureCapacity_real_T(x_pm, i2);
              x_pm_data = x_pm->data;
              i2 = x_mp->size[0];
              x_mp->size[0] = loop_ub;
              emxEnsureCapacity_real_T(x_mp, i2);
              x_mp_data = x_mp->data;
              i2 = x_mm->size[0];
              x_mm->size[0] = loop_ub;
              emxEnsureCapacity_real_T(x_mm, i2);
              x_mm_data = x_mm->data;
              for (i2 = 0; i2 < loop_ub; i2++) {
                x_pp_data[i2] = theta_data[i2];
                x_pm_data[i2] = theta_data[i2];
                x_mp_data[i2] = theta_data[i2];
                x_mm_data[i2] = theta_data[i2];
              }
              x_pp_data[y2] = theta_data[y2] + 0.0001;
              x_pp_data[loop_ub_tmp] += 0.0001;
              x_pm_data[y2] = theta_data[y2] + 0.0001;
              x_pm_data[loop_ub_tmp] -= 0.0001;
              x_mp_data[y2] = theta_data[y2] - 0.0001;
              x_mp_data[loop_ub_tmp] += 0.0001;
              x_mm_data[y2] = theta_data[y2] - 0.0001;
              x_mm_data[loop_ub_tmp] -= 0.0001;
              X_pair_data[y2 + Hess->size[0] * loop_ub_tmp] =
                  (((log(evaluate_copula_pmf(y_pair, X_pair, x_pp, K, q, type,
                                             lambda_fixed_data,
                                             lambda_fixed_size)) -
                     log(evaluate_copula_pmf(y_pair, X_pair, x_pm, K, q, type,
                                             lambda_fixed_data,
                                             lambda_fixed_size))) -
                    log(evaluate_copula_pmf(y_pair, X_pair, x_mp, K, q, type,
                                            lambda_fixed_data,
                                            lambda_fixed_size))) +
                   log(evaluate_copula_pmf(y_pair, X_pair, x_mm, K, q, type,
                                           lambda_fixed_data,
                                           lambda_fixed_size))) /
                  4.0E-8;
            }
          }
        }
        if ((b_I->size[0] == Hess->size[0]) &&
            (b_I->size[1] == Hess->size[1])) {
          loop_ub_tmp = b_I->size[0] * b_I->size[1];
          for (i2 = 0; i2 < loop_ub_tmp; i2++) {
            I_data[i2] += n_outcomes_tmp * -X_pair_data[i2];
          }
        } else {
          binary_expand_op_10(b_I, n_outcomes_tmp, Hess);
          I_data = b_I->data;
        }
      }
    }
  }
  /* v0.2.2: weight the pairwise block accumulation by 1/(n-1), the standard
     composite-likelihood weighting for unequal cluster sizes (Varin, Reid,
     and Firth, 2011, Statistica Sinica 21:5-42). Each observation enters
     n-1 sub-plot pairs, so this restores a common per-observation counting
     rate across blocks of unequal size. For balanced designs the factor is
     a design-independent constant and leaves design selection unchanged. */
  if (Xwp->size[0] > 1) {
    double w_pcl = 1.0 / ((double)Xwp->size[0] - 1.0);
    int w_n = b_I->size[0] * b_I->size[1];
    int w_i;
    for (w_i = 0; w_i < w_n; w_i++) {
      b_I->data[w_i] *= w_pcl;
    }
  }
  emxFree_real_T(&x_mm);
  emxFree_real_T(&x_mp);
  emxFree_real_T(&x_pm);
  emxFree_real_T(&x_pp);
  emxFree_real_T(&Hess);
  emxFree_real_T(&X_pair);
  emxFree_uint32_T(&Y_pairs);
}

/*
 * Arguments    : const emxArray_real_T *y
 *                const emxArray_real_T *Xwp
 *                const emxArray_real_T *th
 *                int K
 *                double q
 *                double type
 *                const double lambda_fixed_data[]
 *                const int lambda_fixed_size[2]
 * Return Type  : double
 */
static double b_evaluate_copula_pmf(const emxArray_real_T *y,
                                    const emxArray_real_T *Xwp,
                                    const emxArray_real_T *th, int K, double q,
                                    double type,
                                    const double lambda_fixed_data[],
                                    const int lambda_fixed_size[2])
{
  emxArray_int8_T *W_current;
  emxArray_real_T *xb;
  emxArray_uint32_T *active_idx;
  const double *Xwp_data;
  const double *th_data;
  const double *y_data;
  double lam;
  double pr;
  double *xb_data;
  int aoffset;
  int b_i;
  int i;
  int k;
  int mc;
  unsigned int *active_idx_data;
  signed char *W_current_data;
  th_data = th->data;
  Xwp_data = Xwp->data;
  y_data = y->data;
  if (((double)K - 1.0) + 1.0 > ((double)K - 1.0) + q) {
    i = 0;
  } else {
    i = (int)(((double)K - 1.0) + 1.0) - 1;
  }
  if ((lambda_fixed_size[0] == 0) || (lambda_fixed_size[1] == 0)) {
    lam = th_data[th->size[0] - 1];
  } else {
    lam = lambda_fixed_data[0];
  }
  emxInit_real_T(&xb, 1);
  emxInit_uint32_T(&active_idx, 1);
  emxInit_int8_T(&W_current);
  if (type == 0.0) {
    int inner;
    int n_tmp;
    unsigned int num_active;
    n_tmp = y->size[0];
    mc = Xwp->size[0] - 1;
    inner = Xwp->size[1];
    k = xb->size[0];
    xb->size[0] = Xwp->size[0];
    emxEnsureCapacity_real_T(xb, k);
    xb_data = xb->data;
    for (b_i = 0; b_i <= mc; b_i++) {
      xb_data[b_i] = 0.0;
    }
    for (k = 0; k < inner; k++) {
      aoffset = k * Xwp->size[0];
      for (b_i = 0; b_i <= mc; b_i++) {
        xb_data[b_i] += Xwp_data[aoffset + b_i] * th_data[i + k];
      }
    }
    num_active = 0U;
    i = active_idx->size[0];
    active_idx->size[0] = y->size[0];
    emxEnsureCapacity_uint32_T(active_idx, i);
    active_idx_data = active_idx->data;
    for (i = 0; i < n_tmp; i++) {
      active_idx_data[i] = 0U;
    }
    for (aoffset = 0; aoffset < n_tmp; aoffset++) {
      if (y_data[aoffset] > 0.0) {
        num_active++;
        active_idx_data[(int)num_active - 1] = (unsigned int)(aoffset + 1);
      }
    }
    pr = 0.0;
    i = (int)rt_powd_snf(2.0, num_active);
    k = (int)num_active;
    for (b_i = 0; b_i < i; b_i++) {
      double TERM1;
      double temp;
      double w_sum;
      bool exitg1;
      bool skip;
      temp = ((double)b_i + 1.0) - 1.0;
      aoffset = W_current->size[0];
      W_current->size[0] = n_tmp;
      emxEnsureCapacity_int8_T(W_current, aoffset);
      W_current_data = W_current->data;
      for (aoffset = 0; aoffset < n_tmp; aoffset++) {
        W_current_data[aoffset] = 0;
      }
      w_sum = 0.0;
      for (mc = 0; mc < k; mc++) {
        if (temp == 0.0) {
          aoffset = 0;
        } else {
          aoffset = (int)fmod(temp, 2.0);
        }
        W_current_data[(int)active_idx_data[((int)num_active - mc) - 1] - 1] =
            (signed char)aoffset;
        w_sum += (double)aoffset;
        temp /= 2.0;
        temp = floor(temp);
      }
      TERM1 = 1.0;
      skip = false;
      aoffset = 0;
      exitg1 = false;
      while ((!exitg1) && (aoffset <= n_tmp - 1)) {
        temp = y_data[aoffset] - (double)W_current_data[aoffset];
        if (temp < 0.0) {
          temp = 0.0;
        } else if (temp >= (double)K - 1.0) {
          temp = 1.0;
        } else {
          temp =
              1.0 /
              (exp(-(th_data[(int)(temp + 1.0) - 1] + xb_data[aoffset])) + 1.0);
        }
        if (temp < 1.0E-10) {
          skip = true;
          exitg1 = true;
        } else {
          TERM1 *= exp(-lam * temp) - 1.0;
          aoffset++;
        }
      }
      if (!skip) {
        pr += rt_powd_snf(-1.0, w_sum) *
              (-(1.0 / lam) *
               log(TERM1 / rt_powd_snf(exp(-lam) - 1.0, (double)n_tmp - 1.0) +
                   1.0));
      }
    }
  } else {
    int inner;
    int n_tmp;
    unsigned int num_active;
    n_tmp = y->size[0];
    mc = Xwp->size[0] - 1;
    inner = Xwp->size[1];
    k = xb->size[0];
    xb->size[0] = Xwp->size[0];
    emxEnsureCapacity_real_T(xb, k);
    xb_data = xb->data;
    for (b_i = 0; b_i <= mc; b_i++) {
      xb_data[b_i] = 0.0;
    }
    for (k = 0; k < inner; k++) {
      aoffset = k * Xwp->size[0];
      for (b_i = 0; b_i <= mc; b_i++) {
        xb_data[b_i] += Xwp_data[aoffset + b_i] * th_data[i + k];
      }
    }
    num_active = 0U;
    i = active_idx->size[0];
    active_idx->size[0] = y->size[0];
    emxEnsureCapacity_uint32_T(active_idx, i);
    active_idx_data = active_idx->data;
    for (i = 0; i < n_tmp; i++) {
      active_idx_data[i] = 0U;
    }
    for (aoffset = 0; aoffset < n_tmp; aoffset++) {
      if (y_data[aoffset] > 0.0) {
        num_active++;
        active_idx_data[(int)num_active - 1] = (unsigned int)(aoffset + 1);
      }
    }
    pr = 0.0;
    i = (int)rt_powd_snf(2.0, num_active);
    k = (int)num_active;
    for (b_i = 0; b_i < i; b_i++) {
      double TERM1;
      double temp;
      double w_sum;
      bool exitg1;
      bool skip;
      temp = ((double)b_i + 1.0) - 1.0;
      aoffset = W_current->size[0];
      W_current->size[0] = n_tmp;
      emxEnsureCapacity_int8_T(W_current, aoffset);
      W_current_data = W_current->data;
      for (aoffset = 0; aoffset < n_tmp; aoffset++) {
        W_current_data[aoffset] = 0;
      }
      w_sum = 0.0;
      for (mc = 0; mc < k; mc++) {
        if (temp == 0.0) {
          aoffset = 0;
        } else {
          aoffset = (int)fmod(temp, 2.0);
        }
        W_current_data[(int)active_idx_data[((int)num_active - mc) - 1] - 1] =
            (signed char)aoffset;
        w_sum += (double)aoffset;
        temp /= 2.0;
        temp = floor(temp);
      }
      TERM1 = 0.0;
      skip = false;
      aoffset = 0;
      exitg1 = false;
      while ((!exitg1) && (aoffset <= n_tmp - 1)) {
        temp = y_data[aoffset] - (double)W_current_data[aoffset];
        if (temp < 0.0) {
          temp = 0.0;
        } else if (temp >= (double)K - 1.0) {
          temp = 1.0;
        } else {
          temp =
              1.0 /
              (exp(-(th_data[(int)(temp + 1.0) - 1] + xb_data[aoffset])) + 1.0);
        }
        if (temp < 1.0E-10) {
          skip = true;
          exitg1 = true;
        } else {
          TERM1 += rt_powd_snf(temp, -lam);
          aoffset++;
        }
      }
      if (!skip) {
        pr += rt_powd_snf(-1.0, w_sum) *
              rt_powd_snf(fmax(TERM1 - ((double)n_tmp - 1.0), 0.0), -1.0 / lam);
      }
    }
  }
  emxFree_int8_T(&W_current);
  emxFree_uint32_T(&active_idx);
  emxFree_real_T(&xb);
  return fmax(pr, 2.2204460492503131E-16);
}

/*
 * Arguments    : emxArray_real_T *in1
 *                int in2
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op(emxArray_real_T *in1, int in2,
                             const emxArray_real_T *in3)
{
  emxArray_real_T *b_in1;
  const double *in3_data;
  double *b_in1_data;
  double *in1_data;
  int i;
  int in1_idx_0_tmp;
  int stride_0_0;
  int stride_1_0;
  in3_data = in3->data;
  in1_data = in1->data;
  in1_idx_0_tmp = in1->size[0];
  emxInit_real_T(&b_in1, 1);
  i = b_in1->size[0];
  b_in1->size[0] = in1_idx_0_tmp;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_1_0 = (in3->size[0] != 1);
  for (i = 0; i < in1_idx_0_tmp; i++) {
    b_in1_data[i] = in1_data[i * stride_0_0 + in1->size[0] * in2] +
                    in3_data[i * stride_1_0];
  }
  for (i = 0; i < in1_idx_0_tmp; i++) {
    in1_data[i + in1->size[0] * (in2 + 1)] = b_in1_data[i];
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : emxArray_real_T *in1
 *                int in2
 *                const emxArray_real_T *in3
 *                const emxArray_real_T *in4
 * Return Type  : void
 */
static void binary_expand_op_1(emxArray_real_T *in1, int in2,
                               const emxArray_real_T *in3,
                               const emxArray_real_T *in4)
{
  const double *in3_data;
  const double *in4_data;
  double *in1_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  in4_data = in4->data;
  in3_data = in3->data;
  in1_data = in1->data;
  stride_0_0 = (in3->size[0] != 1);
  stride_1_0 = (in4->size[0] != 1);
  loop_ub = in1->size[0];
  for (i = 0; i < loop_ub; i++) {
    in1_data[i + in1->size[0] * in2] =
        in3_data[i * stride_0_0] + in4_data[i * stride_1_0];
  }
}

/*
 * Arguments    : emxArray_real_T *in1
 *                double in2
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op_10(emxArray_real_T *in1, double in2,
                                const emxArray_real_T *in3)
{
  emxArray_real_T *b_in1;
  const double *in3_data;
  double *b_in1_data;
  double *in1_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_0_1;
  int stride_1_0;
  int stride_1_1;
  in3_data = in3->data;
  in1_data = in1->data;
  emxInit_real_T(&b_in1, 2);
  if (in3->size[0] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = in3->size[0];
  }
  i = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = loop_ub;
  if (in3->size[1] == 1) {
    b_loop_ub = in1->size[1];
  } else {
    b_loop_ub = in3->size[1];
  }
  b_in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_0_1 = (in1->size[1] != 1);
  stride_1_0 = (in3->size[0] != 1);
  stride_1_1 = (in3->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in1_data[i1 + b_in1->size[0] * i] =
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1] +
          in2 * -in3_data[i1 * stride_1_0 + in3->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  i = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in1_data[i1 + b_in1->size[0] * i];
    }
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op_2(emxArray_real_T *in1, const emxArray_real_T *in2,
                               const emxArray_real_T *in3)
{
  emxArray_real_T *b_in1;
  const double *in2_data;
  const double *in3_data;
  double *b_in1_data;
  double *in1_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  int stride_2_0;
  in3_data = in3->data;
  in2_data = in2->data;
  in1_data = in1->data;
  emxInit_real_T(&b_in1, 1);
  if (in3->size[0] == 1) {
    i = in2->size[0];
  } else {
    i = in3->size[0];
  }
  if (i == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = i;
  }
  i = b_in1->size[0];
  b_in1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_1_0 = (in2->size[0] != 1);
  stride_2_0 = (in3->size[0] != 1);
  for (i = 0; i < loop_ub; i++) {
    b_in1_data[i] = in1_data[i * stride_0_0] *
                    (in2_data[i * stride_1_0] - in3_data[i * stride_2_0]);
  }
  i = in1->size[0];
  in1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  for (i = 0; i < loop_ub; i++) {
    in1_data[i] = b_in1_data[i];
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : emxArray_real_T *in1
 *                double in2
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op_3(emxArray_real_T *in1, double in2,
                               const emxArray_real_T *in3)
{
  emxArray_real_T *b_in1;
  const double *in3_data;
  double *b_in1_data;
  double *in1_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_0_1;
  int stride_1_0;
  int stride_1_1;
  in3_data = in3->data;
  in1_data = in1->data;
  emxInit_real_T(&b_in1, 2);
  if (in3->size[0] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = in3->size[0];
  }
  i = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = loop_ub;
  if (in3->size[1] == 1) {
    b_loop_ub = in1->size[1];
  } else {
    b_loop_ub = in3->size[1];
  }
  b_in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_0_1 = (in1->size[1] != 1);
  stride_1_0 = (in3->size[0] != 1);
  stride_1_1 = (in3->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in1_data[i1 + b_in1->size[0] * i] =
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1] +
          in2 * in3_data[i1 * stride_1_0 + in3->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  i = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in1_data[i1 + b_in1->size[0] * i];
    }
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 *                double in3
 *                const emxArray_real_T *in4
 *                const emxArray_real_T *in5
 * Return Type  : void
 */
static void binary_expand_op_5(emxArray_real_T *in1, const emxArray_real_T *in2,
                               double in3, const emxArray_real_T *in4,
                               const emxArray_real_T *in5)
{
  emxArray_real_T *b_in2;
  const double *in2_data;
  const double *in4_data;
  double *b_in2_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_0_1;
  int stride_1_0;
  int stride_1_1;
  in4_data = in4->data;
  in2_data = in2->data;
  emxInit_real_T(&b_in2, 2);
  if (in4->size[0] == 1) {
    loop_ub = in2->size[0];
  } else {
    loop_ub = in4->size[0];
  }
  i = b_in2->size[0] * b_in2->size[1];
  b_in2->size[0] = loop_ub;
  if (in4->size[1] == 1) {
    b_loop_ub = in2->size[1];
  } else {
    b_loop_ub = in4->size[1];
  }
  b_in2->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(b_in2, i);
  b_in2_data = b_in2->data;
  stride_0_0 = (in2->size[0] != 1);
  stride_0_1 = (in2->size[1] != 1);
  stride_1_0 = (in4->size[0] != 1);
  stride_1_1 = (in4->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in2_data[i1 + b_in2->size[0] * i] =
          in2_data[i1 * stride_0_0 + in2->size[0] * aux_0_1] +
          in3 * in4_data[i1 * stride_1_0 + in4->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  mldivide(b_in2, in5, in1);
  emxFree_real_T(&b_in2);
}

/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 *                int in3
 *                const emxArray_real_T *in4
 * Return Type  : void
 */
static void binary_expand_op_6(emxArray_real_T *in1, const emxArray_real_T *in2,
                               int in3, const emxArray_real_T *in4)
{
  emxArray_real_T *b_in1;
  emxArray_real_T *b_in4;
  const double *in2_data;
  const double *in4_data;
  double b_in2;
  double *b_in1_data;
  double *b_in4_data;
  double *in1_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_0_1;
  int stride_1_0_tmp;
  in4_data = in4->data;
  in2_data = in2->data;
  in1_data = in1->data;
  b_in2 = in2_data[in3];
  emxInit_real_T(&b_in4, 2);
  loop_ub = in4->size[0];
  i = b_in4->size[0] * b_in4->size[1];
  b_in4->size[0] = loop_ub;
  b_in4->size[1] = loop_ub;
  emxEnsureCapacity_real_T(b_in4, i);
  b_in4_data = b_in4->data;
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in4_data[i1 + b_in4->size[0] * i] =
          in4_data[i1 + in4->size[0] * in3] * in4_data[i + in4->size[0] * in3];
    }
  }
  emxInit_real_T(&b_in1, 2);
  if (b_in4->size[0] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = b_in4->size[0];
  }
  i = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = loop_ub;
  if (b_in4->size[1] == 1) {
    b_loop_ub = in1->size[1];
  } else {
    b_loop_ub = b_in4->size[1];
  }
  b_in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_0_1 = (in1->size[1] != 1);
  stride_1_0_tmp = (b_in4->size[0] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in1_data[i1 + b_in1->size[0] * i] =
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1] +
          b_in2 * b_in4_data[i1 * stride_1_0_tmp + b_in4->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_0_tmp;
    aux_0_1 += stride_0_1;
  }
  emxFree_real_T(&b_in4);
  i = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in1_data[i1 + b_in1->size[0] * i];
    }
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : emxArray_real_T *in1
 *                double in2
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op_7(emxArray_real_T *in1, double in2,
                               const emxArray_real_T *in3)
{
  emxArray_real_T *b_in1;
  emxArray_real_T *b_in3;
  const double *in3_data;
  double *b_in1_data;
  double *b_in3_data;
  double *in1_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_0_1;
  int stride_1_0_tmp;
  in3_data = in3->data;
  in1_data = in1->data;
  emxInit_real_T(&b_in3, 2);
  loop_ub = in3->size[0];
  i = b_in3->size[0] * b_in3->size[1];
  b_in3->size[0] = loop_ub;
  b_in3->size[1] = loop_ub;
  emxEnsureCapacity_real_T(b_in3, i);
  b_in3_data = b_in3->data;
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in3_data[i1 + b_in3->size[0] * i] = in3_data[i1] * in3_data[i];
    }
  }
  emxInit_real_T(&b_in1, 2);
  if (b_in3->size[0] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = b_in3->size[0];
  }
  i = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = loop_ub;
  if (b_in3->size[1] == 1) {
    b_loop_ub = in1->size[1];
  } else {
    b_loop_ub = b_in3->size[1];
  }
  b_in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_0_1 = (in1->size[1] != 1);
  stride_1_0_tmp = (b_in3->size[0] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in1_data[i1 + b_in1->size[0] * i] =
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1] +
          in2 * b_in3_data[i1 * stride_1_0_tmp + b_in3->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_0_tmp;
    aux_0_1 += stride_0_1;
  }
  emxFree_real_T(&b_in3);
  i = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in1_data[i1 + b_in1->size[0] * i];
    }
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : const double wp_lev_data[]
 *                const int wp_lev_size[2]
 *                const double sp_lev_data[]
 *                const int sp_lev_size[2]
 *                const emxArray_cell_wrap_0 *modelTerms
 *                emxArray_real_T *xrow
 * Return Type  : void
 */
static void
build_model_row(const double wp_lev_data[], const int wp_lev_size[2],
                const double sp_lev_data[], const int sp_lev_size[2],
                const emxArray_cell_wrap_0 *modelTerms, emxArray_real_T *xrow)
{
  const cell_wrap_0 *modelTerms_data;
  double factor_lev_data[40];
  double *xrow_data;
  int i;
  int k;
  int loop_ub;
  int t;
  modelTerms_data = modelTerms->data;
  loop_ub = wp_lev_size[1];
  if (loop_ub - 1 >= 0) {
    memcpy(&factor_lev_data[0], &wp_lev_data[0],
           (unsigned int)loop_ub * sizeof(double));
  }
  loop_ub = sp_lev_size[1];
  for (i = 0; i < loop_ub; i++) {
    factor_lev_data[i + wp_lev_size[1]] = sp_lev_data[i];
  }
  i = xrow->size[0] * xrow->size[1];
  xrow->size[0] = 1;
  loop_ub = modelTerms->size[1];
  xrow->size[1] = modelTerms->size[1];
  emxEnsureCapacity_real_T(xrow, i);
  xrow_data = xrow->data;
  for (t = 0; t < loop_ub; t++) {
    i = modelTerms_data[t].f1->size[1];
    if (i == 0) {
      xrow_data[t] = 1.0;
    } else {
      double prod_val;
      prod_val = 1.0;
      for (k = 0; k < i; k++) {
        prod_val *= factor_lev_data[(int)modelTerms_data[t].f1->data[k] - 1];
      }
      xrow_data[t] = prod_val;
    }
  }
}

/*
 * Arguments    : const emxArray_real_T *idx_wp
 *                const emxArray_real_T *idx_sp_full
 *                double m
 *                const emxArray_real_T *n
 *                double N
 *                const emxArray_real_T *wp_combos
 *                const emxArray_real_T *sp_combos
 *                double w
 *                double s
 * Return Type  : double
 */
static double calculate_penalty(const emxArray_real_T *idx_wp,
                                const emxArray_real_T *idx_sp_full, double m,
                                const emxArray_real_T *n, double N,
                                const emxArray_real_T *wp_combos,
                                const emxArray_real_T *sp_combos, double w,
                                double s)
{
  emxArray_real_T *fac_vals;
  emxArray_real_T *fac_wp;
  emxArray_real_T *full_factor_X;
  const double *idx_sp_full_data;
  const double *idx_wp_data;
  const double *n_data;
  const double *sp_combos_data;
  const double *wp_combos_data;
  double curr_start;
  double penalty;
  double row;
  double *fac_vals_data;
  double *fac_wp_data;
  double *full_factor_X_data;
  int ff;
  int i;
  int i1;
  int i2;
  int i3;
  int loop_ub;
  int loop_ub_tmp;
  int sub;
  int wp;
  sp_combos_data = sp_combos->data;
  wp_combos_data = wp_combos->data;
  n_data = n->data;
  idx_sp_full_data = idx_sp_full->data;
  idx_wp_data = idx_wp->data;
  /*  =================================================================== */
  /*  SHARED PENALTY LOGIC */
  /*  =================================================================== */
  penalty = 0.0;
  emxInit_real_T(&full_factor_X, 2);
  loop_ub_tmp = (int)N;
  i = full_factor_X->size[0] * full_factor_X->size[1];
  full_factor_X->size[0] = (int)N;
  i1 = (int)(w + s);
  full_factor_X->size[1] = i1;
  emxEnsureCapacity_real_T(full_factor_X, i);
  full_factor_X_data = full_factor_X->data;
  loop_ub = (int)N * i1;
  for (i = 0; i < loop_ub; i++) {
    full_factor_X_data[i] = 0.0;
  }
  curr_start = 1.0;
  i = (int)m;
  for (wp = 0; wp < i; wp++) {
    i2 = (int)n_data[wp];
    for (sub = 0; sub < i2; sub++) {
      row = (curr_start + ((double)sub + 1.0)) - 1.0;
      if (w < 1.0) {
        loop_ub = 0;
      } else {
        loop_ub = (int)w;
      }
      for (ff = 0; ff < loop_ub; ff++) {
        full_factor_X_data[((int)row + full_factor_X->size[0] * ff) - 1] =
            wp_combos_data[((int)idx_wp_data[wp] + wp_combos->size[0] * ff) -
                           1];
      }
      if (w + 1.0 > full_factor_X->size[1]) {
        ff = 0;
        i3 = 0;
      } else {
        ff = (int)(w + 1.0) - 1;
        i3 = i1;
      }
      loop_ub = i3 - ff;
      for (i3 = 0; i3 < loop_ub; i3++) {
        full_factor_X_data[((int)row + full_factor_X->size[0] * (ff + i3)) -
                           1] =
            sp_combos_data[((int)idx_sp_full_data[(int)row - 1] +
                            sp_combos->size[0] * i3) -
                           1];
      }
    }
    curr_start += n_data[wp];
  }
  i1 = (int)w;
  emxInit_real_T(&fac_vals, 1);
  emxInit_real_T(&fac_wp, 1);
  for (ff = 0; ff < i1; ff++) {
    i2 = fac_vals->size[0];
    fac_vals->size[0] = (int)N;
    emxEnsureCapacity_real_T(fac_vals, i2);
    fac_vals_data = fac_vals->data;
    for (loop_ub = 0; loop_ub < loop_ub_tmp; loop_ub++) {
      fac_vals_data[loop_ub] =
          full_factor_X_data[loop_ub + full_factor_X->size[0] * ff];
    }
    row = var(fac_vals);
    if (row < 0.0) {
      penalty += 0.0 * (1.0 / (row + 2.2204460492503131E-16));
    }
    if (fac_vals->size[0] != 0) {
      i2 = fac_wp->size[0];
      fac_wp->size[0] = (int)N;
      emxEnsureCapacity_real_T(fac_wp, i2);
      fac_wp_data = fac_wp->data;
      for (i2 = 0; i2 < loop_ub_tmp; i2++) {
        fac_wp_data[i2] = fac_vals_data[i2];
      }
      sort(fac_wp);
    }
  }
  i1 = (int)s;
  for (ff = 0; ff < i1; ff++) {
    double fcol;
    fcol = w + ((double)ff + 1.0);
    i2 = fac_vals->size[0];
    fac_vals->size[0] = (int)N;
    emxEnsureCapacity_real_T(fac_vals, i2);
    fac_vals_data = fac_vals->data;
    for (loop_ub = 0; loop_ub < loop_ub_tmp; loop_ub++) {
      fac_vals_data[loop_ub] =
          full_factor_X_data[loop_ub +
                             full_factor_X->size[0] * ((int)fcol - 1)];
    }
    row = var(fac_vals);
    if (row < 0.0) {
      penalty += 0.0 * (1.0 / (row + 2.2204460492503131E-16));
    }
    curr_start = 1.0;
    for (wp = 0; wp < i; wp++) {
      loop_ub = (int)n_data[wp];
      i2 = fac_wp->size[0];
      fac_wp->size[0] = (int)n_data[wp];
      emxEnsureCapacity_real_T(fac_wp, i2);
      fac_wp_data = fac_wp->data;
      for (i2 = 0; i2 < loop_ub; i2++) {
        fac_wp_data[i2] = 0.0;
      }
      for (sub = 0; sub < loop_ub; sub++) {
        fac_wp_data[sub] =
            full_factor_X_data[((int)((curr_start + ((double)sub + 1.0)) -
                                      1.0) +
                                full_factor_X->size[0] * ((int)fcol - 1)) -
                               1];
      }
      if (fac_wp->size[0] != 0) {
        sort(fac_wp);
      }
      curr_start += n_data[wp];
    }
  }
  emxFree_real_T(&fac_wp);
  emxFree_real_T(&fac_vals);
  emxFree_real_T(&full_factor_X);
  return penalty;
}

/*
 * Arguments    : const double idx_data[]
 *                double m
 *                const emxArray_real_T *n
 *                double N
 *                const emxArray_real_T *weights
 *                double p
 *                const char crit_mode_data[]
 *                const int crit_mode_size[2]
 *                const emxArray_real_T *wp_combos
 *                const emxArray_real_T *sp_combos
 *                const emxArray_real_T *thetaGrid
 *                int K_int
 *                double q
 *                const double sigma2_fixed_data[]
 *                const int sigma2_fixed_size[2]
 *                const double lambda_fixed_data[]
 *                const int lambda_fixed_size[2]
 *                double copulaType
 *                bool is_glmm_approx
 *                bool is_glmm_exact
 *                bool is_copula_pcl
 *                bool is_copula_pcl_godambe
 *                bool is_indep_glm
 *                const emxArray_cell_wrap_0 *modelTerms
 * Return Type  : double
 */
static double computeCriterion_otf(
    const double idx_data[], double m, const emxArray_real_T *n, double N,
    const emxArray_real_T *weights, double p, const char crit_mode_data[],
    const int crit_mode_size[2], const emxArray_real_T *wp_combos,
    const emxArray_real_T *sp_combos, const emxArray_real_T *thetaGrid,
    int K_int, double q, const double sigma2_fixed_data[],
    const int sigma2_fixed_size[2], const double lambda_fixed_data[],
    const int lambda_fixed_size[2], double copulaType, bool is_glmm_approx,
    bool is_glmm_exact, bool is_copula_pcl, bool is_copula_pcl_godambe,
    bool is_indep_glm, const emxArray_cell_wrap_0 *modelTerms)
{
  emxArray_real_T *H_wp;
  emxArray_real_T *I_cond;
  emxArray_real_T *I_total;
  emxArray_real_T *I_wp;
  emxArray_real_T *PrY;
  emxArray_real_T *Scores;
  emxArray_real_T *X_all;
  emxArray_real_T *Xwp;
  emxArray_real_T *allY;
  emxArray_real_T *alpha;
  emxArray_real_T *b_Xwp;
  emxArray_real_T *idx_sp_full;
  emxArray_real_T *idx_wp;
  emxArray_real_T *logDets;
  emxArray_real_T *r;
  emxArray_real_T *y;
  const double *n_data;
  const double *sp_combos_data;
  const double *thetaGrid_data;
  const double *weights_data;
  const double *wp_combos_data;
  double curr_start;
  double penalty;
  double sp_val_idx;
  double *I_total_data;
  double *I_wp_data;
  double *PrY_data;
  double *Scores_data;
  double *X_all_data;
  double *alpha_data;
  double *idx_sp_full_data;
  double *idx_wp_data;
  double *logDets_data;
  double *y_data;
  int sp_combos_size[2];
  int wp_combos_size[2];
  int b_i;
  int b_loop_ub;
  int b_loop_ub_tmp;
  int c_loop_ub;
  int i;
  int i1;
  int i2;
  int i3;
  int idx;
  int j;
  int loop_ub;
  int loop_ub_tmp;
  int sub;
  int t;
  int wp;
  thetaGrid_data = thetaGrid->data;
  sp_combos_data = sp_combos->data;
  wp_combos_data = wp_combos->data;
  weights_data = weights->data;
  n_data = n->data;
  /*  =================================================================== */
  /*  ON-THE-FLY ENGINE FUNCTIONS (MULTI-THREADED MATH) */
  /*  =================================================================== */
  i = (int)m;
  emxInit_real_T(&idx_wp, 1);
  i1 = idx_wp->size[0];
  idx_wp->size[0] = (int)m;
  emxEnsureCapacity_real_T(idx_wp, i1);
  idx_wp_data = idx_wp->data;
  for (b_i = 0; b_i < i; b_i++) {
    idx_wp_data[b_i] = fmin(idx_data[b_i], wp_combos->size[0]);
  }
  i1 = (int)N;
  emxInit_real_T(&idx_sp_full, 1);
  i2 = idx_sp_full->size[0];
  idx_sp_full->size[0] = (int)N;
  emxEnsureCapacity_real_T(idx_sp_full, i2);
  idx_sp_full_data = idx_sp_full->data;
  for (b_i = 0; b_i < i1; b_i++) {
    idx_sp_full_data[b_i] =
        fmin(idx_data[(int)(m + ((double)b_i + 1.0)) - 1], sp_combos->size[0]);
  }
  penalty =
      calculate_penalty(idx_wp, idx_sp_full, m, n, N, wp_combos, sp_combos,
                        wp_combos->size[1], sp_combos->size[1]);
  /*  Pre-extract entire active design matrix to static 2D array */
  emxInit_real_T(&X_all, 2);
  i1 = X_all->size[0] * X_all->size[1];
  X_all->size[0] = (int)N;
  loop_ub_tmp = (int)q;
  X_all->size[1] = (int)q;
  emxEnsureCapacity_real_T(X_all, i1);
  X_all_data = X_all->data;
  loop_ub = (int)N * (int)q;
  for (i1 = 0; i1 < loop_ub; i1++) {
    X_all_data[i1] = 0.0;
  }
  curr_start = 1.0;
  emxInit_real_T(&alpha, 2);
  for (wp = 0; wp < i; wp++) {
    i1 = (int)n_data[wp];
    if (i1 - 1 >= 0) {
      wp_combos_size[0] = 1;
      sp_combos_size[0] = 1;
    }
    for (sub = 0; sub < i1; sub++) {
      double b_sp_combos_data[20];
      double b_wp_combos_data[20];
      idx = (int)((curr_start + ((double)sub + 1.0)) - 1.0) - 1;
      sp_val_idx = idx_sp_full_data[idx];
      b_loop_ub_tmp = wp_combos->size[1];
      wp_combos_size[1] = wp_combos->size[1];
      for (i2 = 0; i2 < b_loop_ub_tmp; i2++) {
        b_wp_combos_data[i2] =
            wp_combos_data[((int)idx_wp_data[wp] + wp_combos->size[0] * i2) -
                           1];
      }
      b_loop_ub_tmp = sp_combos->size[1];
      sp_combos_size[1] = sp_combos->size[1];
      for (i2 = 0; i2 < b_loop_ub_tmp; i2++) {
        b_sp_combos_data[i2] =
            sp_combos_data[((int)sp_val_idx + sp_combos->size[0] * i2) - 1];
      }
      build_model_row(b_wp_combos_data, wp_combos_size, b_sp_combos_data,
                      sp_combos_size, modelTerms, alpha);
      alpha_data = alpha->data;
      for (i2 = 0; i2 < loop_ub_tmp; i2++) {
        X_all_data[idx + X_all->size[0] * i2] = alpha_data[i2];
      }
    }
    curr_start += n_data[wp];
  }
  i = thetaGrid->size[0];
  emxInit_real_T(&logDets, 1);
  i1 = logDets->size[0];
  logDets->size[0] = thetaGrid->size[0];
  emxEnsureCapacity_real_T(logDets, i1);
  logDets_data = logDets->data;
  if (thetaGrid->size[0] - 1 >= 0) {
    b_loop_ub = (int)p * (int)p;
    i3 = (int)m;
  }
  emxInit_real_T(&I_total, 2);
  emxInit_real_T(&Xwp, 2);
  emxInit_real_T(&I_wp, 2);
  emxInit_real_T(&H_wp, 2);
  emxInit_real_T(&I_cond, 2);
  emxInit_real_T(&allY, 2);
  emxInit_real_T(&PrY, 1);
  emxInit_real_T(&Scores, 2);
  emxInit_real_T(&y, 1);
  emxInit_real_T(&r, 2);
  emxInit_real_T(&b_Xwp, 2);
  for (t = 0; t < i; t++) {
    i1 = I_total->size[0] * I_total->size[1];
    I_total->size[0] = (int)p;
    I_total->size[1] = (int)p;
    emxEnsureCapacity_real_T(I_total, i1);
    I_total_data = I_total->data;
    for (i1 = 0; i1 < b_loop_ub; i1++) {
      I_total_data[i1] = 0.0;
    }
    curr_start = 1.0;
    for (wp = 0; wp < i3; wp++) {
      double d;
      /*  DYNAMIC ALLOCATION: Prevents Coder variable-size slicing error */
      d = n_data[wp];
      loop_ub = (int)n_data[wp];
      i1 = Xwp->size[0] * Xwp->size[1];
      Xwp->size[0] = (int)d;
      Xwp->size[1] = (int)q;
      emxEnsureCapacity_real_T(Xwp, i1);
      idx_wp_data = Xwp->data;
      c_loop_ub = (int)d * (int)q;
      for (i1 = 0; i1 < c_loop_ub; i1++) {
        idx_wp_data[i1] = 0.0;
      }
      for (c_loop_ub = 0; c_loop_ub < loop_ub; c_loop_ub++) {
        double tmp_data[100];
        idx = (int)((curr_start + ((double)c_loop_ub + 1.0)) - 1.0);
        for (i1 = 0; i1 < loop_ub_tmp; i1++) {
          tmp_data[i1] = X_all_data[(idx + X_all->size[0] * i1) - 1];
        }
        for (i1 = 0; i1 < loop_ub_tmp; i1++) {
          idx_wp_data[c_loop_ub + Xwp->size[0] * i1] = tmp_data[i1];
        }
      }
      if (is_glmm_approx) {
        b_loop_ub_tmp = thetaGrid->size[1];
        i1 = idx_sp_full->size[0];
        idx_sp_full->size[0] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(idx_sp_full, i1);
        idx_sp_full_data = idx_sp_full->data;
        for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
          idx_sp_full_data[i1] = thetaGrid_data[t + thetaGrid->size[0] * i1];
        }
        /*  ===================================================================
         */
        /*  1. GLMM APPROXIMATION FISHER */
        /*  ===================================================================
         */
        if ((double)K_int - 1.0 < 1.0) {
          c_loop_ub = 0;
        } else {
          c_loop_ub = K_int - 1;
        }
        i1 = alpha->size[0] * alpha->size[1];
        alpha->size[0] = 1;
        alpha->size[1] = c_loop_ub;
        emxEnsureCapacity_real_T(alpha, i1);
        alpha_data = alpha->data;
        for (i1 = 0; i1 < c_loop_ub; i1++) {
          alpha_data[i1] = idx_sp_full_data[i1];
        }
        if (((double)K_int - 1.0) + 1.0 > ((double)K_int - 1.0) + q) {
          i1 = 1;
        } else {
          i1 = (int)(((double)K_int - 1.0) + 1.0);
        }
        if (sigma2_fixed_data[0] <= 1.0E-8) {
          i1 = I_wp->size[0] * I_wp->size[1];
          I_wp->size[0] = thetaGrid->size[1];
          I_wp->size[1] = thetaGrid->size[1];
          emxEnsureCapacity_real_T(I_wp, i1);
          I_wp_data = I_wp->data;
          loop_ub = idx_sp_full->size[0] * idx_sp_full->size[0];
          for (i1 = 0; i1 < loop_ub; i1++) {
            I_wp_data[i1] = 0.0;
          }
        } else {
          i2 = I_wp->size[0] * I_wp->size[1];
          I_wp->size[0] = thetaGrid->size[1];
          I_wp->size[1] = thetaGrid->size[1];
          emxEnsureCapacity_real_T(I_wp, i2);
          I_wp_data = I_wp->data;
          idx = idx_sp_full->size[0] * idx_sp_full->size[0];
          for (i2 = 0; i2 < idx; i2++) {
            I_wp_data[i2] = 0.0;
          }
          for (j = 0; j < 16; j++) {
            double temp;
            temp = sqrt(2.0 * sigma2_fixed_data[0]) * dv[j];
            i2 = I_cond->size[0] * I_cond->size[1];
            I_cond->size[0] = b_loop_ub_tmp;
            I_cond->size[1] = b_loop_ub_tmp;
            emxEnsureCapacity_real_T(I_cond, i2);
            y_data = I_cond->data;
            for (i2 = 0; i2 < idx; i2++) {
              y_data[i2] = 0.0;
            }
            for (sub = 0; sub < loop_ub; sub++) {
              sp_val_idx = 0.0;
              i2 = b_Xwp->size[0] * b_Xwp->size[1];
              b_Xwp->size[0] = 1;
              b_Xwp->size[1] = (int)q;
              emxEnsureCapacity_real_T(b_Xwp, i2);
              alpha_data = b_Xwp->data;
              for (i2 = 0; i2 < loop_ub_tmp; i2++) {
                d = idx_wp_data[sub + Xwp->size[0] * i2];
                sp_val_idx += d * idx_sp_full_data[(i1 + i2) - 1];
                alpha_data[i2] = d;
              }
              univariateOrdinalFisher(sp_val_idx + temp, alpha, K_int, b_Xwp,
                                      r);
              alpha_data = r->data;
              if ((I_cond->size[0] == r->size[0]) &&
                  (I_cond->size[1] == r->size[1])) {
                c_loop_ub = I_cond->size[0] * I_cond->size[1];
                for (i2 = 0; i2 < c_loop_ub; i2++) {
                  y_data[i2] += alpha_data[i2];
                }
              } else {
                plus(I_cond, r);
                y_data = I_cond->data;
              }
            }
            if ((I_wp->size[0] == I_cond->size[0]) &&
                (I_wp->size[1] == I_cond->size[1])) {
              c_loop_ub = I_wp->size[0] * I_wp->size[1];
              for (i2 = 0; i2 < c_loop_ub; i2++) {
                I_wp_data[i2] += dv1[j] * y_data[i2];
              }
            } else {
              binary_expand_op_3(I_wp, dv1[j], I_cond);
              I_wp_data = I_wp->data;
            }
          }
        }
      } else if (is_glmm_exact) {
        b_loop_ub_tmp = thetaGrid->size[1];
        i1 = idx_wp->size[0];
        idx_wp->size[0] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(idx_wp, i1);
        idx_wp_data = idx_wp->data;
        for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
          idx_wp_data[i1] = thetaGrid_data[t + thetaGrid->size[0] * i1];
        }
        computeFisher_GLMM_Exact(idx_wp, Xwp, K_int, q, sigma2_fixed_data,
                                 sigma2_fixed_size, I_wp);
        I_wp_data = I_wp->data;
      } else if (is_indep_glm) {
        b_loop_ub_tmp = thetaGrid->size[1];
        i1 = idx_sp_full->size[0];
        idx_sp_full->size[0] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(idx_sp_full, i1);
        idx_sp_full_data = idx_sp_full->data;
        for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
          idx_sp_full_data[i1] = thetaGrid_data[t + thetaGrid->size[0] * i1];
        }
        /*  Treats each sub-plot within the block as an independent observation.
         */
        /*  This yields the cumulative-logit Fisher information ignoring the */
        /*  whole-plot random effect. Used as a diagnostic baseline to test */
        /*  whether dependence modeling materially affects design selection */
        /*  in the operating regime of the prior. */
        /*  ===================================================================
         */
        /*  0. INDEPENDENCE GLM FISHER (no random effect, no copula) */
        /*  ===================================================================
         */
        if ((double)K_int - 1.0 < 1.0) {
          c_loop_ub = 0;
        } else {
          c_loop_ub = K_int - 1;
        }
        i1 = alpha->size[0] * alpha->size[1];
        alpha->size[0] = 1;
        alpha->size[1] = c_loop_ub;
        emxEnsureCapacity_real_T(alpha, i1);
        alpha_data = alpha->data;
        for (i1 = 0; i1 < c_loop_ub; i1++) {
          alpha_data[i1] = idx_sp_full_data[i1];
        }
        if (((double)K_int - 1.0) + 1.0 > ((double)K_int - 1.0) + q) {
          i1 = 1;
        } else {
          i1 = (int)(((double)K_int - 1.0) + 1.0);
        }
        i2 = I_wp->size[0] * I_wp->size[1];
        I_wp->size[0] = thetaGrid->size[1];
        I_wp->size[1] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(I_wp, i2);
        I_wp_data = I_wp->data;
        c_loop_ub = idx_sp_full->size[0] * idx_sp_full->size[0];
        for (i2 = 0; i2 < c_loop_ub; i2++) {
          I_wp_data[i2] = 0.0;
        }
        for (sub = 0; sub < loop_ub; sub++) {
          sp_val_idx = 0.0;
          i2 = b_Xwp->size[0] * b_Xwp->size[1];
          b_Xwp->size[0] = 1;
          b_Xwp->size[1] = (int)q;
          emxEnsureCapacity_real_T(b_Xwp, i2);
          alpha_data = b_Xwp->data;
          for (i2 = 0; i2 < loop_ub_tmp; i2++) {
            d = idx_wp_data[sub + Xwp->size[0] * i2];
            sp_val_idx += d * idx_sp_full_data[(i1 + i2) - 1];
            alpha_data[i2] = d;
          }
          univariateOrdinalFisher(sp_val_idx, alpha, K_int, b_Xwp, r);
          alpha_data = r->data;
          if ((I_wp->size[0] == r->size[0]) && (I_wp->size[1] == r->size[1])) {
            c_loop_ub = I_wp->size[0] * I_wp->size[1];
            for (i2 = 0; i2 < c_loop_ub; i2++) {
              I_wp_data[i2] += alpha_data[i2];
            }
          } else {
            plus(I_wp, r);
            I_wp_data = I_wp->data;
          }
        }
      } else if (is_copula_pcl_godambe) {
        /*  PCL with Godambe information G = H J^{-1} H, evaluated per block. */
        /*  Computes J (variability) and H (sensitivity) via the existing */
        /*  PCL accumulator and combines them in matrix form. */
        b_loop_ub_tmp = thetaGrid->size[1];
        i1 = idx_wp->size[0];
        idx_wp->size[0] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(idx_wp, i1);
        idx_wp_data = idx_wp->data;
        for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
          idx_wp_data[i1] = thetaGrid_data[t + thetaGrid->size[0] * i1];
        }
        computeFisher_Copula(idx_wp, Xwp, K_int, q, copulaType,
                             lambda_fixed_data, lambda_fixed_size, I_wp);
        I_wp_data = I_wp->data;
        i1 = idx_wp->size[0];
        idx_wp->size[0] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(idx_wp, i1);
        idx_wp_data = idx_wp->data;
        for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
          idx_wp_data[i1] = thetaGrid_data[t + thetaGrid->size[0] * i1];
        }
        b_computeFisher_Copula(idx_wp, Xwp, K_int, q, copulaType,
                               lambda_fixed_data, lambda_fixed_size, H_wp);
        /*  Combine sensitivity matrix H and variability matrix J into the */
        /*  Godambe information G = H J^{-1} H. Uses ridge stabilization on J */
        /*  to ensure numerical invertibility. */
        /*  ===================================================================
         */
        /*  3. COPULA FISHER (EXACT & PCL) */
        /*  ===================================================================
         */
        /*  GODAMBE COMBINATION: G = H J^{-1} H */
        /*  ===================================================================
         */
        /*  Ridge stabilization: add a tiny multiple of identity scaled to J's
         */
        /*  diagonal magnitude to ensure positive-definite invertibility. */
        if ((I_wp->size[0] == 1) && (I_wp->size[1] == 1)) {
          i1 = idx_wp->size[0];
          idx_wp->size[0] = 1;
          emxEnsureCapacity_real_T(idx_wp, i1);
          idx_wp_data = idx_wp->data;
          idx_wp_data[0] = I_wp_data[0];
        } else {
          c_loop_ub = I_wp->size[0];
          idx = I_wp->size[1];
          if (c_loop_ub <= idx) {
            idx = c_loop_ub;
          }
          if (I_wp->size[1] <= 0) {
            idx = 0;
          }
          i1 = idx_wp->size[0];
          idx_wp->size[0] = idx;
          emxEnsureCapacity_real_T(idx_wp, i1);
          idx_wp_data = idx_wp->data;
          i1 = idx - 1;
          for (loop_ub = 0; loop_ub <= i1; loop_ub++) {
            idx_wp_data[loop_ub] = I_wp_data[loop_ub + I_wp->size[0] * loop_ub];
          }
        }
        c_loop_ub = idx_wp->size[0];
        i1 = y->size[0];
        y->size[0] = idx_wp->size[0];
        emxEnsureCapacity_real_T(y, i1);
        y_data = y->data;
        for (loop_ub = 0; loop_ub < c_loop_ub; loop_ub++) {
          y_data[loop_ub] = fabs(idx_wp_data[loop_ub]);
        }
        if (y->size[0] <= 2) {
          if (y->size[0] == 1) {
            sp_val_idx = y_data[0];
          } else {
            sp_val_idx = y_data[y->size[0] - 1];
            if ((!(y_data[0] < sp_val_idx)) &&
                ((!rtIsNaN(y_data[0])) || rtIsNaN(sp_val_idx))) {
              sp_val_idx = y_data[0];
            }
          }
        } else {
          if (!rtIsNaN(y_data[0])) {
            idx = 1;
          } else {
            bool exitg1;
            idx = 0;
            loop_ub = 2;
            exitg1 = false;
            while ((!exitg1) && (loop_ub <= c_loop_ub)) {
              if (!rtIsNaN(y_data[loop_ub - 1])) {
                idx = loop_ub;
                exitg1 = true;
              } else {
                loop_ub++;
              }
            }
          }
          if (idx == 0) {
            sp_val_idx = y_data[0];
          } else {
            sp_val_idx = y_data[idx - 1];
            i1 = idx + 1;
            for (loop_ub = i1; loop_ub <= c_loop_ub; loop_ub++) {
              d = y_data[loop_ub - 1];
              if (sp_val_idx < d) {
                sp_val_idx = d;
              }
            }
          }
        }
        sp_val_idx = 1.0E-10 * sp_val_idx + 1.0E-12;
        idx = I_wp->size[0];
        i1 = I_cond->size[0] * I_cond->size[1];
        I_cond->size[0] = I_wp->size[0];
        I_cond->size[1] = I_wp->size[0];
        emxEnsureCapacity_real_T(I_cond, i1);
        y_data = I_cond->data;
        loop_ub = I_wp->size[0] * I_wp->size[0];
        for (i1 = 0; i1 < loop_ub; i1++) {
          y_data[i1] = 0.0;
        }
        if (I_wp->size[0] > 0) {
          for (loop_ub = 0; loop_ub < idx; loop_ub++) {
            y_data[loop_ub + I_cond->size[0] * loop_ub] = 1.0;
          }
        }
        if ((I_wp->size[0] == I_cond->size[0]) &&
            (I_wp->size[1] == I_cond->size[1])) {
          i1 = Xwp->size[0] * Xwp->size[1];
          Xwp->size[0] = I_wp->size[0];
          Xwp->size[1] = I_wp->size[1];
          emxEnsureCapacity_real_T(Xwp, i1);
          idx_wp_data = Xwp->data;
          loop_ub = I_wp->size[0] * I_wp->size[1];
          for (i1 = 0; i1 < loop_ub; i1++) {
            idx_wp_data[i1] = I_wp_data[i1] + sp_val_idx * y_data[i1];
          }
          mldivide(Xwp, H_wp, r);
        } else {
          binary_expand_op_5(r, I_wp, sp_val_idx, I_cond, H_wp);
        }
        mtimes(H_wp, r, I_wp);
        I_wp_data = I_wp->data;
        /*  Symmetrize to remove tiny floating-point asymmetries. */
        if (I_wp->size[0] == I_wp->size[1]) {
          loop_ub = I_wp->size[0];
          i1 = Xwp->size[0] * Xwp->size[1];
          Xwp->size[0] = I_wp->size[0];
          c_loop_ub = I_wp->size[1];
          Xwp->size[1] = I_wp->size[1];
          emxEnsureCapacity_real_T(Xwp, i1);
          idx_wp_data = Xwp->data;
          for (i1 = 0; i1 < c_loop_ub; i1++) {
            for (i2 = 0; i2 < loop_ub; i2++) {
              idx_wp_data[i2 + Xwp->size[0] * i1] =
                  0.5 * (I_wp_data[i2 + I_wp->size[0] * i1] +
                         I_wp_data[i1 + I_wp->size[0] * i2]);
            }
          }
          i1 = I_wp->size[0] * I_wp->size[1];
          I_wp->size[0] = loop_ub;
          I_wp->size[1] = c_loop_ub;
          emxEnsureCapacity_real_T(I_wp, i1);
          I_wp_data = I_wp->data;
          loop_ub = Xwp->size[0] * Xwp->size[1];
          for (i1 = 0; i1 < loop_ub; i1++) {
            I_wp_data[i1] = idx_wp_data[i1];
          }
        } else {
          binary_expand_op_4(I_wp);
          I_wp_data = I_wp->data;
        }
      } else if (is_copula_pcl) {
        b_loop_ub_tmp = thetaGrid->size[1];
        i1 = idx_wp->size[0];
        idx_wp->size[0] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(idx_wp, i1);
        idx_wp_data = idx_wp->data;
        for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
          idx_wp_data[i1] = thetaGrid_data[t + thetaGrid->size[0] * i1];
        }
        computeFisher_Copula(idx_wp, Xwp, K_int, q, copulaType,
                             lambda_fixed_data, lambda_fixed_size, I_wp);
        I_wp_data = I_wp->data;
      } else {
        b_loop_ub_tmp = thetaGrid->size[1];
        i1 = idx_sp_full->size[0];
        idx_sp_full->size[0] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(idx_sp_full, i1);
        idx_sp_full_data = idx_sp_full->data;
        for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
          idx_sp_full_data[i1] = thetaGrid_data[t + thetaGrid->size[0] * i1];
        }
        i1 = I_wp->size[0] * I_wp->size[1];
        I_wp->size[0] = thetaGrid->size[1];
        I_wp->size[1] = thetaGrid->size[1];
        emxEnsureCapacity_real_T(I_wp, i1);
        I_wp_data = I_wp->data;
        c_loop_ub = idx_sp_full->size[0] * idx_sp_full->size[0];
        for (i1 = 0; i1 < c_loop_ub; i1++) {
          I_wp_data[i1] = 0.0;
        }
        sub = Xwp->size[0] - 1;
        sp_val_idx = rt_powd_snf(K_int, Xwp->size[0]);
        c_loop_ub = (int)rt_powd_snf(K_int, Xwp->size[0]);
        i1 = allY->size[0] * allY->size[1];
        allY->size[0] = (int)sp_val_idx;
        allY->size[1] = (int)d;
        emxEnsureCapacity_real_T(allY, i1);
        alpha_data = allY->data;
        for (b_i = 0; b_i < c_loop_ub; b_i++) {
          double temp;
          temp = ((double)b_i + 1.0) - 1.0;
          for (j = 0; j <= sub; j++) {
            alpha_data[b_i + allY->size[0] * (sub - j)] = b_mod(temp, K_int);
            temp /= (double)K_int;
            temp = floor(temp);
          }
        }
        i1 = PrY->size[0];
        PrY->size[0] = (int)sp_val_idx;
        emxEnsureCapacity_real_T(PrY, i1);
        PrY_data = PrY->data;
        i1 = Scores->size[0] * Scores->size[1];
        Scores->size[0] = thetaGrid->size[1];
        Scores->size[1] = (int)sp_val_idx;
        emxEnsureCapacity_real_T(Scores, i1);
        Scores_data = Scores->data;
        for (b_i = 0; b_i < c_loop_ub; b_i++) {
          i1 = y->size[0];
          y->size[0] = (int)d;
          emxEnsureCapacity_real_T(y, i1);
          y_data = y->data;
          for (i1 = 0; i1 < loop_ub; i1++) {
            y_data[i1] = alpha_data[b_i + allY->size[0] * i1];
          }
          PrY_data[b_i] =
              b_evaluate_copula_pmf(y, Xwp, idx_sp_full, K_int, q, copulaType,
                                    lambda_fixed_data, lambda_fixed_size);
          for (idx = 0; idx < b_loop_ub_tmp; idx++) {
            i1 = idx_wp->size[0];
            idx_wp->size[0] = b_loop_ub_tmp;
            emxEnsureCapacity_real_T(idx_wp, i1);
            idx_wp_data = idx_wp->data;
            for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
              idx_wp_data[i1] = idx_sp_full_data[i1];
            }
            idx_wp_data[idx] = idx_sp_full_data[idx] + 0.0001;
            sp_val_idx =
                b_evaluate_copula_pmf(y, Xwp, idx_wp, K_int, q, copulaType,
                                      lambda_fixed_data, lambda_fixed_size);
            i1 = idx_wp->size[0];
            idx_wp->size[0] = b_loop_ub_tmp;
            emxEnsureCapacity_real_T(idx_wp, i1);
            idx_wp_data = idx_wp->data;
            for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
              idx_wp_data[i1] = idx_sp_full_data[i1];
            }
            idx_wp_data[idx] = idx_sp_full_data[idx] - 0.0001;
            Scores_data[idx + Scores->size[0] * b_i] =
                (log(fmax(sp_val_idx, 1.0E-16)) -
                 log(fmax(b_evaluate_copula_pmf(y, Xwp, idx_wp, K_int, q,
                                                copulaType, lambda_fixed_data,
                                                lambda_fixed_size),
                          1.0E-16))) /
                0.0002;
          }
        }
        for (i1 = 0; i1 < c_loop_ub; i1++) {
          sp_val_idx = PrY_data[i1];
          PrY_data[i1] = fmax(sp_val_idx, 2.2204460492503131E-16);
        }
        sp_val_idx = blockedSummation(PrY, PrY->size[0]);
        for (i1 = 0; i1 < c_loop_ub; i1++) {
          PrY_data[i1] /= sp_val_idx;
        }
        for (b_i = 0; b_i < c_loop_ub; b_i++) {
          b_loop_ub_tmp = Scores->size[0];
          if ((I_wp->size[0] == Scores->size[0]) &&
              (Scores->size[0] == I_wp->size[1])) {
            i1 = Xwp->size[0] * Xwp->size[1];
            Xwp->size[0] = Scores->size[0];
            Xwp->size[1] = Scores->size[0];
            emxEnsureCapacity_real_T(Xwp, i1);
            idx_wp_data = Xwp->data;
            for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
              for (i2 = 0; i2 < b_loop_ub_tmp; i2++) {
                idx_wp_data[i2 + Xwp->size[0] * i1] =
                    Scores_data[i2 + Scores->size[0] * b_i] *
                    Scores_data[i1 + Scores->size[0] * b_i];
              }
            }
            loop_ub = I_wp->size[0] * I_wp->size[1];
            for (i1 = 0; i1 < loop_ub; i1++) {
              I_wp_data[i1] += PrY_data[b_i] * idx_wp_data[i1];
            }
          } else {
            binary_expand_op_6(I_wp, PrY, b_i, Scores);
            I_wp_data = I_wp->data;
          }
        }
      }
      if ((I_total->size[0] == I_wp->size[0]) &&
          (I_total->size[1] == I_wp->size[1])) {
        loop_ub = I_total->size[0] * I_total->size[1];
        for (i1 = 0; i1 < loop_ub; i1++) {
          I_total_data[i1] += I_wp_data[i1];
        }
      } else {
        plus(I_total, I_wp);
        I_total_data = I_total->data;
      }
      curr_start += n_data[wp];
    }
    /*  =================================================================== */
    /*  UTILITY HELPERS */
    /*  =================================================================== */
    i1 = I_total->size[0];
    for (b_i = 0; b_i < i1; b_i++) {
      I_total_data[b_i + I_total->size[0] * b_i] += 0.0001;
    }
    i1 = I_cond->size[0] * I_cond->size[1];
    I_cond->size[0] = I_total->size[0];
    I_cond->size[1] = I_total->size[1];
    emxEnsureCapacity_real_T(I_cond, i1);
    y_data = I_cond->data;
    loop_ub = I_total->size[0] * I_total->size[1];
    for (i1 = 0; i1 < loop_ub; i1++) {
      y_data[i1] = I_total_data[i1];
    }
    c_loop_ub = I_total->size[0];
    sub = I_total->size[1];
    if (c_loop_ub <= sub) {
      sub = c_loop_ub;
    }
    idx = 0;
    loop_ub = 0;
    if (sub != 0) {
      loop_ub = xpotrf(sub, I_cond, I_total->size[0]);
      y_data = I_cond->data;
      if (loop_ub == 0) {
        idx = sub;
      } else {
        idx = loop_ub - 1;
      }
      for (j = 0; j <= idx - 2; j++) {
        i1 = j + 2;
        for (b_i = i1; b_i <= idx; b_i++) {
          y_data[(b_i + I_cond->size[0] * j) - 1] = 0.0;
        }
      }
    }
    if (idx < 1) {
      c_loop_ub = 0;
    } else {
      c_loop_ub = idx;
    }
    if (loop_ub == 0) {
      if (c_loop_ub == 1) {
        i1 = idx_wp->size[0];
        idx_wp->size[0] = 1;
        emxEnsureCapacity_real_T(idx_wp, i1);
        idx_wp_data = idx_wp->data;
        idx_wp_data[0] = y_data[0];
      } else {
        if (c_loop_ub > 0) {
          idx = c_loop_ub;
        } else {
          idx = 0;
        }
        i1 = idx_wp->size[0];
        idx_wp->size[0] = idx;
        emxEnsureCapacity_real_T(idx_wp, i1);
        idx_wp_data = idx_wp->data;
        i1 = idx - 1;
        for (loop_ub = 0; loop_ub <= i1; loop_ub++) {
          idx_wp_data[loop_ub] = y_data[loop_ub + I_cond->size[0] * loop_ub];
        }
      }
      b_loop_ub_tmp = idx_wp->size[0];
      for (i1 = 0; i1 < b_loop_ub_tmp; i1++) {
        sp_val_idx = idx_wp_data[i1];
        idx_wp_data[i1] = fmax(sp_val_idx, 1.0E-100);
      }
      for (loop_ub = 0; loop_ub < b_loop_ub_tmp; loop_ub++) {
        idx_wp_data[loop_ub] = log(idx_wp_data[loop_ub]);
      }
      logDets_data[t] = 2.0 * blockedSummation(idx_wp, idx_wp->size[0]);
    } else {
      logDets_data[t] = -1.0E+20;
    }
  }
  emxFree_real_T(&b_Xwp);
  emxFree_real_T(&r);
  emxFree_real_T(&y);
  emxFree_real_T(&Scores);
  emxFree_real_T(&PrY);
  emxFree_real_T(&allY);
  emxFree_real_T(&I_cond);
  emxFree_real_T(&alpha);
  emxFree_real_T(&H_wp);
  emxFree_real_T(&I_wp);
  emxFree_real_T(&Xwp);
  emxFree_real_T(&I_total);
  emxFree_real_T(&X_all);
  emxFree_real_T(&idx_sp_full);
  emxFree_real_T(&idx_wp);
  if (b_strcmp(crit_mode_data, crit_mode_size)) {
    sp_val_idx = 0.0;
    loop_ub = weights->size[0];
    for (i = 0; i < loop_ub; i++) {
      sp_val_idx += weights_data[i] * logDets_data[i];
    }
  } else {
    sp_val_idx = minimum(logDets);
  }
  emxFree_real_T(&logDets);
  return sp_val_idx - penalty;
}

/*
 * Arguments    : const emxArray_real_T *theta
 *                const emxArray_real_T *Xwp
 *                int K
 *                double q
 *                double type
 *                const double lambda_fixed_data[]
 *                const int lambda_fixed_size[2]
 *                emxArray_real_T *b_I
 * Return Type  : void
 */
static void computeFisher_Copula(const emxArray_real_T *theta,
                                 const emxArray_real_T *Xwp, int K, double q,
                                 double type, const double lambda_fixed_data[],
                                 const int lambda_fixed_size[2],
                                 emxArray_real_T *b_I)
{
  emxArray_real_T *U;
  emxArray_real_T *X_pair;
  emxArray_real_T *b_U;
  emxArray_real_T *x1;
  emxArray_uint32_T *Y_pairs;
  const double *Xwp_data;
  const double *theta_data;
  double n_outcomes_tmp;
  double *I_data;
  double *U_data;
  double *X_pair_data;
  int b_i;
  int b_y1;
  int i;
  int i1;
  int i2;
  int j;
  int k;
  int loop_ub;
  int loop_ub_tmp;
  unsigned int row;
  int y2;
  unsigned int *Y_pairs_data;
  Xwp_data = Xwp->data;
  theta_data = theta->data;
  loop_ub = theta->size[0];
  i = b_I->size[0] * b_I->size[1];
  b_I->size[0] = theta->size[0];
  b_I->size[1] = theta->size[0];
  emxEnsureCapacity_real_T(b_I, i);
  I_data = b_I->data;
  loop_ub_tmp = theta->size[0] * theta->size[0];
  for (i = 0; i < loop_ub_tmp; i++) {
    I_data[i] = 0.0;
  }
  n_outcomes_tmp = (double)K * (double)K;
  emxInit_uint32_T(&Y_pairs, 2);
  i = (int)n_outcomes_tmp;
  i1 = Y_pairs->size[0] * Y_pairs->size[1];
  Y_pairs->size[0] = (int)n_outcomes_tmp;
  Y_pairs->size[1] = 2;
  emxEnsureCapacity_uint32_T(Y_pairs, i1);
  Y_pairs_data = Y_pairs->data;
  loop_ub_tmp = (int)n_outcomes_tmp << 1;
  for (i1 = 0; i1 < loop_ub_tmp; i1++) {
    Y_pairs_data[i1] = 0U;
  }
  row = 1U;
  for (b_y1 = 0; b_y1 < K; b_y1++) {
    for (y2 = 0; y2 < K; y2++) {
      loop_ub_tmp = (int)(row + (unsigned int)y2) - 1;
      Y_pairs_data[loop_ub_tmp] = (unsigned int)b_y1;
      Y_pairs_data[loop_ub_tmp + Y_pairs->size[0]] = (unsigned int)y2;
    }
    if (K - 1 >= 0) {
      row += (unsigned int)K;
    }
  }
  i1 = Xwp->size[0];
  emxInit_real_T(&X_pair, 2);
  emxInit_real_T(&U, 1);
  emxInit_real_T(&x1, 1);
  emxInit_real_T(&b_U, 2);
  for (b_i = 0; b_i <= i1 - 2; b_i++) {
    b_y1 = Xwp->size[0] - b_i;
    for (j = 0; j <= b_y1 - 2; j++) {
      y2 = b_i + j;
      i2 = X_pair->size[0] * X_pair->size[1];
      X_pair->size[0] = 2;
      loop_ub_tmp = Xwp->size[1];
      X_pair->size[1] = Xwp->size[1];
      emxEnsureCapacity_real_T(X_pair, i2);
      X_pair_data = X_pair->data;
      for (i2 = 0; i2 < loop_ub_tmp; i2++) {
        X_pair_data[2 * i2] = Xwp_data[b_i + Xwp->size[0] * i2];
        X_pair_data[2 * i2 + 1] = Xwp_data[(y2 + Xwp->size[0] * i2) + 1];
      }
      for (k = 0; k < i; k++) {
        double y_pair[2];
        y_pair[0] = Y_pairs_data[k];
        y_pair[1] = Y_pairs_data[k + Y_pairs->size[0]];
        n_outcomes_tmp =
            evaluate_copula_pmf(y_pair, X_pair, theta, K, q, type,
                                lambda_fixed_data, lambda_fixed_size);
        i2 = U->size[0];
        U->size[0] = loop_ub;
        emxEnsureCapacity_real_T(U, i2);
        U_data = U->data;
        for (y2 = 0; y2 < loop_ub; y2++) {
          double x;
          i2 = x1->size[0];
          x1->size[0] = loop_ub;
          emxEnsureCapacity_real_T(x1, i2);
          X_pair_data = x1->data;
          for (i2 = 0; i2 < loop_ub; i2++) {
            X_pair_data[i2] = theta_data[i2];
          }
          X_pair_data[y2] = theta_data[y2] + 0.0001;
          x = evaluate_copula_pmf(y_pair, X_pair, x1, K, q, type,
                                  lambda_fixed_data, lambda_fixed_size);
          i2 = x1->size[0];
          x1->size[0] = loop_ub;
          emxEnsureCapacity_real_T(x1, i2);
          X_pair_data = x1->data;
          for (i2 = 0; i2 < loop_ub; i2++) {
            X_pair_data[i2] = theta_data[i2];
          }
          X_pair_data[y2] = theta_data[y2] - 0.0001;
          U_data[y2] = (log(fmax(x, 1.0E-16)) -
                        log(fmax(evaluate_copula_pmf(y_pair, X_pair, x1, K, q,
                                                     type, lambda_fixed_data,
                                                     lambda_fixed_size),
                                 1.0E-16))) /
                       0.0002;
        }
        if ((b_I->size[0] == U->size[0]) && (U->size[0] == b_I->size[1])) {
          i2 = b_U->size[0] * b_U->size[1];
          b_U->size[0] = loop_ub;
          b_U->size[1] = loop_ub;
          emxEnsureCapacity_real_T(b_U, i2);
          X_pair_data = b_U->data;
          for (i2 = 0; i2 < loop_ub; i2++) {
            for (loop_ub_tmp = 0; loop_ub_tmp < loop_ub; loop_ub_tmp++) {
              X_pair_data[loop_ub_tmp + b_U->size[0] * i2] =
                  U_data[loop_ub_tmp] * U_data[i2];
            }
          }
          loop_ub_tmp = b_I->size[0] * b_I->size[1];
          for (i2 = 0; i2 < loop_ub_tmp; i2++) {
            I_data[i2] += n_outcomes_tmp * X_pair_data[i2];
          }
        } else {
          binary_expand_op_7(b_I, n_outcomes_tmp, U);
          I_data = b_I->data;
        }
      }
    }
  }
  /* v0.2.2: weight the pairwise block accumulation by 1/(n-1), the standard
     composite-likelihood weighting for unequal cluster sizes (Varin, Reid,
     and Firth, 2011, Statistica Sinica 21:5-42). Each observation enters
     n-1 sub-plot pairs, so this restores a common per-observation counting
     rate across blocks of unequal size. For balanced designs the factor is
     a design-independent constant and leaves design selection unchanged. */
  if (Xwp->size[0] > 1) {
    double w_pcl = 1.0 / ((double)Xwp->size[0] - 1.0);
    int w_n = b_I->size[0] * b_I->size[1];
    int w_i;
    for (w_i = 0; w_i < w_n; w_i++) {
      b_I->data[w_i] *= w_pcl;
    }
  }
  emxFree_real_T(&b_U);
  emxFree_real_T(&x1);
  emxFree_real_T(&U);
  emxFree_real_T(&X_pair);
  emxFree_uint32_T(&Y_pairs);
}

/*
 * Arguments    : const emxArray_real_T *theta
 *                const emxArray_real_T *Xwp
 *                int K
 *                double q
 *                const double sigma2_fixed_data[]
 *                const int sigma2_fixed_size[2]
 *                emxArray_real_T *b_I
 * Return Type  : void
 */
static void computeFisher_GLMM_Exact(const emxArray_real_T *theta,
                                     const emxArray_real_T *Xwp, int K,
                                     double q, const double sigma2_fixed_data[],
                                     const int sigma2_fixed_size[2],
                                     emxArray_real_T *b_I)
{
  emxArray_real_T *PrY;
  emxArray_real_T *Scores;
  emxArray_real_T *allY;
  emxArray_real_T *alpha;
  emxArray_real_T *b_th1;
  emxArray_real_T *beta;
  emxArray_real_T *c_th1;
  emxArray_real_T *th1;
  emxArray_real_T *y;
  const double *theta_data;
  double num;
  double s2;
  double s21;
  double *PrY_data;
  double *Scores_data;
  double *allY_data;
  double *alpha_data;
  double *th1_data;
  int b_i;
  int b_loop_ub;
  int c_loop_ub;
  int d_loop_ub;
  int e_loop_ub;
  int f_loop_ub;
  int g_loop_ub;
  int i;
  int i1;
  int i2;
  int i3;
  int i4;
  int i5;
  int i6;
  int j;
  int loop_ub;
  int loop_ub_tmp;
  int n;
  bool b;
  theta_data = theta->data;
  /*  =================================================================== */
  /*  2. GLMM EXACT FISHER */
  /*  =================================================================== */
  if ((double)K - 1.0 < 1.0) {
    loop_ub_tmp = 0;
  } else {
    loop_ub_tmp = K - 1;
  }
  emxInit_real_T(&alpha, 2);
  i = alpha->size[0] * alpha->size[1];
  alpha->size[0] = 1;
  alpha->size[1] = loop_ub_tmp;
  emxEnsureCapacity_real_T(alpha, i);
  alpha_data = alpha->data;
  for (i = 0; i < loop_ub_tmp; i++) {
    alpha_data[i] = theta_data[i];
  }
  s21 = ((double)K - 1.0) + q;
  if (((double)K - 1.0) + 1.0 > s21) {
    i = 0;
    i1 = 0;
  } else {
    i = (int)(((double)K - 1.0) + 1.0) - 1;
    i1 = (int)s21;
  }
  emxInit_real_T(&beta, 1);
  loop_ub = i1 - i;
  i1 = beta->size[0];
  beta->size[0] = loop_ub;
  emxEnsureCapacity_real_T(beta, i1);
  alpha_data = beta->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    alpha_data[i1] = theta_data[i + i1];
  }
  b = ((sigma2_fixed_size[0] == 0) || (sigma2_fixed_size[1] == 0));
  if (b) {
    s2 = theta_data[theta->size[0] - 1];
  } else {
    s2 = sigma2_fixed_data[0];
  }
  n = Xwp->size[0] - 1;
  num = rt_powd_snf(K, Xwp->size[0]);
  loop_ub = (int)rt_powd_snf(K, Xwp->size[0]);
  emxInit_real_T(&allY, 2);
  i = allY->size[0] * allY->size[1];
  allY->size[0] = (int)num;
  i1 = Xwp->size[0];
  allY->size[1] = Xwp->size[0];
  emxEnsureCapacity_real_T(allY, i);
  allY_data = allY->data;
  for (b_i = 0; b_i < loop_ub; b_i++) {
    double temp;
    temp = ((double)b_i + 1.0) - 1.0;
    for (j = 0; j <= n; j++) {
      allY_data[b_i + allY->size[0] * (n - j)] = b_mod(temp, K);
      temp /= (double)K;
      temp = floor(temp);
    }
  }
  emxInit_real_T(&PrY, 1);
  i = PrY->size[0];
  PrY->size[0] = (int)num;
  emxEnsureCapacity_real_T(PrY, i);
  PrY_data = PrY->data;
  i = theta->size[0];
  emxInit_real_T(&Scores, 2);
  n = Scores->size[0] * Scores->size[1];
  Scores->size[0] = theta->size[0];
  Scores->size[1] = (int)num;
  emxEnsureCapacity_real_T(Scores, n);
  Scores_data = Scores->data;
  if (allY->size[0] - 1 >= 0) {
    b_loop_ub = Xwp->size[0];
    i2 = theta->size[0];
    if (theta->size[0] - 1 >= 0) {
      c_loop_ub = theta->size[0];
      d_loop_ub = loop_ub_tmp;
      if (((double)K - 1.0) + 1.0 > s21) {
        i3 = 0;
        i4 = 0;
        i5 = 0;
        i6 = 0;
      } else {
        i3 = (int)(((double)K - 1.0) + 1.0) - 1;
        i4 = (int)s21;
        i5 = i3;
        i6 = (int)s21;
      }
      e_loop_ub = i4 - i3;
      f_loop_ub = theta->size[0];
      g_loop_ub = i6 - i5;
    }
  }
  emxInit_real_T(&y, 2);
  emxInit_real_T(&th1, 1);
  emxInit_real_T(&b_th1, 1);
  emxInit_real_T(&c_th1, 2);
  for (j = 0; j < loop_ub; j++) {
    n = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = i1;
    emxEnsureCapacity_real_T(y, n);
    alpha_data = y->data;
    for (n = 0; n < b_loop_ub; n++) {
      alpha_data[n] = allY_data[j + allY->size[0] * n];
    }
    PrY_data[j] = GLMM_ordinal_marginalPMF_cg(y, Xwp, beta, alpha, s2, K);
    for (b_i = 0; b_i < i2; b_i++) {
      n = th1->size[0];
      th1->size[0] = i;
      emxEnsureCapacity_real_T(th1, n);
      alpha_data = th1->data;
      for (n = 0; n < c_loop_ub; n++) {
        alpha_data[n] = theta_data[n];
      }
      alpha_data[b_i] = theta_data[b_i] + 0.0001;
      if (b) {
        s21 = alpha_data[th1->size[0] - 1];
      } else {
        s21 = sigma2_fixed_data[0];
      }
      n = b_th1->size[0];
      b_th1->size[0] = i4 - i3;
      emxEnsureCapacity_real_T(b_th1, n);
      th1_data = b_th1->data;
      for (n = 0; n < e_loop_ub; n++) {
        th1_data[n] = alpha_data[i3 + n];
      }
      n = c_th1->size[0] * c_th1->size[1];
      c_th1->size[0] = 1;
      c_th1->size[1] = d_loop_ub;
      emxEnsureCapacity_real_T(c_th1, n);
      th1_data = c_th1->data;
      for (n = 0; n < d_loop_ub; n++) {
        th1_data[n] = alpha_data[n];
      }
      s21 = GLMM_ordinal_marginalPMF_cg(y, Xwp, b_th1, c_th1, s21, K);
      n = th1->size[0];
      th1->size[0] = i;
      emxEnsureCapacity_real_T(th1, n);
      alpha_data = th1->data;
      for (n = 0; n < f_loop_ub; n++) {
        alpha_data[n] = theta_data[n];
      }
      alpha_data[b_i] = theta_data[b_i] - 0.0001;
      if (b) {
        num = alpha_data[th1->size[0] - 1];
      } else {
        num = sigma2_fixed_data[0];
      }
      n = b_th1->size[0];
      b_th1->size[0] = i6 - i5;
      emxEnsureCapacity_real_T(b_th1, n);
      th1_data = b_th1->data;
      for (n = 0; n < g_loop_ub; n++) {
        th1_data[n] = alpha_data[i5 + n];
      }
      n = c_th1->size[0] * c_th1->size[1];
      c_th1->size[0] = 1;
      c_th1->size[1] = d_loop_ub;
      emxEnsureCapacity_real_T(c_th1, n);
      th1_data = c_th1->data;
      for (n = 0; n < d_loop_ub; n++) {
        th1_data[n] = alpha_data[n];
      }
      Scores_data[b_i + Scores->size[0] * j] =
          (log(fmax(s21, 1.0E-16)) -
           log(fmax(GLMM_ordinal_marginalPMF_cg(y, Xwp, b_th1, c_th1, num, K),
                    1.0E-16))) /
          0.0002;
    }
  }
  emxFree_real_T(&c_th1);
  emxFree_real_T(&b_th1);
  emxFree_real_T(&th1);
  emxFree_real_T(&y);
  emxFree_real_T(&beta);
  emxFree_real_T(&alpha);
  for (i = 0; i < loop_ub; i++) {
    s21 = PrY_data[i];
    PrY_data[i] = fmax(s21, 2.2204460492503131E-16);
  }
  s21 = blockedSummation(PrY, PrY->size[0]);
  for (i = 0; i < loop_ub; i++) {
    PrY_data[i] /= s21;
  }
  i = b_I->size[0] * b_I->size[1];
  b_I->size[0] = theta->size[0];
  b_I->size[1] = theta->size[0];
  emxEnsureCapacity_real_T(b_I, i);
  alpha_data = b_I->data;
  b_loop_ub = theta->size[0] * theta->size[0];
  for (i = 0; i < b_loop_ub; i++) {
    alpha_data[i] = 0.0;
  }
  for (j = 0; j < loop_ub; j++) {
    loop_ub_tmp = Scores->size[0];
    if ((b_I->size[0] == Scores->size[0]) &&
        (Scores->size[0] == b_I->size[1])) {
      i = allY->size[0] * allY->size[1];
      allY->size[0] = Scores->size[0];
      allY->size[1] = Scores->size[0];
      emxEnsureCapacity_real_T(allY, i);
      allY_data = allY->data;
      for (i = 0; i < loop_ub_tmp; i++) {
        for (i1 = 0; i1 < loop_ub_tmp; i1++) {
          allY_data[i1 + allY->size[0] * i] =
              Scores_data[i1 + Scores->size[0] * j] *
              Scores_data[i + Scores->size[0] * j];
        }
      }
      b_loop_ub = b_I->size[0] * b_I->size[1];
      for (i = 0; i < b_loop_ub; i++) {
        alpha_data[i] += PrY_data[j] * allY_data[i];
      }
    } else {
      binary_expand_op_6(b_I, PrY, j, Scores);
      alpha_data = b_I->data;
    }
  }
  emxFree_real_T(&Scores);
  emxFree_real_T(&PrY);
  emxFree_real_T(&allY);
}

/*
 * Arguments    : double m
 *                const emxArray_real_T *n
 *                double N
 *                const emxArray_real_T *weights
 *                double p
 *                const char crit_mode_data[]
 *                const int crit_mode_size[2]
 *                double nvars
 *                const emxArray_real_T *lb
 *                const emxArray_real_T *ub
 *                const emxArray_real_T *wp_combos
 *                const emxArray_real_T *sp_combos
 *                const emxArray_real_T *thetaGrid
 *                int K_int
 *                double q
 *                const double sigma2_fixed_data[]
 *                const int sigma2_fixed_size[2]
 *                const double lambda_fixed_data[]
 *                const int lambda_fixed_size[2]
 *                double copulaType
 *                bool is_glmm_approx
 *                bool is_glmm_exact
 *                bool is_copula_pcl
 *                bool is_copula_pcl_godambe
 *                bool is_indep_glm
 *                const emxArray_cell_wrap_0 *modelTerms
 *                double best_idx_data[]
 *                double *best_crit
 * Return Type  : int
 */
static int coordinate_exchange_otf(
    double m, const emxArray_real_T *n, double N,
    const emxArray_real_T *weights, double p, const char crit_mode_data[],
    const int crit_mode_size[2], double nvars, const emxArray_real_T *lb,
    const emxArray_real_T *ub, const emxArray_real_T *wp_combos,
    const emxArray_real_T *sp_combos, const emxArray_real_T *thetaGrid,
    int K_int, double q, const double sigma2_fixed_data[],
    const int sigma2_fixed_size[2], const double lambda_fixed_data[],
    const int lambda_fixed_size[2], double copulaType, bool is_glmm_approx,
    bool is_glmm_exact, bool is_copula_pcl, bool is_copula_pcl_godambe,
    bool is_indep_glm, const emxArray_cell_wrap_0 *modelTerms,
    double best_idx_data[], double *best_crit)
{
  emxArray_real_T *best_idxs;
  emxArray_real_T *r;
  emxArray_real_T *start_designs;
  double idx_data[5000];
  double best_crits[PCL_MAX_STARTS];
  const double *lb_data;
  const double *ub_data;
  double best_v_crit;
  double best_val;
  double current_crit;
  double d1;
  double new_crit;
  double *best_idxs_data;
  double *r1;
  double *start_designs_data;
  int b_start_idx;
  int best_idx_size;
  int i;
  int i1;
  int i2;
  int iter;
  int k;
  int nx;
  int start_idx;
  int v;
  int val;
  bool improved;
  ub_data = ub->data;
  lb_data = lb->data;
  emxInit_real_T(&best_idxs, 2);
  best_idx_size = (int)nvars;
  i = best_idxs->size[0] * best_idxs->size[1];
  best_idxs->size[0] = (int)nvars;
  best_idxs->size[1] = pcl_num_starts;
  emxEnsureCapacity_real_T(best_idxs, i);
  best_idxs_data = best_idxs->data;
  nx = (int)nvars * pcl_num_starts;
  for (i = 0; i < nx; i++) {
    best_idxs_data[i] = 0.0;
  }
  emxInit_real_T(&start_designs, 2);
  i = start_designs->size[0] * start_designs->size[1];
  start_designs->size[0] = (int)nvars;
  start_designs->size[1] = pcl_num_starts;
  emxEnsureCapacity_real_T(start_designs, i);
  start_designs_data = start_designs->data;
  for (i = 0; i < nx; i++) {
    start_designs_data[i] = 0.0;
  }
  emxInit_real_T(&r, 1);
  for (start_idx = 0; start_idx < pcl_num_starts; start_idx++) {
    b_rand(nvars, r);
    r1 = r->data;
    if (ub->size[0] == 1) {
      i = lb->size[0];
    } else {
      i = ub->size[0];
    }
    if ((ub->size[0] == lb->size[0]) && (r->size[0] == i)) {
      nx = r->size[0];
      for (i = 0; i < nx; i++) {
        r1[i] *= ub_data[i] - lb_data[i];
      }
    } else {
      binary_expand_op_2(r, ub, lb);
      r1 = r->data;
    }
    nx = r->size[0];
    for (k = 0; k < nx; k++) {
      r1[k] = rt_roundd_snf(r1[k]);
    }
    if (lb->size[0] == r->size[0]) {
      nx = start_designs->size[0];
      for (i = 0; i < nx; i++) {
        start_designs_data[i + start_designs->size[0] * start_idx] =
            lb_data[i] + r1[i];
      }
    } else {
      binary_expand_op_1(start_designs, start_idx, lb, r);
      start_designs_data = start_designs->data;
    }
  }
  emxFree_real_T(&r);
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
        new_crit, best_v_crit, best_val, iter, improved, current_crit,         \
            idx_data, i1, v, d1, i2, val)

  for (b_start_idx = 0; b_start_idx < pcl_num_starts; b_start_idx++) {
    iter = start_designs->size[0];
    for (i1 = 0; i1 < iter; i1++) {
      idx_data[i1] =
          start_designs_data[i1 + start_designs->size[0] * b_start_idx];
    }
    current_crit = computeCriterion_otf(
        idx_data, m, n, N, weights, p, crit_mode_data, crit_mode_size,
        wp_combos, sp_combos, thetaGrid, K_int, q, sigma2_fixed_data,
        sigma2_fixed_size, lambda_fixed_data, lambda_fixed_size, copulaType,
        is_glmm_approx, is_glmm_exact, is_copula_pcl, is_copula_pcl_godambe,
        is_indep_glm, modelTerms);
    improved = true;
    iter = 0;
    while (improved && (iter < 120)) {
      improved = false;
      iter++;
      i1 = (int)nvars;
      for (v = 0; v < i1; v++) {
        d1 = idx_data[v];
        best_val = d1;
        best_v_crit = current_crit;
        i2 = (int)ub_data[v];
        for (val = 0; val < i2; val++) {
          if (!((double)val + 1.0 == d1)) {
            idx_data[v] = (double)val + 1.0;
            new_crit = computeCriterion_otf(
                idx_data, m, n, N, weights, p, crit_mode_data, crit_mode_size,
                wp_combos, sp_combos, thetaGrid, K_int, q, sigma2_fixed_data,
                sigma2_fixed_size, lambda_fixed_data, lambda_fixed_size,
                copulaType, is_glmm_approx, is_glmm_exact, is_copula_pcl,
                is_copula_pcl_godambe, is_indep_glm, modelTerms);
            if (new_crit > best_v_crit + 1.0E-9) {
              best_v_crit = new_crit;
              best_val = (double)val + 1.0;
            }
          }
        }
        if (best_val != d1) {
          idx_data[v] = best_val;
          current_crit = best_v_crit;
          improved = true;
        } else {
          idx_data[v] = d1;
        }
      }
    }
    best_crits[b_start_idx] = current_crit;
    iter = best_idxs->size[0];
    for (i1 = 0; i1 < iter; i1++) {
      best_idxs_data[i1 + best_idxs->size[0] * b_start_idx] = idx_data[i1];
    }
  }
  emxFree_real_T(&start_designs);
  if (!rtIsNaN(best_crits[0])) {
    nx = 1;
  } else {
    bool exitg1;
    nx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < pcl_num_starts + 1)) {
      if (!rtIsNaN(best_crits[k - 1])) {
        nx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (nx == 0) {
    *best_crit = best_crits[0];
    nx = 1;
  } else {
    *best_crit = best_crits[nx - 1];
    i = nx + 1;
    for (k = i; k < pcl_num_starts + 1; k++) {
      double d;
      d = best_crits[k - 1];
      if (*best_crit < d) {
        *best_crit = d;
        nx = k;
      }
    }
  }
  for (i = 0; i < best_idx_size; i++) {
    best_idx_data[i] = best_idxs_data[i + best_idxs->size[0] * (nx - 1)];
  }
  emxFree_real_T(&best_idxs);
  return best_idx_size;
}

/*
 * Arguments    : const double y[2]
 *                const emxArray_real_T *Xwp
 *                const emxArray_real_T *th
 *                int K
 *                double q
 *                double type
 *                const double lambda_fixed_data[]
 *                const int lambda_fixed_size[2]
 * Return Type  : double
 */
static double evaluate_copula_pmf(const double y[2], const emxArray_real_T *Xwp,
                                  const emxArray_real_T *th, int K, double q,
                                  double type, const double lambda_fixed_data[],
                                  const int lambda_fixed_size[2])
{
  const double *Xwp_data;
  const double *th_data;
  double lam;
  double pr;
  int b_i;
  int i;
  int k;
  th_data = th->data;
  Xwp_data = Xwp->data;
  if (((double)K - 1.0) + 1.0 > ((double)K - 1.0) + q) {
    i = 0;
  } else {
    i = (int)(((double)K - 1.0) + 1.0) - 1;
  }
  if ((lambda_fixed_size[0] == 0) || (lambda_fixed_size[1] == 0)) {
    lam = th_data[th->size[0] - 1];
  } else {
    lam = lambda_fixed_data[0];
  }
  if (type == 0.0) {
    double xb[2];
    double temp;
    int aoffset;
    int bit;
    signed char active_idx[2];
    bit = Xwp->size[1];
    xb[0] = 0.0;
    xb[1] = 0.0;
    for (k = 0; k < bit; k++) {
      aoffset = k << 1;
      temp = th_data[i + k];
      xb[0] += Xwp_data[aoffset] * temp;
      xb[1] += Xwp_data[aoffset + 1] * temp;
    }
    aoffset = -1;
    active_idx[0] = 0;
    active_idx[1] = 0;
    if (y[0] > 0.0) {
      aoffset = 0;
      active_idx[0] = 1;
    }
    if (y[1] > 0.0) {
      aoffset++;
      active_idx[aoffset] = 2;
    }
    pr = 0.0;
    i = (int)rt_powd_snf(2.0, (double)aoffset + 1.0);
    for (b_i = 0; b_i < i; b_i++) {
      double TERM1;
      double w_sum;
      signed char W_current[2];
      bool exitg1;
      bool skip;
      temp = ((double)b_i + 1.0) - 1.0;
      W_current[0] = 0;
      W_current[1] = 0;
      w_sum = 0.0;
      for (k = 0; k <= aoffset; k++) {
        if (temp == 0.0) {
          bit = 0;
        } else {
          bit = (int)fmod(temp, 2.0);
        }
        W_current[active_idx[aoffset - k] - 1] = (signed char)bit;
        w_sum += (double)bit;
        temp /= 2.0;
        temp = floor(temp);
      }
      TERM1 = 1.0;
      skip = false;
      bit = 0;
      exitg1 = false;
      while ((!exitg1) && (bit < 2)) {
        temp = y[bit] - (double)W_current[bit];
        if (temp < 0.0) {
          temp = 0.0;
        } else if (temp >= (double)K - 1.0) {
          temp = 1.0;
        } else {
          temp = 1.0 / (exp(-(th_data[(int)(temp + 1.0) - 1] + xb[bit])) + 1.0);
        }
        if (temp < 1.0E-10) {
          skip = true;
          exitg1 = true;
        } else {
          TERM1 *= exp(-lam * temp) - 1.0;
          bit++;
        }
      }
      if (!skip) {
        pr += rt_powd_snf(-1.0, w_sum) *
              (-(1.0 / lam) * log(TERM1 / (exp(-lam) - 1.0) + 1.0));
      }
    }
  } else {
    double xb[2];
    double temp;
    int aoffset;
    int bit;
    signed char active_idx[2];
    bit = Xwp->size[1];
    xb[0] = 0.0;
    xb[1] = 0.0;
    for (k = 0; k < bit; k++) {
      aoffset = k << 1;
      temp = th_data[i + k];
      xb[0] += Xwp_data[aoffset] * temp;
      xb[1] += Xwp_data[aoffset + 1] * temp;
    }
    aoffset = -1;
    active_idx[0] = 0;
    active_idx[1] = 0;
    if (y[0] > 0.0) {
      aoffset = 0;
      active_idx[0] = 1;
    }
    if (y[1] > 0.0) {
      aoffset++;
      active_idx[aoffset] = 2;
    }
    pr = 0.0;
    i = (int)rt_powd_snf(2.0, (double)aoffset + 1.0);
    for (b_i = 0; b_i < i; b_i++) {
      double TERM1;
      double w_sum;
      signed char W_current[2];
      bool exitg1;
      bool skip;
      temp = ((double)b_i + 1.0) - 1.0;
      W_current[0] = 0;
      W_current[1] = 0;
      w_sum = 0.0;
      for (k = 0; k <= aoffset; k++) {
        if (temp == 0.0) {
          bit = 0;
        } else {
          bit = (int)fmod(temp, 2.0);
        }
        W_current[active_idx[aoffset - k] - 1] = (signed char)bit;
        w_sum += (double)bit;
        temp /= 2.0;
        temp = floor(temp);
      }
      TERM1 = 0.0;
      skip = false;
      bit = 0;
      exitg1 = false;
      while ((!exitg1) && (bit < 2)) {
        temp = y[bit] - (double)W_current[bit];
        if (temp < 0.0) {
          temp = 0.0;
        } else if (temp >= (double)K - 1.0) {
          temp = 1.0;
        } else {
          temp = 1.0 / (exp(-(th_data[(int)(temp + 1.0) - 1] + xb[bit])) + 1.0);
        }
        if (temp < 1.0E-10) {
          skip = true;
          exitg1 = true;
        } else {
          TERM1 += rt_powd_snf(temp, -lam);
          bit++;
        }
      }
      if (!skip) {
        pr += rt_powd_snf(-1.0, w_sum) *
              rt_powd_snf(fmax(TERM1 - 1.0, 0.0), -1.0 / lam);
      }
    }
  }
  return fmax(pr, 2.2204460492503131E-16);
}

/*
 * Arguments    : const cell_wrap_0 levelSets_data[]
 *                const int levelSets_size[2]
 *                emxArray_real_T *combos
 * Return Type  : double
 */
static double generate_combos(const cell_wrap_0 levelSets_data[],
                              const int levelSets_size[2],
                              emxArray_real_T *combos)
{
  double n_combos;
  double *combos_data;
  int b_i;
  int combos_tmp;
  int i;
  int i1;
  int j;
  n_combos = 1.0;
  i = levelSets_size[1];
  for (b_i = 0; b_i < i; b_i++) {
    n_combos *= (double)levelSets_data[b_i].f1->size[1];
  }
  i1 = (int)n_combos;
  combos_tmp = combos->size[0] * combos->size[1];
  combos->size[0] = (int)n_combos;
  combos->size[1] = levelSets_size[1];
  emxEnsureCapacity_real_T(combos, combos_tmp);
  combos_data = combos->data;
  for (b_i = 0; b_i < i1; b_i++) {
    double temp;
    temp = ((double)b_i + 1.0) - 1.0;
    for (j = 0; j < i; j++) {
      combos_tmp = levelSets_data[j].f1->size[1];
      combos_data[b_i + combos->size[0] * j] =
          levelSets_data[j].f1->data[(int)(b_mod(temp, combos_tmp) + 1.0) - 1];
      temp /= (double)combos_tmp;
      temp = floor(temp);
    }
  }
  return n_combos;
}

/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 * Return Type  : void
 */
static void plus(emxArray_real_T *in1, const emxArray_real_T *in2)
{
  emxArray_real_T *b_in1;
  const double *in2_data;
  double *b_in1_data;
  double *in1_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_0;
  int stride_0_1;
  int stride_1_0;
  int stride_1_1;
  in2_data = in2->data;
  in1_data = in1->data;
  emxInit_real_T(&b_in1, 2);
  if (in2->size[0] == 1) {
    loop_ub = in1->size[0];
  } else {
    loop_ub = in2->size[0];
  }
  i = b_in1->size[0] * b_in1->size[1];
  b_in1->size[0] = loop_ub;
  if (in2->size[1] == 1) {
    b_loop_ub = in1->size[1];
  } else {
    b_loop_ub = in2->size[1];
  }
  b_in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(b_in1, i);
  b_in1_data = b_in1->data;
  stride_0_0 = (in1->size[0] != 1);
  stride_0_1 = (in1->size[1] != 1);
  stride_1_0 = (in2->size[0] != 1);
  stride_1_1 = (in2->size[1] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_in1_data[i1 + b_in1->size[0] * i] =
          in1_data[i1 * stride_0_0 + in1->size[0] * aux_0_1] +
          in2_data[i1 * stride_1_0 + in2->size[0] * aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
  i = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] = b_in1_data[i1 + b_in1->size[0] * i];
    }
  }
  emxFree_real_T(&b_in1);
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }
  return y;
}

/*
 * Arguments    : double linpred
 *                const emxArray_real_T *alpha
 *                int K
 *                const emxArray_real_T *xrow
 *                emxArray_real_T *I1
 * Return Type  : void
 */
static void univariateOrdinalFisher(double linpred,
                                    const emxArray_real_T *alpha, int K,
                                    const emxArray_real_T *xrow,
                                    emxArray_real_T *I1)
{
  emxArray_real_T *I_loc;
  emxArray_real_T *J;
  emxArray_real_T *U_c;
  emxArray_real_T *th0;
  emxArray_real_T *th1;
  emxArray_real_T *y;
  const double *alpha_data;
  const double *xrow_data;
  double F_up;
  double *I_loc_data;
  double *J_data;
  double *U_c_data;
  double *th0_data;
  double *th1_data;
  int b_i;
  int b_loop_ub;
  int boffset;
  int coffset;
  int i;
  int j;
  int k;
  int loop_ub;
  int mc;
  xrow_data = xrow->data;
  alpha_data = alpha->data;
  emxInit_real_T(&I_loc, 2);
  i = I_loc->size[0] * I_loc->size[1];
  coffset = (int)(((double)K - 1.0) + 1.0);
  I_loc->size[0] = coffset;
  I_loc->size[1] = K;
  emxEnsureCapacity_real_T(I_loc, i);
  I_loc_data = I_loc->data;
  loop_ub = coffset * K;
  for (i = 0; i < loop_ub; i++) {
    I_loc_data[i] = 0.0;
  }
  emxInit_real_T(&th0, 1);
  loop_ub = alpha->size[1] + 1;
  i = th0->size[0];
  th0->size[0] = alpha->size[1] + 1;
  emxEnsureCapacity_real_T(th0, i);
  th0_data = th0->data;
  th0_data[0] = linpred;
  b_loop_ub = alpha->size[1];
  for (i = 0; i < b_loop_ub; i++) {
    th0_data[i + 1] = alpha_data[i];
  }
  emxInit_real_T(&U_c, 1);
  emxInit_real_T(&th1, 1);
  emxInit_real_T(&J, 2);
  for (boffset = 0; boffset < K; boffset++) {
    double F_lo;
    if (boffset >= (double)K - 1.0) {
      F_up = 1.0;
    } else {
      F_up = 1.0 / (exp(-(alpha_data[boffset] + linpred)) + 1.0);
    }
    if (boffset - 1 < 0) {
      F_lo = 0.0;
    } else if ((double)boffset - 1.0 >= (double)K - 1.0) {
      F_lo = 1.0;
    } else {
      F_lo = 1.0 / (exp(-(alpha_data[boffset - 1] + linpred)) + 1.0);
    }
    F_up = fmax(F_up - F_lo, 2.2204460492503131E-16);
    if (!(F_up < 1.0E-12)) {
      i = U_c->size[0];
      U_c->size[0] = K;
      emxEnsureCapacity_real_T(U_c, i);
      U_c_data = U_c->data;
      for (b_i = 0; b_i < K; b_i++) {
        double b_u;
        double c_u;
        double u;
        i = th1->size[0];
        th1->size[0] = loop_ub;
        emxEnsureCapacity_real_T(th1, i);
        th1_data = th1->data;
        for (i = 0; i < loop_ub; i++) {
          th1_data[i] = th0_data[i];
        }
        th1_data[b_i] = th0_data[b_i] + 0.0001;
        if (boffset >= (double)K - 1.0) {
          F_lo = 1.0;
        } else {
          F_lo =
              1.0 /
              (exp(-(th1_data[(th1->size[0] >= 2) + boffset] + th1_data[0])) +
               1.0);
        }
        if (boffset - 1 < 0) {
          u = 0.0;
        } else if ((double)boffset - 1.0 >= (double)K - 1.0) {
          u = 1.0;
        } else {
          if (th1->size[0] < 2) {
            i = -1;
          } else {
            i = 0;
          }
          u = 1.0 / (exp(-(th1_data[i + boffset] + th1_data[0])) + 1.0);
        }
        i = th1->size[0];
        th1->size[0] = loop_ub;
        emxEnsureCapacity_real_T(th1, i);
        th1_data = th1->data;
        for (i = 0; i < loop_ub; i++) {
          th1_data[i] = th0_data[i];
        }
        th1_data[b_i] = th0_data[b_i] - 0.0001;
        if (boffset >= (double)K - 1.0) {
          b_u = 1.0;
        } else {
          b_u = 1.0 /
                (exp(-(th1_data[(th1->size[0] >= 2) + boffset] + th1_data[0])) +
                 1.0);
        }
        if (boffset - 1 < 0) {
          c_u = 0.0;
        } else if ((double)boffset - 1.0 >= (double)K - 1.0) {
          c_u = 1.0;
        } else {
          if (th1->size[0] < 2) {
            i = -1;
          } else {
            i = 0;
          }
          c_u = 1.0 / (exp(-(th1_data[i + boffset] + th1_data[0])) + 1.0);
        }
        U_c_data[b_i] = (log(fmax(F_lo - u, 2.2204460492503131E-16)) -
                         log(fmax(b_u - c_u, 2.2204460492503131E-16))) /
                        0.0002;
      }
      b_loop_ub = U_c->size[0];
      if ((I_loc->size[0] == U_c->size[0]) &&
          (U_c->size[0] == I_loc->size[1])) {
        i = J->size[0] * J->size[1];
        J->size[0] = U_c->size[0];
        J->size[1] = U_c->size[0];
        emxEnsureCapacity_real_T(J, i);
        J_data = J->data;
        for (i = 0; i < b_loop_ub; i++) {
          for (coffset = 0; coffset < b_loop_ub; coffset++) {
            J_data[coffset + J->size[0] * i] = U_c_data[coffset] * U_c_data[i];
          }
        }
        b_loop_ub = I_loc->size[0] * I_loc->size[1];
        for (i = 0; i < b_loop_ub; i++) {
          I_loc_data[i] += F_up * J_data[i];
        }
      } else {
        binary_expand_op_7(I_loc, F_up, U_c);
        I_loc_data = I_loc->data;
      }
    }
  }
  emxFree_real_T(&th1);
  emxFree_real_T(&U_c);
  emxFree_real_T(&th0);
  i = J->size[0] * J->size[1];
  J->size[0] = K;
  mc = (int)((double)xrow->size[1] + ((double)K - 1.0));
  J->size[1] = mc;
  emxEnsureCapacity_real_T(J, i);
  J_data = J->data;
  loop_ub = K * mc;
  for (i = 0; i < loop_ub; i++) {
    J_data[i] = 0.0;
  }
  loop_ub = xrow->size[1];
  for (i = 0; i < loop_ub; i++) {
    J_data[J->size[0] * i] = xrow_data[i];
  }
  for (b_i = 0; b_i <= K - 2; b_i++) {
    J_data[(b_i + J->size[0] *
                      (int)((unsigned int)xrow->size[1] + (unsigned int)b_i)) +
           1] = 1.0;
  }
  b_loop_ub = J->size[0];
  loop_ub = I_loc->size[1];
  emxInit_real_T(&y, 2);
  i = y->size[0] * y->size[1];
  y->size[0] = mc;
  y->size[1] = I_loc->size[1];
  emxEnsureCapacity_real_T(y, i);
  th1_data = y->data;
  for (j = 0; j < loop_ub; j++) {
    coffset = j * mc;
    boffset = j * I_loc->size[0];
    for (b_i = 0; b_i < mc; b_i++) {
      th1_data[coffset + b_i] = 0.0;
    }
    for (k = 0; k < b_loop_ub; k++) {
      F_up = I_loc_data[boffset + k];
      for (b_i = 0; b_i < mc; b_i++) {
        i = coffset + b_i;
        th1_data[i] += J_data[b_i * J->size[0] + k] * F_up;
      }
    }
  }
  emxFree_real_T(&I_loc);
  mtimes(y, J, I1);
  emxFree_real_T(&y);
  emxFree_real_T(&J);
}

/*
 * Arguments    : double m
 *                emxArray_real_T *n
 *                double K
 *                const emxArray_real_T *priorMean
 *                const emxArray_real_T *priorCov
 *                double numPoints
 *                const char priorMethod_data[]
 *                const int priorMethod_size[2]
 *                const cell_wrap_0 wp_levelSets_data[]
 *                const int wp_levelSets_size[2]
 *                const cell_wrap_0 sp_levelSets_data[]
 *                const int sp_levelSets_size[2]
 *                const emxArray_cell_wrap_0 *modelTerms
 *                char evalMethod_data[]
 *                int evalMethod_size[2]
 *                char crit_mode_data[]
 *                int crit_mode_size[2]
 *                double copulaType
 *                double sigma2_fixed_data[]
 *                int sigma2_fixed_size[2]
 *                const double lambda_fixed_data[]
 *                int lambda_fixed_size[2]
 *                double seed
 *                emxArray_real_T *optimalX
 *                double *optimalCrit
 * Return Type  : void
 */
void DesignWizardVn_App_GapPrimary(
    double m, emxArray_real_T *n, double K, const emxArray_real_T *priorMean,
    const emxArray_real_T *priorCov, double numPoints,
    const char priorMethod_data[], const int priorMethod_size[2],
    const cell_wrap_0 wp_levelSets_data[], const int wp_levelSets_size[2],
    const cell_wrap_0 sp_levelSets_data[], const int sp_levelSets_size[2],
    const emxArray_cell_wrap_0 *modelTerms, char evalMethod_data[],
    int evalMethod_size[2], char crit_mode_data[], int crit_mode_size[2],
    double copulaType, double sigma2_fixed_data[], int sigma2_fixed_size[2],
    const double lambda_fixed_data[], int lambda_fixed_size[2], double seed,
    emxArray_real_T *optimalX, double *optimalCrit)
{
  static const char cv5[18] = {'c', 'o', 'p', 'u', 'l', 'a', '_', 'p', 'c',
                               'l', '_', 'g', 'o', 'd', 'a', 'm', 'b', 'e'};
  static const char b_cv[11] = {'g', 'l', 'm', 'm', '_', 'a',
                                'p', 'p', 'r', 'o', 'x'};
  static const char cv2[11] = {'g', 'l', 'm', 'm', '_', 'a',
                               'p', 'p', 'r', 'o', 'x'};
  static const char cv3[10] = {'g', 'l', 'm', 'm', '_',
                               'e', 'x', 'a', 'c', 't'};
  static const char cv4[10] = {'c', 'o', 'p', 'u', 'l',
                               'a', '_', 'p', 'c', 'l'};
  static const char cv6[9] = {'i', 'n', 'd', 'e', 'p', '_', 'g', 'l', 'm'};
  static const char cv1[7] = {'a', 'v', 'e', 'r', 'a', 'g', 'e'};
  emxArray_real_T *A;
  emxArray_real_T *b_r;
  emxArray_real_T *idx_sp;
  emxArray_real_T *idx_wp;
  emxArray_real_T *r2;
  emxArray_real_T *r3;
  emxArray_real_T *sp_combos;
  emxArray_real_T *thetaGrid;
  emxArray_real_T *thetaGrid_gap;
  emxArray_real_T *ub;
  emxArray_real_T *weights;
  emxArray_real_T *wp_combos;
  double idxOpt_data[5000];
  const double *priorCov_data;
  const double *priorMean_data;
  double N;
  double num_pts;
  double num_sp_combos;
  double p;
  double radius;
  double thetaGrid_tmp;
  double *n_data;
  double *r1;
  double *sp_combos_data;
  double *thetaGrid_data;
  double *thetaGrid_gap_data;
  double *ub_data;
  double *wp_combos_data;
  int sp_combos_size[2];
  int wp_combos_size[2];
  int exitg1;
  int i;
  int ib;
  int ibmat;
  int jmax;
  int k;
  int lastBlockLength;
  int nblocks;
  unsigned int r;
  bool is_copula_pcl;
  bool is_copula_pcl_godambe;
  bool is_glmm;
  bool is_glmm_approx;
  bool is_glmm_exact;
  bool is_indep_glm;
  (void)numPoints;
  (void)priorMethod_data;
  (void)priorMethod_size;
  if (!isInitialized_DesignWizardVn_App_GapPrimary) {
    DesignWizardVn_App_GapPrimary_initialize();
  }
  priorCov_data = priorCov->data;
  priorMean_data = priorMean->data;
  n_data = n->data;
  /*  =========================================================================
   */
  /*  DesignWizardVn_App_GapPrimary: Gap-Reparameterized Production Generator */
  /*  =========================================================================
   */
  /*  v4 production criterion for ordinal split-plot designs. This is the */
  /*  gap-reparameterized analogue of DesignWizardVn_App.m: the prior is */
  /*  specified directly on the unconstrained gap coordinates */
  /*  */
  /*    theta_gap = (alpha_1, log(Delta_1), log(Delta_2), beta_1, ..., beta_q)
   */
  /*  */
  /*  where Delta_k = alpha_{k+1} - alpha_k > 0. Inside the criterion */
  /*  evaluator, each GJS quadrature node is transformed back to cutpoint */
  /*  coordinates */
  /*  */
  /*    theta = (alpha_1, alpha_2, alpha_3, beta_1, ..., beta_q) */
  /*  */
  /*  with alpha_2 = alpha_1 + exp(log(Delta_1)) and alpha_3 = alpha_2 + */
  /*  exp(log(Delta_2)), so the cumulative-logit model evaluation is in its */
  /*  natural parameterization. The ordering constraint */
  /*  alpha_1 < alpha_2 < alpha_3 holds at every node by construction; the */
  /*  eps-clamps inside the Fisher evaluators (lines 306 of */
  /*  univariateOrdinalFisher, line 397 of GLMM_ordinal_marginalPMF_cg, line */
  /*  505 of evaluate_copula_pmf) are retained as defensive guards but are */
  /*  empirically never engaged under this entry point. */
  /*  */
  /*  Signature differences from DesignWizardVn_App.m: */
  /*    - priorMean is interpreted as gap-coordinate (1 x p) mean vector */
  /*    - priorCov  is interpreted as gap-coordinate (p x p) covariance */
  /*    - priorMethod = 'sampled' is NOT SUPPORTED in this entry point */
  /*      (the sampled-prior pathway requires a separate gap-coordinate */
  /*       MC sampler; left as future work) */
  /*    - All other arguments are identical to DesignWizardVn_App.m */
  /*  */
  /*  Coordinate-transformation rule (applied inside the prior-generation */
  /*  block before the thetaGrid is passed downstream): */
  /*  */
  /*    thetaGrid_gap(t, :)    = priorMean + nodes(t, :) * chol(priorCov) */
  /*    thetaGrid_cut(t, 1)    = thetaGrid_gap(t, 1)                          %
   * alpha_1 */
  /*    thetaGrid_cut(t, 2)    = thetaGrid_gap(t, 1) + exp(thetaGrid_gap(t, 2))
   */
  /*    thetaGrid_cut(t, 3)    = thetaGrid_cut(t, 2) + exp(thetaGrid_gap(t, 3))
   */
  /*    thetaGrid_cut(t, 4:p)  = thetaGrid_gap(t, 4:p)                        %
   * beta unchanged */
  /*  */
  /*  Limitations */
  /*  ----------- */
  /*  (1) Only priorMethod = 'quadrature' is supported. The 'sampled' branch */
  /*      was removed; gap-coordinate sampled-prior MC is straightforward but */
  /*      left as future work. */
  /*  */
  /*  (2) When sigma2_fixed = [] (i.e., sigma^2 is treated as a free parameter
   */
  /*      to be estimated rather than held fixed), this entry point still */
  /*      interprets the last component of priorMean/priorCov as the prior */
  /*      mean/variance of sigma^2 directly (NOT log sigma^2). A future */
  /*      refinement would reparameterize sigma^2 -> log sigma^2 in gap */
  /*      coordinates to make the variance prior positivity-respecting by */
  /*      construction; for now, that pathway inherits the same numerical */
  /*      guards as v3 (the `s2 <= 1e-8` check at line 369 of
   * computeFisher_GLMM_Approx). */
  /*      The same caveat applies to lambda when lambda_fixed = []. In the */
  /*      production use cases of the present paper, sigma^2 and lambda are */
  /*      always held fixed, so this limitation does not affect any reported */
  /*      results. */
  /*  */
  /*  Companion: DesignWizardVn_App.m (v3, cutpoint-coordinate primary) */
  /*  Build script: Compile_DesignWizardVn_GapPrimary_C_Fast.m */
  /*  =========================================================================
   */
  /*  ====================== DEFAULTS & ROUTING ====================== */
  if (evalMethod_size[1] == 0) {
    evalMethod_size[1] = 11;
    for (i = 0; i < 11; i++) {
      evalMethod_data[i] = b_cv[i];
    }
  }
  if (crit_mode_size[1] == 0) {
    crit_mode_size[0] = 1;
    crit_mode_size[1] = 7;
    for (i = 0; i < 7; i++) {
      crit_mode_data[i] = cv1[i];
    }
  }
  if ((sigma2_fixed_size[0] == 0) || (sigma2_fixed_size[1] == 0)) {
    sigma2_fixed_size[0] = 0;
    sigma2_fixed_size[1] = 0;
  }
  if ((lambda_fixed_size[0] == 0) || (lambda_fixed_size[1] == 0)) {
    lambda_fixed_size[0] = 0;
    lambda_fixed_size[1] = 0;
  }
  if (seed < 4.294967296E+9) {
    if (seed >= 0.0) {
      r = (unsigned int)seed;
    } else {
      r = 0U;
    }
  } else if (seed >= 4.294967296E+9) {
    r = MAX_uint32_T;
  } else {
    r = 0U;
  }
  if (r == 0U) {
    r = 5489U;
  }
  state[0] = r;
  for (jmax = 0; jmax < 623; jmax++) {
    r = ((r ^ r >> 30U) * 1812433253U + (unsigned int)jmax) + 1U;
    state[jmax + 1] = r;
  }
  state[624] = 624U;
  is_glmm_approx = false;
  if (evalMethod_size[1] == 11) {
    jmax = 0;
    do {
      exitg1 = 0;
      if (jmax < 11) {
        if (cv[(unsigned char)evalMethod_data[jmax] & 127] !=
            cv[(int)cv2[jmax]]) {
          exitg1 = 1;
        } else {
          jmax++;
        }
      } else {
        is_glmm_approx = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  is_glmm_exact = false;
  if (evalMethod_size[1] == 10) {
    jmax = 0;
    do {
      exitg1 = 0;
      if (jmax < 10) {
        if (cv[(unsigned char)evalMethod_data[jmax] & 127] !=
            cv[(int)cv3[jmax]]) {
          exitg1 = 1;
        } else {
          jmax++;
        }
      } else {
        is_glmm_exact = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  is_copula_pcl = false;
  if (evalMethod_size[1] == 10) {
    jmax = 0;
    do {
      exitg1 = 0;
      if (jmax < 10) {
        if (cv[(unsigned char)evalMethod_data[jmax] & 127] !=
            cv[(int)cv4[jmax]]) {
          exitg1 = 1;
        } else {
          jmax++;
        }
      } else {
        is_copula_pcl = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  is_copula_pcl_godambe = false;
  if (evalMethod_size[1] == 18) {
    jmax = 0;
    do {
      exitg1 = 0;
      if (jmax < 18) {
        if (cv[(unsigned char)evalMethod_data[jmax] & 127] !=
            cv[(int)cv5[jmax]]) {
          exitg1 = 1;
        } else {
          jmax++;
        }
      } else {
        is_copula_pcl_godambe = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  is_indep_glm = false;
  if (evalMethod_size[1] == 9) {
    jmax = 0;
    do {
      exitg1 = 0;
      if (jmax < 9) {
        if (cv[(unsigned char)evalMethod_data[jmax] & 127] !=
            cv[(int)cv6[jmax]]) {
          exitg1 = 1;
        } else {
          jmax++;
        }
      } else {
        is_indep_glm = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  if (is_glmm_approx || is_glmm_exact) {
    is_glmm = true;
  } else {
    is_glmm = false;
  }
  num_pts = rt_roundd_snf(K);
  if (num_pts < 2.147483648E+9) {
    if (num_pts >= -2.147483648E+9) {
      i = (int)num_pts;
    } else {
      i = MIN_int32_T;
    }
  } else if (num_pts >= 2.147483648E+9) {
    i = MAX_int32_T;
  } else {
    i = 0;
  }
  /*  ====================== PARAMETER DIMENSIONS ====================== */
  if (is_glmm_approx) {
    if ((sigma2_fixed_size[0] == 0) || (sigma2_fixed_size[1] == 0)) {
      sigma2_fixed_size[0] = 1;
      sigma2_fixed_size[1] = 1;
      sigma2_fixed_data[0] = 1.0;
    }
    p = ((double)i - 1.0) + (double)modelTerms->size[1];
  } else if (is_indep_glm) {
    /*  Independence GLM has no variance and no dependence parameter */
    p = ((double)i - 1.0) + (double)modelTerms->size[1];
  } else if (is_glmm) {
    if ((sigma2_fixed_size[0] == 0) || (sigma2_fixed_size[1] == 0)) {
      p = (((double)i - 1.0) + (double)modelTerms->size[1]) + 1.0;
    } else {
      p = ((double)i - 1.0) + (double)modelTerms->size[1];
    }
  } else if ((lambda_fixed_size[0] == 0) || (lambda_fixed_size[1] == 0)) {
    p = (((double)i - 1.0) + (double)modelTerms->size[1]) + 1.0;
  } else {
    p = ((double)i - 1.0) + (double)modelTerms->size[1];
  }
  /*  ====================== PRIOR GENERATION (GAP-PRIMARY)
   * ====================== */
  /*  priorMean and priorCov are interpreted as the mean and covariance */
  /*  of the GAP-COORDINATE prior: */
  /*    theta_gap = (alpha_1, log(Delta_1), log(Delta_2), beta_1, ..., beta_q)
   */
  /*  Each GJS quadrature node is rescaled in gap coordinates, then */
  /*  transformed to cutpoint coordinates before being passed to the */
  /*  criterion evaluator. The transformation makes the ordering constraint */
  /*  alpha_1 < alpha_2 < alpha_3 hold by construction at every node. */
  num_pts = 2.0 * p;
  emxInit_real_T(&thetaGrid, 2);
  nblocks = (int)num_pts;
  k = thetaGrid->size[0] * thetaGrid->size[1];
  thetaGrid->size[0] = (int)num_pts;
  ib = (int)p;
  thetaGrid->size[1] = (int)p;
  emxEnsureCapacity_real_T(thetaGrid, k);
  thetaGrid_data = thetaGrid->data;
  jmax = (int)num_pts * (int)p;
  for (k = 0; k < jmax; k++) {
    thetaGrid_data[k] = 0.0;
  }
  emxInit_real_T(&weights, 1);
  k = weights->size[0];
  weights->size[0] = (int)num_pts;
  emxEnsureCapacity_real_T(weights, k);
  ub_data = weights->data;
  for (k = 0; k < nblocks; k++) {
    ub_data[k] = 1.0 / num_pts;
  }
  radius = sqrt(p);
  for (ibmat = 0; ibmat < ib; ibmat++) {
    thetaGrid_tmp = 2.0 * ((double)ibmat + 1.0);
    thetaGrid_data[((int)(thetaGrid_tmp - 1.0) + thetaGrid->size[0] * ibmat) -
                   1] = radius;
    thetaGrid_data[((int)thetaGrid_tmp + thetaGrid->size[0] * ibmat) - 1] =
        -radius;
  }
  emxInit_real_T(&thetaGrid_gap, 2);
  k = thetaGrid_gap->size[0] * thetaGrid_gap->size[1];
  thetaGrid_gap->size[0] = (int)num_pts;
  ib = priorMean->size[1];
  thetaGrid_gap->size[1] = priorMean->size[1];
  emxEnsureCapacity_real_T(thetaGrid_gap, k);
  thetaGrid_gap_data = thetaGrid_gap->data;
  for (jmax = 0; jmax < ib; jmax++) {
    ibmat = jmax * thetaGrid->size[0];
    for (lastBlockLength = 0; lastBlockLength < nblocks; lastBlockLength++) {
      thetaGrid_gap_data[ibmat + lastBlockLength] = priorMean_data[jmax];
    }
  }
  emxInit_real_T(&A, 2);
  k = A->size[0] * A->size[1];
  A->size[0] = priorCov->size[0];
  A->size[1] = priorCov->size[1];
  emxEnsureCapacity_real_T(A, k);
  ub_data = A->data;
  jmax = priorCov->size[0] * priorCov->size[1];
  for (k = 0; k < jmax; k++) {
    ub_data[k] = priorCov_data[k];
  }
  jmax = priorCov->size[0];
  ibmat = priorCov->size[1];
  if (jmax <= ibmat) {
    ibmat = jmax;
  }
  if (ibmat != 0) {
    jmax = xpotrf(ibmat, A, priorCov->size[0]);
    ub_data = A->data;
    if (jmax == 0) {
      jmax = ibmat;
    } else {
      jmax--;
    }
    for (nblocks = 0; nblocks <= jmax - 2; nblocks++) {
      k = nblocks + 2;
      for (ibmat = k; ibmat <= jmax; ibmat++) {
        ub_data[(ibmat + A->size[0] * nblocks) - 1] = 0.0;
      }
    }
  }
  emxInit_real_T(&b_r, 2);
  mtimes(thetaGrid, A, b_r);
  r1 = b_r->data;
  emxFree_real_T(&A);
  if ((thetaGrid_gap->size[0] == b_r->size[0]) &&
      (thetaGrid_gap->size[1] == b_r->size[1])) {
    jmax = thetaGrid_gap->size[0] * thetaGrid_gap->size[1];
    for (k = 0; k < jmax; k++) {
      thetaGrid_gap_data[k] += r1[k];
    }
  } else {
    plus(thetaGrid_gap, b_r);
    thetaGrid_gap_data = thetaGrid_gap->data;
  }
  emxFree_real_T(&b_r);
  /*  Gap-to-cutpoint transformation, generalized to arbitrary K: */
  /*    thetaGrid(:, 1)    = alpha_1                          = thetaGrid_gap(:,
   * 1) */
  /*    thetaGrid(:, j+1)  = alpha_j + exp(log(Delta_j))      for j
   * = 1..num_alphas-1 */
  /*    thetaGrid(:, k+1:p) = beta                            = thetaGrid_gap(:,
   * k+1:p), k=num_alphas */
  /*  This makes alpha_1 < alpha_2 < ... < alpha_{K-1} hold by construction. */
  jmax = thetaGrid_gap->size[0];
  k = thetaGrid->size[0] * thetaGrid->size[1];
  thetaGrid->size[0] = thetaGrid_gap->size[0];
  thetaGrid->size[1] = thetaGrid_gap->size[1];
  emxEnsureCapacity_real_T(thetaGrid, k);
  thetaGrid_data = thetaGrid->data;
  ibmat = thetaGrid_gap->size[0] * thetaGrid_gap->size[1];
  for (k = 0; k < ibmat; k++) {
    thetaGrid_data[k] = thetaGrid_gap_data[k];
  }
  /*  allocate with same shape */
  for (k = 0; k < jmax; k++) {
    thetaGrid_data[k] = thetaGrid_gap_data[k];
  }
  emxInit_real_T(&ub, 1);
  for (nblocks = 0; nblocks <= i - 3; nblocks++) {
    k = ub->size[0];
    ub->size[0] = jmax;
    emxEnsureCapacity_real_T(ub, k);
    ub_data = ub->data;
    for (k = 0; k < jmax; k++) {
      ub_data[k] =
          exp(thetaGrid_gap_data[k + thetaGrid_gap->size[0] * (nblocks + 1)]);
    }
    if (thetaGrid->size[0] == ub->size[0]) {
      ibmat = thetaGrid->size[0];
      k = ub->size[0];
      ub->size[0] = thetaGrid->size[0];
      emxEnsureCapacity_real_T(ub, k);
      ub_data = ub->data;
      for (k = 0; k < ibmat; k++) {
        ub_data[k] += thetaGrid_data[k + thetaGrid->size[0] * nblocks];
      }
      for (k = 0; k < ibmat; k++) {
        thetaGrid_data[k + thetaGrid->size[0] * (nblocks + 1)] = ub_data[k];
      }
    } else {
      binary_expand_op(thetaGrid, nblocks, ub);
      thetaGrid_data = thetaGrid->data;
    }
  }
  /*  regression-coefficient block: unchanged */
  if (((double)i - 1.0) + 1.0 > p) {
    k = 0;
    ib = 0;
    ibmat = 0;
  } else {
    k = (int)(((double)i - 1.0) + 1.0) - 1;
    ib = (int)p;
    ibmat = k;
  }
  nblocks = ib - k;
  for (ib = 0; ib < nblocks; ib++) {
    for (lastBlockLength = 0; lastBlockLength < jmax; lastBlockLength++) {
      thetaGrid_gap_data[lastBlockLength + jmax * ib] =
          thetaGrid_gap_data[lastBlockLength +
                             thetaGrid_gap->size[0] * (k + ib)];
    }
  }
  k = thetaGrid_gap->size[0] * thetaGrid_gap->size[1];
  thetaGrid_gap->size[1] = nblocks;
  emxEnsureCapacity_real_T(thetaGrid_gap, k);
  thetaGrid_gap_data = thetaGrid_gap->data;
  jmax = thetaGrid->size[0];
  for (k = 0; k < nblocks; k++) {
    for (ib = 0; ib < jmax; ib++) {
      thetaGrid_data[ib + thetaGrid->size[0] * (ibmat + k)] =
          thetaGrid_gap_data[ib + jmax * k];
    }
  }
  emxFree_real_T(&thetaGrid_gap);
  emxInit_real_T(&wp_combos, 2);
  thetaGrid_tmp =
      generate_combos(wp_levelSets_data, wp_levelSets_size, wp_combos);
  wp_combos_data = wp_combos->data;
  emxInit_real_T(&sp_combos, 2);
  num_sp_combos =
      generate_combos(sp_levelSets_data, sp_levelSets_size, sp_combos);
  sp_combos_data = sp_combos->data;
  /*  ====================== UNBALANCED SIZING ====================== */
  /*  Ensure n is explicitly a 1 x m row vector to match coder signature */
  if (n->size[1] == 1) {
    num_pts = n_data[0];
    k = n->size[0] * n->size[1];
    n->size[0] = 1;
    ib = (int)m;
    n->size[1] = (int)m;
    emxEnsureCapacity_real_T(n, k);
    n_data = n->data;
    for (ibmat = 0; ibmat < ib; ibmat++) {
      n_data[ibmat] = num_pts;
    }
  }
  if (n->size[1] == 0) {
    N = 0.0;
  } else {
    if (n->size[1] <= 1024) {
      ibmat = n->size[1];
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      ibmat = 1024;
      nblocks = (int)((unsigned int)n->size[1] >> 10);
      lastBlockLength = n->size[1] - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    N = n_data[0];
    for (k = 2; k <= ibmat; k++) {
      N += n_data[k - 1];
    }
    for (ib = 2; ib <= nblocks; ib++) {
      jmax = (ib - 1) << 10;
      num_pts = n_data[jmax];
      if (ib == nblocks) {
        ibmat = lastBlockLength;
      } else {
        ibmat = 1024;
      }
      for (k = 2; k <= ibmat; k++) {
        num_pts += n_data[(jmax + k) - 1];
      }
      N += num_pts;
    }
  }
  /*  ====================== OPTIMIZER SETUP ====================== */
  radius = m + N;
  nblocks = (int)radius;
  k = ub->size[0];
  ub->size[0] = (int)radius;
  emxEnsureCapacity_real_T(ub, k);
  ub_data = ub->data;
  for (k = 0; k < nblocks; k++) {
    ub_data[k] = 0.0;
  }
  k = (int)m;
  for (ibmat = 0; ibmat < k; ibmat++) {
    ub_data[ibmat] = thetaGrid_tmp;
  }
  ib = (int)(radius + (1.0 - (m + 1.0)));
  for (ibmat = 0; ibmat < ib; ibmat++) {
    ub_data[(int)((m + 1.0) + (double)ibmat) - 1] = num_sp_combos;
  }
  emxInit_real_T(&r2, 1);
  ib = r2->size[0];
  r2->size[0] = (int)radius;
  emxEnsureCapacity_real_T(r2, ib);
  r1 = r2->data;
  for (ib = 0; ib < nblocks; ib++) {
    r1[ib] = 1.0;
  }
  coordinate_exchange_otf(m, n, N, weights, p, crit_mode_data, crit_mode_size,
                          radius, r2, ub, wp_combos, sp_combos, thetaGrid, i,
                          modelTerms->size[1], sigma2_fixed_data,
                          sigma2_fixed_size, lambda_fixed_data,
                          lambda_fixed_size, copulaType, is_glmm_approx,
                          is_glmm_exact, is_copula_pcl, is_copula_pcl_godambe,
                          is_indep_glm, modelTerms, idxOpt_data, &num_pts);
  emxFree_real_T(&r2);
  emxFree_real_T(&ub);
  *optimalCrit = computeCriterion_otf(
      idxOpt_data, m, n, N, weights, p, crit_mode_data, crit_mode_size,
      wp_combos, sp_combos, thetaGrid, i, modelTerms->size[1],
      sigma2_fixed_data, sigma2_fixed_size, lambda_fixed_data,
      lambda_fixed_size, copulaType, is_glmm_approx, is_glmm_exact,
      is_copula_pcl, is_copula_pcl_godambe, is_indep_glm, modelTerms);
  emxFree_real_T(&weights);
  emxFree_real_T(&thetaGrid);
  emxInit_real_T(&idx_wp, 1);
  i = idx_wp->size[0];
  idx_wp->size[0] = (int)m;
  emxEnsureCapacity_real_T(idx_wp, i);
  ub_data = idx_wp->data;
  for (ibmat = 0; ibmat < k; ibmat++) {
    ub_data[ibmat] = idxOpt_data[ibmat];
  }
  i = (int)N;
  emxInit_real_T(&idx_sp, 1);
  ib = idx_sp->size[0];
  idx_sp->size[0] = (int)N;
  emxEnsureCapacity_real_T(idx_sp, ib);
  thetaGrid_data = idx_sp->data;
  for (ibmat = 0; ibmat < i; ibmat++) {
    thetaGrid_data[ibmat] = idxOpt_data[(int)(m + ((double)ibmat + 1.0)) - 1];
  }
  i = optimalX->size[0] * optimalX->size[1];
  optimalX->size[0] = (int)N;
  jmax = modelTerms->size[1];
  optimalX->size[1] = modelTerms->size[1];
  emxEnsureCapacity_real_T(optimalX, i);
  thetaGrid_gap_data = optimalX->data;
  ibmat = (int)N * modelTerms->size[1];
  for (i = 0; i < ibmat; i++) {
    thetaGrid_gap_data[i] = 0.0;
  }
  num_pts = 1.0;
  emxInit_real_T(&r3, 2);
  for (ibmat = 0; ibmat < k; ibmat++) {
    i = (int)n_data[ibmat];
    if (i - 1 >= 0) {
      wp_combos_size[0] = 1;
      sp_combos_size[0] = 1;
    }
    for (lastBlockLength = 0; lastBlockLength < i; lastBlockLength++) {
      double b_sp_combos_data[20];
      double b_wp_combos_data[20];
      radius = (num_pts + ((double)lastBlockLength + 1.0)) - 1.0;
      nblocks = wp_combos->size[1];
      wp_combos_size[1] = wp_combos->size[1];
      for (ib = 0; ib < nblocks; ib++) {
        b_wp_combos_data[ib] =
            wp_combos_data[((int)ub_data[ibmat] + wp_combos->size[0] * ib) - 1];
      }
      nblocks = sp_combos->size[1];
      sp_combos_size[1] = sp_combos->size[1];
      for (ib = 0; ib < nblocks; ib++) {
        b_sp_combos_data[ib] =
            sp_combos_data[((int)thetaGrid_data[(int)radius - 1] +
                            sp_combos->size[0] * ib) -
                           1];
      }
      build_model_row(b_wp_combos_data, wp_combos_size, b_sp_combos_data,
                      sp_combos_size, modelTerms, r3);
      r1 = r3->data;
      for (ib = 0; ib < jmax; ib++) {
        thetaGrid_gap_data[((int)radius + optimalX->size[0] * ib) - 1] = r1[ib];
      }
    }
    num_pts += n_data[ibmat];
  }
  emxFree_real_T(&r3);
  emxFree_real_T(&idx_sp);
  emxFree_real_T(&idx_wp);
  emxFree_real_T(&sp_combos);
  emxFree_real_T(&wp_combos);
}

/*
 * File trailer for DesignWizardVn_App_GapPrimary.c
 *
 * [EOF]
 */
