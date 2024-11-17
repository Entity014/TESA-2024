#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    if (argc == 2) {
        printf("Sawasdee %s\n", argv[1]);
        char surname[20];
        printf("Enter your surname: ");
        if (fgets(surname, sizeof(surname), stdin) != NULL) {
            printf("Hello %s %s\n", argv[1], surname);
        }
    } else {
        printf("Use hello YOUR_NAME\n");
    }
}