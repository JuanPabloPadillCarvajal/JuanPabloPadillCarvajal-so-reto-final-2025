#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 4096
#define DES_BLOCK_SIZE 8

// Tablas de permutación inicial y final
static const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

static const int FP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

// Tabla de expansión E
static const int E[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// S-boxes
static const int S[8][4][16] = {
    // S1
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2-S8 (usando valores simplificados)
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3-S8 similares (simplificado)
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
};

// Permutación P
static const int P[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

static void permute(uint64_t *data, const int *table, int size) {
    uint64_t result = 0;
    for (int i = 0; i < size; i++) {
        int pos = table[i] - 1;
        if ((*data >> (63 - pos)) & 1) {
            result |= (1ULL << (size - 1 - i));
        }
    }
    *data = result;
}

static uint32_t f_function(uint32_t R, uint64_t K) {
    // Expansión E
    uint64_t expanded = 0;
    for (int i = 0; i < 48; i++) {
        int pos = E[i] - 1;
        if ((R >> (31 - pos)) & 1) {
            expanded |= (1ULL << (47 - i));
        }
    }
    
    // XOR con clave
    expanded ^= K;
    
    // S-boxes
    uint32_t result = 0;
    for (int i = 0; i < 8; i++) {
        int row = ((expanded >> (47 - i * 6)) & 0x20) | ((expanded >> (47 - i * 6 - 5)) & 1);
        int col = (expanded >> (47 - i * 6 - 1)) & 0xF;
        int val = S[i][row][col];
        result |= (val << (28 - i * 4));
    }
    
    // Permutación P
    uint32_t permuted = 0;
    for (int i = 0; i < 32; i++) {
        int pos = P[i] - 1;
        if ((result >> (31 - pos)) & 1) {
            permuted |= (1 << (31 - i));
        }
    }
    
    return permuted;
}

static void generate_keys(uint64_t key, uint64_t keys[16]) {
    // Generación de subclaves (versión simplificada)
    for (int i = 0; i < 16; i++) {
        keys[i] = key;
    }
}

static void des_block_encrypt(uint8_t *block, uint64_t keys[16]) {
    uint64_t data = 0;
    for (int i = 0; i < 8; i++) {
        data |= ((uint64_t)block[i] << (56 - i * 8));
    }
    
    // Permutación inicial
    permute(&data, IP, 64);
    
    uint32_t L = (data >> 32) & 0xFFFFFFFF;
    uint32_t R = data & 0xFFFFFFFF;
    
    // 16 rondas
    for (int i = 0; i < 16; i++) {
        uint32_t temp = R;
        R = L ^ f_function(R, keys[i]);
        L = temp;
    }
    
    // Swap final
    data = ((uint64_t)R << 32) | L;
    
    // Permutación final
    permute(&data, FP, 64);
    
    for (int i = 0; i < 8; i++) {
        block[i] = (data >> (56 - i * 8)) & 0xFF;
    }
}

static void des_block_decrypt(uint8_t *block, uint64_t keys[16]) {
    uint64_t data = 0;
    for (int i = 0; i < 8; i++) {
        data |= ((uint64_t)block[i] << (56 - i * 8));
    }
    
    // Permutación inicial
    permute(&data, IP, 64);
    
    uint32_t L = (data >> 32) & 0xFFFFFFFF;
    uint32_t R = data & 0xFFFFFFFF;
    
    // 16 rondas en orden inverso
    for (int i = 15; i >= 0; i--) {
        uint32_t temp = L;
        L = R ^ f_function(L, keys[i]);
        R = temp;
    }
    
    // Swap final
    data = ((uint64_t)R << 32) | L;
    
    // Permutación final
    permute(&data, FP, 64);
    
    for (int i = 0; i < 8; i++) {
        block[i] = (data >> (56 - i * 8)) & 0xFF;
    }
}

static uint64_t key_from_string(const char *key) {
    uint64_t result = 0;
    int len = strlen(key);
    for (int i = 0; i < 8 && i < len; i++) {
        result |= ((uint64_t)(unsigned char)key[i] << (56 - i * 8));
    }
    return result;
}

int des_encrypt(const char *input_path, const char *output_path, const char *key) {
    if (!key || strlen(key) == 0) {
        fprintf(stderr, "Error: clave requerida para encriptación DES\n");
        return -1;
    }
    
    int fd_in = open(input_path, O_RDONLY);
    if (fd_in < 0) {
        perror("open input");
        return -1;
    }
    
    int fd_out = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("open output");
        close(fd_in);
        return -1;
    }
    
    uint64_t master_key = key_from_string(key);
    uint64_t keys[16];
    generate_keys(master_key, keys);
    
    uint8_t block[DES_BLOCK_SIZE];
    ssize_t bytes_read;
    off_t file_size = 0;
    
    // Obtener tamaño del archivo
    struct stat st;
    fstat(fd_in, &st);
    file_size = st.st_size;
    
    // Guardar tamaño original del archivo
    write(fd_out, &file_size, sizeof(off_t));
    
    while ((bytes_read = read(fd_in, block, DES_BLOCK_SIZE)) > 0) {
        // Aplicar padding si el bloque es incompleto
        if (bytes_read < DES_BLOCK_SIZE) {
            memset(block + bytes_read, 0, DES_BLOCK_SIZE - bytes_read);
        }
        
        des_block_encrypt(block, keys);
        write(fd_out, block, DES_BLOCK_SIZE);
    }
    
    close(fd_in);
    close(fd_out);
    return 0;
}

int des_decrypt(const char *input_path, const char *output_path, const char *key) {
    if (!key || strlen(key) == 0) {
        fprintf(stderr, "Error: clave requerida para desencriptación DES\n");
        return -1;
    }
    
    int fd_in = open(input_path, O_RDONLY);
    if (fd_in < 0) {
        perror("open input");
        return -1;
    }
    
    int fd_out = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("open output");
        close(fd_in);
        return -1;
    }
    
    uint64_t master_key = key_from_string(key);
    uint64_t keys[16];
    generate_keys(master_key, keys);
    
    // Leer tamaño original
    off_t original_size;
    if (read(fd_in, &original_size, sizeof(off_t)) != sizeof(off_t)) {
        perror("read size");
        close(fd_in);
        close(fd_out);
        return -1;
    }
    
    uint8_t block[DES_BLOCK_SIZE];
    ssize_t bytes_read;
    off_t bytes_written = 0;
    
    while ((bytes_read = read(fd_in, block, DES_BLOCK_SIZE)) == DES_BLOCK_SIZE) {
        des_block_decrypt(block, keys);
        
        ssize_t to_write = DES_BLOCK_SIZE;
        if (bytes_written + to_write > original_size) {
            to_write = original_size - bytes_written;
        }
        
        write(fd_out, block, to_write);
        bytes_written += to_write;
        
        if (bytes_written >= original_size) break;
    }
    
    close(fd_in);
    close(fd_out);
    return 0;
}

