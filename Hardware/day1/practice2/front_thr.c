#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "MQTTClient.h"
#include <readline/readline.h>

// MQTT constants
#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "MQTTClientExample"
#define TOPIC       "tgr2024/team/REAI_CMU:_Manatee"
#define PAYLOAD     "Hello MQTT"
#define QOS         1
#define TIMEOUT     10000L

// Shared data and synchronization
int shared_data = -1; // Data shared between threads
pthread_mutex_t data_cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_cond = PTHREAD_COND_INITIALIZER;

// MQTT client instance
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

// Function to handle messages received via MQTT
void message_arrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    printf("Message received on topic %s: %s\n", topicName, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
}

// MQTT subscription thread function
void* mqtt_thread(void* ptr) {
    int rc;

    // Create MQTT client
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, message_arrived, NULL);

    // Set up MQTT connection options
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect to the MQTT broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        fprintf(stderr, "Failed to connect to broker, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Connected to broker\n");

    // Subscribe to the topic
    if ((rc = MQTTClient_subscribe(client, TOPIC, QOS)) != MQTTCLIENT_SUCCESS) {
        fprintf(stderr, "Failed to subscribe to topic %s, return code %d\n", TOPIC, rc);
        exit(EXIT_FAILURE);
    }

    printf("Subscribed to topic %s\n", TOPIC);

    // Loop to listen for incoming messages
    while (1) {
        sleep(1);
    }

    // Clean up MQTT client
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return NULL;
}

// // User input thread function
// void* front_thr_fcn(void* ptr) {
//     time_t now;
//     struct tm* timeinfo;

//     // Setup and start time logging
//     time(&now);
//     timeinfo = localtime(&now);
//     printf("Thread front starts at: %s", asctime(timeinfo));

//     while (1) {
//         // Get user input
//         char* line = readline("Enter a number (^c to end): ");
//         if (line) {
//             printf("Got %s\n", line);
//             int value = atoi(line);
//             free(line);  // Free the input line

//             // If user inputs -1, exit the loop
//             if (value == -1) {
//                 break;
//             }

//             // Signal the MQTT thread with new shared data
//             pthread_mutex_lock(&data_cond_mutex);
//             shared_data = value;
//             time(&now);
//             timeinfo = localtime(&now);
//             printf("Thread front signals at %s", asctime(timeinfo));
//             pthread_cond_signal(&data_cond); // Signal MQTT thread that new data is available
//             pthread_mutex_unlock(&data_cond_mutex);
//         }
//     }
//     return NULL;
// }

int main() {
    pthread_t mqtt_tid;

    // Create MQTT thread
    if (pthread_create(&mqtt_tid, NULL, mqtt_thread, NULL) != 0) {
        perror("Failed to create MQTT thread");
        exit(EXIT_FAILURE);
    }

    // // Create user input thread
    // if (pthread_create(&input_tid, NULL, front_thr_fcn, NULL) != 0) {
    //     perror("Failed to create input thread");
    //     exit(EXIT_FAILURE);
    // }

    // Wait for threads to finish (this example runs indefinitely until user exits)
    // pthread_join(input_tid, NULL);
    // pthread_join(mqtt_tid, NULL);

    // Cleanup and exit
    // printf("Exiting program\n");
    return 0;
}
