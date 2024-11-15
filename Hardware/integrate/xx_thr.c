#include "sound_app.h"




void *xx_thr_fcn(void *ptr) {
    

    while (1) {


        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        pthread_mutex_unlock(&data_cond_mutex);
        sleep(2);

     

    }
}
