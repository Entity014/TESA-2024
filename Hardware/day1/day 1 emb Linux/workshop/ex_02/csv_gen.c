#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    const char fname[] = "data.csv";
    if (argc == 2) {
        struct stat buffer;
        FILE *file;
        if (stat(fname, &buffer) != 0) {
            file = /* TBD */(fname, /* TBD */);
            /* TBD */(file, "index, value\n");
        } else {
            file = /* TBD */(fname, /* TBD */);
        }
        int rows = atoi(argv[1]);
        for (int i=0; i < rows; i++) {
            /* TBD */(file, "%d, %f\n", i, (float)rand()/RAND_MAX);
        }
    } else {
        printf("Use ./csv_gen NUM_ROWS\n");
    }
    return 0;
}