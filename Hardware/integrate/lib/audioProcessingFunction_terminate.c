/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * audioProcessingFunction_terminate.c
 *
 * Code generation for function 'audioProcessingFunction_terminate'
 *
 */

/* Include files */
#include "audioProcessingFunction_terminate.h"
#include "audioProcessingFunction_data.h"
#include "rt_nonfinite.h"
#include "omp.h"

/* Function Definitions */
void audioProcessingFunction_terminate(void)
{
  omp_destroy_nest_lock(&audioProcessingFunction_nestLockGlobal);
  isInitialized_audioProcessingFunction = false;
}

/* End of code generation (audioProcessingFunction_terminate.c) */
