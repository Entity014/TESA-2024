#ifndef DB_HELPER_H
#define DB_HELPER_H

// include files
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

// function prototypes
void dbase_init(const char *db_name);
int dbase_append(const char *db_name, int value);
int dbase_query(const char *db_name);

#endif // DB_HELPER_H