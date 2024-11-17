/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * abs.c
 *
 * Code generation for function 'abs'
 *
 */

/* Include files */
#include "abs.h"
#include "audioProcessingFunction_emxutil.h"
#include "audioProcessingFunction_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void b_abs(const emxArray_real_T *x, emxArray_real_T *y)
{
  const double *x_data;
  double *y_data;
  int k;
  int nx_tmp;
  x_data = x->data;
  nx_tmp = x->size[0];
  k = y->size[0];
  y->size[0] = x->size[0];
  emxEnsureCapacity_real_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx_tmp; k++) {
    y_data[k] = fabs(x_data[k]);
  }
}

/* End of code generation (abs.c) */
