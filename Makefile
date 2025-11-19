CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./src/include -std=c11
SRC = src/main.c \
      src/utils/cli_parser.c \
      src/utils/file_utils.c \
      src/compression/compression.c \
      src/compression/rle.c \
      src/compression/huffman.c \
      src/compression/lzw.c \
      src/encryption/encryption.c \
      src/encryption/vigenere.c \
      src/encryption/des.c
OUT = gsea

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT) *.o

.PHONY: all clean
