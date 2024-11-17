/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * movmean.c
 *
 * Code generation for function 'movmean'
 *
 */

/* Include files */
#include "movmean.h"
#include "audioProcessingFunction_types.h"
#include "movSumProdOrMean.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void movmean(const emxArray_real_T *x, double k, emxArray_real_T *y)
{
  int kleft;
  int kright;
  if (k >= 2.147483647E+9) {
    kleft = x->size[0];
    kright = x->size[0];
  } else {
    double b_y;
    b_y = floor(k / 2.0);
    kleft = (int)b_y;
    kright = (int)b_y;
    if ((int)b_y << 1 == k) {
      kright = (int)b_y - 1;
    }
    if ((int)b_y > x->size[0]) {
      kleft = x->size[0];
    }
    if (kright > x->size[0]) {
      kright = x->size[0];
    }
  }
  vmovfun(x, x->size[0], kleft, kright, y);
}

/* End of code generation (movmean.c) */
