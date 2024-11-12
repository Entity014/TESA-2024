#include <stdio.h>
#include <string.h>
#include <curl/curl.h>


int main() {
    // Initialize a CURL handle
    CURL *curl;
    CURLcode res;

    // Firebase URL and the data to be sent
    const char *url = "https://tesa2024-reai-cmu-manatee-default-rtdb.asia-southeast1.firebasedatabase.app/test.json";
    const char *jsonData = "{\"mem\": \"pha\"}";

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

    return 0;
}
