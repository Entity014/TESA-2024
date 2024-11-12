#include "iot_app.h"

pthread_mutex_t data_cond_mutex;
pthread_cond_t data_cond;
int shared_value = 0;

int main(int argc, char *argv[]) {
    pthread_t rest_thr, mqtt_thr;

    if (argc < 3) {
        printf("Use iot_app REST_ENDPOINT MQTT_TOPIC\n");
        exit(1);
    }

    // prepare IPC objects
    pthread_mutex_init(&data_cond_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);

    // initialize threads
    pthread_create(&rest_thr, NULL, rest_thr_fcn, (void*)argv[1]);
    pthread_create(&mqtt_thr, NULL, mqtt_thr_fcn, (void*)argv[2]);

    // waiting for all thread terminates
    pthread_join(rest_thr, NULL);
    pthread_join(mqtt_thr, NULL); 
}