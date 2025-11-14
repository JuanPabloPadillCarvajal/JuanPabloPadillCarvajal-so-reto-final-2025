CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./src/include
SRC = $(wildcard src/*.c src/utils/*.c src/compression/*.c src/encryption/*.c)
OUT = gsea

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT) *.o
