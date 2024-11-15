#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define FILENAME "audio.json"
#define LOGIN_URL "http://192.168.21.251:5000/login"
#define DATA_URL "http://192.168.21.251:5000/data"

// Structure to store HTTP response
struct Memory {
    char *response;
    size_t size;
};

// Callback function for curl to write response data
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t totalSize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + totalSize + 1);
    if (ptr == NULL) {
        fprintf(stderr, "Not enough memory to allocate.\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), contents, totalSize);
    mem->size += totalSize;
    mem->response[mem->size] = '\0';

    return totalSize;
}

// Function to perform POST request for login
char *login(const char *url, const char *username, const char *password) {
    CURL *curl;
    CURLcode res;
    struct Memory chunk = { .response = NULL, .size = 0 };
    char postData[256];

    // Create JSON payload
    snprintf(postData, sizeof(postData), "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return chunk.response;
}

// Function to read file content
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

void send_data(const char *url, const char *json_data, const char *access_token) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    time_t now;
    struct tm * timeinfo;


    //setup
    time (&now);
    timeinfo = localtime ( &now );

    // Initialize curl
    curl = curl_easy_init();
    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the HTTP POST method
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Set the headers
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", access_token);
        headers = curl_slist_append(headers, auth_header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the data to send
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

// Main function
int main() {
    const char *username = "password";
    const char *password = "username";

    // Step 1: Login and get access token
    char *loginResponse = login(LOGIN_URL, username, password);
    if (!loginResponse) {
        fprintf(stderr, "Failed to login.\n");
        return 1;
    }

    // Parse login response
    cJSON *loginJson = cJSON_Parse(loginResponse);
    const cJSON *accessToken = cJSON_GetObjectItemCaseSensitive(loginJson, "access_token");
    if (!cJSON_IsString(accessToken)) {
        fprintf(stderr, "Failed to retrieve access token.\n");
        free(loginResponse);
        cJSON_Delete(loginJson);
        return 1;
    }

    printf("Access Token: %s\n", accessToken->valuestring);

    // Step 2: Read JSON data from file
    char *json_data = read_file(FILENAME);
    if (!json_data) {
        free(loginResponse);
        cJSON_Delete(loginJson);
        return 1;
    }

    

    // Step 3: Send JSON data with the access token
    send_data(DATA_URL, json_data, accessToken->valuestring);

    // Cleanup
    free(loginResponse);
    free(json_data);
    cJSON_Delete(loginJson);

    return 0;
}
