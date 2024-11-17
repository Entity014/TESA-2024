#include "sound_app.h"

pthread_mutex_t data_cond_mutex;
pthread_cond_t data_cond;
short shared_buf[9600];

int main(int argc, char *argv[]) {
    pthread_t alsa_thr, detect_thr, mqtt_thread, autosend_thr, sub_mqtt_thr, xx_thr;
    
    pthread_create(&alsa_thr, NULL, alsa_thr_fcn, NULL);
    pthread_create(&detect_thr, NULL, detect_thr_fcn, NULL);
    pthread_create(&mqtt_thread, NULL, mqtt_thr_fcn, NULL);
    // pthread_create(&autosend_thr, NULL, autosend_thr, NULL);
    pthread_create(&sub_mqtt_thr, NULL, mqtt_sub_thr_fcn, NULL);
    // pthread_create(&xx_thr, NULL, xx_thr, NULL);

    pthread_join(alsa_thr, NULL);
    pthread_join(detect_thr, NULL);
    pthread_join(mqtt_thread, NULL);
    // pthread_join(autosend_thr,NULL);
    pthread_join(sub_mqtt_thr, NULL);
    // pthread_join(xx_thr,NULL);
}