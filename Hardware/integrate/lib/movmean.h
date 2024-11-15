/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * movmean.h
 *
 * Code generation for function 'movmean'
 *
 */

#ifndef MOVMEAN_H
#define MOVMEAN_H

/* Include files */
#include "audioProcessingFunction_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void movmean(const emxArray_real_T *x, double k, emxArray_real_T *y);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (movmean.h) */
