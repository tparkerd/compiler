CC=gcc
CFLAGS=
LDFLAGS=
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=compiler

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f cleaninput.txt leemelist.txt lexemetable.txt main.o mcode.txt parseoutput.txt stacktrace.txt symlist.txt compiler
