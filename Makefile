CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread -Isrc/application -Isrc/scheduler -Isrc/communication -Isrc/communication/sockets -Isrc/services -Isrc/services/someip
LDFLAGS := 
BUILD_DIR := build

SRC_DIRS := src/scheduler src/communication/sockets src/services/someip

SOURCES := $(wildcard src/scheduler/*.cpp) $(wildcard src/communication/sockets/*.cpp) $(wildcard src/services/someip/*.cpp)

OBJECTS := $(patsubst src/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))

EXECUTABLE := $(BUILD_DIR)/ECU_BC2

.PHONY: all clean directories

all: directories $(EXECUTABLE)

directories:
	@mkdir -p $(BUILD_DIR)/scheduler
	@mkdir -p $(BUILD_DIR)/communication/sockets
	@mkdir -p $(BUILD_DIR)/services/someip

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)