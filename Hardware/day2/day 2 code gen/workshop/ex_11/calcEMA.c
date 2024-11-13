/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * calcEMA.c
 *
 * Code generation for function 'calcEMA'
 *
 */

/* Include files */
#include "calcEMA.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }
  return y;
}

void calcEMA(const double data[100], unsigned int N, double ema[100])
{
  int alpha;
  unsigned int qY;
  int t;
  /*  https://www.investopedia.com/terms/e/ema.asp */
  qY = N + 1U;
  if (N + 1U < N) {
    qY = MAX_uint32_T;
  }
  alpha = (int)(unsigned int)rt_roundd_snf(2.0 / (double)qY);
  /*  Calculate the smoothing factor */
  memset(&ema[0], 0, 100U * sizeof(double));
  /*  Preallocate EMA array */
  ema[0] = data[0];
  /*  Initialize the first EMA value */
  /*  Calculate EMA using vector operations */
  for (t = 0; t < 99; t++) {
    double d;
    unsigned int q0;
    d = rt_roundd_snf((double)alpha * data[t + 1]);
    if (d < 4.294967296E+9) {
      if (d >= 0.0) {
        q0 = (unsigned int)d;
      } else {
        q0 = 0U;
      }
    } else if (d >= 4.294967296E+9) {
      q0 = MAX_uint32_T;
    } else {
      q0 = 0U;
    }
    qY = 1U - (unsigned int)alpha;
    if (1U - (unsigned int)alpha > 1U) {
      qY = 0U;
    }
    d = rt_roundd_snf((double)qY * ema[t]);
    if (d < 4.294967296E+9) {
      if (d >= 0.0) {
        qY = (unsigned int)d;
      } else {
        qY = 0U;
      }
    } else if (d >= 4.294967296E+9) {
      qY = MAX_uint32_T;
    } else {
      qY = 0U;
    }
    qY += q0;
    if (qY < q0) {
      qY = MAX_uint32_T;
    }
    ema[t + 1] = qY;
  }
}

/* End of code generation (calcEMA.c) */
