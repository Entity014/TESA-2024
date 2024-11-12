#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[100];
    fp = popen("python3 hello.py", "r");
    if (fp != NULL) {
        if (fgets(buffer, 100, fp) != NULL) {
            printf("Got %s\n", buffer);
        }
    } 
}
