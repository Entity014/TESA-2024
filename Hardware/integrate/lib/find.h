/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * find.h
 *
 * Code generation for function 'find'
 *
 */

#ifndef FIND_H
#define FIND_H

/* Include files */
#include "audioProcessingFunction_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void binary_expand_op(emxArray_int32_T *in1, const emxArray_real_T *in2,
                      const emxArray_real_T *in3);

void eml_find(const emxArray_boolean_T *x, emxArray_int32_T *i);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (find.h) */
