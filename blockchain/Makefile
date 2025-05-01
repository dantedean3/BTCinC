CC = gcc
CFLAGS = -Wall -Wextra -std=c89

all: blockchain

blockchain: blockchain.c ../sha256/sha256.c
	$(CC) $(CFLAGS) blockchain.c ../sha256/sha256.c -o blockchain

clean:
	rm -f blockchain chain.dat
