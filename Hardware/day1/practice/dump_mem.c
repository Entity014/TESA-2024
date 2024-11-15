#include "dump_mem.h"

// This function returns the memory value for a given field (like "MemTotal:" or "MemAvailable:")
long get_memory_value(const char *field) {
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

    char dw[] = "dwagjef";
    const char db_name[] = "/home/reai_bot/mem.db";
    dbase_init(db_name);
    dbase_append(db_name, dw);

    return memory_value;
}

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <field_name>\n", argv[0]);
//         exit(1);
//     }

//     const char *field = argv[1];  // Field name passed as command line argument (e.g., "MemTotal")
//     get_memory_value(field);

//     return 0;
// }