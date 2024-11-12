#include "db_helper.h"

// private constants
const char INIT_SQL_CMD[] = "CREATE TABLE IF NOT EXISTS data_table (\
    _id INTEGER PRIMARY KEY AUTOINCREMENT, \
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,\
    value INTEGER \
)";

const char APPEND_SQL_CMD[] = "INSERT INTO data_table (value) VALUES (?)";

const char QUERY_SQL_CMD[] = "SELECT * FROM data_table ORDER BY timestamp DESC LIMIT 1";

// private functions

// private variables

// initialize database
void dbase_init(const char *db_name) {
    sqlite3 *db;

    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening %s database\n", db_name);
        sqlite3_close(db);
        return;
    }
    if (sqlite3_exec(db, INIT_SQL_CMD, NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error executing %s: %s\n", INIT_SQL_CMD, sqlite3_errmsg(db)); 
        return;
    }
    sqlite3_close(db);
}

// append data to the table
int dbase_append(const char *db_name, int value) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening %s database\n", db_name);
        sqlite3_close(db);
        return -1;
    }
    if (sqlite3_prepare_v2(db, APPEND_SQL_CMD, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL statement %s\n", sqlite3_sql(stmt));
        fprintf(stderr, "Error executing %s: %s\n", APPEND_SQL_CMD, sqlite3_errmsg(db)); 
        return -1;
    }
    sqlite3_bind_int(stmt, 1, value);   // substitute value into SQL statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error step SQL\n");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

// query last value from the table
int dbase_query(const char *db_name) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int last_value = -1;

    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening %s database\n", db_name);
        sqlite3_close(db);
        return -1;
    }
    if (sqlite3_prepare_v2(db, QUERY_SQL_CMD, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error executing %s: %s\n", QUERY_SQL_CMD, sqlite3_errmsg(db)); 
        return -1;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *timestamp = sqlite3_column_text(stmt, 1);
        printf("Data timestamp: %s\n", timestamp);
        last_value = sqlite3_column_int(stmt, 2); 
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return last_value;
}