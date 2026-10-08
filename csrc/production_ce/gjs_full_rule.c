/*
 * Prerelease License - for engineering feedback and testing purposes
 * only. Not for sale.
 * File: gjs_full_rule.c
 *
 * MATLAB Coder version            : 24.1
 * C/C++ source code generated on  : 08-Oct-2026 16:49:52
 */

/* Include Files */
#include "gjs_full_rule.h"
#include "DesignWizardVn_App_GapPrimary_emxutil.h"
#include "DesignWizardVn_App_GapPrimary_rtwutil.h"
#include "DesignWizardVn_App_GapPrimary_types.h"
#include "eig.h"
#include "eye.h"
#include "mldivide.h"
#include "mtimes.h"
#include "norm.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "svd.h"
#include "xgeqrf.h"
#include "xorgqr.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void binary_expand_op_1(emxArray_real_T *in1, const emxArray_real_T *in2,
                               const emxArray_real_T *in3);

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op_1(emxArray_real_T *in1, const emxArray_real_T *in2,
                               const emxArray_real_T *in3)
{
  const double *in2_data;
  const double *in3_data;
  double *in1_data;
  int aux_0_1;
  int aux_1_1;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in3_data = in3->data;
  in2_data = in2->data;
  loop_ub = in2->size[0];
  i = in1->size[0] * in1->size[1];
  in1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  if (in3->size[0] == 1) {
    b_loop_ub = in2->size[1];
  } else {
    b_loop_ub = in3->size[0];
  }
  i = in1->size[0] * in1->size[1];
  in1->size[1] = b_loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->size[0] != 1);
  aux_0_1 = 0;
  aux_1_1 = 0;
  for (i = 0; i < b_loop_ub; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      in1_data[i1 + in1->size[0] * i] =
          in2_data[i1 + in2->size[0] * aux_0_1] * in3_data[aux_1_1];
    }
    aux_1_1 += stride_1_1;
    aux_0_1 += stride_0_1;
  }
}

/*
 * Gotwalt, Jones and Steinberg (2009, Technometrics 51:88-95), Section 3.1, eq.
 * (10): quadrature for E f(Z), Z ~ N(0, I_p). Radial: point at 0 plus nR
 * nonzero radii tau_i = 2 x roots of the generalized Laguerre polynomial with
 * parameter p/2 (Golub-Welsch); weights by matching the chi-square(p) moments
 * E[tau^k], k = 0..nR (Gauss-Radau, exact to degree nR in tau). Spherical:
 * Mysovskikh (1980) extended simplex = p+1 simplex vertices, the p(p+1)/2
 * normalized edge midpoints, and all negatives ((p+1)(p+2) points); weights
 * p(7-p)/(2(p+1)^2(p+2)) (vertices) and 2(p-1)^2/(p(p+1)^2(p+2)) (midpoints).
 *     Vertex weights are negative for p > 7, as in the paper's formula.
 *   nQ random orthogonal rotations per radius, averaged. nR = 2 integrates any
 * quintic. Rotations use a self-contained Park-Miller generator (deterministic,
 * codegen-safe, does not touch the global RNG stream). Checked against the
 * paper's Table 1.
 *
 * Arguments    : double p
 *                double nR
 *                double nQ
 *                emxArray_real_T *Z
 *                emxArray_real_T *w
 * Return Type  : void
 */
void gjs_full_rule(double p, double nR, double nQ, emxArray_real_T *Z,
                   emxArray_real_T *w)
{
  emxArray_int32_T *r;
  emxArray_int32_T *r1;
  emxArray_int8_T *c_b;
  emxArray_real_T *E;
  emxArray_real_T *G;
  emxArray_real_T *M;
  emxArray_real_T *P;
  emxArray_real_T *U;
  emxArray_real_T *V;
  emxArray_real_T *a__2;
  emxArray_real_T *dg;
  emxArray_real_T *v;
  emxArray_real_T *wS;
  creal_T tmp_data[9];
  double A_data[100];
  double J_data[81];
  double mo_data[10];
  double tau_data[10];
  double a;
  double b;
  double c;
  double nm;
  double s;
  double *E_data;
  double *M_data;
  double *P_data;
  double *Z_data;
  double *a__2_data;
  double *dg_data;
  double *wS_data;
  double *w_data;
  int A_size[2];
  int J_size[2];
  int aoffset;
  int b_i;
  int b_input_sizes_idx_0;
  int c_i;
  int c_input_sizes_idx_0;
  int i;
  int i1;
  int i2;
  int input_sizes_idx_0;
  int j;
  int loop_ub;
  int loop_ub_tmp;
  int m;
  int mc;
  int q;
  int sizes_idx_0;
  int *r2;
  signed char *b_data;
  bool b_b;
  bool empty_non_axis_sizes;
  a = p / 2.0;
  i = (int)nR;
  J_size[0] = (int)nR;
  J_size[1] = (int)nR;
  loop_ub = (int)nR * (int)nR;
  if (loop_ub - 1 >= 0) {
    memset(&J_data[0], 0, (unsigned int)loop_ub * sizeof(double));
  }
  for (sizes_idx_0 = 0; sizes_idx_0 < i; sizes_idx_0++) {
    J_data[sizes_idx_0 + (int)nR * sizes_idx_0] =
        (2.0 * (((double)sizes_idx_0 + 1.0) - 1.0) + a) + 1.0;
  }
  i1 = (int)(nR - 1.0);
  for (sizes_idx_0 = 0; sizes_idx_0 < i1; sizes_idx_0++) {
    b = sqrt(((double)sizes_idx_0 + 1.0) * (((double)sizes_idx_0 + 1.0) + a));
    J_data[sizes_idx_0 + (int)nR * (sizes_idx_0 + 1)] = b;
    J_data[(sizes_idx_0 + (int)nR * sizes_idx_0) + 1] = b;
  }
  m = eig(J_data, J_size, tmp_data);
  emxInit_real_T(&dg, 1);
  i1 = dg->size[0];
  dg->size[0] = m;
  emxEnsureCapacity_real_T(dg, i1);
  dg_data = dg->data;
  for (i1 = 0; i1 < m; i1++) {
    dg_data[i1] = tmp_data[i1].re;
  }
  sort(dg);
  dg_data = dg->data;
  tau_data[0] = 0.0;
  loop_ub = dg->size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    tau_data[i1 + 1] = 2.0 * dg_data[i1];
  }
  loop_ub_tmp = (int)(nR + 1.0);
  A_size[0] = (int)(nR + 1.0);
  A_size[1] = (int)(nR + 1.0);
  loop_ub = (int)(nR + 1.0) * (int)(nR + 1.0);
  if (loop_ub - 1 >= 0) {
    memset(&A_data[0], 0, (unsigned int)loop_ub * sizeof(double));
  }
  m = (int)(nR + 1.0);
  for (sizes_idx_0 = 0; sizes_idx_0 < loop_ub_tmp; sizes_idx_0++) {
    mo_data[sizes_idx_0] = 0.0;
    for (i1 = 0; i1 < loop_ub_tmp; i1++) {
      A_data[sizes_idx_0 + (int)(nR + 1.0) * i1] =
          rt_powd_snf(tau_data[i1], sizes_idx_0);
    }
    b = 1.0;
    for (j = 0; j < sizes_idx_0; j++) {
      b *= p + 2.0 * (double)j;
    }
    mo_data[sizes_idx_0] = b;
  }
  A_data[0] = 1.0;
  /*  0^0 */
  b_mldivide(A_data, A_size, mo_data, &m);
  emxInit_real_T(&E, 2);
  eye(p + 1.0, E);
  E_data = E->data;
  loop_ub = E->size[0] * E->size[1];
  b = 1.0 / (p + 1.0);
  for (i1 = 0; i1 < loop_ub; i1++) {
    E_data[i1] -= b;
  }
  emxInit_real_T(&a__2, 2);
  emxInit_real_T(&M, 2);
  emxInit_real_T(&U, 2);
  svd(E, U, M, a__2);
  M_data = U->data;
  if (p < 1.0) {
    loop_ub = 0;
  } else {
    loop_ub = (int)p;
  }
  m = U->size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    for (i2 = 0; i2 < m; i2++) {
      M_data[i2 + m * i1] = M_data[i2 + U->size[0] * i1];
    }
  }
  i1 = U->size[0] * U->size[1];
  U->size[1] = loop_ub;
  emxEnsureCapacity_real_T(U, i1);
  emxInit_real_T(&V, 2);
  mtimes(E, U, V);
  dg_data = V->data;
  i1 = (int)(p + 1.0);
  emxInit_real_T(&v, 2);
  for (b_i = 0; b_i < i1; b_i++) {
    i2 = v->size[0] * v->size[1];
    v->size[0] = 1;
    loop_ub_tmp = V->size[1];
    v->size[1] = V->size[1];
    emxEnsureCapacity_real_T(v, i2);
    E_data = v->data;
    for (i2 = 0; i2 < loop_ub_tmp; i2++) {
      E_data[i2] = dg_data[b_i + V->size[0] * i2];
    }
    b = b_norm(v);
    for (i2 = 0; i2 < loop_ub_tmp; i2++) {
      dg_data[b_i + V->size[0] * i2] /= b;
    }
  }
  nm = p * (p + 1.0) / 2.0;
  i1 = M->size[0] * M->size[1];
  M->size[0] = (int)nm;
  loop_ub_tmp = (int)p;
  M->size[1] = (int)p;
  emxEnsureCapacity_real_T(M, i1);
  M_data = M->data;
  m = (int)nm * (int)p;
  for (i1 = 0; i1 < m; i1++) {
    M_data[i1] = 0.0;
  }
  c = 0.0;
  for (b_i = 0; b_i < loop_ub_tmp; b_i++) {
    i1 = (int)p - b_i;
    for (j = 0; j < i1; j++) {
      c++;
      c_i = b_i + j;
      i2 = v->size[0] * v->size[1];
      v->size[0] = 1;
      input_sizes_idx_0 = V->size[1];
      v->size[1] = V->size[1];
      emxEnsureCapacity_real_T(v, i2);
      E_data = v->data;
      for (i2 = 0; i2 < input_sizes_idx_0; i2++) {
        E_data[i2] = dg_data[b_i + V->size[0] * i2] +
                     dg_data[(c_i + V->size[0] * i2) + 1];
      }
      b = b_norm(v);
      for (i2 = 0; i2 < loop_ub_tmp; i2++) {
        M_data[((int)c + M->size[0] * i2) - 1] = E_data[i2] / b;
      }
    }
  }
  i1 = a__2->size[0] * a__2->size[1];
  a__2->size[0] = V->size[0];
  a__2->size[1] = V->size[1];
  emxEnsureCapacity_real_T(a__2, i1);
  a__2_data = a__2->data;
  loop_ub = V->size[0] * V->size[1];
  for (i1 = 0; i1 < loop_ub; i1++) {
    a__2_data[i1] = -dg_data[i1];
  }
  i1 = E->size[0] * E->size[1];
  E->size[0] = (int)nm;
  E->size[1] = (int)p;
  emxEnsureCapacity_real_T(E, i1);
  E_data = E->data;
  for (i1 = 0; i1 < m; i1++) {
    E_data[i1] = -M_data[i1];
  }
  b_b = ((V->size[0] != 0) && (V->size[1] != 0));
  if (b_b) {
    m = V->size[1];
  } else {
    empty_non_axis_sizes = ((M->size[0] != 0) && (M->size[1] != 0));
    if (empty_non_axis_sizes) {
      m = (int)p;
    } else {
      m = V->size[1];
      if (a__2->size[1] > V->size[1]) {
        m = a__2->size[1];
      }
      if (M->size[1] > m) {
        m = M->size[1];
      }
      if (E->size[1] > m) {
        m = (int)p;
      }
    }
  }
  empty_non_axis_sizes = (m == 0);
  if (empty_non_axis_sizes || b_b) {
    input_sizes_idx_0 = V->size[0];
  } else {
    input_sizes_idx_0 = 0;
  }
  if (empty_non_axis_sizes || b_b) {
    b_input_sizes_idx_0 = a__2->size[0];
  } else {
    b_input_sizes_idx_0 = 0;
  }
  if (empty_non_axis_sizes || ((M->size[0] != 0) && (M->size[1] != 0))) {
    c_input_sizes_idx_0 = M->size[0];
  } else {
    c_input_sizes_idx_0 = 0;
  }
  if (empty_non_axis_sizes || ((E->size[0] != 0) && (E->size[1] != 0))) {
    sizes_idx_0 = (int)nm;
  } else {
    sizes_idx_0 = 0;
  }
  aoffset = input_sizes_idx_0;
  emxInit_real_T(&P, 2);
  mc = ((aoffset + b_input_sizes_idx_0) + c_input_sizes_idx_0) + sizes_idx_0;
  i1 = P->size[0] * P->size[1];
  P->size[0] = mc;
  P->size[1] = m;
  emxEnsureCapacity_real_T(P, i1);
  P_data = P->data;
  for (i1 = 0; i1 < m; i1++) {
    for (i2 = 0; i2 < aoffset; i2++) {
      P_data[i2 + P->size[0] * i1] = dg_data[i2 + aoffset * i1];
    }
    for (i2 = 0; i2 < b_input_sizes_idx_0; i2++) {
      P_data[(i2 + aoffset) + P->size[0] * i1] =
          a__2_data[i2 + b_input_sizes_idx_0 * i1];
    }
    for (i2 = 0; i2 < c_input_sizes_idx_0; i2++) {
      P_data[((i2 + aoffset) + b_input_sizes_idx_0) + P->size[0] * i1] =
          M_data[i2 + c_input_sizes_idx_0 * i1];
    }
    for (i2 = 0; i2 < sizes_idx_0; i2++) {
      P_data[(((i2 + aoffset) + b_input_sizes_idx_0) + c_input_sizes_idx_0) +
             P->size[0] * i1] = E_data[i2 + sizes_idx_0 * i1];
    }
  }
  b = (p + 1.0) * (p + 1.0);
  a = p * (7.0 - p) / (2.0 * b * (p + 2.0));
  b = 2.0 * ((p - 1.0) * (p - 1.0)) / (p * b * (p + 2.0));
  loop_ub = (int)(2.0 * (p + 1.0));
  i1 = dg->size[0];
  dg->size[0] = loop_ub;
  emxEnsureCapacity_real_T(dg, i1);
  dg_data = dg->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    dg_data[i1] = 1.0;
  }
  emxInit_int8_T(&c_b);
  m = (int)(2.0 * nm);
  i1 = c_b->size[0];
  c_b->size[0] = m;
  emxEnsureCapacity_int8_T(c_b, i1);
  b_data = c_b->data;
  for (i1 = 0; i1 < m; i1++) {
    b_data[i1] = 1;
  }
  emxInit_real_T(&wS, 1);
  i1 = wS->size[0];
  wS->size[0] = dg->size[0] + c_b->size[0];
  emxFree_int8_T(&c_b);
  emxEnsureCapacity_real_T(wS, i1);
  wS_data = wS->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    wS_data[i1] = a;
  }
  for (i1 = 0; i1 < m; i1++) {
    wS_data[i1 + dg->size[0]] = b;
  }
  loop_ub = (int)(nR * nQ * (double)P->size[0] + 1.0);
  i1 = Z->size[0] * Z->size[1];
  Z->size[0] = loop_ub;
  Z->size[1] = (int)p;
  emxEnsureCapacity_real_T(Z, i1);
  Z_data = Z->data;
  m = loop_ub * (int)p;
  for (i1 = 0; i1 < m; i1++) {
    Z_data[i1] = 0.0;
  }
  i1 = w->size[0];
  w->size[0] = loop_ub;
  emxEnsureCapacity_real_T(w, i1);
  w_data = w->data;
  for (i1 = 0; i1 < loop_ub; i1++) {
    w_data[i1] = 0.0;
  }
  w_data[0] = mo_data[0];
  c = 1.0;
  s = 8.675309E+6;
  /*  Park-Miller state */
  i1 = (int)nQ;
  emxInit_real_T(&G, 2);
  emxInit_int32_T(&r, 1);
  emxInit_int32_T(&r1, 2);
  for (b_i = 0; b_i < i; b_i++) {
    for (q = 0; q < i1; q++) {
      i2 = G->size[0] * G->size[1];
      G->size[0] = (int)p;
      G->size[1] = (int)p;
      emxEnsureCapacity_real_T(G, i2);
      dg_data = G->data;
      for (input_sizes_idx_0 = 0; input_sizes_idx_0 < loop_ub_tmp;
           input_sizes_idx_0++) {
        for (b_input_sizes_idx_0 = 0; b_input_sizes_idx_0 < loop_ub_tmp;
             b_input_sizes_idx_0++) {
          b = 16807.0 * s;
          if (rtIsNaN(b)) {
            s = rtNaN;
          } else if (b == 0.0) {
            s = 0.0;
          } else {
            s = fmod(b, 2.147483647E+9);
            if ((!(s == 0.0)) && (b < 0.0)) {
              s += 2.147483647E+9;
            }
          }
          b = s / 2.147483647E+9;
          nm = 16807.0 * s;
          if (rtIsNaN(nm)) {
            s = rtNaN;
          } else if (nm == 0.0) {
            s = 0.0;
          } else {
            s = fmod(nm, 2.147483647E+9);
            if ((!(s == 0.0)) && (nm < 0.0)) {
              s += 2.147483647E+9;
            }
          }
          b = log(b);
          dg_data[input_sizes_idx_0 + G->size[0] * b_input_sizes_idx_0] =
              sqrt(-2.0 * b) * cos(6.2831853071795862 * (s / 2.147483647E+9));
        }
      }
      m = G->size[0] - 1;
      i2 = U->size[0] * U->size[1];
      U->size[0] = (int)p;
      U->size[1] = (int)p;
      emxEnsureCapacity_real_T(U, i2);
      M_data = U->data;
      i2 = a__2->size[0] * a__2->size[1];
      a__2->size[0] = (int)p;
      a__2->size[1] = (int)p;
      emxEnsureCapacity_real_T(a__2, i2);
      a__2_data = a__2->data;
      if (G->size[0] > G->size[1]) {
        for (j = 0; j < loop_ub_tmp; j++) {
          for (c_i = 0; c_i <= m; c_i++) {
            M_data[c_i + U->size[0] * j] = dg_data[c_i + G->size[0] * j];
          }
        }
        i2 = G->size[1] + 1;
        for (j = i2; j <= m + 1; j++) {
          for (c_i = 0; c_i <= m; c_i++) {
            M_data[c_i + U->size[0] * (j - 1)] = 0.0;
          }
        }
        xgeqrf(U, dg);
        M_data = U->data;
        for (j = 0; j < loop_ub_tmp; j++) {
          for (c_i = 0; c_i <= j; c_i++) {
            a__2_data[c_i + a__2->size[0] * j] = M_data[c_i + U->size[0] * j];
          }
          i2 = j + 2;
          for (c_i = i2; c_i <= m + 1; c_i++) {
            a__2_data[(c_i + a__2->size[0] * j) - 1] = 0.0;
          }
        }
        xorgqr(G->size[0], G->size[0], G->size[1], U, G->size[0], dg);
        M_data = U->data;
      } else {
        i2 = E->size[0] * E->size[1];
        E->size[0] = (int)p;
        E->size[1] = (int)p;
        emxEnsureCapacity_real_T(E, i2);
        E_data = E->data;
        loop_ub = G->size[0] * G->size[1];
        for (i2 = 0; i2 < loop_ub; i2++) {
          E_data[i2] = dg_data[i2];
        }
        xgeqrf(E, dg);
        E_data = E->data;
        for (j = 0; j <= m; j++) {
          for (c_i = 0; c_i <= j; c_i++) {
            a__2_data[c_i + a__2->size[0] * j] = E_data[c_i + E->size[0] * j];
          }
          i2 = j + 2;
          for (c_i = i2; c_i <= m + 1; c_i++) {
            a__2_data[(c_i + a__2->size[0] * j) - 1] = 0.0;
          }
        }
        i2 = G->size[0] + 1;
        for (j = i2; j <= loop_ub_tmp; j++) {
          for (c_i = 0; c_i <= m; c_i++) {
            a__2_data[c_i + a__2->size[0] * (j - 1)] =
                E_data[c_i + E->size[0] * (j - 1)];
          }
        }
        xorgqr(G->size[0], G->size[0], G->size[0], E, G->size[0], dg);
        E_data = E->data;
        for (j = 0; j <= m; j++) {
          for (c_i = 0; c_i <= m; c_i++) {
            M_data[c_i + U->size[0] * j] = E_data[c_i + E->size[0] * j];
          }
        }
      }
      if ((a__2->size[0] == 1) && (a__2->size[1] == 1)) {
        i2 = dg->size[0];
        dg->size[0] = 1;
        emxEnsureCapacity_real_T(dg, i2);
        dg_data = dg->data;
        dg_data[0] = a__2_data[0];
      } else {
        m = a__2->size[0];
        input_sizes_idx_0 = a__2->size[1];
        if (m <= input_sizes_idx_0) {
          input_sizes_idx_0 = m;
        }
        if (a__2->size[1] > 0) {
          m = input_sizes_idx_0;
        } else {
          m = 0;
        }
        i2 = dg->size[0];
        dg->size[0] = m;
        emxEnsureCapacity_real_T(dg, i2);
        dg_data = dg->data;
        i2 = m - 1;
        for (sizes_idx_0 = 0; sizes_idx_0 <= i2; sizes_idx_0++) {
          dg_data[sizes_idx_0] =
              a__2_data[sizes_idx_0 + a__2->size[0] * sizes_idx_0];
        }
      }
      m = dg->size[0];
      for (sizes_idx_0 = 0; sizes_idx_0 < m; sizes_idx_0++) {
        if (rtIsNaN(dg_data[sizes_idx_0])) {
          dg_data[sizes_idx_0] = rtNaN;
        } else if (dg_data[sizes_idx_0] < 0.0) {
          dg_data[sizes_idx_0] = -1.0;
        } else {
          dg_data[sizes_idx_0] = (dg_data[sizes_idx_0] > 0.0);
        }
      }
      input_sizes_idx_0 = dg->size[0] - 1;
      for (c_i = 0; c_i <= input_sizes_idx_0; c_i++) {
        if (dg_data[c_i] == 0.0) {
          dg_data[c_i] = 1.0;
        }
      }
      if (mc < 1) {
        v->size[1] = 0;
      } else {
        i2 = v->size[0] * v->size[1];
        v->size[0] = 1;
        v->size[1] = mc;
        emxEnsureCapacity_real_T(v, i2);
        E_data = v->data;
        loop_ub = mc - 1;
        for (i2 = 0; i2 <= loop_ub; i2++) {
          E_data[i2] = (double)i2 + 1.0;
        }
      }
      i2 = v->size[0] * v->size[1];
      v->size[0] = 1;
      emxEnsureCapacity_real_T(v, i2);
      E_data = v->data;
      loop_ub = v->size[1] - 1;
      for (i2 = 0; i2 <= loop_ub; i2++) {
        E_data[i2] += c;
      }
      loop_ub = v->size[1];
      i2 = r->size[0];
      r->size[0] = v->size[1];
      emxEnsureCapacity_int32_T(r, i2);
      r2 = r->data;
      for (i2 = 0; i2 < loop_ub; i2++) {
        r2[i2] = (int)E_data[i2] - 1;
      }
      a = sqrt(tau_data[b_i + 1]);
      if (dg->size[0] == U->size[1]) {
        m = U->size[0];
        i2 = a__2->size[0] * a__2->size[1];
        a__2->size[0] = U->size[0];
        input_sizes_idx_0 = U->size[1];
        a__2->size[1] = U->size[1];
        emxEnsureCapacity_real_T(a__2, i2);
        a__2_data = a__2->data;
        for (i2 = 0; i2 < input_sizes_idx_0; i2++) {
          for (b_input_sizes_idx_0 = 0; b_input_sizes_idx_0 < m;
               b_input_sizes_idx_0++) {
            a__2_data[b_input_sizes_idx_0 + a__2->size[0] * i2] =
                M_data[b_input_sizes_idx_0 + U->size[0] * i2] * dg_data[i2];
          }
        }
      } else {
        binary_expand_op_1(a__2, U, dg);
        a__2_data = a__2->data;
      }
      input_sizes_idx_0 = P->size[1];
      b_input_sizes_idx_0 = a__2->size[0];
      i2 = M->size[0] * M->size[1];
      M->size[0] = mc;
      M->size[1] = a__2->size[0];
      emxEnsureCapacity_real_T(M, i2);
      M_data = M->data;
      for (j = 0; j < b_input_sizes_idx_0; j++) {
        c_input_sizes_idx_0 = j * mc;
        for (c_i = 0; c_i < mc; c_i++) {
          M_data[c_input_sizes_idx_0 + c_i] = 0.0;
        }
        for (sizes_idx_0 = 0; sizes_idx_0 < input_sizes_idx_0; sizes_idx_0++) {
          aoffset = sizes_idx_0 * P->size[0];
          b = a__2_data[sizes_idx_0 * a__2->size[0] + j];
          for (c_i = 0; c_i < mc; c_i++) {
            i2 = c_input_sizes_idx_0 + c_i;
            M_data[i2] += P_data[aoffset + c_i] * b;
          }
        }
      }
      i2 = V->size[0] * V->size[1];
      V->size[0] = mc;
      V->size[1] = a__2->size[0];
      emxEnsureCapacity_real_T(V, i2);
      dg_data = V->data;
      m = M->size[0] * M->size[1];
      for (i2 = 0; i2 < m; i2++) {
        dg_data[i2] = a * M_data[i2];
      }
      sizes_idx_0 = v->size[1];
      for (i2 = 0; i2 < loop_ub_tmp; i2++) {
        for (b_input_sizes_idx_0 = 0; b_input_sizes_idx_0 < loop_ub;
             b_input_sizes_idx_0++) {
          Z_data[r2[b_input_sizes_idx_0] + Z->size[0] * i2] =
              dg_data[b_input_sizes_idx_0 + sizes_idx_0 * i2];
        }
      }
      i2 = r1->size[0] * r1->size[1];
      r1->size[0] = 1;
      r1->size[1] = v->size[1];
      emxEnsureCapacity_int32_T(r1, i2);
      r2 = r1->data;
      for (i2 = 0; i2 < loop_ub; i2++) {
        r2[i2] = (int)E_data[i2];
      }
      for (i2 = 0; i2 < loop_ub; i2++) {
        w_data[r2[i2] - 1] = mo_data[b_i + 1] * wS_data[i2] / nQ;
      }
      c += (double)mc;
    }
  }
  emxFree_int32_T(&r1);
  emxFree_int32_T(&r);
  emxFree_real_T(&U);
  emxFree_real_T(&dg);
  emxFree_real_T(&G);
  emxFree_real_T(&wS);
  emxFree_real_T(&P);
  emxFree_real_T(&v);
  emxFree_real_T(&M);
  emxFree_real_T(&V);
  emxFree_real_T(&a__2);
  emxFree_real_T(&E);
}

/*
 * File trailer for gjs_full_rule.c
 *
 * [EOF]
 */
