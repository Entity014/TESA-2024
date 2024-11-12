#include "iot_app.h"
#include <string.h>
#include <MQTTClient.h>

// private constants
const char base_topic[] = "tgr2024/team/%s";
const char MQTT_BROKER[] = "tcp://broker.emqx.io:1883";
const char MQTT_CLIENTID[] = "TGR2024_Manjaaaaaaaaa_78520";


void *mqtt_thr_fcn( void *ptr ) {
    // setup 
    char topic[100];
    int rc;

    printf("Starting MQTT thread\n");
    MQTTClient mqtt_client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_deliveryToken token;
    MQTTClient_create(&mqtt_client, MQTT_BROKER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    sprintf(topic, base_topic, (char*)ptr);
    printf("MQTT TOPIC: %s\n", topic);
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    while(1) {
        // loop
        float value;

        // 
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        value = shared_value;
        pthread_mutex_unlock(&data_cond_mutex);
        //
         
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "team", (char*)ptr);
        cJSON_AddNumberToObject(json, "value", value);
        char *json_payload = cJSON_Print(json);
        pubmsg.payload = json_payload;
        pubmsg.payloadlen = strlen(json_payload);
        pubmsg.qos = 0;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(mqtt_client, topic, &pubmsg, &token);
        printf("Publish to %s\n", topic);
        cJSON_free(json);
    }
    MQTTClient_destroy(mqtt_client);
}