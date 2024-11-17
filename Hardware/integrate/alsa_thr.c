#include "sound_app.h"
#include <alsa/asoundlib.h>

#define MAX_DEVICES 5 // Adjust this based on how many devices you want to try

void *alsa_thr_fcn(void *ptr) {
    int err;
    short buf[4096];
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;

    const char* device_names[MAX_DEVICES] = {
        "plughw:0,0", "plughw:1,0", "plughw:2,0", "plughw:3,0", "plughw:4,0"
    };

    // Try opening the audio devices sequentially
    int device_found = 0;
    for (int i = 0; i < MAX_DEVICES; i++) {
        const char* device = device_names[i];
        if ((err = snd_pcm_open(&capture_handle, device, SND_PCM_STREAM_CAPTURE, 0)) >= 0) {
            printf("Successfully opened device: %s\n", device);
            device_found = 1;
            break; // Exit loop on success
        } else {
            fprintf(stderr, "Cannot open audio device %s (%s)\n", device, snd_strerror(err));
        }
    }

    if (!device_found) {
        fprintf(stderr, "No suitable audio device found.\n");
        exit(1); // Exit if no device is found
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
    printf("HW init OK\n");

    snd_pcm_sw_params_current(capture_handle, sw_params);
    snd_pcm_sw_params_set_start_threshold(capture_handle, sw_params, 0);
    snd_pcm_sw_params(capture_handle, sw_params);
    printf("SW init OK\n");

    // Start the audio stream
    if ((err = snd_pcm_prepare(capture_handle)) < 0) {
        fprintf(stderr, "Unable to prepare audio stream: %s\n", snd_strerror(err));
        exit(1);
    }

    while (1) {
        if ((err = snd_pcm_readi(capture_handle, buf, 4096)) != 4096) {
            fprintf(stderr, "Read from audio interface failed (%s)\n", snd_strerror(err));
            break;
        }
        pthread_mutex_lock(&data_cond_mutex);
        memcpy(shared_buf, buf, 4096 * sizeof(short));
        pthread_cond_signal(&data_cond);
        pthread_mutex_unlock(&data_cond_mutex);
    }

    snd_pcm_close(capture_handle);
    snd_pcm_hw_params_free(hw_params);
    snd_pcm_sw_params_free(sw_params);
    return NULL;
}
