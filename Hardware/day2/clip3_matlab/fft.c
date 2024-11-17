/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: fft.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 21:44:27
 */

/* Include Files */
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double x[24001]
 *                creal_T y[32768]
 * Return Type  : void
 */
void fft(const double x[24001], creal_T y[32768])
{
  static double costab[16385];
  static double sintab[16385];
  static double costab1q[8193];
  int k;
  costab1q[0] = 1.0;
  for (k = 0; k < 4096; k++) {
    costab1q[k + 1] = cos(0.00019174759848570515 * ((double)k + 1.0));
  }
  for (k = 0; k < 4095; k++) {
    costab1q[k + 4097] =
        sin(0.00019174759848570515 * (8192.0 - ((double)k + 4097.0)));
  }
  costab1q[8192] = 0.0;
  costab[0] = 1.0;
  sintab[0] = 0.0;
  for (k = 0; k < 8192; k++) {
    double costab_tmp;
    double sintab_tmp;
    costab_tmp = costab1q[k + 1];
    costab[k + 1] = costab_tmp;
    sintab_tmp = -costab1q[8191 - k];
    sintab[k + 1] = sintab_tmp;
    costab[k + 8193] = sintab_tmp;
    sintab[k + 8193] = -costab_tmp;
  }
  c_FFTImplementationCallback_doH(x, costab, sintab, y);
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */
