#include <stdio.h>
#include <stdlib.h>
#include "db_helper.h"

int main(int argc, char *argv[]) {
    const char db_name[] = "data.db";
    if (argc >= 2) {
        if (strcmp(argv[1], "INIT") == 0) {
            dbase_init(db_name);
        } else if (strcmp(argv[1], "ADD") == 0) {
            int value = atoi(argv[2]);
            dbase_append(db_name, value);
        } else if (strcmp(argv[1], "QUERY") == 0) {
            int value = dbase_query(db_name);
            printf("Last data: %d\n", value);
        } else {
            printf("Unknown command %s\n", argv[1]);
        }
    } else {
        printf("Use db_app CMD ARG\n");
    }
}