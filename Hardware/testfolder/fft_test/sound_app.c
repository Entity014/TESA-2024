#include "sound_app.h"
#include "db_helper.h"

pthread_mutex_t data_cond_mutex;
pthread_cond_t data_cond;
short shared_buf[9600];

int main(int argc, char *argv[]) {
    pthread_t alsa_thr, fft_thr;

    // prepare IPC objects
    pthread_mutex_init(&data_cond_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);

    // initialize threads
    pthread_create(&alsa_thr, NULL, alsa_thr_fcn, "plughw:0,0");
    pthread_create(&fft_thr, NULL, fft_thr_fcn, NULL);

    // waiting for all thread terminates
    pthread_join(alsa_thr, NULL);
    pthread_join(fft_thr, NULL); 
}