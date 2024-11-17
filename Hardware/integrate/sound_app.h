#ifndef SOUND_APP_H
#define SOUND_APP_H

// include files
#include "lib/audioProcessingFunction.h"
#include "db_helper/db_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <sndfile.h>



extern pthread_mutex_t data_cond_mutex;
extern pthread_cond_t data_cond;
// extern short shared_buf[4096]; // Assuming shared buffer for audio data





// shared variables
extern pthread_cond_t  data_cond;
extern pthread_mutex_t data_cond_mutex;
extern short shared_buf[];
extern float shared_value;
extern char record_filename[100];
extern double tmp_buf[4096];

extern const char *BROKER_ADDRESS;
extern const int BROKER_PORT;
// extern const char *TOPIC;
extern const char *USERNAME;
extern const char *PASSWORD;

extern time_t start_time;
extern time_t stop_time;
extern bool lastState;
// extern bool state;
// extern bool start;


// function prototypes
void *alsa_thr_fcn( void *ptr );
void *detect_thr_fcn( void *ptr );
void *mqtt_thr_fcn( void *ptr );
// void *autosend_thr_fcn( void *ptr );
void *mqtt_sub_thr_fcn( void *ptr);
// void *xx_thr_fcn( void *ptr );
// void *read_and_process_audio_thr(void *arg);

#endif // SOUND_APP_H