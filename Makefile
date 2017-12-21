default: compile

# Build target
TARGET = lispy
TARGET_MAIN = lispy.c

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_SRC_DIR = test/src
TEST_BUILD_DIR = test/build
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
CC = cc
CSTD = -std=c99
CWARN_OPTS = -Wall -Wextra -Werror -Wno-unused-parameter


CFLAGS =        $(CSTD) -O0 -c -I$(SRC_DIR) -I$(SRC_DIR)/mpc -ggdb $(CWARN_OPTS)
CFLAGS_DEPLOY = $(CSTD) -O0 -c -I$(SRC_DIR) -I$(SRC_DIR)/mpc
CFLAGS_TEST =   $(CSTD) -O0 -c -I$(SRC_DIR) -I$(SRC_DIR)/mpc -I$(TEST_SRC_DIR) $(CWARN_OPTS) -Wno-unused-variable -Wno-unused-function

LFLAGS =        -ggdb
LFLAGS_DEPLOY = -static-libgcc
LFLAGS_TEST = -I$(TEST_SRC_DIR)

#
# ========== NO CONFIG BEYOND THIS LINE ==========
#
.PHONY: clean compile deploy run test drmemory debug

# Define sources and objects
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(subst $(SRC_DIR)/,,$(SOURCES:.c=.o)))
TEST_SOURCES = $(wildcard $(TEST_SRC_DIR)/*.c) $(wildcard $(TEST_SRC_DIR)/*/*.c)
TEST_OBJECTS = $(addprefix $(TEST_BUILD_DIR)/,$(subst $(TEST_SRC_DIR)/,,$(TEST_SOURCES:.c=.o)))

# Are we deploying?
ifeq ($(MAKECMDGOALS),deploy)
COMPILE_FLAGS = $(CFLAGS_DEPLOY)
LINK_FLAGS    = $(LFLAGS_DEPLOY)
else
COMPILE_FLAGS = $(CFLAGS)
LINK_FLAGS    = $(LFLAGS)
endif
COMPILE_FLAGS_TEST = $(CFLAGS_TEST)
LINK_FLAGS_TEST = $(LFLAGS_TEST)

# Sources location
vpath %.c $(SRC_DIR)
vpath %.c $(TEST_SRC_DIR)

# Targets beyond this line

# Target - clean
clean:
	$(RM) -R $(BUILD_DIR)
	$(RM) -R $(TEST_BUILD_DIR)

# Target - compile
compile: $(DIST_DIR)/$(TARGET)

# Target - deploy
deploy: $(DIST_DIR)/dist-$(TARGET)

# Target - run
run: $(DIST_DIR)/$(TARGET)
	$(DIST_DIR)/$(TARGET)

# Target - test
test: $(DIST_DIR)/test $(DIST_DIR)/$(TARGET)
	$(DIST_DIR)/test
	$(DIST_DIR)/$(TARGET) "test/std/test-stdlib.lp"

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

# .c test file
$(TEST_BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(COMPILE_FLAGS_TEST) $< -o $@
	
# Main binary
$(DIST_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(DIST_DIR)
	$(CC) $(COMPILE_FLAGS) -o $(BUILD_DIR)/$(TARGET_MAIN:.c=.o) $(SRC_DIR)/$(TARGET_MAIN)
	$(CC) $(LINK_FLAGS) -o $(DIST_DIR)/$(TARGET) $(OBJECTS)

# Test binary
$(DIST_DIR)/test: $(TEST_OBJECTS) $(OBJECTS)
	@mkdir -p $(DIST_DIR)
	$(CC) $(COMPILE_FLAGS) -DBUILDING_TESTS -o $(BUILD_DIR)/$(TARGET_MAIN:.c=.o) $(SRC_DIR)/$(TARGET_MAIN)
	$(CC) $(LINK_FLAGS_TEST) -o $(DIST_DIR)/test $(TEST_OBJECTS) $(OBJECTS)

# Deployment binary
$(DIST_DIR)/dist-$(TARGET):	clean $(OBJECTS)
	@mkdir -p $(DIST_DIR)
	$(CC) $(LINK_FLAGS) -o $(DIST_DIR)/dist-$(TARGET) $(OBJECTS)
