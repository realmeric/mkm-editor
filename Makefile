CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -O2 -Iinclude
LDFLAGS :=

SRC := \
  src/mkm.c \
  src/fileio.c

OBJ := $(SRC:src/%.c=build/%.o)

BIN := build/mkm

.PHONY: all clean run

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(BIN) sample.txt

clean:
	rm -rf build