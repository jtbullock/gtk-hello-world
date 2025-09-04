CC     = cc
CFLAGS = $(shell pkg-config --cflags gtk4)
LIBS   = $(shell pkg-config --libs gtk4)

SRC_DIR   := src
BIN_DIR   := bin
OBJ_DIR   := obj

# Automatically find all .c files in the current directory
SRC = $(wildcard $(SRC_DIR)/*.c)

# Convert .c filenames to .o filenames
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET    = $(BIN_DIR)/gtk-hello-world

.PHONY: all clean dirs

# Default target
all: dirs $(TARGET)

# Create build directories if they don't exist
dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)

run: $(TARGET)
	$(TARGET)

watch: 
	watchexec -r -e c,h "make run"
