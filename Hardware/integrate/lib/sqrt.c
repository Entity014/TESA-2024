/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * sqrt.c
 *
 * Code generation for function 'sqrt'
 *
 */

/* Include files */
#include "sqrt.h"
#include "audioProcessingFunction_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void b_sqrt(emxArray_real_T *x)
{
  double *x_data;
  int k;
  int nx;
  x_data = x->data;
  nx = x->size[0];
  for (k = 0; k < nx; k++) {
    x_data[k] = sqrt(x_data[k]);
  }
}

/* End of code generation (sqrt.c) */
