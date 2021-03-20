# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)

EXEC_DIR := ./bin
BUILD_DIR := ./build
SRC_DIRS := ./src

# Find all the C and C++ files we want to compile
SRCS := $(shell find $(SRC_DIRS) -maxdepth 1 -name *.c)
EXEC_SRCS := $(shell find $(SRC_DIRS)/mains/ -maxdepth 1 -name *.c)
EXECUTABLES := $(patsubst $(SRC_DIRS)/mains/%.c, $(EXEC_DIR)/%, $(EXEC_SRCS))

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(shell sdl2-config --cflags)

LDFLAGS := $(shell sdl2-config --libs) -lm -lcheck

all: $(EXECUTABLES)

# The final build step.
$(EXEC_DIR)/%: $(OBJS) $(BUILD_DIR)/src/mains/%.c.o
	mkdir -p $(dir $@)
	mkdir -p $(EXEC_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# Build step for C sources
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
