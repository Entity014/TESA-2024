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
        /* TBD */(&avg_data_cond_mutex);
        /* TBD */(&avg_data_cond, &avg_data_cond_mutex);
        time (&now);
        timeinfo = localtime ( &now );
        printf("Final result: %f at %s\n", avg_shared_data, asctime (timeinfo));
        /* TBD */(&avg_data_cond_mutex);
    }
}