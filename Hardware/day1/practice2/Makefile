# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -lm -lsqlite3

# Libraries to link
LIBS = -lm -lreadline -lpthread -lpaho-mqtt3c -lcurl -lsqlite3 -lcjson

# Target application name
TARGET = thr_app

# Source files and object files
SRCS = db_helper.c thr_app.c front_thr.c mid_thr.c end_thr.c
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