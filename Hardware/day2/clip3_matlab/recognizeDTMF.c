/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: recognizeDTMF.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 19:28:48
 */

/* Include Files */
#include "recognizeDTMF.h"
#include "FFTImplementationCallback.h"
#include "abs.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Compute frequency spectrum using sound_freq
 *
 * Arguments    : const double audioSignal[4096]
 *                double fs
 * Return Type  : char
 */
char recognizeDTMF(const double audioSignal[4096], double fs)
{
  static creal_T yCol[4096];
  static double x[8192];
  static const char keyMap[12] = {'1', '4', '7', '*', '2', '5',
                                  '8', '0', '3', '6', '9', '#'};
  double varargin_1[8192];
  double dv[4096];
  double dv1[2048];
  double spectrum[2048];
  double ex[4];
  double b;
  double d;
  int idx[4];
  int k;
  int x_tmp;
  c_FFTImplementationCallback_doH(audioSignal, yCol);
  b_abs(yCol, dv);
  /*  Define DTMF frequencies */
  /*  Find the closest frequency bins to DTMF frequencies */
  b = fs / 4096.0;
  for (k = 0; k < 2048; k++) {
    spectrum[k] = dv[k];
    d = ((double)k + 1.0) * b;
    dv1[k] = d;
    x_tmp = k << 2;
    x[x_tmp] = d - 697.0;
    x[x_tmp + 1] = d - 770.0;
    x[x_tmp + 2] = d - 852.0;
    x[x_tmp + 3] = d - 941.0;
  }
  for (k = 0; k < 4096; k++) {
    varargin_1[k] = fabs(x[k]);
  }
  minimum(varargin_1, ex, idx);
  for (k = 0; k < 2048; k++) {
    x_tmp = k << 2;
    d = dv1[k];
    x[x_tmp] = d - 1209.0;
    x[x_tmp + 1] = d - 1336.0;
    x[x_tmp + 2] = d - 1477.0;
    x[x_tmp + 3] = d - 1633.0;
  }
  for (k = 0; k < 4096; k++) {
    varargin_1[k] = fabs(x[k]);
  }
  int b_idx[4];
  minimum(varargin_1, ex, b_idx);
  /*  Find peaks corresponding to the low and high frequencies */
  /*  Identify the highest peaks in the spectrum */
  ex[0] = spectrum[idx[0] - 1];
  ex[1] = spectrum[idx[1] - 1];
  ex[2] = spectrum[idx[2] - 1];
  ex[3] = spectrum[idx[3] - 1];
  maximum(ex, &k);
  ex[0] = spectrum[b_idx[0] - 1];
  ex[1] = spectrum[b_idx[1] - 1];
  ex[2] = spectrum[b_idx[2] - 1];
  ex[3] = spectrum[b_idx[3] - 1];
  maximum(ex, &x_tmp);
  /*  Map to the DTMF keypad */
  /*  Output the detected key */
  return keyMap[(k + ((x_tmp - 1) << 2)) - 1];
}

/*
 * File trailer for recognizeDTMF.c
 *
 * [EOF]
 */
