/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 * File: find.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 21:44:27
 */

/* Include Files */
#include "find.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const bool x[4]
 *                int i_data[]
 *                int i_size[2]
 * Return Type  : void
 */
void eml_find(const bool x[4], int i_data[], int i_size[2])
{
  int idx;
  int ii;
  bool exitg1;
  idx = 0;
  i_size[0] = 1;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii < 4)) {
    if (x[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= 4) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (idx < 1) {
    i_size[1] = 0;
  } else {
    i_size[1] = idx;
  }
}

/*
 * File trailer for find.c
 *
 * [EOF]
 */
