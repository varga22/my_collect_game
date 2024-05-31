# Compiler and linker settings
CC = gcc
CFLAGS = -Wall -g -I"C:/freeglut/include" -Iinclude
LDFLAGS = -L"C:/freeglut/lib/x64" -Llib -lfreeglut -lglu32 -lopengl32

# Directory settings
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Source and object files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
# Executable file
EXECUTABLE = $(BIN_DIR)/my_collect_game.exe

# Default target
all: $(EXECUTABLE)

# Link object files to create the executable
$(EXECUTABLE): $(OBJECTS)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean up build files
clean:
	@if exist $(OBJ_DIR) (forfiles /P $(OBJ_DIR) /M *.o /C "cmd /c del @path")
	@if exist $(EXECUTABLE) del /Q $(EXECUTABLE)

# Declare targets that are not actual files
.PHONY: all clean
