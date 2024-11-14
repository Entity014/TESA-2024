#include "sound_app.h"
#include "recognizeDTMF.h"
#include "recognizeDTMF_terminate.h"
#include "rt_nonfinite.h"
#include <limits.h>
#include <stdio.h>
#include <math.h>

// // DTMF frequency t+able for low and high frequencies (rows and columns)
// const double low_freqs[] = {697.0, 770.0, 852.0, 941.0};  // Low frequencies (Hz)
// const double high_freqs[] = {1209.0, 1336.0, 1477.0, 1633.0}; // High frequencies (Hz)
// const char dtmf_digits[4][4] = {
//     {'1', '2', '3', '\0'},   // Row 1: 697 Hz
//     {'4', '5', '6', '\0'},   // Row 2: 770 Hz
//     {'7', '8', '9', '\0'},   // Row 3: 852 Hz
//     {'*', '0', '#', '\0'}    // Row 4: 941 Hz
// };

// // Function to detect DTMF tone and recognize the digit
// char recognize_dtmf(double freq1, double freq2) {
//     int row = -1, col = -1;
    
//     // Find the closest low frequency match for freq1 and freq2
//     for (int i = 0; i < 4; i++) {
//         if (fabs(freq1 - low_freqs[i]) < 30.0) { // Allowing some tolerance for frequency variation
//             row = i;
//         }
//         if (fabs(freq2 - low_freqs[i]) < 30.0) {
//             row = i;
//         }
//     }
    
//     // Find the closest high frequency match for freq1 and freq2
//     for (int i = 0; i < 4; i++) {
//         if (fabs(freq1 - high_freqs[i]) < 30.0) { // Allowing some tolerance for frequency variation
//             col = i;
//         }
//         if (fabs(freq2 - high_freqs[i]) < 30.0) {
//             col = i;
//         }
//     }

//     if (row != -1 && col != -1) {
//         return dtmf_digits[row][col];  // Return the corresponding digit
//     } else {
//         return '\0';  // Invalid frequency pair
//     }
// }

void *fft_thr_fcn(void *ptr) {
    // Setup
    static double tmp_buf[4096];
    static double freq_buf[4096];
    double fs = 48000.0;  // Sampling rate is 48kHz
    int N = 4096;  // FFT size

    while (1) {
        // Loop
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        // Convert raw data to double (normalized)
        for (int i = 0; i < N; i++) {
            tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
        }

        // // Perform FFT and store the result in freq_buf
        // // recognizeDTMF(freq_buf, fs);
        // // printf("%f\n", freq_buf[0]);

        // // Find the top two frequencies (dominant peaks)
        // double top_freqs[2] = {0.0, 0.0};  // To store the two most dominant frequencies
        // double top_magnitudes[2] = {0.0, 0.0};

        // for (int i = 0; i < N / 2; i++) {
        //     double magnitude = freq_buf[i];

        //     // Keep track of the two largest magnitudes and their corresponding frequencies
        //     if (magnitude > top_magnitudes[0]) {
        //         top_magnitudes[1] = top_magnitudes[0];
        //         top_freqs[1] = top_freqs[0];
        //         top_magnitudes[0] = magnitude;
        //         top_freqs[0] = (i * fs) / N;  // Convert bin index to frequency in Hz
        //     } else if (magnitude > top_magnitudes[1]) {
        //         top_magnitudes[1] = magnitude;
        //         top_freqs[1] = (i * fs) / N;  // Convert bin index to frequency in Hz
        //     }
        // }

        // Recognize the digit based on the two top frequencies
        char recognized_digit; 
        int s[2] = {2, 1};
        recognized_digit = recognizeDTMF(tmp_buf, fs);
        printf("%c", recognized_digit);
        if (recognized_digit != '\0') {
            // Print the recognized digit on the same line without a newline
            printf("%c", recognized_digit);
            fflush(stdout);  // Ensure the output is immediately printed
        } else {
            // Optionally print a placeholder (like a space) when no valid digit is recognized
            // printf(" ");  // Print a space to keep the line flowing
            // fflush(stdout);
        }

        pthread_mutex_unlock(&data_cond_mutex);
    }
}
