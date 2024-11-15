/*
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * audioProcessingFunction.c
 *
 * Code generation for function 'audioProcessingFunction'
 *
 */

/* Include files */
#include "audioProcessingFunction.h"
#include "abs.h"
#include "audioProcessingFunction_data.h"
#include "audioProcessingFunction_emxutil.h"
#include "audioProcessingFunction_initialize.h"
#include "audioProcessingFunction_types.h"
#include "find.h"
#include "minOrMax.h"
#include "movmean.h"
#include "rt_nonfinite.h"
#include "sqrt.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

void audioProcessingFunction(bool *isStart, const double audioData[4096], double Fs)
{
  emxArray_boolean_T *b_segmentIndices;
  emxArray_int32_T *r;
  emxArray_real_T *audioBuffer;
  emxArray_real_T *b_audioBuffer;
  emxArray_real_T *b_signal;
  emxArray_real_T *r1;
  emxArray_real_T *segmentIndices;
  emxArray_real_T *squaredSignal_tmp;
  double bufferLength;
  double bufferPosition;
  double longWinLength;
  double maxPeak;
  double shortWinLength;
  double signalPeak;
  double *audioBuffer_data;
  double *segmentIndices_data;
  double *signal_data;
  int audioPositionSinceSync;
  int frameLength;
  int i;
  int i1;
  int loop_ub;
  int *r2;
  bool exitg1;
  bool *b_segmentIndices_data;  


  if (!isInitialized_audioProcessingFunction) {
    audioProcessingFunction_initialize();
  }
  // printf("%d\n",audioData[0]);
  /*  audioData: อาร์เรย์ของข้อมูลเสียงที่ส่งมา */
  /*  Fs: ตัวอย่างอัตราการสุ่มตัวอย่าง (เช่น 48000) */
  /*  ตั้งค่าพารามิเตอร์ */
  frameLength = 256;
  bufferLength = 5.0 * Fs;
  if (fabs(bufferLength) < 4.503599627370496E+15) {
    if (bufferLength >= 0.5) {
      bufferLength = floor(bufferLength + 0.5);
    } else if (bufferLength > -0.5) {
      bufferLength *= 0.0;
    } else {
      bufferLength = ceil(bufferLength - 0.5);
    }
  }
  /*  ความยาวบัฟเฟอร์ */
  emxInit_real_T(&audioBuffer, 2);
  i = audioBuffer->size[0] * audioBuffer->size[1];
  audioBuffer->size[0] = 1;
  audioPositionSinceSync = (int)bufferLength;
  audioBuffer->size[1] = (int)bufferLength;
  emxEnsureCapacity_real_T(audioBuffer, i);
  audioBuffer_data = audioBuffer->data;
  for (i = 0; i < audioPositionSinceSync; i++) {
    audioBuffer_data[i] = 0.0;
  }
  bufferPosition = 1.0;
  audioPositionSinceSync = 1;
  maxPeak = 0.0;
  /*  ค่า threshold สำหรับการตรวจจับสัญญาณ */
  shortWinLength = 0.3 * Fs;
  if (fabs(shortWinLength) < 4.503599627370496E+15) {
    if (shortWinLength >= 0.5) {
      shortWinLength = floor(shortWinLength + 0.5);
    } else if (shortWinLength > -0.5) {
      shortWinLength *= 0.0;
    } else {
      shortWinLength = ceil(shortWinLength - 0.5);
    }
  }
  longWinLength = 2.0 * Fs;
  if (fabs(longWinLength) < 4.503599627370496E+15) {
    if (longWinLength >= 0.5) {
      longWinLength = floor(longWinLength + 0.5);
    } else if (longWinLength > -0.5) {
      longWinLength *= 0.0;
    } else {
      longWinLength = ceil(longWinLength - 0.5);
    }
  }
  /*  รอให้สัญญาณเริ่มต้นที่มีความแรงมากกว่าระดับ -30 dBFS */
  emxInit_real_T(&b_signal, 1);
  exitg1 = false;
  while ((!exitg1) && (audioPositionSinceSync <= 4096)) {
    if (audioPositionSinceSync > audioPositionSinceSync + 255) {
      i = 0;
      i1 = -1;
    } else {
      i = audioPositionSinceSync - 1;
      i1 = audioPositionSinceSync + 254;
    }
    loop_ub = i1 - i;
    i1 = b_signal->size[0];
    b_signal->size[0] = loop_ub + 1;
    emxEnsureCapacity_real_T(b_signal, i1);
    signal_data = b_signal->data;
    for (i1 = 0; i1 <= loop_ub; i1++) {
      signal_data[i1] = audioData[i + i1];
    }

    signalPeak = maximum(b_signal);
    if (signalPeak > 0.031622776601683791) {
      maxPeak = signalPeak;
      exitg1 = true;
    } else {
      audioPositionSinceSync += 256;
    }
  }
  /*  ลูปหลักสำหรับการประมวลผลเสียง */
  emxInit_real_T(&segmentIndices, 1);
  emxInit_real_T(&squaredSignal_tmp, 1);
  emxInit_int32_T(&r);
  emxInit_real_T(&b_audioBuffer, 2);
  emxInit_boolean_T(&b_segmentIndices);
  emxInit_real_T(&r1, 1);
  exitg1 = false;
  while ((!exitg1) && (audioPositionSinceSync <= 4096)) {
    i = (audioPositionSinceSync + frameLength) - 1;
    if (audioPositionSinceSync > i) {
      i1 = 0;
      i = 0;
    } else {
      i1 = audioPositionSinceSync - 1;
    }
    /*  การปรับค่าเสียง */
    frameLength = i - i1;
    i = b_signal->size[0];
    b_signal->size[0] = frameLength;
    emxEnsureCapacity_real_T(b_signal, i);
    signal_data = b_signal->data;
    for (i = 0; i < frameLength; i++) {
      signal_data[i] = audioData[i1 + i];
    }
    b_abs(b_signal, r1);
    signalPeak = maximum(r1);
    if (signalPeak > maxPeak) {
      maxPeak = signalPeak;
    }
    i = b_signal->size[0];
    b_signal->size[0] = frameLength;
    emxEnsureCapacity_real_T(b_signal, i);
    signal_data = b_signal->data;
    for (i = 0; i < frameLength; i++) {
      signal_data[i] = audioData[i1 + i] / maxPeak;
    }
    /*  การจัดการบัฟเฟอร์ */
    signalPeak = (bufferLength - bufferPosition) + 1.0;
    if (signalPeak < b_signal->size[0]) {
      int i2;
      signalPeak = (double)b_signal->size[0] - signalPeak;
      if (signalPeak + 1.0 > bufferPosition - 1.0) {
        i = 0;
        i1 = 0;
      } else {
        i = (int)(signalPeak + 1.0) - 1;
        i1 = (int)(bufferPosition - 1.0);
      }
      i2 = b_audioBuffer->size[0] * b_audioBuffer->size[1];
      b_audioBuffer->size[0] = 1;
      loop_ub = i1 - i;
      b_audioBuffer->size[1] = loop_ub;
      emxEnsureCapacity_real_T(b_audioBuffer, i2);
      segmentIndices_data = b_audioBuffer->data;
      for (i1 = 0; i1 < loop_ub; i1++) {
        segmentIndices_data[i1] = audioBuffer_data[i + i1];
      }
      for (i = 0; i < loop_ub; i++) {
        audioBuffer_data[i] = segmentIndices_data[i];
      }
      bufferPosition -= signalPeak;
    }
    signalPeak = bufferPosition + (double)b_signal->size[0];
    if (bufferPosition > signalPeak - 1.0) {
      i = 0;
      i1 = 0;
    } else {
      i = (int)bufferPosition - 1;
      i1 = (int)(signalPeak - 1.0);
    }
    loop_ub = i1 - i;
    for (i1 = 0; i1 < loop_ub; i1++) {
      audioBuffer_data[i + i1] = signal_data[i1];
    }
    bufferPosition = signalPeak;
    audioPositionSinceSync += b_signal->size[0];
    /*  การตรวจสอบขอบเขตก่อนเข้าถึงข้อมูลใน audioBuffer */
    if ((audioPositionSinceSync + b_signal->size[0]) - 1 > 4096) {
      exitg1 = true;
    } else {
      /*  การตรวจจับเหตุการณ์ในเสียง */
      i = squaredSignal_tmp->size[0];
      squaredSignal_tmp->size[0] = frameLength;
      emxEnsureCapacity_real_T(squaredSignal_tmp, i);
      segmentIndices_data = squaredSignal_tmp->data;
      for (i = 0; i < frameLength; i++) {
        signalPeak = signal_data[i];
        segmentIndices_data[i] = signalPeak * signalPeak;
      }
      /*  กำลังสองของสัญญาณ */
      /*  RMS จาก moving average ของกำลังสอง */
      /*  กำลังสองของสัญญาณ */
      /*  RMS จาก moving average ของกำลังสอง */
      movmean(squaredSignal_tmp, shortWinLength, segmentIndices);
      b_sqrt(segmentIndices);
      segmentIndices_data = segmentIndices->data;
      movmean(squaredSignal_tmp, longWinLength, r1);
      b_sqrt(r1);
      signal_data = r1->data;
      if (segmentIndices->size[0] == r1->size[0]) {
        loop_ub = segmentIndices->size[0];
        i = b_segmentIndices->size[0];
        b_segmentIndices->size[0] = segmentIndices->size[0];
        emxEnsureCapacity_boolean_T(b_segmentIndices, i);
        b_segmentIndices_data = b_segmentIndices->data;
        for (i = 0; i < loop_ub; i++) {
          b_segmentIndices_data[i] =
              (segmentIndices_data[i] + signal_data[i] > 0.95);
        }
        eml_find(b_segmentIndices, r);
        r2 = r->data;
      } else {
        binary_expand_op(r, segmentIndices, r1);
        r2 = r->data;
      }
      loop_ub = r->size[0];
      i = segmentIndices->size[0];
      segmentIndices->size[0] = r->size[0];
      emxEnsureCapacity_real_T(segmentIndices, i);
      segmentIndices_data = segmentIndices->data;
      for (i = 0; i < loop_ub; i++) {
        segmentIndices_data[i] = r2[i];
      }
      /*  หากพบส่วนที่มีเหตุการณ์, ทำการจำแนกและรายงานผล */
      if ((segmentIndices->size[0] != 0) &&
          (maximum(segmentIndices) <= audioBuffer->size[1])) {
        /*  การตรวจสอบขอบเขตก่อนการใช้ segmentIndices */
        signalPeak = segmentIndices_data[segmentIndices->size[0] - 1];
        if (segmentIndices_data[0] > signalPeak) {
          i = 1;
          i1 = 0;
        } else {
          i = (int)segmentIndices_data[0];
          i1 = (int)signalPeak;
        }
        if ((i1 - i) + 1 >= b_signal->size[0]) {
          // start = true;
          // check();
          // printf("Audio Position: %.2f\n", (double)audioPositionSinceSync);
          // printf("Start\n");
          *isStart = true;  // Timestamp of last noise detected
          // localtime(&start_time);
          // fflush(stdout);
        }
      }
      else{
        *isStart = false;
        // printf("Stop\n");
        // *stop_time = time(NULL);
        // printf("%f\n", localtime(&stop_time));
        // localtime(&stop_time);
        // fflush(stdout);
      }
    }
  }
  emxFree_real_T(&r1);
  emxFree_boolean_T(&b_segmentIndices);
  emxFree_real_T(&b_audioBuffer);
  emxFree_int32_T(&r);
  emxFree_real_T(&squaredSignal_tmp);
  emxFree_real_T(&segmentIndices);
  emxFree_real_T(&b_signal);
  emxFree_real_T(&audioBuffer);
}

/* End of code generation (audioProcessingFunction.c) */
