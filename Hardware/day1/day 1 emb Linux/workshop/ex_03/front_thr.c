#include <readline/readline.h>
#include "thr_app.h"

// private constants

// private variables

// private functions

void *front_thr_fcn( void *ptr ) {
    time_t now;
    struct tm * timeinfo;

    // setup
    time (&now);
    timeinfo = localtime ( &now );
    printf ( "Thread front starts at: %s", asctime (timeinfo) );
    while(1) {
        // loop
        int value = -1;
        char *line = NULL;
        line = readline("Enter a number (^c to end): ");
        if (line) {
            printf("Got %s\n", line);
            value = atoi(line);
            if (value == -1) {
                break;
            }
            pthread_mutex_lock(&data_cond_mutex);
            pthread_cond_signal(&data_cond);
            time (&now);
            timeinfo = localtime ( &now );
            shared_data = value;
            printf("Thread front signals at %s", asctime (timeinfo));
            pthread_mutex_unlock(&data_cond_mutex);
        }
    }
    return 0;
}