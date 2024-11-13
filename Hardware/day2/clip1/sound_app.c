#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <time.h>

typedef int bool;
#define false 0
#define true 1

int main(int argc) {
    int i, err;
    short buf[9600];
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    char hardware[] = "plughw:0,0";
    
    bool have_noise = false;
    bool last_state = false;  // False = silent, True = noise detected
    bool printed_silent_message = false;  // Flag to ensure "Sound silent" is printed once
    time_t last_noise_time = time(NULL);  // Timestamp of last noise detected
	localtime(&last_noise_time);
	bool point_on_start = false;

    // Initialize sound
    if ((err = snd_pcm_open(&capture_handle, hardware, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "cannot open audio device %s (%s)\n", hardware, snd_strerror(err));
        exit(1);
    } else {
        printf("snd_pcm_open OK\n");
    }
    
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_sw_params_alloca(&sw_params);

    unsigned int rate = 48000;
    unsigned int channels = 1;
    snd_pcm_hw_params_any(capture_handle, hw_params);
    snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(capture_handle, hw_params, channels);
    snd_pcm_hw_params(capture_handle, hw_params);
    printf("HW init ok\n");

    snd_pcm_sw_params_current(capture_handle, sw_params);
    snd_pcm_sw_params_set_start_threshold(capture_handle, sw_params, 0);
    snd_pcm_sw_params(capture_handle, sw_params);
    printf("SW init ok\n");

    // Start the audio stream
    if ((err = snd_pcm_prepare(capture_handle)) < 0) {
        fprintf(stderr, "unable to prepare audio stream: %s\n", snd_strerror(err));
        exit(1);
    }

    // Main loop: process audio data
    while (1) {
        // Read audio data into buffer
        if ((err = snd_pcm_readi(capture_handle, buf, 4800)) != 4800) {
            fprintf(stderr, "read from audio interface failed (%s)\n", snd_strerror(err));
            exit(1);
        }
        
        // Calculate average absolute value of the buffer
        float avg_val = 0.0;
        for (int j = 0; j < 4800; j++) {
            avg_val += abs(buf[j]);
        }
        avg_val /= 4800;

        // Get the current time for each iteration
        time_t current_time = time(NULL);
		localtime(&current_time);

        // If sound is detected
        if (avg_val > 20) {
            // Reset silence tracking and print "Sound detected"
            if (!last_state) {  // Only print if the state transitions to "sound detected"
                printf("Sound detected at %02d:%02d:%02d\n",
                       localtime(&current_time)->tm_hour,
                       localtime(&current_time)->tm_min,
                       localtime(&current_time)->tm_sec);
                last_state = true;  // Update last state to "noise detected"
                printed_silent_message = false;  // Reset silent message flag
            }
            last_noise_time = current_time;  // Update the last noise detection timestamp
        } else {
			if (!point_on_start) {
				printf(".\n");
				point_on_start = true;
			}
            // If no noise, check for silence (more than 4 seconds)
            if (last_state && difftime(current_time, last_noise_time) >= 3.5) {
                // Print "Sound silent" only once after 4 seconds of silence
                if (!printed_silent_message) {
                    printf("Sound silent at %02d:%02d:%02d\n",
                           localtime(&current_time)->tm_hour,
                           localtime(&current_time)->tm_min,
                           localtime(&current_time)->tm_sec);
                    printed_silent_message = true;  // Prevent printing again during the same silence period
                }
                last_state = false;  // Update state to silent
            }
        }
    }

    // Close the ALSA capture device
    snd_pcm_close(capture_handle);
    snd_pcm_hw_params_free(hw_params);
    snd_pcm_sw_params_free(sw_params);

    return 0;
}
