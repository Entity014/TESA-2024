# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -fopenmp 


# Libraries to link
LIBS = -lm -lreadline -lsndfile -lpthread -lpaho-mqtt3c -lcurl -lsqlite3 -lcjson -lmosquitto -lasound 

# Target application name
TARGET = sound_app

# Source files and object files
SRCS = sound_app.c db_helper/db_helper.c mqtt_sub_thr.c alsa_thr.c mqtt_thr.c detect_thr.c lib/audioProcessingFunction_data.c lib/audioProcessingFunction_emxutil.c lib/audioProcessingFunction_initialize.c lib/audioProcessingFunction_terminate.c lib/audioProcessingFunction.c lib/find.c lib/minOrMax.c lib/movmean.c lib/movSumProdOrMean.c lib/rt_nonfinite.c lib/rtGetInf.c lib/rtGetNaN.c lib/sqrt.c lib/abs.c 
#lsa_thr.c fft_thr.c mqtt_thr.c
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