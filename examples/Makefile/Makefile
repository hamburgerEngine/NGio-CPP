CXX = g++
CXXFLAGS = -std=c++14 -Wall -I../../include

CURL_CFLAGS := $(shell curl-config --cflags)
CURL_LIBS := $(shell curl-config --libs)

CXXFLAGS += $(CURL_CFLAGS)

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = ../../src

MAIN_SRC = $(SRC_DIR)/main.cpp
NG_SRCS = $(wildcard $(LIB_DIR)/io/newgrounds/*.cpp) \
          $(wildcard $(LIB_DIR)/io/newgrounds/core/*.cpp) \
          $(wildcard $(LIB_DIR)/io/newgrounds/crypto/*.cpp)

OBJECTS = $(MAIN_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o) \
          $(NG_SRCS:$(LIB_DIR)/%.cpp=$(BUILD_DIR)/%.o)

LIBS = $(CURL_LIBS) -lpthread

TARGET = $(BUILD_DIR)/ngio_test

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/io/newgrounds
	mkdir -p $(BUILD_DIR)/io/newgrounds/core
	mkdir -p $(BUILD_DIR)/io/newgrounds/crypto

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean