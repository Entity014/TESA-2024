#include "sound_app.h"
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include "db_helper/db_helper.h"
#include <sys/stat.h>

time_t start_time;
time_t last_noise_time;
bool lastState = false;
bool isStart = false;
bool prevState = false;  // Store previous state
bool printed_silent_message = false;

SNDFILE *sf;
SF_INFO sfinfo;
double tmp_buf[4096];

void *detect_thr_fcn(void *ptr) {
    // Setup
    double fs = 48000.0;  // sampling rate is 48kHz
    int N = 4096;  // FFT size
    start_time = time(NULL);  // Timestamp of last noise detected
    last_noise_time = time(NULL);
    lastState = false;
    
    // SNDFILE setup (WAV recording)
    SNDFILE *sndfile = NULL;
    SF_INFO sfinfo;
    bool isRecording = false;
    char filename[64];
    time_t record_start_time;

    // Directory path for audio files
    const char *dir_path = "/home/reai_bot/TESA-2024/Hardware/integrate/record";

    // Create the directory if it doesn't exist
    if (access(dir_path, F_OK) == -1) {
        if (mkdir(dir_path, 0777) == -1) {
            perror("Error creating directory");
            return NULL;
        }
    }

    while (1) {
        // Wait for signal from another thread
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        double avg_val = 0.0;
        // Convert raw data to double (normalized)
        for (int i = 0; i < N; i++) {
            tmp_buf[i] = (double) shared_buf[i] / SHRT_MAX;
        }

        audioProcessingFunction(&isStart, tmp_buf, 48000);

        time_t current_time = time(NULL);
        localtime(&current_time);

        if (isStart) {
            // Start recording when Punch is detected
            if (!prevState) {  // Only print if the state transitions to "sound detected"
                // printf("Punch Start at %02d:%02d:%02d\n",
                //        localtime(&current_time)->tm_hour,
                //        localtime(&current_time)->tm_min,
                //        localtime(&current_time)->tm_sec);

                // Open WAV file to record audio
                char filename[128];
                strftime(filename, sizeof(filename), "/home/reai_bot/TESA-2024/Hardware/integrate/record/%Y%m%d_%H%M%S.wav", localtime(&current_time));
                
                sfinfo.samplerate = 48000;
                sfinfo.channels = 1;  // Mono channel
                sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;  // WAV format, 16-bit PCM
                sf = sf_open(filename, SFM_WRITE, &sfinfo);
                if (!sf) {
                    printf("Error opening WAV file for writing: %s\n", sf_strerror(NULL));
                }
                
                struct stat file_info;
                if (stat(filename, &file_info) == 0) {
                    // If stat is successful, print the file size
                    // printf("File size: %ld bytes\n", file_info.st_size);
                    char file_size_str[64];
                    snprintf(file_size_str, sizeof(file_size_str), "%ld bytes", file_info.st_size);
                    const char db_name[] = "/home/reai_bot/TESA-2024/Hardware/integrate/db_helper/punchStart.db";
                    dbase_init(db_name);
                    dbase_append(db_name, file_size_str);
                } else {
                    // If stat fails (file doesn't exist or an error occurs)
                    perror("Error getting file size");
                }
                
                const char db_name[] = "/home/reai_bot/TESA-2024/Hardware/integrate/db_helper/punchStart.db";
                dbase_init(db_name);
                dbase_append(db_name, "Punch Start");

            

                prevState = true;  // Update last state to "noise detected"
                printed_silent_message = false;  // Reset silent message flag
            }
            last_noise_time = current_time;  // Update the last noise detection timestamp

            // Write audio data to WAV file
            sf_writef_double(sf, tmp_buf, N);
        } else {
            // If no noise, check for silence (more than 4 seconds)
            if (prevState && difftime(current_time, last_noise_time) >= 0.5) {
                // Print "Sound silent" only once after 4 seconds of silence
                if (!printed_silent_message) {
                    // printf("Punch Stop at %02d:%02d:%02d\n",
                    //        localtime(&current_time)->tm_hour,
                    //        localtime(&current_time)->tm_min,
                    //        localtime(&current_time)->tm_sec);

                    // Stop the recording
                    if (sf) {
                        sf_close(sf);  // Close the WAV file
                        sf = NULL;
                    }

                    const char db_name[] = "/home/reai_bot/TESA-2024/Hardware/integrate/db_helper/punchStart.db";
                    dbase_init(db_name);
                    dbase_append(db_name, "Punch Stop");
                    printed_silent_message = true;  // Prevent printing again during the same silence period
                }
                prevState = false;  // Update state to silent
            }
        }

        pthread_mutex_unlock(&data_cond_mutex);
    }
}
