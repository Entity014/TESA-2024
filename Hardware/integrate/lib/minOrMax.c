/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * minOrMax.c
 *
 * Code generation for function 'minOrMax'
 *
 */

/* Include files */
#include "minOrMax.h"
#include "audioProcessingFunction_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
double maximum(const emxArray_real_T *x)
{
  const double *x_data;
  double ex;
  int k;
  int last;
  x_data = x->data;
  last = x->size[0];
  if (x->size[0] <= 2) {
    if (x->size[0] == 1) {
      ex = x_data[0];
    } else {
      ex = x_data[x->size[0] - 1];
      if ((!(x_data[0] < ex)) && ((!rtIsNaN(x_data[0])) || rtIsNaN(ex))) {
        ex = x_data[0];
      }
    }
  } else {
    int idx;
    if (!rtIsNaN(x_data[0])) {
      idx = 1;
    } else {
      bool exitg1;
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= last)) {
        if (!rtIsNaN(x_data[k - 1])) {
          idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (idx == 0) {
      ex = x_data[0];
    } else {
      ex = x_data[idx - 1];
      idx++;
      for (k = idx; k <= last; k++) {
        double d;
        d = x_data[k - 1];
        if (ex < d) {
          ex = d;
        }
      }
    }
  }
  return ex;
}

/* End of code generation (minOrMax.c) */
