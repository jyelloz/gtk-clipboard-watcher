CFLAGS := $(CFLAGS_ADD) `pkg-config --cflags gtk4`
LDFLAGS := $(LDFLAGS_ADD) `pkg-config --libs gtk4`

PREFIX ?= $(HOME)/.local

SRC = gtk-clipboard-watcher.c
BIN = $(SRC:.c=)

all: $(BIN)

install: $(BIN)
	install -D -m 0755 -t $(DESTDIR)$(PREFIX)/bin $(BIN)

clean:
	rm -f $(BIN) compile_commands.json

compile_commands.json: $(SRC) Makefile
	bear -- $(MAKE) $(BIN)

.PHONY: clean install
