#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "MQTTClientExample"
#define TOPIC       "tgr2024/team/REAI_CMU:_Manatee"
#define PAYLOAD     "Hello MQTT"
#define QOS         1
#define TIMEOUT     10000L

void message_arrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    printf("Message received on topic %s: %s\n", (char*)topicName, (char*)message->payload);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, message_arrived, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

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

    // Wait for messages
    while (1) {
        sleep(1);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}
