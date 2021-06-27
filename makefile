APP_Name := app
CC := aarch64-linux-gnu-g++-8
CCV := -std=gnu++2a
DEPFLAGS := -MMD
WFLAGS := -Wall -Werror -Wextra -Wpedantic
CXXFLAGS := $(WFLAGS) $(CCV) $(DEPFLAGS)
BINDIR := $(DESTDIR)/usr/bin

CXXFILES := main.cpp $(wildcard **/*.cpp)

BUILD_OBJS := $(CXXFILES:.cpp=.o)
BUILD_DEPS := $(BUILD_OBJS:.o=.d)

-include $(BUILD_DEPS)

app: $(objs)
	$(CC) -o $(APP_Name) $^

clean:
	rm -f $(objs) $(BUILD_DEPS) $(APP_Name)

install: app
	install --mode=755 $(APP_Name) $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(APP_Name)