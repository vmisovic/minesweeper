CC = gcc

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.h)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

CFLAGS = -Wall -I$(INCLUDE_DIR) -O3
LDFLAGS = -lraylib

EXECUTABLE = minesweeper

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

.PHONY: all clean

all: $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

-include $(OBJ_FILES:.o=.d)

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -MM $(CFLAGS) $< -MF $@ -MT '$(patsubst %.d,%.o,$@)'

-include $(BUILD_DIR)/*.d
