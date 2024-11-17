#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include <time.h>

int main()  
{
    int rc;
    struct mosquitto *mosq;

    // Initialize Mosquitto library
    mosquitto_lib_init();
    mosq = mosquitto_new("publisher-test", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Failed to create Mosquitto instance.\n");
        return -1;
    }

    // Set username and password
    const char *username = "password";  // Replace with your actual username
    const char *password = "username";  // Replace with your actual password
    mosquitto_username_pw_set(mosq, username, password);

    // Connect to MQTT broker
    rc = mosquitto_connect(mosq, "192.168.163.251", 1883, 60);
    if (rc != 0) {
        printf("Client could not connect to broker! Error Code: %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    }
    printf("We are now connected to the broker!\n");

    // Example data
    const char *id = "12345";
    const char *fullname = "John Doe";
    const char *major = "Computer Science";
    double gpa = 3.8;
    while (1) {
        // Create JSON object
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "id", id);
        cJSON_AddStringToObject(json, "fullname", fullname);
        cJSON_AddStringToObject(json, "major", major);
        cJSON_AddNumberToObject(json, "gpa", gpa);
        char *json_payload = cJSON_Print(json);

        // Publish JSON message to the MQTT topic
        rc = mosquitto_publish(mosq, NULL, "prediction/loop", strlen(json_payload), json_payload, 0, false);
        if (rc != 0) {
            printf("Failed to publish message. Error Code: %d\n", rc);
        } else {
            printf("Published message: %s\n", json_payload);
        }

        // Free allocated memory
        cJSON_free(json_payload);
        cJSON_Delete(json);

        // Introduce a 2-second delay
        sleep(2);
    }

    // Cleanup (this line will never be reached in the current state)
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
