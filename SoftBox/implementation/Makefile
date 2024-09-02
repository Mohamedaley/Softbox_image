CC=gcc
CFLAGS=-I./include -Wall -Wextra -pedantic
LDFLAGS=
SOURCES=src/main.c src/container.c src/namespace.c src/cgroups.c src/filesystem.c src/network.c src/image.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=softbox

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)