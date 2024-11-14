# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Libraries to link
LIBS = -lm -lasound

# Target application name
TARGET = sound_app

# Source files and object files
SRCS = sound_app.c abs.c alsa_thr.c fft_thr.c FFTImplementationCallback.c minOrMax.c recognizeDTMF_initialize.c recognizeDTMF_terminate.c recognizeDTMF.c rt_nonfinite.c rtGetInf.c rtGetNaN.c fft.c find.c
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