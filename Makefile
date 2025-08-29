CC     = cc
CFLAGS = $(shell pkgconf --cflags gtk4)
LIBS   = $(shell pkgconf --libs gtk4)
SRC    = gtk-hello-world.c
OUT    = gtk-hello-world

all: gtk-hello-world

gtk-hello-world: gtk-hello-world.c
	$(CC) $(CFLAGS) gtk-hello-world.c -o bin/gtk-hello-world $(LIBS)

clean:
	rm -f gtk-hello-world
