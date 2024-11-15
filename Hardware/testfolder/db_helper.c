#include "db_helper.h"

// private constants
const char INIT_SQL_CMD[] = "CREATE TABLE IF NOT EXISTS data_table (\
    _id INTEGER PRIMARY KEY AUTOINCREMENT, \
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, \
    value INTEGER \
)";

const char APPEND_SQL_CMD[] = "INSERT INTO data_table (value) VALUES (?)";

const char QUERY_SQL_CMD[] = "SELECT * FROM data_table ORDER BY timestamp DESC LIMIT 1";

// initialize database
void dbase_init(const char *db_name) {  // Change to void
    sqlite3 *db;

    int result = sqlite3_open(db_name, &db);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error opening %s database: %s\n", db_name, sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    result = sqlite3_exec(db, INIT_SQL_CMD, NULL, NULL, NULL);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error executing SQL command: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_close(db);
}

// append data to the table
int dbase_append(const char *db_name, const char *field) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result = sqlite3_open(db_name, &db);

    if (result != SQLITE_OK) {
        fprintf(stderr, "Error opening %s database: %s\n", db_name, sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_prepare_v2(db, APPEND_SQL_CMD, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return result;
    }

    result = sqlite3_bind_text(stmt, 1, field, -1, SQLITE_STATIC);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error binding data to SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return result;
    }

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        fprintf(stderr, "Error executing SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return result;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return SQLITE_OK;
}

// query last value from the table
int dbase_query(const char *db_name) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int last_value = -1;

    int result = sqlite3_open(db_name, &db);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error opening %s database: %s\n", db_name, sqlite3_errmsg(db));
        return result;
    }

    result = sqlite3_prepare_v2(db, QUERY_SQL_CMD, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return result;
    }

    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        const char *timestamp = (const char *)sqlite3_column_text(stmt, 1);
        printf("Data timestamp: %s\n", timestamp);
        last_value = sqlite3_column_int(stmt, 2);
    } else if (result == SQLITE_DONE) {
        printf("No data found.\n");
    } else {
        fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return last_value;
}
