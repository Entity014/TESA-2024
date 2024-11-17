#include "thr_app.h"

// private constants

// private variables

// private functions

void *end_thr_fcn( void *ptr ) {
    time_t now;
    struct tm * timeinfo;

    // setup
    time (&now);
    timeinfo = localtime ( &now );
    printf ( "Thread end starts at: %s", asctime (timeinfo) );
    while(1) {
        // loop
        pthread_mutex_lock(&avg_data_cond_mutex);
        pthread_cond_wait(&avg_data_cond, &avg_data_cond_mutex);
        time (&now);
        timeinfo = localtime ( &now );
        printf("Final result: %f at %s\n", avg_shared_data, asctime (timeinfo));
        pthread_mutex_unlock(&avg_data_cond_mutex);
    }
}