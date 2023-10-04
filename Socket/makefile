CC = gcc
INC = -I./inc
SRC = ./src

main: $(SRC)
	$(CC) $(INC) $(SRC)/server.c -o server -pthread
	$(CC) $(INC) $(SRC)/client.c -o client

clean:
	rm -rf server client