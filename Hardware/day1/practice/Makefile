# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -lm -lsqlite3

# Libraries to link
# LIBS = -lm -lpthread -lcurl -lpaho-mqtt3c -lcjson
LIBS = -lm -lsqlite3

# Target application name
# TARGET = iot_app
TARGET = dump_mem
# Source files and object files
# SRCS = iot_app.c rest_thr.c mqtt_thr.c 
SRCS = dump_mem.c db_helper.c
OBJS = $(SRCS:.c=.o)

# Default target to build the application
all: $(TARGET)

# Rule to link object files and create the executable with libraries
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run