#ifndef IOT_APP_H
#define IOT_APP_H

// include files
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <cjson/cJSON.h>

// shared variables
extern pthread_mutex_t data_cond_mutex;
extern pthread_cond_t data_cond;
extern int shared_value;

// function prototypes
void *rest_thr_fcn( void *ptr );
void *mqtt_thr_fcn( void *ptr );

#endif // IOT_APP_H