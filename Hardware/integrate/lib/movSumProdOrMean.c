/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * movSumProdOrMean.c
 *
 * Code generation for function 'movSumProdOrMean'
 *
 */

/* Include files */
#include "movSumProdOrMean.h"
#include "audioProcessingFunction_emxutil.h"
#include "audioProcessingFunction_types.h"
#include "rt_nonfinite.h"
#include "omp.h"

/* Function Definitions */
void vmovfun(const emxArray_real_T *x, int nx, int kleft, int kright,
             emxArray_real_T *y)
{
  const double *x_data;
  double b_y;
  double bsum;
  double *y_data;
  int b_k;
  int hi;
  int i;
  int iLeft;
  int ib;
  int ipnf;
  int k;
  int lastBlockLength;
  int loop_ub;
  int nblocks;
  int vlen;
  x_data = x->data;
  loop_ub = x->size[0];
  i = y->size[0];
  y->size[0] = x->size[0];
  emxEnsureCapacity_real_T(y, i);
  y_data = y->data;
  for (i = 0; i < loop_ub; i++) {
    y_data[i] = 0.0;
  }
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
        iLeft, ipnf, vlen, b_y, lastBlockLength, nblocks, b_k, ib, bsum, hi)

  for (k = 0; k < nx; k++) {
    if (k + 1 <= kleft) {
      iLeft = 0;
    } else {
      iLeft = k - kleft;
    }
    ipnf = (k + kright) + 1;
    if (ipnf > x->size[0]) {
      ipnf = x->size[0];
    }
    vlen = ipnf - iLeft;
    if ((x->size[0] == 0) || (vlen == 0)) {
      b_y = 0.0;
    } else {
      if (vlen <= 1024) {
        ipnf = vlen;
        lastBlockLength = 0;
        nblocks = 1;
      } else {
        ipnf = 1024;
        nblocks = vlen / 1024;
        lastBlockLength = vlen - (nblocks << 10);
        if (lastBlockLength > 0) {
          nblocks++;
        } else {
          lastBlockLength = 1024;
        }
      }
      b_y = x_data[iLeft];
      for (b_k = 2; b_k <= ipnf; b_k++) {
        b_y += x_data[(iLeft + b_k) - 1];
      }
      for (ib = 2; ib <= nblocks; ib++) {
        ipnf = iLeft + ((ib - 1) << 10);
        bsum = x_data[ipnf];
        if (ib == nblocks) {
          hi = lastBlockLength;
        } else {
          hi = 1024;
        }
        for (b_k = 2; b_k <= hi; b_k++) {
          bsum += x_data[(ipnf + b_k) - 1];
        }
        b_y += bsum;
      }
    }
    y_data[k] = b_y / (double)vlen;
  }
}

/* End of code generation (movSumProdOrMean.c) */
