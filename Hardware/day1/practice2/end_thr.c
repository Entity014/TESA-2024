#include "thr_app.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>


void pub_FireBase(const char *memfield, long memory_value) {
    // Initialize a CURL handle
    CURL *curl;
    CURLcode res;

    // Firebase URL and the data to be sent
    const char *url = "https://tesa2024-reai-cmu-manatee-default-rtdb.asia-southeast1.firebasedatabase.app/test.json";

    // Prepare the JSON data dynamically with the memory field and value
    char jsonData[200];  // Sufficient size for the JSON string
    snprintf(jsonData, sizeof(jsonData), "{\"%s\": \"%ld\"}", memfield, memory_value);

    // Initialize the CURL library
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the URL for the PUT request
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the HTTP request method to PUT
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        // Set the request body (the data to send)
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

        // Set the content type to application/json
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
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

void *end_thr_fcn( void *ptr ) {
    time_t now;
    struct tm * timeinfo;

    // setup
    time (&now);
    timeinfo = localtime ( &now );
    printf ( "Thread end starts at: %s", asctime (timeinfo) );
    while(1) {
        // loop

        // const char *memfield = &shared_data;  // Memory field name passed as command line argument (e.g., "MemTotal")


        // Send the memory value to Firebase
        pub_FireBase(shared_data, avg_shared_data);
        pthread_mutex_lock(&avg_data_cond_mutex);
        pthread_cond_wait(&avg_data_cond, &avg_data_cond_mutex);
        pthread_mutex_unlock(&avg_data_cond_mutex);

    }
}