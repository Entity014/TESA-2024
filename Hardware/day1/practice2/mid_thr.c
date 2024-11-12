#include "thr_app.h"

// private functions

long get_memory_value(char *field) {
    FILE *fh;
    char buf[5000];
    char *tmp_buf;
    long memory_value = 0;
    char search_field[50];

    // Create the search string like "MemTotal:" by appending ':' to the field
    snprintf(search_field, sizeof(search_field), "%s:", field);

    fh = fopen("/proc/meminfo", "r");
    if (fh == NULL) {
        perror("Error opening /proc/meminfo");
        exit(1);
    }

    // Read the entire file into buf
    fread(buf, sizeof(char), sizeof(buf) - 1, fh);
    fclose(fh);
    buf[sizeof(buf) - 1] = '\0'; // Ensure null termination

    tmp_buf = strtok(buf, "\n");
    while (tmp_buf) {
        // Trim leading spaces from the line (to avoid issues with extra spaces before the field)
        while (*tmp_buf == ' ' || *tmp_buf == '\t') {
            tmp_buf++;
        }

        // Print the current line being processed (for debugging)
        printf("Processing line: %s\n", tmp_buf);

        // Check if the current line starts with the search_field
        if (strncmp(tmp_buf, search_field, strlen(search_field)) == 0) {
            // Use sscanf to extract the number after the field name
            if (sscanf(tmp_buf, "%*s %ld", &memory_value) == 1) {
                break;  // Exit the loop once the memory value is found
            } else {
                printf("Error: Could not parse memory value from line: %s\n", tmp_buf);
                break;
            }
        }
        tmp_buf = strtok(NULL, "\n");  // Move to the next line
    }

    // Print the result
    if (memory_value == 0) {
        printf("Error: Could not find memory value for %s\n", search_field);
    } else {
        printf("%s %ld kB\n", search_field, memory_value);
    }

    const char db_name[] = "/home/reai_bot/mem.db";
    dbase_init(db_name);
    dbase_append(db_name, field);

    return memory_value;
}

void *mid_thr_fcn( void *ptr ) {
    time_t now;
    struct tm * timeinfo;

    // setup
    // time (&now);
    // timeinfo = localtime ( &now );
    // printf ( "Thread mid starts at: %s", asctime (timeinfo) );
    while(1) {
        // loop
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        avg_shared_data = get_memory_value(shared_data);

        // time (&now);
        // timeinfo = localtime ( &now );
        // printf ( "Thread mid runs at: %s", asctime (timeinfo) );
        // data_buf[data_buf_idx++] = shared_data;
        // if (data_buf_idx == DATA_BUF_SZ) {
        //     data_buf_idx = 0;
        //     avg_shared_data = 0.0;
        //     for (int i=0; i < DATA_BUF_SZ; i++) {
        //         avg_shared_data += data_buf[i];
        //     }
        //     avg_shared_data /= DATA_BUF_SZ;

        // printf("Report data %.2f\n", avg_shared_data);
        pthread_mutex_lock(&avg_data_cond_mutex);
        pthread_cond_signal(&avg_data_cond);
        pthread_mutex_unlock(&avg_data_cond_mutex);
        // }
        pthread_mutex_unlock(&data_cond_mutex);
    }
}