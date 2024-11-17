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
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void b_abs(const creal_T x[4096], double y[4096])
{
  int k;
  for (k = 0; k < 4096; k++) {
    double a;
    double b;
    a = fabs(x[k].re);
    b = fabs(x[k].im);
    if (a < b) {
      a /= b;
      y[k] = b * sqrt(a * a + 1.0);
    } else if (a > b) {
      b /= a;
      y[k] = a * sqrt(b * b + 1.0);
    } else if (rtIsNaN(b)) {
      y[k] = rtNaN;
    } else {
      y[k] = a * 1.4142135623730951;
    }
  }
}

/* End of code generation (abs.c) */
