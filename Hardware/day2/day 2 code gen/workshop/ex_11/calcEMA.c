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
#include <string.h>

/* Function Definitions */
void calcEMA(const double data[100], unsigned int N, double ema[100])
{
  double alpha;
  int t;
  /*  https://www.investopedia.com/terms/e/ema.asp */
  alpha = 2.0 / ((double)N + 1.0);
  /*  Calculate the smoothing factor */
  memset(&ema[0], 0, 100U * sizeof(double));
  /*  Preallocate EMA array */
  ema[0] = data[0];
  /*  Initialize the first EMA value */
  /*  Calculate EMA using vector operations */
  for (t = 0; t < 99; t++) {
    ema[t + 1] = alpha * data[t + 1] + (1.0 - alpha) * ema[t];
  }
}

/* End of code generation (calcEMA.c) */
