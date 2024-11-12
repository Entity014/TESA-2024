#include "thr_app.h"

// shared variables
pthread_cond_t  data_cond;
pthread_mutex_t data_cond_mutex;
int shared_data = -1;
pthread_cond_t  avg_data_cond;
pthread_mutex_t avg_data_cond_mutex;
float avg_shared_data = -1;

int main(int argc, char *argv[]) {
    pthread_t front_thr, mid_thr, end_thr;

    // prepare IPC objects
    pthread_mutex_init(&data_cond_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);
    pthread_mutex_init(&avg_data_cond_mutex, NULL);
    pthread_cond_init(&avg_data_cond, NULL);  

    // initialize threads
    pthread_create(&end_thr, NULL, end_thr_fcn, NULL);
    pthread_create(&mid_thr, NULL, mid_thr_fcn, NULL);
    pthread_create(&front_thr, NULL, front_thr_fcn, NULL);

    // waiting for all thread terminates
    pthread_join(front_thr, NULL);
    pthread_join(mid_thr, NULL); 
    pthread_join(end_thr, NULL);
}