CC=gcc
all: main

main: main.c
	$(CC) main.c -o compiler

run:
	./compiler

clean:
	rm -f cleaninput.txt leemelist.txt lexemetable.txt main.o mcode.txt parseoutput.txt stacktrace.txt symlist.txt compiler *.out
