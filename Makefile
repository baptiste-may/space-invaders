CC := gcc
SRC_DIR ?= src
OBJ_DIR ?= build
ifeq '$(DEBUG)' '1'
CFLAGS ?= -Wall -MMD -g
else
CFLAGS ?= -Wall -MMD -O3 -DNDEBUG
endif
LDFLAGS = -lncurses -lSDL3 -lSDL3_ttf

OBJS := $(shell find $(SRC_DIR) -name "*.c" | sed 's/.c$$/.o/g' | sed 's/$(SRC_DIR)/$(OBJ_DIR)/g')
DEPS := $(OBJS:.o=.d)
TARGET ?= main

all: $(TARGET)

run-ncurses: $(TARGET)
	@./$(TARGET) ncurses

run-sdl: $(TARGET)
	@./$(TARGET) sdl

valgrind: $(TARGET)
	@valgrind ./$(TARGET) ncurses

doc:
	doxygen

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)
	rm -rf doc
	rm -f $(TARGET)

-include $(DEPS)
