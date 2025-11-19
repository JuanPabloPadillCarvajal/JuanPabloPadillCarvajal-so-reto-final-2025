#include <string.h>
#include <stdio.h>
#include "../include/compression.h"

// Declaraciones de funciones de algoritmos específicos
extern int rle_compress(const char *input_path, const char *output_path);
extern int rle_decompress(const char *input_path, const char *output_path);
extern int huffman_compress(const char *input_path, const char *output_path);
extern int huffman_decompress(const char *input_path, const char *output_path);
extern int lzw_compress(const char *input_path, const char *output_path);
extern int lzw_decompress(const char *input_path, const char *output_path);

int compress_file(const char *input_path, const char *output_path, const char *algorithm) {
    if (!algorithm) {
        fprintf(stderr, "Error: algoritmo de compresión no especificado\n");
        return -1;
    }
    
    if (strcmp(algorithm, "rle") == 0) {
        return rle_compress(input_path, output_path);
    } else if (strcmp(algorithm, "huffman") == 0) {
        return huffman_compress(input_path, output_path);
    } else if (strcmp(algorithm, "lzw") == 0) {
        return lzw_compress(input_path, output_path);
    } else {
        fprintf(stderr, "Error: algoritmo de compresión desconocido: %s\n", algorithm);
        fprintf(stderr, "Algoritmos disponibles: rle, huffman, lzw\n");
        return -1;
    }
}

int decompress_file(const char *input_path, const char *output_path, const char *algorithm) {
    if (!algorithm) {
        fprintf(stderr, "Error: algoritmo de compresión no especificado\n");
        return -1;
    }
    
    if (strcmp(algorithm, "rle") == 0) {
        return rle_decompress(input_path, output_path);
    } else if (strcmp(algorithm, "huffman") == 0) {
        return huffman_decompress(input_path, output_path);
    } else if (strcmp(algorithm, "lzw") == 0) {
        return lzw_decompress(input_path, output_path);
    } else {
        fprintf(stderr, "Error: algoritmo de compresión desconocido: %s\n", algorithm);
        fprintf(stderr, "Algoritmos disponibles: rle, huffman, lzw\n");
        return -1;
    }
}

