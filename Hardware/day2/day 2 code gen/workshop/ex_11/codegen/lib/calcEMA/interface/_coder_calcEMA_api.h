/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * _coder_calcEMA_api.h
 *
 * Code generation for function 'calcEMA'
 *
 */

#ifndef _CODER_CALCEMA_API_H
#define _CODER_CALCEMA_API_H

/* Include files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void calcEMA(real_T data[100], uint32_T N, real_T ema[100]);

void calcEMA_api(const mxArray *const prhs[2], const mxArray **plhs);

void calcEMA_atexit(void);

void calcEMA_initialize(void);

void calcEMA_terminate(void);

void calcEMA_xil_shutdown(void);

void calcEMA_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (_coder_calcEMA_api.h) */
