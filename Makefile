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

WEB_DIR = web
OUT_HTML = game.html
IN_HTML = template.html
RAYLIB_SRC = raylib/src
LIBRAYLIB_A = $(RAYLIB_SRC)/libraylib.a

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(WEB_DIR) $(EXECUTABLE)

.PHONY: all clean

all: $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

web: all
	mkdir -p $(WEB_DIR)
	emcc -o $(WEB_DIR)/$(OUT_HTML) $(SRC_FILES) -Wall -I$(INCLUDE_DIR) $(LIBRAYLIB_A) -I$(RAYLIB_SRC) -L$(RAYLIB_SRC) -Os -s USE_GLFW=3 --shell-file $(IN_HTML) -DPLATFORM_WEB

-include $(OBJ_FILES:.o=.d)

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -MM $(CFLAGS) $< -MF $@ -MT '$(patsubst %.d,%.o,$@)'

-include $(BUILD_DIR)/*.d
