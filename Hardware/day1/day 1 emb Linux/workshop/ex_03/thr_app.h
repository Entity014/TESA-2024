#ifndef THR_APP_H
#define THR_APP_H

// include files
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

// shared variables
extern pthread_cond_t  data_cond;
extern pthread_mutex_t data_cond_mutex;
extern int shared_data;
extern pthread_cond_t  avg_data_cond;
extern pthread_mutex_t avg_data_cond_mutex;
extern float avg_shared_data;

// function prototypes
void *front_thr_fcn( void *ptr );
void *mid_thr_fcn( void *ptr );
void *end_thr_fcn( void *ptr );

#endif // THR_APP_H