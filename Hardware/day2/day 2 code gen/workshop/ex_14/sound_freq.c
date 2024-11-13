/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * sound_freq.c
 *
 * Code generation for function 'sound_freq'
 *
 */

/* Include files */
#include "sound_freq.h"
#include "FFTImplementationCallback.h"
#include "abs.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void sound_freq(const double X[4096], double spectrum[2048])
{
  creal_T yCol[4096];
  double dv[4096];
  c_FFTImplementationCallback_doH(X, yCol);
  b_abs(yCol, dv);
  memcpy(&spectrum[0], &dv[0], 2048U * sizeof(double));
}

/* End of code generation (sound_freq.c) */
