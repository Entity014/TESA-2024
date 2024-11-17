#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <cjson/cJSON.h>

// Function to encode binary data to Base64 (same as before)
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

void process_file(const char *file_name, const char *directory_path) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    // Read the entire file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *audio_data = malloc(file_size);
    if (!audio_data) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }
    fread(audio_data, 1, file_size, file);
    fclose(file);

    // Encode to Base64
    size_t output_length;
    char *base64_audio = base64_encode(audio_data, file_size, &output_length);
    free(audio_data);

    if (!base64_audio) {
        perror("Base64 encoding failed");
        return;
    }

    // Extract file name without the directory path
    const char *base_name = strrchr(file_name, '/');
    if (!base_name) {
        base_name = file_name; // No directory path, use full file name
    } else {
        base_name++; // Skip the '/'
    }

    // Create JSON structure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "file_name", base_name);  // Add base name only
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
        return;
    }

    // Save to JSON file in the record directory
    char json_file_name[256];
    snprintf(json_file_name, sizeof(json_file_name), "%s/%s.json", directory_path, base_name); // Save in the same directory
    FILE *json_file = fopen(json_file_name, "w");
    if (!json_file) {
        perror("Unable to open JSON file");
        free(base64_audio);
        cJSON_Delete(root);
        free(json_string);
        return;
    }
    fputs(json_string, json_file);
    fclose(json_file);

    // Clean up
    free(base64_audio);
    free(json_string);
    cJSON_Delete(root);

    printf("Processed and saved: %s\n", json_file_name);
}

int main() {
    const char *directory_path = "raw_rec";
    DIR *dir = opendir(directory_path);
    if (!dir) {
        perror("Unable to open directory");
        return 1;
    }

    struct dirent *entry;
    char file_path[256];
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".wav")) {
            snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);
            process_file(file_path, directory_path);
        }
    }
    closedir(dir);

    printf("All files processed.\n");
    return 0;
}
