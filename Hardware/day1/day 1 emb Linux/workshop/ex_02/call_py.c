#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[100];
    fp = /* TBD */("./hello.py", "r");
    if (fp == NULL) {
        printf("Error running Python code");
        exit(1);
    }
    if (/* TBD */(buffer, 100, fp) != NULL) {
        printf("Got %s\n", buffer);
    }
}