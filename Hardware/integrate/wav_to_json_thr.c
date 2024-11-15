#include "sound_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>


// Mutex and condition variable for thread synchronization
pthread_mutex_t data_cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_cond = PTHREAD_COND_INITIALIZER;
int processing_done = 0;

// Function to encode binary data to Base64
void base64_encode_thr(const uint8_t *data, size_t input_length, char **output_data, size_t *output_length) {
    static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const char padding_char = '=';
    *output_length = 4 * ((input_length + 2) / 3);
    *output_data = malloc(*output_length + 1);
    if (*output_data == NULL) return;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        (*output_data)[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        (*output_data)[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        (*output_data)[j++] = (i > input_length + 1) ? padding_char : encoding_table[(triple >> 1 * 6) & 0x3F];
        (*output_data)[j++] = (i > input_length) ? padding_char : encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    (*output_data)[*output_length] = '\0';
}

void create_json_file(const char *file_name, const char *base64_audio, size_t audio_length) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "file_name", file_name);
    cJSON_AddStringToObject(root, "audio_data", base64_audio);
    cJSON_AddNumberToObject(root, "sample_rate", 48000);
    cJSON_AddNumberToObject(root, "num_channels", 1);
    cJSON_AddNumberToObject(root, "num_frames", audio_length / 2);  // Assuming 16-bit audio

    char *json_string = cJSON_Print(root);
    if (json_string) {
        FILE *json_file = fopen("record/audio.json", "w");
        if (json_file) {
            fputs(json_string, json_file);
            fclose(json_file);
        }
        free(json_string);
    }
    cJSON_Delete(root);
}

void *read_and_process_audio_thr(void *arg) {
    time_t t;
    struct tm *tm_info;
    // char file_name[100];

    // Get the current time
    time(&t);
    tm_info = localtime(&t);

    // Format the time into a file name
    strftime(record_filename, sizeof(record_filename), "record/recording_%Y%m%d_%H%M%S.wav", tm_info);

    // Create the file with current timestamp (simulating "touch")
    FILE *file = fopen(record_filename, "wb");
    if (!file) {
        perror("Unable to create file"); 
        
        return NULL;
    }
    fclose(file);

    // Open the file for reading
    file = fopen(record_filename, "rb");
    if (!file) {
        perror("Unable to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *audio_data = malloc(file_size);
    if (!audio_data) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }
    fread(audio_data, 1, file_size, file);
    fclose(file);

    char *base64_audio;
    size_t base64_length;
    base64_encode_thr(audio_data, file_size, &base64_audio, &base64_length);
    if (base64_audio) {
        create_json_file(record_filename, base64_audio, file_size);
        free(base64_audio);
    }
    free(audio_data);

    // Signal that processing is complete
    pthread_mutex_lock(&data_cond_mutex);
    processing_done = 1;
    pthread_cond_signal(&data_cond);
    pthread_mutex_unlock(&data_cond_mutex);

    return NULL;
}

void *wait_for_processing_thr(void *arg) {
    (void)arg;  // Unused parameter

    pthread_mutex_lock(&data_cond_mutex);
    while (!processing_done) {
        pthread_cond_wait(&data_cond, &data_cond_mutex);
    }
    pthread_mutex_unlock(&data_cond_mutex);

    printf("Audio processing is complete. Signal received.\n");

    return NULL;
}

int main() {
    pthread_t audio_thread, wait_thread;

    pthread_create(&audio_thread, NULL, read_and_process_audio_thr, NULL);
    pthread_create(&wait_thread, NULL, wait_for_processing_thr, NULL);
    

    pthread_join(audio_thread, NULL);
    pthread_join(wait_thread, NULL);

    return 0;
}
