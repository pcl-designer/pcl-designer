#include "main.h"
#include "DesignWizardVn_App_GapPrimary.h"
#include "DesignWizardVn_App_GapPrimary_terminate.h"
#include "DesignWizardVn_App_GapPrimary_emxAPI.h"
#include "DesignWizardVn_App_GapPrimary_initialize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

/* v0.2.5: num_starts is now an entry INPUT ARGUMENT (back-ported into the
 * MATLAB source), not a post-codegen extern global. main owns the parsed
 * value and passes it to the entry. */
#define PCL_MAX_STARTS 1024
int pcl_num_starts = 15;

/*
 * v4 main wrapper for DesignWizardVn_App_GapPrimary.
 *
 * Identical to v3 main.c except:
 *   - Includes the GapPrimary headers
 *   - Calls DesignWizardVn_App_GapPrimary() instead of DesignWizardVn_App()
 *
 * IMPORTANT: priorMean.csv and priorCov.csv must contain GAP-COORDINATE
 * prior parameters in v4. The file format is the same as v3 (a row vector
 * and a square matrix of doubles, comma-separated), but the interpretation
 * differs:
 *
 *   priorMean = [mu_alpha_1, mu_logDelta_1, mu_logDelta_2, mu_beta_1, ..., mu_beta_q]
 *   priorCov  = (p x p) covariance in the gap coordinates
 *
 * Use the moment-matching routines (moment_match_gap_prior.m) to produce
 * the gap-coordinate prior from a cutpoint-coordinate elicitation.
 */

/* --- Helper: Robust String Trimming --- */
void trim(char *s) {
    char *p = s;
    int l = (int)strlen(p);
    while(l > 0 && isspace((unsigned char)p[l - 1])) p[--l] = 0;
    while(*p && isspace((unsigned char)*p)) ++p, --l;
    memmove(s, p, l + 1);
}

/* --- Helper: Config Parser (identical to v3) --- */
void parse_config(double *m, double *K, double *sd,
                  double *cType, double *s2, double *lam,
                  char *evalMeth, char *critMode) {
    FILE *f = fopen("config.txt", "r");
    if (!f) {
        printf("Warning: config.txt not found. Using defaults.\n");
        return;
    }
    char line[256], key[128], val[128];
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%[^=]=%s", key, val) == 2) {
            trim(key); trim(val);
            if (strcmp(key, "m") == 0) *m = atof(val);
            else if (strcmp(key, "K") == 0) *K = atof(val);
            else if (strcmp(key, "seed") == 0) *sd = atof(val);
            else if (strcmp(key, "copula_type") == 0) *cType = atof(val);
            else if (strcmp(key, "sigma2_fixed") == 0) *s2 = atof(val);
            else if (strcmp(key, "lambda_fixed") == 0) *lam = atof(val);
            else if (strcmp(key, "evalMethod") == 0) strcpy(evalMeth, val);
            else if (strcmp(key, "crit_mode") == 0) strcpy(critMode, val);
            else if (strcmp(key, "num_starts") == 0) pcl_num_starts = (int)atof(val);
        }
    }
    fclose(f);
}

/* --- Utility: Load CSV into emxArray --- */
emxArray_real_T* load_csv(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;
    int r = 0, c = 0;
    char line[2048];
    while (fgets(line, sizeof(line), f)) {
        if (r == 0) {
            char *tmp = strdup(line);
            char *tok = strtok(tmp, ",");
            while (tok) { c++; tok = strtok(NULL, ","); }
            free(tmp);
        }
        r++;
    }
    rewind(f);
    emxArray_real_T *emx = emxCreate_real_T(r, c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            double v;
            if (fscanf(f, "%lf%*[, \t\n]", &v) == 1) emx->data[i + j * r] = v;
            else emx->data[i + j * r] = NAN;
        }
    }
    fclose(f);
    return emx;
}

/* --- Utility: Export results --- */
void export_to_csv(const char* filename, emxArray_real_T *optimalX) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    for (int i = 0; i < optimalX->size[0]; i++) {
        for (int j = 0; j < optimalX->size[1]; j++) {
            fprintf(f, "%.8f%s", optimalX->data[i + j * optimalX->size[0]],
                    (j == optimalX->size[1]-1) ? "" : ",");
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(int argc, char **argv) {
    DesignWizardVn_App_GapPrimary_initialize();

    /* 1. Defaults & Parsing */
    double m = 20.0, K = 3.0, sd = 8675309.0;
    double cType = 0.0, s2 = 1.0, lam = 1.0;
    char evalMeth[64] = "glmm_approx";
    char critMode[64] = "average";

    parse_config(&m, &K, &sd, &cType, &s2, &lam, evalMeth, critMode);

    /* 2. Load Input Data (NOTE: priorMean.csv and priorCov.csv are in
     *    GAP COORDINATES for v4. Format unchanged from v3.) */
    emxArray_real_T *pMean = load_csv("priorMean.csv");
    emxArray_real_T *pCov  = load_csv("priorCov.csv");
    emxArray_real_T *wpRaw = load_csv("wp_levels.csv");
    emxArray_real_T *spRaw = load_csv("sp_levels.csv");
    emxArray_real_T *termsRaw = load_csv("modelTerms.csv");

    /* Load Variable 'n' array */
    emxArray_real_T *n_arr = load_csv("n_sizes.csv");
    if (!n_arr) {
        printf("Warning: n_sizes.csv not found. Defaulting to balanced scalar n = 4.\n");
        n_arr = emxCreate_real_T(1, 1);
        n_arr->data[0] = 4.0;
    }

    /* 3. Level Sets (NaN Filtering) */
    cell_wrap_0 wpL_data[20], spL_data[20];
    int wpL_size[2] = {1, wpRaw->size[0]};
    int spL_size[2] = {1, spRaw->size[0]};

    for(int i=0; i < wpRaw->size[0]; i++) {
        int count = 0;
        for(int j=0; j < wpRaw->size[1]; j++) if (!isnan(wpRaw->data[i + j*wpRaw->size[0]])) count++;
        wpL_data[i].f1 = emxCreate_real_T(1, count);
        int idx = 0;
        for(int j=0; j < wpRaw->size[1]; j++) {
            double v = wpRaw->data[i + j*wpRaw->size[0]];
            if (!isnan(v)) wpL_data[i].f1->data[idx++] = v;
        }
    }

    for(int i=0; i < spRaw->size[0]; i++) {
        int count = 0;
        for(int j=0; j < spRaw->size[1]; j++) if (!isnan(spRaw->data[i + j*spRaw->size[0]])) count++;
        spL_data[i].f1 = emxCreate_real_T(1, count);
        int idx = 0;
        for(int j=0; j < spRaw->size[1]; j++) {
            double v = spRaw->data[i + j*spRaw->size[0]];
            if (!isnan(v)) spL_data[i].f1->data[idx++] = v;
        }
    }

    /* 4. Model Terms Setup */
    int q = termsRaw->size[0];
    emxArray_cell_wrap_0 *mTerms = emxCreate_cell_wrap_0(1, q);
    for(int i=0; i<q; i++) {
        int tlen = 0;
        for(int j=0; j < termsRaw->size[1]; j++) if(termsRaw->data[i + j*q] > 0) tlen++;
        mTerms->data[i].f1 = emxCreate_real_T(1, tlen);
        int cur = 0;
        for(int j=0; j < termsRaw->size[1]; j++) if(termsRaw->data[i + j*q] > 0) mTerms->data[i].f1->data[cur++] = termsRaw->data[i + j*q];
    }

    /* 5. Execution Setup */
    int pSz[2] = {1, (int)strlen(evalMeth)};
    int cSz[2] = {1, (int)strlen(critMode)};
    int qSz[2] = {1, 10}; /* "quadrature" */

    /* sigma2_fixed and lambda_fixed are codegen'd as variable-size [1 x 1]
     * scalars (optionalScalar = coder.typeof(0, [1 1], [true true])).
     * Signature: ..., double sigma2_fixed_data[], int sigma2_fixed_size[2],
     *            const double lambda_fixed_data[], int lambda_fixed_size[2], ...
     * We pass length-1 arrays containing the scalar values. */
    double s2_data[1]  = { s2 };
    int    s2_sz[2]    = { 1, 1 };
    double lam_data[1] = { lam };
    int    lam_sz[2]   = { 1, 1 };

    printf("--- D-Optimal Search Engine (Gap-Reparameterized v4) ---\n");
    printf("Framework: [%s] | Mode: [%s] | Seed: %.0f\n", evalMeth, critMode, sd);
    printf("Params: Copula=%.0f, Sigma2=%.2f, Lambda=%.2f\n", cType, s2, lam);
    if (pcl_num_starts < 1) pcl_num_starts = 1;
    if (pcl_num_starts > PCL_MAX_STARTS) pcl_num_starts = PCL_MAX_STARTS;
    printf("Restarts: %d\n", pcl_num_starts);
    printf("Prior coordinates: GAP (alpha_1, log Delta_1, log Delta_2, beta)\n");

    time_t start_t, end_t;
    time(&start_t);

    emxArray_real_T *optX;
    double optCrit;
    emxInitArray_real_T(&optX, 2);

    /* 6. Run Search (num_starts passed as the final input argument) */
    DesignWizardVn_App_GapPrimary(m, n_arr, K, pMean, pCov, 1000.0,
        "quadrature", qSz, wpL_data, wpL_size, spL_data, spL_size, mTerms,
        evalMeth, pSz, critMode, cSz, cType,
        s2_data, s2_sz, lam_data, lam_sz, sd,
        (double)pcl_num_starts,
        optX, &optCrit);

    time(&end_t);
    double diff_t = difftime(end_t, start_t);

    export_to_csv("OptimalDesign_Output.csv", optX);
    printf("Complete! Time: %.2f seconds | D-Criterion: %f\n", diff_t, optCrit);

    DesignWizardVn_App_GapPrimary_terminate();
    return 0;
}
