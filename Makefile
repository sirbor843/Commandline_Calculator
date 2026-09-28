# CONCEPT: Makefile for modular C development
ifeq ($(shell uname -s),Darwin)
  ifneq ($(wildcard /Library/Developer/CommandLineTools/usr/bin),)
    export DEVELOPER_DIR ?= /Library/Developer/CommandLineTools
  endif
endif
CC ?= gcc
TARGET ?= calculator
BUILD ?= debug

WARN_FLAGS = -Wall -Wextra -Werror -Wpedantic
STD_FLAG = -std=c99
INCLUDE_FLAGS = -Iinclude -Iinclude/core
BASE_FLAGS = $(WARN_FLAGS) $(STD_FLAG) $(INCLUDE_FLAGS)
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG
ASAN_FLAGS = -g -O1 -fsanitize=address -fno-omit-frame-pointer
UBSAN_FLAGS = -g -O1 -fsanitize=undefined

ifeq ($(BUILD),release)
  CFLAGS = $(BASE_FLAGS) $(RELEASE_FLAGS)
else ifeq ($(BUILD),asan)
  CFLAGS = $(BASE_FLAGS) $(ASAN_FLAGS)
else ifeq ($(BUILD),ubsan)
  CFLAGS = $(BASE_FLAGS) $(UBSAN_FLAGS)
else
  CFLAGS = $(BASE_FLAGS) $(DEBUG_FLAGS)
endif

LIBS = -lm
LDFLAGS =
ifneq (,$(findstring -fsanitize=address,$(CFLAGS)))
  LDFLAGS += -fsanitize=address
endif
ifneq (,$(findstring -fsanitize=undefined,$(CFLAGS)))
  LDFLAGS += -fsanitize=undefined
endif

SRCS = $(shell find src -type f -name '*.c')
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

help:
	@echo "Targets:"
	@echo "  make [all]         Build calculator (BUILD=debug|release|asan|ubsan)"
	@echo "  make run           Run calculator"
	@echo "  make test          Run CLI regression tests"
	@echo "  make valgrind      Run valgrind leak checks"
	@echo "  make clean         Remove binary and objects"
	@echo "  make rebuild       Clean + build"
	@echo "  make ci            Rebuild + test"

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compile C source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Fast shell-based CLI tests
test: $(TARGET)
	./tests/run_tests.sh

# CI-like local verification
ci: rebuild test

# Clean rule for build maintenance
clean:
	rm -f $(TARGET) $(OBJS)

rebuild: clean all

# Valgrind for memory leak detection
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

.PHONY: all help run test ci clean rebuild valgrind
