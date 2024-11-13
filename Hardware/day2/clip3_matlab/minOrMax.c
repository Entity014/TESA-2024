/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: minOrMax.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 19:28:48
 */

/* Include Files */
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const double x[4]
 *                int *idx
 * Return Type  : double
 */
double maximum(const double x[4], int *idx)
{
  double ex;
  int k;
  if (!rtIsNaN(x[0])) {
    *idx = 1;
  } else {
    bool exitg1;
    *idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 5)) {
      if (!rtIsNaN(x[k - 1])) {
        *idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (*idx == 0) {
    ex = x[0];
    *idx = 1;
  } else {
    int i;
    ex = x[*idx - 1];
    i = *idx + 1;
    for (k = i; k < 5; k++) {
      double d;
      d = x[k - 1];
      if (ex < d) {
        ex = d;
        *idx = k;
      }
    }
  }
  return ex;
}

/*
 * Arguments    : const double x[8192]
 *                double ex[4]
 *                int idx[4]
 * Return Type  : void
 */
void minimum(const double x[8192], double ex[4], int idx[4])
{
  int i;
  int j;
  idx[0] = 1;
  ex[0] = x[0];
  idx[1] = 1;
  ex[1] = x[1];
  idx[2] = 1;
  ex[2] = x[2];
  idx[3] = 1;
  ex[3] = x[3];
  for (j = 0; j < 2047; j++) {
    for (i = 0; i < 4; i++) {
      double d;
      bool p;
      d = x[i + ((j + 1) << 2)];
      if (rtIsNaN(d)) {
        p = false;
      } else {
        double d1;
        d1 = ex[i];
        if (rtIsNaN(d1)) {
          p = true;
        } else {
          p = (d1 > d);
        }
      }
      if (p) {
        ex[i] = d;
        idx[i] = j + 2;
      }
    }
  }
}

/*
 * File trailer for minOrMax.c
 *
 * [EOF]
 */
