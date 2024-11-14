/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: recognizeDTMF.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 21:44:27
 */

#ifndef RECOGNIZEDTMF_H
#define RECOGNIZEDTMF_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void recognizeDTMF(const double b_signal[24001], double fs,
                          char digit_data[], int digit_size[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for recognizeDTMF.h
 *
 * [EOF]
 */
