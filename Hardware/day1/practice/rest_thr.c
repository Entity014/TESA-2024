#include "iot_app.h"
#include <curl/curl.h>

// private constants
const char base_url[] = "https://tesa2024-reai-cmu-manatee-default-rtdb.asia-southeast1.firebasedatabase.app/%s.json";

// private functions
size_t resp_callback(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    printf("Got %s\n", buffer);

    // Parse JSON
    cJSON *json = cJSON_Parse(buffer);
    cJSON *test_kv = cJSON_GetObjectItem(json, "test");
    int value = cJSON_GetNumberValue(test_kv);
    printf("Parse as %d\n", value);
    cJSON_Delete(json);

    // Trigger
    pthread_mutex_lock(&data_cond_mutex);
    shared_value = value;
    pthread_cond_signal(&data_cond);
    pthread_mutex_unlock(&data_cond_mutex);

    return realsize;
}

void *rest_thr_fcn( void *ptr ) {
    // setup
    CURL *curl_handle;
    CURLcode res;
    char url[100];
 
    printf("Starting REST thread\n");
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_handle = curl_easy_init();
    if (curl_handle == NULL) {
        fprintf(stderr, "Error init curl\n");
        return NULL;
    }
    sprintf(url, base_url, (char*)ptr);
    printf("REST endpoint: %s\n", url);
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER , 1);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST , 1);
    curl_easy_setopt(curl_handle, CURLOPT_CAINFO, "GTS Root R1.crt");
    curl_easy_setopt(curl_handle, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, resp_callback);
    while(1) {
        // loop
        res = curl_easy_perform(curl_handle);
        if (res) {
            fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
        }
        sleep(3);
    }
    curl_easy_cleanup(curl_handle);
}