#include "rest_pub.h"

// Function to publish data to Firebase
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

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <memfield_name>\n", argv[0]);
//         exit(1);
//     }

//     const char *memfield = argv[1];  // Memory field name passed as command line argument (e.g., "MemTotal")

//     // Retrieve the memory value for the given memfield
//     long memory_value = get_memory_value(memfield);

//     // Send the memory value to Firebase
//     pub_FireBase(memfield, memory_value);

//     return 0;
// }
