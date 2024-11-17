#ifndef SOUND_APP_H
#define SOUND_APP_H

// include files
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// shared variables
extern pthread_cond_t  data_cond;
extern pthread_mutex_t data_cond_mutex;
extern short shared_buf[];

// function prototypes
void *alsa_thr_fcn( void *ptr );
void *fft_thr_fcn( void *ptr );

#endif // SOUND_APP_H