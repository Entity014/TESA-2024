#include <stdio.h>
#include <stdlib.h>

int main(/* TBD */) {
    if (/* TBD */) {
        printf("Sawasdee %s\n", /* TBD */);
        char surname[20];
        printf("Enter your surname: ");
        if (fgets(surname, sizeof(surname), /* TBD */) != NULL) {
            printf("Hello %s %s", /* TBD */, surname);
        }
    } else {
        printf("Use ./hello YOUR_NAME\n");
    }
}