/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: recognizeDTMF.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 21:44:27
 */

/* Include Files */
#include "recognizeDTMF.h"
#include "abs.h"
#include "fft.h"
#include "find.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static double recognizeDTMF_anonFcn1(const double freq_axis[32768], double f);

/* Function Definitions */
/*
 * Arguments    : const double freq_axis[32768]
 *                double f
 * Return Type  : double
 */
static double recognizeDTMF_anonFcn1(const double freq_axis[32768], double f)
{
  static double varargin_1[32768];
  int k;
  /*  Helper function to find the closest frequency index */
  for (k = 0; k < 32768; k++) {
    varargin_1[k] = fabs(freq_axis[k] - f);
  }
  minimum(varargin_1, &k);
  return k;
}

/*
 * signal: Input audio signal (1xN)
 *  fs: Sampling frequency (Hz)
 *
 * Arguments    : const double b_signal[24001]
 *                double fs
 *                char digit_data[]
 *                int digit_size[2]
 * Return Type  : void
 */
void recognizeDTMF(const double b_signal[24001], double fs, char digit_data[],
                   int digit_size[2])
{
  static creal_T dcv[32768];
  static double low_idx_tmp_workspace_freq_axis[32768];
  static double spectrum[16384];
  static const short iv[4] = {697, 770, 852, 941};
  static const short iv1[4] = {1209, 1336, 1477, 1633};
  static const char dtmf_digits[12] = {'1', '4', '7', '*', '2', '5',
                                       '8', '0', '3', '6', '9', '#'};
  int col_data[4];
  int row_data[4];
  int i;
  int iindx;
  int loop_ub;
  int row_size_idx_1;
  bool y[4];
  /*  Increase FFT resolution by zero-padding */
  /*  Zero padding to next power of 2 for better resolution */
  /*  Compute the magnitude of the FFT spectrum with zero-padding */
  /*  Zero-padded FFT */
  fft(b_signal, dcv);
  b_abs(dcv, low_idx_tmp_workspace_freq_axis);
  memcpy(&spectrum[0], &low_idx_tmp_workspace_freq_axis[0],
         16384U * sizeof(double));
  /*  Only take the positive frequencies */
  /*  Compute spectrum with higher resolution */
  /*  Define the low and high frequencies (Hz) for DTMF */
  /*  Row frequencies */
  /*  Column frequencies */
  /*  Compute the frequency axis (corresponding to the DFT bins) */
  for (iindx = 0; iindx < 32768; iindx++) {
    low_idx_tmp_workspace_freq_axis[iindx] =
        3.0517578125E-5 * (double)iindx * fs;
  }
  double b_spectrum[4];
  /*  Find the closest frequencies in the spectrum for low and high frequencies
   */
  /*  Get the magnitudes of the corresponding frequencies */
  /*  Identify the strongest low and high frequency peaks */
  b_spectrum[0] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 697.0) -
                           1];
  b_spectrum[1] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 770.0) -
                           1];
  b_spectrum[2] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 852.0) -
                           1];
  b_spectrum[3] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 941.0) -
                           1];
  maximum(b_spectrum, &iindx);
  /*  Get the corresponding frequencies */
  /*  DTMF Digit Mapping */
  /*  Map the detected frequencies to a DTMF digit */
  b_spectrum[0] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 1209.0) -
                           1];
  iindx = iv[iindx - 1];
  y[0] = (iindx - 697 < 20);
  b_spectrum[1] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 1336.0) -
                           1];
  y[1] = (fabs((double)iindx - 770.0) < 20.0);
  b_spectrum[2] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 1477.0) -
                           1];
  y[2] = (fabs((double)iindx - 852.0) < 20.0);
  b_spectrum[3] = spectrum[(int)recognizeDTMF_anonFcn1(
                               low_idx_tmp_workspace_freq_axis, 1633.0) -
                           1];
  y[3] = (fabs((double)iindx - 941.0) < 20.0);
  maximum(b_spectrum, &iindx);
  eml_find(y, col_data, digit_size);
  loop_ub = digit_size[1];
  row_size_idx_1 = digit_size[1];
  if (loop_ub - 1 >= 0) {
    memcpy(&row_data[0], &col_data[0], (unsigned int)loop_ub * sizeof(int));
  }
  iindx = iv1[iindx - 1];
  y[0] = (iindx - 1209 < 20);
  y[1] = (fabs((double)iindx - 1336.0) < 20.0);
  y[2] = (fabs((double)iindx - 1477.0) < 20.0);
  y[3] = (fabs((double)iindx - 1633.0) < 20.0);
  eml_find(y, col_data, digit_size);
  if ((row_size_idx_1 != 0) && (digit_size[1] != 0)) {
    digit_size[0] = loop_ub;
    row_size_idx_1 = digit_size[1];
    for (iindx = 0; iindx < row_size_idx_1; iindx++) {
      for (i = 0; i < loop_ub; i++) {
        digit_data[i + loop_ub * iindx] =
            dtmf_digits[(row_data[i] + ((col_data[iindx] - 1) << 2)) - 1];
      }
    }
  } else {
    digit_size[0] = 1;
    digit_size[1] = 0;
  }
}

/*
 * File trailer for recognizeDTMF.c
 *
 * [EOF]
 */
