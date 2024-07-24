# There are following modes: RELEASE_MODE, DEBUG_MODE, TEST_MODE. Test mode includes in itself DEBUG_MODE.

PROJECT = dutfconv

BUILD_DIR = ./build
SRC_DIR = ./src

MAIN_FILE = $(SRC_DIR)/main.c
TEST_MAIN_FILE = $(SRC_DIR)/test.c
EXTENSION_FILES = $(SRC_DIR)/error.c $(SRC_DIR)/bsf_buffer.c \
				  $(SRC_DIR)/char_iterator.c $(SRC_DIR)/char_buffer.c \
				  $(SRC_DIR)/to_basic_form/all.c $(SRC_DIR)/from_basic_form/all.c \
				  $(SRC_DIR)/cli.c
COMPILE_FILES = $(MAIN_FILE) $(EXTENSION_FILES)
TEST_COMPILE_FILES = $(TEST_MAIN_FILE) $(EXTENSION_FILES)

DEBUG_DIR_PATH = $(BUILD_DIR)/debug
RELEASE_DIR_PATH = $(BUILD_DIR)/release
TEST_DIR_PATH = $(BUILD_DIR)/tests

CC = gcc
DEBUG_CC_ARGS = -D DEBUG_MODE -g -o $(DEBUG_DIR_PATH)/$(PROJECT) $(COMPILE_FILES)
RELEASE_CC_ARGS = -D RELEASE_MODE -O2 -o $(RELEASE_DIR_PATH)/$(PROJECT) $(COMPILE_FILES)
TEST_CC_ARGS = -D TEST_MODE -D DEBUG_MODE -o $(TEST_DIR_PATH)/$(PROJECT) $(TEST_COMPILE_FILES)

debug: $(DEBUG_DIR_PATH) $(COMPILE_FILES)
	$(CC) $(DEBUG_CC_ARGS)

release: $(RELEASE_DIR_PATH) $(COMPILE_FILES)
	$(CC) $(RELEASE_CC_ARGS)

test: $(TEST_DIR_PATH) $(TEST_COMPILE_FILES)
	$(CC) $(TEST_CC_ARGS)
	$(TEST_DIR_PATH)/$(PROJECT)

$(DEBUG_DIR_PATH): $(BUILD_DIR)
	mkdir $(DEBUG_DIR_PATH) || true

$(RELEASE_DIR_PATH): $(BUILD_DIR)
	mkdir $(RELEASE_DIR_PATH) || true

$(TEST_DIR_PATH): $(BUILD_DIR)
	mkdir $(TEST_DIR_PATH)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean: $(BUILD_DIR)
	rm -rf $(BUILD_DIR)
