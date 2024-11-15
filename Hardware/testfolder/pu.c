#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Function to read the contents of a file into a buffer
char* read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Find the size of the file
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory to store the file contents
    char *buffer = malloc(filesize + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the file contents into the buffer
    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';  // Null-terminate the string

    fclose(file);
    return buffer;
}

// Function to publish data to Firebase using curl
void pub_FireBase(const char *json_data, const char *firebase_token) {
    CURL *curl;
    CURLcode res;

    // Initialize the CURL library
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL for the POST request
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.163.251:5000/data");

        // Set the HTTP request method to POST
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);

        // Set the request body (the data to send)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Set the content type to application/json
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Add the Authorization header with the Firebase token
        char authHeader[300];
        snprintf(authHeader, sizeof(authHeader), "Authorization: Bearer %s", firebase_token);
        headers = curl_slist_append(headers, authHeader);

        // Set the headers for the request
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Data successfully sent to Firebase!\n");
        }

        // Cleanup the headers
        curl_slist_free_all(headers);

        // Cleanup CURL
        curl_easy_cleanup(curl);
    }

    // Cleanup global CURL library
    curl_global_cleanup();
}

int main() {
    const char *filename = "audio.json";  // Replace with the path to your .json file
    const char *firebase_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJmcmVzaCI6ZmFsc2UsImlhdCI6MTczMTU3ODY4NSwianRpIjoiMzRmMWY0MDQtMjcwOS00N2ZiLThlZGYtOTBkZTlkY2QxN2U2IiwidHlwZSI6ImFjY2VzcyIsInN1YiI6InBhc3N3b3JkIiwibmJmIjoxNzMxNTc4Njg1LCJjc3JmIjoiN2I0ODU4YTctOWZiYi00NmJhLTg0MTItMWE0MDMxMGYwMmY2IiwiZXhwIjoxNzMxNTgyMjg1fQ.eZB317uApKp9Kk4OjVgzlA4vSBiCVeyPySr2VhX2UME";  // Your Firebase token

    // Read the JSON data from the file
    char *json_data = read_file(filename);

    if (json_data) {
        // Send the data to Firebase
        pub_FireBase(json_data, firebase_token);

        // Free the allocated memory for the JSON data
        free(json_data);
    }

    return 0;
}
