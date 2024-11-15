/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * find.c
 *
 * Code generation for function 'find'
 *
 */

/* Include files */
#include "find.h"
#include "audioProcessingFunction_emxutil.h"
#include "audioProcessingFunction_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void binary_expand_op(emxArray_int32_T *in1, const emxArray_real_T *in2,
                      const emxArray_real_T *in3)
{
  emxArray_boolean_T *b_in2;
  const double *in2_data;
  const double *in3_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  bool *b_in2_data;
  in3_data = in3->data;
  in2_data = in2->data;
  emxInit_boolean_T(&b_in2);
  if (in3->size[0] == 1) {
    loop_ub = in2->size[0];
  } else {
    loop_ub = in3->size[0];
  }
  i = b_in2->size[0];
  b_in2->size[0] = loop_ub;
  emxEnsureCapacity_boolean_T(b_in2, i);
  b_in2_data = b_in2->data;
  stride_0_0 = (in2->size[0] != 1);
  stride_1_0 = (in3->size[0] != 1);
  for (i = 0; i < loop_ub; i++) {
    b_in2_data[i] = (in2_data[i * stride_0_0] + in3_data[i * stride_1_0] > 0.9);
  }
  eml_find(b_in2, in1);
  emxFree_boolean_T(&b_in2);
}

void eml_find(const emxArray_boolean_T *x, emxArray_int32_T *i)
{
  int idx;
  int ii;
  int nx_tmp;
  int *i_data;
  const bool *x_data;
  bool exitg1;
  x_data = x->data;
  nx_tmp = x->size[0];
  idx = 0;
  ii = i->size[0];
  i->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(i, ii);
  i_data = i->data;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx_tmp - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= nx_tmp) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (x->size[0] == 1) {
    if (idx == 0) {
      i->size[0] = 0;
    }
  } else {
    ii = i->size[0];
    if (idx < 1) {
      i->size[0] = 0;
    } else {
      i->size[0] = idx;
    }
    emxEnsureCapacity_int32_T(i, ii);
  }
}

/* End of code generation (find.c) */
