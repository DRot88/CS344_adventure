#CC will be compiler to use
#CFLAGS will be options to pass
#SOURCES will be .c files
#OBJECTS will be .o files
#EXECUTABLE will be completed program
# -c creates object files/does not link
# -g for debugging information
# -Wall for all warnings
CC = gcc
STD = -std=c89
CFLAGS = -c -g -Wall
SOURCES = rotenbed.buildrooms.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = rotenbed.buildrooms

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(STD) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *o $(EXECUTABLE)