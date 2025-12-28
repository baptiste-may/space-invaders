CC := gcc
SRC_DIR ?= src
OBJ_DIR ?= build
ifeq '$(DEBUG)' '1'
CFLAGS ?= -Wall -MMD -g
else
CFLAGS ?= -Wall -MMD -O3 -DNDEBUG
endif
LDFLAGS = -lm -lncurses -lSDL3 -lSDL3_image -lSDL3_ttf

INCLUDE_PATH ?=-I3rdParty/SDL/include -I3rdParty/SDL_image/include -I3rdParty/SDL_ttf/include
LIBRARY_PATH ?=-L3rdParty/SDL/build -L3rdParty/SDL_image/build -L3rdParty/SDL_ttf/build
LD_LIB := 3rdParty/SDL/build:3rdParty/SDL_image/build:3rdParty/SDL_ttf/build:$(LD_LIBRARY_PATH)

OBJS := $(shell find $(SRC_DIR) -name "*.c" | sed 's/.c$$/.o/g' | sed 's/$(SRC_DIR)/$(OBJ_DIR)/g')
DEPS := $(OBJS:.o=.d)
TARGET ?= $(OBJ_DIR)/main

all: $(TARGET)

run-ncurses: $(TARGET)
	@LD_LIBRARY_PATH=$(LD_LIB) ./$(TARGET) ncurses

run-sdl: $(TARGET)
	@LD_LIBRARY_PATH=$(LD_LIB) ./$(TARGET) sdl

valgrind: $(TARGET)
	@LD_LIBRARY_PATH=$(LD_LIB) valgrind ./$(TARGET) ncurses

doc:
	doxygen

$(TARGET): 3rdParty/SDL/build 3rdParty/SDL_image/build 3rdParty/SDL_ttf/build $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBRARY_PATH)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@

3rdParty/SDL/build:
	@sh build.sh

3rdParty/SDL_image/build:
	@sh build.sh

3rdParty/SDL_ttf/build:
	@sh build.sh

.PHONY: clean
clean :
	@rm -rf $(OBJ_DIR)
	@rm -rf doc
	@rm -f $(TARGET)
	@rm -rf 3rdParty/SDL/build
	@rm -rf 3rdParty/SDL_image/build
	@rm -rf 3rdParty/SDL_ttf/build

-include $(DEPS)
