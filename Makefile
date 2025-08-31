CC     = cc
CFLAGS = $(shell pkgconf --cflags gtk4)
LIBS   = $(shell pkgconf --libs gtk4)

SRC_DIR   := src
BIN_DIR   := bin

TARGET    = $(BIN_DIR)/gtk-hello-world

.PHONY: all clean dirs

# Default target
all: dirs $(TARGET)

# Create build directories if they don't exist
dirs:
	@mkdir -p $(BIN_DIR)

$(TARGET): $(SRC_DIR)/gtk-hello-world.c
	$(CC) $(CFLAGS) $(SRC_DIR)/gtk-hello-world.c -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	$(TARGET)
