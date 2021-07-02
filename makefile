# compiler setup
CC := g++
DEP_FLAGS := -MMD
DEBUG_FLAGS := -g
CC_STD_VERSION := -std=gnu++2a
W_FLAGS := -Wall -Werror -Wextra -Wpedantic $(DEBUG_FLAGS)
CXXFLAGS := $(W_FLAGS) $(CC_STD_VERSION) $(DEP_FLAGS)

# app setup
APP_NAME := pwmfan
BUILD_DIR := build
BIN_DIR := $(DESTDIR)/usr/bin

# source setup
CXXFILES := main.cpp $(wildcard ./**/*.cpp)
BUILD_OBJS := $(CXXFILES:.cpp=.o)
BUILD_DEPS := $(BUILD_OBJS:.o=.d)

all: $(APP_NAME)

pwmfan: $(BUILD_OBJS)
	$(CC) -o $(APP_NAME) $^

clean:
	rm -f $(BUILD_OBJS) $(BUILD_DEPS) $(APP_NAME)

purge:
	rm -f $(BUILD_OBJS) $(BUILD_DEPS) $(APP_NAME)
	make uninstall

install: app
	install --mode=755 $(APP_NAME) $(BIN_DIR)
	make clean

uninstall:
	rm -f $(BIN_DIR)/$(APP_NAME)

.PHONY : all

-include $(BUILD_DEPS)