#include "sound_app.h"


#define FILENAME "audio.json"
#define LOGIN_URL "http://192.168.163.251:5000/login"
#define DATA_URL "http://192.168.163.251:5000/data"

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

// Function to send data with the access token
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
        } else {

            const char db_name[] = "/home/reai_bot/TESA-2024/Hardware/integrate/db_helper/autosend.db";
            dbase_init(db_name);
            dbase_append(db_name, record_filename);
            printf("POST request completed successfully.\n");
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

// Mutex and condition variable for synchronization
pthread_mutex_t data_cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_cond = PTHREAD_COND_INITIALIZER;

// Function that will run in the background to periodically send data
void *autosend_thr_fcn(void *ptr) {
    const char *username = "password";
    const char *password = "username";

    // Step 1: Login and get access token
    char *loginResponse = login(LOGIN_URL, username, password);
    if (!loginResponse) {
        fprintf(stderr, "Failed to login.\n");
        return NULL;
    }

    // Parse login response
    cJSON *loginJson = cJSON_Parse(loginResponse);
    const cJSON *accessToken = cJSON_GetObjectItemCaseSensitive(loginJson, "access_token");
    if (!cJSON_IsString(accessToken)) {
        fprintf(stderr, "Failed to retrieve access token.\n");
        free(loginResponse);
        cJSON_Delete(loginJson);
        return NULL;
    }

    printf("Access Token: %s\n", accessToken->valuestring);

    // Loop to send data periodically
    while (1) {
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        pthread_mutex_unlock(&data_cond_mutex);

        // Step 2: Read JSON data from file
        char *json_data = read_file(FILENAME);
        if (!json_data) {
            free(loginResponse);
            cJSON_Delete(loginJson);
            return NULL;
        }

        // Step 3: Send JSON data with the access token
        send_data(DATA_URL, json_data, accessToken->valuestring);
        free(json_data);

        // Sleep for 2 seconds before the next send
        sleep(2);
    }

    // Cleanup
    free(loginResponse);
    cJSON_Delete(loginJson);

    return NULL;
}

// Main function
int main() {
    pthread_t autosend_thread;

    pthread_mutex_lock(&data_cond_mutex);
    pthread_cond_wait(&data_cond, &data_cond_mutex);
    pthread_mutex_unlock(&data_cond_mutex);

    // Create autosend thread
    if (pthread_create(&autosend_thread, NULL, autosend_thr_fcn, NULL) != 0) {
        fprintf(stderr, "Failed to create thread\n");
        return 1;
    }

    // Trigger autosend periodically (for demonstration purposes)
    while (1) {
        sleep(10); // Sleep for 10 seconds before triggering autosend
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_signal(&data_cond); // Signal the autosend thread to send data
        pthread_mutex_unlock(&data_cond_mutex);
    }

    // Join thread before exiting (not really necessary in this infinite loop scenario)
    pthread_join(autosend_thread, NULL);

    return 0;
}
