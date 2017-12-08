default: compile

# Build target
TARGET = lispy

# Directories
SRC_DIR = src
BUILD_DIR = build
DIST_DIR = dist

# Define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -c 	   compiles to an unlinked object, instead of an executable
#  -g      generates debug information
#  -Wall   turns on most, but not all, compiler warnings
#  -Wextra turns on extra compiler warnings
#  -O3     sets optimization level
#  -I.     adds the current working directory to the include path
#
CC = gcc
CSTD = -std=c99
CWARN_OPTS = -Wall -Wextra -Werror -Wno-unused-parameter


CFLAGS =        $(CSTD) -I$(SRC_DIR) -I$(SRC_DIR)/mpc -c -O0 $(CWARN_OPTS) -ggdb
CFLAGS_DEPLOY = $(CSTD) -I$(SRC_DIR) -I$(SRC_DIR)/mpc -c -O3 

LFLAGS =        -ggdb
LFLAGS_DEPLOY = -static-libgcc

#
# ========== NO CONFIG BEYOND THIS LINE ==========
#
.PHONY: clean compile deploy run test drmemory debug

# Define sources and objects
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(subst $(SRC_DIR)/,,$(SOURCES:.c=.o)))

# Are we deploying?
ifeq ($(MAKECMDGOALS),deploy)
COMPILE_FLAGS = $(CFLAGS_DEPLOY)
LINK_FLAGS    = $(LFLAGS_DEPLOY)
else
COMPILE_FLAGS = $(CFLAGS)
LINK_FLAGS    = $(LFLAGS)
endif

# Sources location
vpath %.c $(SRC_DIR)

# Targets beyond this line

# Target - clean
clean:
	$(RM) -R $(BUILD_DIR)

# Target - compile
compile: $(DIST_DIR)/$(TARGET)

# Target - deploy
deploy: $(DIST_DIR)/dist-$(TARGET)

# Target - run
run: $(DIST_DIR)/$(TARGET)
	$(DIST_DIR)/$(TARGET)

# Target - test
test: $(DIST_DIR)/$(TARGET)
	$(DIST_DIR)/$(TARGET) "test/test-stdlib.lp"

# Target - drmemory
drmemory: compile
	drmemory -show_reachable -- $(DIST_DIR)/$(TARGET)	

# Target - debug
debug: compile
	gdb $(DIST_DIR)/$(TARGET)

# .c source file
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(COMPILE_FLAGS) $< -o $@

# Main binary
$(DIST_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(DIST_DIR)
	$(CC) $(LINK_FLAGS) -o $(DIST_DIR)/$(TARGET) $(OBJECTS)

# Deployment binary
$(DIST_DIR)/dist-$(TARGET):	clean $(OBJECTS)
	@mkdir -p $(DIST_DIR)
	$(CC) $(LINK_FLAGS) -o $(DIST_DIR)/dist-$(TARGET) $(OBJECTS)
