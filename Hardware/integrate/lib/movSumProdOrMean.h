/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * movSumProdOrMean.h
 *
 * Code generation for function 'movSumProdOrMean'
 *
 */

#ifndef MOVSUMPRODORMEAN_H
#define MOVSUMPRODORMEAN_H

/* Include files */
#include "audioProcessingFunction_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void vmovfun(const emxArray_real_T *x, int nx, int kleft, int kright,
             emxArray_real_T *y);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (movSumProdOrMean.h) */
