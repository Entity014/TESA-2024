#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <alsa/asoundlib.h>

// กำหนด threshold สำหรับการตรวจจับเสียง
#define SOUND_THRESHOLD 1000

void print_current_time() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%H:%M:%S", timeinfo);
    printf("%s\n", buffer);
}

int main(int argc, char *argv[]) {
    int i, err;
    short buf[9600];
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;

    // initialize sound
    if ((err = snd_pcm_open(&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
        exit(1);
    } else {
        printf("snd_pcm_open OK\n");
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_sw_params_alloca(&sw_params);

    unsigned int rate = atoi(argv[2]);
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

    // Start playing sound (simulate a sound for testing)
    printf("Playing sound for detection...\n");
    system("aplay -q /dev/urandom &"); // Play random noise for 3 seconds

    // loop reading sound data
    int sound_detected = 0;
    for (i = 0; i < 1000; ++i) {
        if ((err = snd_pcm_readi(capture_handle, buf, 4800)) != 4800) {
            fprintf(stderr, "read from audio interface failed (%s)\n", snd_strerror(err));
            exit(1);
        }

        float avg_val = 0.0;
        for (int j = 0; j < 4800; j++) {
            avg_val += abs(buf[j]);
        }
        avg_val /= 4800;

        if (avg_val > SOUND_THRESHOLD) {
            // แสดงผลว่าได้ยินเสียง
            if (!sound_detected) {
                sound_detected = 1;
                printf("sound detected at ");
                print_current_time();
            }
        } else {
            // แสดงผลว่าไม่มีเสียง
            if (sound_detected) {
                sound_detected = 0;
                printf("sound silent at ");
                print_current_time();
            }
            // แสดงจุด (.) หากไม่มีเสียง
            printf(".");
        }

        // หน่วงเวลา 1 วินาที ก่อนตรวจจับเสียงใหม่
        usleep(1000000);  // 1 วินาที
    }

    // Stop playing sound after 3-5 seconds
    printf("\nStopping sound...\n");
    system("pkill aplay"); // หยุดการเล่นเสียง

    snd_pcm_close(capture_handle);
    snd_pcm_hw_params_free(hw_params);
    snd_pcm_sw_params_free(sw_params);

    exit(0);
}
