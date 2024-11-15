#include "sound_app.h"
#include <mosquitto.h>
#include <limits.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>


void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    SF_INFO sfinfo;
    SNDFILE *sf = NULL;
    time_t current_time = time(NULL);
    localtime(&current_time);
    char filename[128];
    strftime(filename, sizeof(filename), "/home/reai_bot/TESA-2024/Hardware/integrate/raw_rec/%Y%m%d_%H%M%S.wav", localtime(&current_time));
    
    sfinfo.samplerate = 48000;
    sfinfo.channels = 1;  // Mono channel
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;  // WAV format, 16-bit PCM
    if(sf){
        sf_writef_double(sf, tmp_buf, 4096);
    }
    if (msg->payloadlen) {
        // Print the received message payload
        printf("Received message: %s\n", (char *)msg->payload);

        // Parse the JSON message
        cJSON *json = cJSON_Parse((char *)msg->payload);
        if (json) {
            // Extract the "hardware" key
            cJSON *hardware_item = cJSON_GetObjectItem(json, "hardware");
            if (json) {
                cJSON *hardware_item = cJSON_GetObjectItem(json, "hardware");
                if (hardware_item && cJSON_IsString(hardware_item) && strcmp(hardware_item->valuestring, "sent") == 0) {
                    // printf("runnnn\n");
                    FILE *fp1 = popen("./wav_to_json", "r");
                    if (fp1) {
                        // Handle fp1 output if needed
                        pclose(fp1);
                    }
                    FILE *fp2 = popen("./autosend", "r");
                    if (fp2) {
                        // Handle fp2 output if needed
                        pclose(fp2);
                    }
                }
                if (hardware_item && cJSON_IsString(hardware_item) && strcmp(hardware_item->valuestring, "sent2") == 0) {
                    // printf("runnnn\n");
                    FILE *fp1 = popen("python3 download_from_mongodb.py", "r");
                    if (fp1) {
                        // Handle fp1 output if needed
                        pclose(fp1);
                    }
                }
                if (hardware_item && cJSON_IsString(hardware_item) && strcmp(hardware_item->valuestring, "true") == 0) {
                    sf = sf_open(filename, SFM_WRITE, &sfinfo);
                    if (!sf) {
                        printf("Error opening WAV file for writing: %s\n", sf_strerror(NULL));
                    }

                }
                if (hardware_item && cJSON_IsString(hardware_item) && strcmp(hardware_item->valuestring, "false") == 0) {
                    if (sf) {
                        sf_close(sf);  // Close the WAV file
                        sf = NULL;
                    }
                }
            } else {
                printf("Received 'hardware' key with a different value or key not found.\n");
            }
            cJSON_Delete(json);
        } else {
            printf("Failed to parse JSON message.\n");
        }
    } else {
        printf("Received empty message.\n");
    }
}


void *mqtt_sub_thr_fcn(void *ptr) {
    const char *BROKER_ADDRESS = "192.168.163.251";
    const int BROKER_PORT = 1883;
    const char *TOPIC = "hardware/sent";
    const char *USERNAME = "password";
    const char *PASSWORD = "username";
    int rc;
    struct mosquitto *mosq = mosquitto_new("mqtt-sub-client", true, NULL);
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
    printf("MQTT subscriber thread connected.\n");

    // Set the message callback function
    mosquitto_message_callback_set(mosq, message_callback);

    // Subscribe to the topic
    rc = mosquitto_subscribe(mosq, NULL, TOPIC, 0);
    if (rc != MOSQ_ERR_SUCCESS) {
        printf("Failed to subscribe to topic. Error Code: %d\n", rc);
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return NULL;
    }

    // Start the loop to receive messages
    while (1) {
        if (mosquitto_loop(mosq, -1, 1) != MOSQ_ERR_SUCCESS) {
            printf("Connection lost. Reconnecting...\n");
            while ((rc = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60)) != MOSQ_ERR_SUCCESS) {
                fprintf(stderr, "Reconnection failed. Error code: %d\n", rc);
                sleep(5); // Wait before retrying
            }
            printf("Reconnected to broker.\n");
            mosquitto_subscribe(mosq, NULL, TOPIC, 0);
        }
    }

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

// int main() {
//     pthread_t mqtt_publish_thread, mqtt_subscribe_thread;

//     // Initialize Mosquitto library
//     mosquitto_lib_init();

//     // Create threads for publishing and subscribing
//     pthread_create(&mqtt_publish_thread, NULL, mqtt_thr_fcn, NULL);
//     pthread_create(&mqtt_subscribe_thread, NULL, mqtt_sub_thr_fcn, NULL);

//     // Wait for threads to finish (optional, if you want to let them run forever, you can join them later)
//     pthread_join(mqtt_publish_thread, NULL);
//     pthread_join(mqtt_subscribe_thread, NULL);

//     return 0;
// }
