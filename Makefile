CFLAGS=-O3 -Wall -lpthread
HEADER=-I lib

all: main.o board.o ai.o rule.o
	gcc $(CFLAGS) -o ./bin/othello ./obj/*
%.o: ./src/%.c
	gcc $(CFLAGS) $(HEADER) -c -o ./obj/$@ $<
clear:
	rm ./obj/* ./bin/*
