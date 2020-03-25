CC = gcc
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
CFLAGS = -Wall

out: $(SRC)
	$(CC) -o conv $(SRC) $(CFLAGS) 