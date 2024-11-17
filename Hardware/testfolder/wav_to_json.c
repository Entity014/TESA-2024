#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cjson/cJSON.h>

// Function to encode binary data to Base64
char *base64_encode(const uint8_t *data, size_t input_length, size_t *output_length) {
    static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const char padding_char = '=';
    *output_length = 4 * ((input_length + 2) / 3);
    char *encoded_data = malloc(*output_length + 1);
    if (encoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = (i > input_length + 1) ? padding_char : encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = (i > input_length) ? padding_char : encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    encoded_data[*output_length] = '\0';
    return encoded_data;
}

int main() {
    const char *file_name = "*.wav";  // Specify the file name here

    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Unable to open file");
        return 1;
    }

    // Read header (skip if unnecessary, just sample code)
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *audio_data = malloc(file_size);
    if (!audio_data) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }
    fread(audio_data, 1, file_size, file);
    fclose(file);

    size_t output_length;
    char *base64_audio = base64_encode(audio_data, file_size, &output_length);
    free(audio_data);

    if (!base64_audio) {
        perror("Base64 encoding failed");
        return 1;
    }

    // JSON structure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "file_name", file_name);  // Add the file name to the JSON
    cJSON_AddStringToObject(root, "audio_data", base64_audio);
    cJSON_AddNumberToObject(root, "sample_rate", 48000);
    cJSON_AddNumberToObject(root, "num_channels", 1);
    cJSON_AddNumberToObject(root, "num_frames", file_size / 2); // Assuming 16-bit audio

    // Convert to JSON string
    char *json_string = cJSON_Print(root);
    if (!json_string) {
        perror("Failed to print JSON");
        free(base64_audio);
        cJSON_Delete(root);
        return 1;
    }

    // Save to file
    FILE *json_file = fopen("audio.json", "w");
    if (!json_file) {
        perror("Unable to open JSON file");
        free(base64_audio);
        cJSON_Delete(root);
        free(json_string);
        return 1;
    }
    fputs(json_string, json_file);
    fclose(json_file);

    // Clean up
    free(base64_audio);
    free(json_string);
    cJSON_Delete(root);

    printf("Audio data encoded and saved as JSON.\n");
    return 0;
}
