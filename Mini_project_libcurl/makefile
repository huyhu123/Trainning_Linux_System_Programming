#sudo apt install libjson-c-dev
#sudo apt-get install -y libjansson-dev
CC = gcc
INC = -I./inc
SRC = ./src/*.c

all: run clean

run: main
	./main

main: main.c $(SRC)
	$(CC) $(INC) main.c $(SRC) -o main -lcurl -pthread -lm

debug_memory: main.c $(SRC)
	$(CC) $(INC) main.c $(SRC) -std=c11 -lm -o debug
	valgrind --track-origins=yes --leak-check=full ./debug
	rm -rf debug

debug_gdb: main.c $(SRC)
	$(CC) $(INC) main.c $(SRC) -o gdb_debug -g
	gdb ./gdb_debug
	rm -rf gdb_debug

clean:
	rm -rf main