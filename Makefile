CFLAGS   = -g -Wall -Wextra -Wpedantic -Werror -std=c11 -Og
CPPFLAGS =

LDFLAGS  = -lcmocka

DBG      = gdb
DBGFLAGS =

BINDIR   = bin
SRCDIR   = src
OBJDIR   = obj

BIN      = str_test
OBJ      = str_test.o str.o

.PHONY: all build clean debug run setup $(BIN)

all: build

build: setup $(BIN)

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/$(BIN)

debug: build
	$(DBG) $(DBGFLAGS) $(BINDIR)/$(BIN)

run: build
	$(BINDIR)/$(BIN)

setup:
	@mkdir -p $(BINDIR) $(OBJDIR)

$(BIN): $(addprefix $(OBJDIR)/,$(OBJ))
	$(CC) $(LDFLAGS) $^ -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
