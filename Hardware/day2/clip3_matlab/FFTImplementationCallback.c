/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 21:44:27
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double x[24001]
 *                const double costab[16385]
 *                const double sintab[16385]
 *                creal_T y[32768]
 * Return Type  : void
 */
void c_FFTImplementationCallback_doH(const double x[24001],
                                     const double costab[16385],
                                     const double sintab[16385],
                                     creal_T y[32768])
{
  static creal_T reconVar1[16384];
  static creal_T reconVar2[16384];
  static double hcostab[8192];
  double hsintab[8192];
  double temp2_im;
  double temp2_re;
  double temp_im;
  double temp_im_tmp;
  double temp_re;
  double temp_re_tmp;
  int bitrevIndex[16384];
  int wrapIndex[16384];
  int i;
  int iheight;
  int iy;
  int j;
  int ju;
  int k;
  memset(&y[0], 0, 32768U * sizeof(creal_T));
  for (i = 0; i < 8192; i++) {
    iy = ((i + 1) << 1) - 2;
    hcostab[i] = costab[iy];
    hsintab[i] = sintab[iy];
  }
  for (i = 0; i < 16384; i++) {
    temp2_im = sintab[i];
    reconVar1[i].re = temp2_im + 1.0;
    temp_re = costab[i];
    reconVar1[i].im = -temp_re;
    reconVar2[i].re = 1.0 - temp2_im;
    reconVar2[i].im = temp_re;
    if (i + 1 != 1) {
      wrapIndex[i] = 16385 - i;
    } else {
      wrapIndex[0] = 1;
    }
  }
  ju = 0;
  iy = 1;
  for (k = 0; k < 16383; k++) {
    bool tst;
    bitrevIndex[k] = iy;
    iy = 16384;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju + 1;
  }
  bitrevIndex[16383] = iy;
  for (i = 0; i < 12000; i++) {
    iy = i << 1;
    ju = bitrevIndex[i];
    y[ju - 1].re = x[iy];
    y[ju - 1].im = x[iy + 1];
  }
  y[bitrevIndex[12000] - 1].re = x[24000];
  y[bitrevIndex[12000] - 1].im = 0.0;
  for (i = 0; i <= 16382; i += 2) {
    temp_re_tmp = y[i + 1].re;
    temp2_im = y[i + 1].im;
    temp_im = temp2_im;
    temp_re = y[i].re;
    temp2_re = y[i].im;
    y[i + 1].re = temp_re - temp_re_tmp;
    temp2_im = temp2_re - temp2_im;
    y[i + 1].im = temp2_im;
    y[i].re = temp_re + temp_re_tmp;
    y[i].im = temp2_re + temp_im;
  }
  iy = 2;
  ju = 4;
  k = 4096;
  iheight = 16381;
  while (k > 0) {
    int b_temp_re_tmp;
    int istart;
    for (i = 0; i < iheight; i += ju) {
      b_temp_re_tmp = i + iy;
      temp_re = y[b_temp_re_tmp].re;
      temp_im = y[b_temp_re_tmp].im;
      y[b_temp_re_tmp].re = y[i].re - temp_re;
      y[b_temp_re_tmp].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }
    istart = 1;
    for (j = k; j < 8192; j += k) {
      int ihi;
      temp2_re = hcostab[j];
      temp2_im = hsintab[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        b_temp_re_tmp = i + iy;
        temp_re_tmp = y[b_temp_re_tmp].im;
        temp_im = y[b_temp_re_tmp].re;
        temp_re = temp2_re * temp_im - temp2_im * temp_re_tmp;
        temp_im = temp2_re * temp_re_tmp + temp2_im * temp_im;
        y[b_temp_re_tmp].re = y[i].re - temp_re;
        y[b_temp_re_tmp].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ju;
      }
      istart++;
    }
    k /= 2;
    iy = ju;
    ju += ju;
    iheight -= iy;
  }
  temp_im_tmp = y[0].im;
  temp2_im = y[0].re * reconVar1[0].re;
  temp_re = y[0].re * reconVar1[0].im;
  temp_im = y[0].re * reconVar2[0].re;
  temp2_re = y[0].re * reconVar2[0].im;
  y[0].re = 0.5 * ((temp2_im - y[0].im * reconVar1[0].im) +
                   (temp_im - -y[0].im * reconVar2[0].im));
  y[0].im = 0.5 * ((temp_re + y[0].im * reconVar1[0].re) +
                   (temp2_re + -y[0].im * reconVar2[0].re));
  y[16384].re = 0.5 * ((temp_im - temp_im_tmp * reconVar2[0].im) +
                       (temp2_im - -temp_im_tmp * reconVar1[0].im));
  y[16384].im = 0.5 * ((temp2_re + temp_im_tmp * reconVar2[0].re) +
                       (temp_re + -temp_im_tmp * reconVar1[0].re));
  for (i = 0; i < 8191; i++) {
    double temp2_im_tmp;
    double temp2_re_tmp;
    temp_re_tmp = y[i + 1].re;
    temp_im_tmp = y[i + 1].im;
    iy = wrapIndex[i + 1];
    temp2_re_tmp = y[iy - 1].re;
    temp2_im_tmp = y[iy - 1].im;
    temp2_im = reconVar1[i + 1].im;
    temp_im = reconVar1[i + 1].re;
    temp_re = reconVar2[i + 1].im;
    temp2_re = reconVar2[i + 1].re;
    y[i + 1].re = 0.5 * ((temp_re_tmp * temp_im - temp_im_tmp * temp2_im) +
                         (temp2_re_tmp * temp2_re - -temp2_im_tmp * temp_re));
    y[i + 1].im = 0.5 * ((temp_re_tmp * temp2_im + temp_im_tmp * temp_im) +
                         (temp2_re_tmp * temp_re + -temp2_im_tmp * temp2_re));
    y[i + 16385].re =
        0.5 * ((temp_re_tmp * temp2_re - temp_im_tmp * temp_re) +
               (temp2_re_tmp * temp_im - -temp2_im_tmp * temp2_im));
    y[i + 16385].im =
        0.5 * ((temp_re_tmp * temp_re + temp_im_tmp * temp2_re) +
               (temp2_re_tmp * temp2_im + -temp2_im_tmp * temp_im));
    temp_re = reconVar1[iy - 1].im;
    temp2_re = reconVar1[iy - 1].re;
    temp_im = reconVar2[iy - 1].im;
    temp2_im = reconVar2[iy - 1].re;
    y[iy - 1].re = 0.5 * ((temp2_re_tmp * temp2_re - temp2_im_tmp * temp_re) +
                          (temp_re_tmp * temp2_im - -temp_im_tmp * temp_im));
    y[iy - 1].im = 0.5 * ((temp2_re_tmp * temp_re + temp2_im_tmp * temp2_re) +
                          (temp_re_tmp * temp_im + -temp_im_tmp * temp2_im));
    y[iy + 16383].re =
        0.5 * ((temp2_re_tmp * temp2_im - temp2_im_tmp * temp_im) +
               (temp_re_tmp * temp2_re - -temp_im_tmp * temp_re));
    y[iy + 16383].im =
        0.5 * ((temp2_re_tmp * temp_im + temp2_im_tmp * temp2_im) +
               (temp_re_tmp * temp_re + -temp_im_tmp * temp2_re));
  }
  temp_im_tmp = y[8192].im;
  temp2_im = y[8192].re * reconVar1[8192].re;
  temp_re = y[8192].re * reconVar1[8192].im;
  temp_im = y[8192].re * reconVar2[8192].re;
  temp2_re = y[8192].re * reconVar2[8192].im;
  y[8192].re = 0.5 * ((temp2_im - y[8192].im * reconVar1[8192].im) +
                      (temp_im - -y[8192].im * reconVar2[8192].im));
  y[8192].im = 0.5 * ((temp_re + y[8192].im * reconVar1[8192].re) +
                      (temp2_re + -y[8192].im * reconVar2[8192].re));
  y[24576].re = 0.5 * ((temp_im - temp_im_tmp * reconVar2[8192].im) +
                       (temp2_im - -temp_im_tmp * reconVar1[8192].im));
  y[24576].im = 0.5 * ((temp2_re + temp_im_tmp * reconVar2[8192].re) +
                       (temp_re + -temp_im_tmp * reconVar1[8192].re));
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
