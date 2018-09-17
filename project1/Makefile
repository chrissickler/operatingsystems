CC = gcc
LD = ld
CFLAGS =-Wall -Wextra -g -D DEBUG_ENABLE
SOURCES = $(wildcard *.c)
EXECUTABLE = main


OBJECTS = $(SOURCES:.c=.o)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS)


all: $(EXECUTABLE)
	@echo "Program built"

clean:
	rm *.o $(EXECUTABLE)

test:
	./$(EXECUTABLE)

mem:
	valgrind --leak-check=yes ./$(EXECUTABLE)

gdb:
	gdb ./$(EXECUTABLE)