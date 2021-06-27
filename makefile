APP_Name := app
CC := aarch64-linux-gnu-g++-8
CCSTDV := -std=gnu++2a
DEPFLAGS := -MMD
WFLAGS := -Wall -Werror -Wextra -Wpedantic
CXXFLAGS := $(WFLAGS) $(CCSTDV) $(DEPFLAGS)
BINDIR := $(DESTDIR)/usr/bin

CXXFILES := main.cpp $(wildcard **/*.cpp)
BUILD_OBJS := $(CXXFILES:.cpp=.o)
BUILD_DEPS := $(BUILD_OBJS:.o=.d)

-include $(BUILD_DEPS)

app: $(BUILD_OBJS)
	$(CC) -o $(APP_Name) $^

.PHONY: clean
clean:
	rm -f $(BUILD_OBJS) $(BUILD_DEPS)

.PHONY: purge
purge:
	rm -f $(BUILD_OBJS) $(BUILD_DEPS) $(APP_Name)
	rm -f $(BINDIR)/$(APP_Name)

install: app
	install --mode=755 $(APP_Name) $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(APP_Name)