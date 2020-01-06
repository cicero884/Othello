CC=clang
CFLAGS=-Wall -O3
HEADER=-I lib

all: main.o board.o ai.o basic.o list.o
	$(CC) $(CFLAGS) -lpthread -o ./bin/othello ./obj/*
%.o: ./src/%.c
	$(CC) $(CFLAGS) $(HEADER) -c -o ./obj/$@ $<
clear:
	rm ./obj/* ./bin/*
