/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: abs.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 21:44:27
 */

/* Include Files */
#include "abs.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const creal_T x[32768]
 *                double y[32768]
 * Return Type  : void
 */
void b_abs(const creal_T x[32768], double y[32768])
{
  int k;
  for (k = 0; k < 32768; k++) {
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

/*
 * File trailer for abs.c
 *
 * [EOF]
 */
