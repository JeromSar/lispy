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
CC = cc
CSTD = -std=c11
CWARN_OPTS = -Wall -Wextra -Werror -Wno-unused-parameter


CFLAGS =        $(CSTD) -I$(SRC_DIR) -I$(SRC_DIR)/mpc -c -O0 $(CWARN_OPTS) -gdwarf-3
CFLAGS_DEPLOY = $(CSTD) -I$(SRC_DIR) -I$(SRC_DIR)/mpc -c -O3 

LFLAGS =        -ggdb
LFLAGS_DEPLOY = -static-libgcc

#
# ========== NO CONFIG BEYOND THIS LINE ==========
#
.PHONY: clean compile deploy run test memcheck debug

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

	
# Target - .c file
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(COMPILE_FLAGS) $< -o $@

# Target - compile
compile: $(OBJECTS)
	@mkdir -p $(DIST_DIR)
	$(CC) $(LINK_FLAGS) -o $(DIST_DIR)/$(TARGET) $(OBJECTS)
	@echo Compiled binary: $(DIST_DIR)/$(TARGET)
	
# Target - deploy
deploy: $(OBJECTS)
	@mkdir -p $(DIST_DIR)
	$(CC) $(LINK_FLAGS) -o $(DIST_DIR)/dist-$(TARGET) $(OBJECTS)

run: compile
	@echo.
	@$(DIST_DIR)/$(TARGET)
  
test: compile
	@echo.
	@$(DIST_DIR)/$(TARGET) "test/test-stdlib.lp"

memcheck: compile
	cls
	@echo.
	drmemory -show_reachable -- $(DIST_DIR)/$(TARGET)	

debug: compile
	cls
	@echo.
	gdb $(DIST_DIR)/$(TARGET)
	


