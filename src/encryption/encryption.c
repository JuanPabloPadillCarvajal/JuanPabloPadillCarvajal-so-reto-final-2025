#include <string.h>
#include <stdio.h>
#include "../include/encryption.h"

// Declaraciones de funciones de algoritmos específicos
extern int vigenere_encrypt(const char *input_path, const char *output_path, const char *key);
extern int vigenere_decrypt(const char *input_path, const char *output_path, const char *key);
extern int des_encrypt(const char *input_path, const char *output_path, const char *key);
extern int des_decrypt(const char *input_path, const char *output_path, const char *key);

int encrypt_file(const char *input_path, const char *output_path,
                 const char *algorithm, const char *key) {
    if (!algorithm) {
        fprintf(stderr, "Error: algoritmo de encriptación no especificado\n");
        return -1;
    }
    
    if (!key) {
        fprintf(stderr, "Error: clave requerida para encriptación\n");
        return -1;
    }
    
    if (strcmp(algorithm, "vigenere") == 0) {
        return vigenere_encrypt(input_path, output_path, key);
    } else if (strcmp(algorithm, "des") == 0) {
        return des_encrypt(input_path, output_path, key);
    } else {
        fprintf(stderr, "Error: algoritmo de encriptación desconocido: %s\n", algorithm);
        fprintf(stderr, "Algoritmos disponibles: vigenere, des\n");
        return -1;
    }
}

int decrypt_file(const char *input_path, const char *output_path,
                 const char *algorithm, const char *key) {
    if (!algorithm) {
        fprintf(stderr, "Error: algoritmo de encriptación no especificado\n");
        return -1;
    }
    
    if (!key) {
        fprintf(stderr, "Error: clave requerida para desencriptación\n");
        return -1;
    }
    
    if (strcmp(algorithm, "vigenere") == 0) {
        return vigenere_decrypt(input_path, output_path, key);
    } else if (strcmp(algorithm, "des") == 0) {
        return des_decrypt(input_path, output_path, key);
    } else {
        fprintf(stderr, "Error: algoritmo de encriptación desconocido: %s\n", algorithm);
        fprintf(stderr, "Algoritmos disponibles: vigenere, des\n");
        return -1;
    }
}

