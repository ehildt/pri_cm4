APP_Name := pwmfan
CC := g++
CCSTDV := -std=gnu++2a
DEPFLAGS := -MMD
WFLAGS := -Wall -Werror -Wextra -Wpedantic -g
CXXFLAGS := $(WFLAGS) $(CCSTDV) $(DEPFLAGS)
BINDIR := $(DESTDIR)/usr/bin

CXXFILES := main.cpp $(wildcard **/*.cpp)
BUILD_OBJS := $(CXXFILES:.cpp=.o)
BUILD_DEPS := $(BUILD_OBJS:.o=.d)

-include $(BUILD_DEPS)

all: $(APP_Name)

pwmfan: $(BUILD_OBJS)
	$(CC) -o $(APP_Name) $^

clean:
	rm -f $(BUILD_OBJS) $(BUILD_DEPS) $(APP_Name)

purge:
	rm -f $(BUILD_OBJS) $(BUILD_DEPS) $(APP_Name)
	make uninstall

install: app
	install --mode=755 $(APP_Name) $(BINDIR)
	make clean

uninstall:
	rm -f $(BINDIR)/$(APP_Name)

.PHONY : all