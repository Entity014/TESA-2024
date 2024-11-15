#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#define FILENAME "audio.json"

char* read_file(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    // Seek to the end to find file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory for the file content
    char *content = (char *)malloc(file_size + 1);
    if (!content) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read the file into the buffer
    fread(content, 1, file_size, file);
    content[file_size] = '\0'; // Null-terminate the string

    fclose(file);
    return content;
}

int main() {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    // Read the JSON content from the file
    char *json_data = read_file(FILENAME);
    if (!json_data) {
        return 1; // Exit if file reading fails
    }

    // Initialize curl
    curl = curl_easy_init();
    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.163.251:5000/data");

        // Set the HTTP POST method
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Set the headers
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJmcmVzaCI6ZmFsc2UsImlhdCI6MTczMTU4NjA2NSwianRpIjoiNDY4MjgxMDEtNzViYS00ZGZmLTgyYWEtOGE4MTA2MjI4MzhjIiwidHlwZSI6ImFjY2VzcyIsInN1YiI6InBhc3N3b3JkIiwibmJmIjoxNzMxNTg2MDY1LCJjc3JmIjoiMjVkYzBjY2MtNTFlNC00ZjFjLTkyOTgtNzYxNGE4NzA0NTEzIiwiZXhwIjoxNzMxNTg5NjY1fQ.NdoLL7JPfRp6Tv2KAbcIAG2gFhq95swxXhrCuNeIdI0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the data to send
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("POST request completed successfully.\n");
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    // Free allocated memory
    free(json_data);

    return 0;
}
