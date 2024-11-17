#include <stdio.h>
#include <stdlib.h>
#include "mongo-c-driver-master/src/libmongoc/src/mongoc/mongoc.h"

int main() {
    mongoc_client_t *client;
    mongoc_gridfs_t *gridfs;
    mongoc_gridfs_file_t *file;
    mongoc_stream_t *stream;
    bson_error_t error;

    const char *uri_string = "mongodb://username:password@192.168.163.251:27017";
    const char *filename = "net.onnx";  // The file to download
    const char *output_path = "net_downloaded.onnx";  // Output file path

    // Initialize MongoDB driver
    mongoc_init();

    // Connect to MongoDB server
    client = mongoc_client_new(uri_string);
    if (!client) {
        fprintf(stderr, "Failed to connect to MongoDB server.\n");
        return EXIT_FAILURE;
    }

    // Get GridFS instance
    gridfs = mongoc_client_get_gridfs(client, "mydb", "fs", &error);
    if (!gridfs) {
        fprintf(stderr, "Failed to get GridFS: %s\n", error.message);
        mongoc_client_destroy(client);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }

    // Find file in GridFS
    file = mongoc_gridfs_find_one_by_filename(gridfs, filename, &error);
    if (!file) {
        fprintf(stderr, "File '%s' not found in GridFS: %s\n", filename, error.message);
        mongoc_gridfs_destroy(gridfs);
        mongoc_client_destroy(client);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }

    // Open the output file for writing
    FILE *output_file = fopen(output_path, "wb");
    if (!output_file) {
        fprintf(stderr, "Failed to open file '%s' for writing.\n", output_path);
        mongoc_gridfs_file_destroy(file);
        mongoc_gridfs_destroy(gridfs);
        mongoc_client_destroy(client);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }

    // Read the file from GridFS and write to output
    stream = mongoc_stream_gridfs_new(file);
    char buffer[4096];
    ssize_t bytes_read;

    while ((bytes_read = mongoc_stream_read(stream, buffer, sizeof(buffer), 1, -1)) > 0) {
        fwrite(buffer, 1, bytes_read, output_file);
    }

    // Clean up
    fclose(output_file);
    mongoc_stream_destroy(stream);
    mongoc_gridfs_file_destroy(file);
    mongoc_gridfs_destroy(gridfs);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    printf("File downloaded successfully to '%s'.\n", output_path);
    return EXIT_SUCCESS;
}
