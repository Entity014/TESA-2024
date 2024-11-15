#include "sound_app.h"
#include <mosquitto.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include <string.h>

const char *BROKER_ADDRESS = "192.168.163.251";
const int BROKER_PORT = 1883;
const char *TOPIC = "hardware/data";
const char *USERNAME = "password";
const char *PASSWORD = "username";

double gpa = 3.8;
float shared_value = 0.0;


void *mqtt_thr_fcn(void *ptr) {
    // MQTT details
    int rc;
    struct mosquitto *mosq = mosquitto_new("mqtt-thread-client", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Failed to create Mosquitto instance.\n");
        return NULL;
    }
    mosquitto_username_pw_set(mosq, USERNAME, PASSWORD);

    // Connect to the MQTT broker
    while ((rc = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60)) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Client could not connect to broker! Error code: %d\n", rc);
        sleep(5); // Wait before retrying
    }
    printf("MQTT thread connected.\n");

    while (1) {
        
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        pthread_mutex_unlock(&data_cond_mutex);

        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "gpa", shared_value);
        char *json_payload = cJSON_Print(json);

        // Publish JSON message to the MQTT topic
        rc = mosquitto_publish(mosq, NULL, TOPIC, strlen(json_payload), json_payload, 0, false);
        if (rc != MOSQ_ERR_SUCCESS) {
            printf("Failed to publish message. Error Code: %d\n", rc);
        } else {
            printf("Published message: %s\n", json_payload);
        }

        // Free allocated memory
        cJSON_free(json_payload);
        cJSON_Delete(json);

        printf("%f\n", shared_value);

        // Check if connection is still active, if not, try reconnecting
        if (mosquitto_loop(mosq, -1, 1) != MOSQ_ERR_SUCCESS) {
            printf("Connection lost. Reconnecting...\n");
            while ((rc = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60)) != MOSQ_ERR_SUCCESS) {
                fprintf(stderr, "Reconnection failed. Error code: %d\n", rc);
                sleep(5); // Wait before retrying
            }
            printf("Reconnected to broker.\n");
        }
    }

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}