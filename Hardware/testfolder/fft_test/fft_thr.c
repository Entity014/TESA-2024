    #include "sound_app.h"
    #include "sound_freq.h"
    #include <limits.h>
    #include <stdio.h>
    #include <time.h>

    void *fft_thr_fcn(void *ptr) {
        // Setup
        static double tmp_buf[4096];
        static double freq_buf[4096];
        double fs = 48000.0;  // sampling rate is 48khz
        int N = 4096;  // FFT size
        time_t last_sound_time = time(NULL);  // Timestamp of last noise detected
        localtime(&last_sound_time);

        while (1) {
            // Loop
            pthread_mutex_lock(&data_cond_mutex);
            pthread_cond_wait(&data_cond, &data_cond_mutex);
            time_t current_time = time(NULL);
            localtime(&current_time);
            // printf("Start processing\n");

            // Convert raw data to double (normalized)
            for (int i = 0; i < N; i++) {
                tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
            }

            // Perform FFT and store the result in freq_buf
            sound_freq(tmp_buf, freq_buf);

            // Find the top 3 highest magnitudes and their corresponding bin indices
            int top_indices[3] = {-1, -1, -1}; // To store the indices of the top 3 bins
            double top_magnitudes[3] = {0.0, 0.0, 0.0}; // To store the magnitudes of the top 3 bins
            double top_amplitudes[3] = {0.0, 0.0, 0.0}; // To store the amplitudes of the top 3 bins

            for (int i = 0; i < (double) N / 2.0; i++) {
                // Magnitude for the current bin
                double magnitude = freq_buf[i];
        
                double amplitude = magnitude;  // Adjust factor as necessary for your signal type

                // If the current bin's magnitude is greater than the smallest in the top 3
                if (magnitude > top_magnitudes[2]) {
                    // Insert the current bin magnitude and amplitude into the sorted top 3
                    if (magnitude > top_magnitudes[1]) {
                        if (magnitude > top_magnitudes[0]) {
                            // Shift the others down
                            top_magnitudes[2] = top_magnitudes[1];
                            top_indices[2] = top_indices[1];
                            top_amplitudes[2] = top_amplitudes[1];
                            top_magnitudes[1] = top_magnitudes[0];
                            top_indices[1] = top_indices[0];
                            top_amplitudes[1] = top_amplitudes[0];
                            top_magnitudes[0] = magnitude;
                            top_indices[0] = i;
                            top_amplitudes[0] = amplitude;
                        } else {
                            top_magnitudes[2] = top_magnitudes[1];
                            top_indices[2] = top_indices[1];
                            top_amplitudes[2] = top_amplitudes[1];
                            top_magnitudes[1] = magnitude;
                            top_indices[1] = i;
                            top_amplitudes[1] = amplitude;
                        }
                    } else {
                        top_magnitudes[2] = magnitude;
                        top_indices[2] = i;
                        top_amplitudes[2] = amplitude;
                    }
                }
            }

            // Print the top 3 frequencies, their magnitudes, and amplitudes
            if (top_amplitudes[0] > 1){
                printf("\n");
                last_sound_time = time(NULL);
                localtime(&last_sound_time);
                for (int i = 0; i < 3; i++) {
                double freq = (top_indices[i] * fs) / N;  // Convert bin index to frequency in Hz
                printf("Top %d: Frequency: %.2f Hz, Amplitude: %.2f\n", i + 1, freq, top_amplitudes[i]);
                }
            }
            else{
                if(difftime(current_time, last_sound_time) <= 2.5) {
                    printf("\n");
                    for (int i = 0; i < 3; i++) {
                    double freq = (top_indices[i] * fs) / N;  // Convert bin index to frequency in Hz
                    
                    
                    printf("Top %d: Frequency: %.2f Hz, Amplitude: %.2f\n", i + 1, freq, top_amplitudes[i]);
                    }
                }
            }
            pthread_mutex_unlock(&data_cond_mutex);
        }
    }
