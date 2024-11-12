#include "thr_app.h"

// private constants
#define DATA_BUF_SZ     4

// private variables
static int data_buf[DATA_BUF_SZ] = {0};
static int data_buf_idx = 0;

// private functions

void *mid_thr_fcn( void *ptr ) {
    time_t now;
    struct tm * timeinfo;

    // setup
    time (&now);
    timeinfo = localtime ( &now );
    printf ( "Thread mid starts at: %s", asctime (timeinfo) );
    while(1) {
        // loop
        /* TBD */(&data_cond_mutex);
        /* TBD */(&data_cond, &data_cond_mutex);
        time (&now);
        timeinfo = localtime ( &now );
        printf ( "Thread mid runs at: %s", asctime (timeinfo) );
        data_buf[data_buf_idx++] = shared_data;
        if (data_buf_idx == DATA_BUF_SZ) {
            data_buf_idx = 0;
            avg_shared_data = 0.0;
            for (int i=0; i < DATA_BUF_SZ; i++) {
                avg_shared_data += data_buf[i];
            }
            avg_shared_data /= DATA_BUF_SZ;
            printf("Report data %.2f\n", avg_shared_data);
            /* TBD */(&avg_data_cond_mutex);
            /* TBD */(&avg_data_cond);
            /* TBD */(&avg_data_cond_mutex);
        }
        /* TBD */(&data_cond_mutex);
    }
}