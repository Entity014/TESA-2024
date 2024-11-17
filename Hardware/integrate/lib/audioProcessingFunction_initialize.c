/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * audioProcessingFunction_initialize.c
 *
 * Code generation for function 'audioProcessingFunction_initialize'
 *
 */

/* Include files */
#include "audioProcessingFunction_initialize.h"
#include "audioProcessingFunction_data.h"
#include "rt_nonfinite.h"
#include "omp.h"

/* Function Definitions */
void audioProcessingFunction_initialize(void)
{
  omp_init_nest_lock(&audioProcessingFunction_nestLockGlobal);
  isInitialized_audioProcessingFunction = true;
}

/* End of code generation (audioProcessingFunction_initialize.c) */
