CC=gcc
TARGET = $(target)
CFLAGS = -W -Wall
LIBS=-lncurses
LDFLAGS = -lmenu -lncurses
OBJECTS = $(SOURCES:.c=.o)

default:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET) $(LDFLAGS)
	./$(TARGET)

clean:
	find . -perm +100 -type f -delete