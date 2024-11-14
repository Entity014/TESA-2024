# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Libraries to link
LIBS = -lmosquitto

# Default source and output file names
SRC = mqtt_client.cpp
OUT = out

# If 'file' variable is passed, use it for custom build names (i.e., test.cpp and test)
SRC = $(if $(file), $(file).cpp, $(SRC))
# OUT = $(if $(file), $(file), $(OUT))

# Default target
all: $(OUT)

# Rule for building the output executable
$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	chmod +x $(OUT)
	./$(OUT)
	rm -rf $(OUT)

# Target to run the program after building
run: $(OUT)
	./$(OUT)
	rm -rf $(OUT)

# Clean up
clean:
	rm -f $(OUT)

# Phony targets (so that they aren't confused with filenames)
.PHONY: all clean run
