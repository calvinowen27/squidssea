SRC_DIR = src
DBG_DIR = build/debug
RLS_DIR = build/release
ANIMATION_DIR = $(SRC_DIR)/Animation
ENTITY_DIR = $(SRC_DIR)/Entity
INPUT_DIR = $(SRC_DIR)/Input
OBJECTS_DIR = $(SRC_DIR)/Objects
UI_DIR = $(SRC_DIR)/UI
UTIL_DIR = $(SRC_DIR)/Util

INCLUDE_DIR = include
INCLUDE_GAME = $(INCLUDE_DIR)/game
INCLUDE_ANIMATION = $(INCLUDE_GAME)/Animation
INCLUDE_ENTITY = $(INCLUDE_GAME)/Entity
INCLUDE_INPUT = $(INCLUDE_GAME)/Input
INCLUDE_OBJECTS = $(INCLUDE_GAME)/Objects
INCLUDE_UI = $(INCLUDE_GAME)/UI
INCLUDE_UTIL = $(INCLUDE_GAME)/Util
INCLUDE_JSON = $(INCLUDE_DIR)/nlohmann_json

SRC_DIR_O = build/obj
ANIMATION_DIR_O = $(SRC_DIR_O)/Animation
ENTITY_DIR_O = $(SRC_DIR_O)/Entity
INPUT_DIR_O = $(SRC_DIR_O)/Input
OBJECTS_DIR_O = $(SRC_DIR_O)/Objects
UI_DIR_O = $(SRC_DIR_O)/UI
UTIL_DIR_O = $(SRC_DIR_O)/Util

BUILD_DIR = $(DBG_DIR)

SRC_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR_O)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
ANIMATION_OBJS = $(patsubst $(ANIMATION_DIR)/%.cpp, $(ANIMATION_DIR_O)/%.o, $(wildcard $(ANIMATION_DIR)/*.cpp))
ENTITY_OBJS = $(patsubst $(ENTITY_DIR)/%.cpp, $(ENTITY_DIR_O)/%.o, $(wildcard $(ENTITY_DIR)/*.cpp))
INPUT_OBJS = $(patsubst $(INPUT_DIR)/%.cpp, $(INPUT_DIR_O)/%.o, $(wildcard $(INPUT_DIR)/*.cpp))
OBJECTS_OBJS = $(patsubst $(OBJECTS_DIR)/%.cpp, $(OBJECTS_DIR_O)/%.o, $(wildcard $(OBJECTS_DIR)/*.cpp))
UI_OBJS = $(patsubst $(UI_DIR)/%.cpp, $(UI_DIR_O)/%.o, $(wildcard $(UI_DIR)/*.cpp))
UTIL_OBJS = $(patsubst $(UTIL_DIR)/%.cpp, $(UTIL_DIR_O)/%.o, $(wildcard $(UTIL_DIR)/*.cpp))

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic

LDFLAGS = -L./lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lUser32

EXECUTABLE = squid

CORES := $(shell nproc)
MAKEFLAGS += -j$(CORES)

debug: CFLAGS += -g3
debug: BUILD_DIR = $(DBG_DIR)
debug: build

release: BUILD_DIR = $(RLS_DIR)
release: build

build: make_dirs refresh_addtl $(SRC_OBJS) $(ANIMATION_OBJS) $(ENTITY_OBJS) $(INPUT_OBJS) $(OBJECTS_OBJS) $(UI_OBJS) $(UTIL_OBJS)
	$(CC) $(SRC_OBJS) $(ANIMATION_OBJS) $(ENTITY_OBJS) $(INPUT_OBJS) $(OBJECTS_OBJS) $(UI_OBJS) $(UTIL_OBJS) $(LDFLAGS) -o $(BUILD_DIR)/$(EXECUTABLE) -static-libgcc -static-libstdc++

refresh_addtl: refresh_content refresh_data

### CREATE OBJECTS ###

$(SRC_DIR_O)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_GAME)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ANIMATION_DIR_O)/%.o: $(ANIMATION_DIR)/%.cpp $(INCLUDE_ANIMATION)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ENTITY_DIR_O)/%.o: $(ENTITY_DIR)/%.cpp $(INCLUDE_ENTITY)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(INPUT_DIR_O)/%.o: $(INPUT_DIR)/%.cpp $(INCLUDE_INPUT)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIR_O)/%.o: $(OBJECTS_DIR)/%.cpp $(INCLUDE_OBJECTS)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(UI_DIR_O)/%.o: $(UI_DIR)/%.cpp $(INCLUDE_UI)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(UTIL_DIR_O)/%.o: $(UTIL_DIR)/%.cpp $(UTIL_INPUT)/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(DBG_DIR)/$(EXECUTABLE) $(RLS_DIR)/$(EXECUTABLE) $(SRC_OBJS) $(ANIMATION_OBJS) $(ENTITY_OBJS) $(INPUT_OBJS) $(OBJECTS_OBJS) $(UI_OBJS) $(UTIL_OBJS)

make_dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(SRC_DIR_O)
	mkdir -p $(ANIMATION_DIR_O)
	mkdir -p $(ENTITY_DIR_O)
	mkdir -p $(INPUT_DIR_O)
	mkdir -p $(OBJECTS_DIR_O)
	mkdir -p $(UI_DIR_O)
	mkdir -p $(UTIL_DIR_O)

refresh_content:
	rm -rf $(BUILD_DIR)/content
	mkdir -p $(BUILD_DIR)/content
	cp -r content $(BUILD_DIR)

refresh_level:
	rm -rf $(BUILD_DIR)/level
	mkdir -p $(BUILD_DIR)/level
	cp -r level $(BUILD_DIR)

refresh_data:
	rm -rf $(BUILD_DIR)/data
	mkdir -p $(BUILD_DIR)/data
	cp -r data $(BUILD_DIR)