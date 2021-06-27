APP_Name := app
CC := aarch64-linux-gnu-g++-8
CCV := -std=gnu++2a
DEPFLAGS := -MMD
WFLAGS := -Wall -Werror -Wextra -Wpedantic
CXXFLAGS := $(WFLAGS) $(CCV) $(DEPFLAGS)
BINDIR := $(DESTDIR)/usr/bin

src := $(wildcard *.cpp) $(wildcard gpio/*.cpp)
objs := $(src:.cpp=.o)
deps := $(objs:.o=.d)

-include $(deps)

app: $(objs)
	$(CC) -o $(APP_Name) $^

clean:
	rm -f $(objs) $(deps) $(APP_Name)

install: app
	install --mode=755 $(APP_Name) $(BINDIR)

uninstall:
	rm -f $(BINDIR)/$(APP_Name)