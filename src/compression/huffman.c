#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 4096
#define MAX_SYMBOLS 256

typedef struct huffman_node {
    unsigned char symbol;
    int frequency;
    struct huffman_node *left;
    struct huffman_node *right;
} huffman_node_t;

typedef struct {
    unsigned char code[32];
    int code_length;
} huffman_code_t;

static huffman_node_t *create_node(unsigned char symbol, int frequency) {
    huffman_node_t *node = malloc(sizeof(huffman_node_t));
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void free_tree(huffman_node_t *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

static int compare_nodes(const void *a, const void *b) {
    huffman_node_t *node_a = *(huffman_node_t **)a;
    huffman_node_t *node_b = *(huffman_node_t **)b;
    return node_a->frequency - node_b->frequency;
}

static huffman_node_t *build_huffman_tree(int frequencies[]) {
    huffman_node_t *nodes[MAX_SYMBOLS];
    int node_count = 0;
    
    // Crear nodos para símbolos presentes
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (frequencies[i] > 0) {
            nodes[node_count++] = create_node(i, frequencies[i]);
        }
    }
    
    if (node_count == 0) return NULL;
    if (node_count == 1) return nodes[0];
    
    // Construir árbol de Huffman
    while (node_count > 1) {
        // Ordenar nodos por frecuencia
        qsort(nodes, node_count, sizeof(huffman_node_t *), compare_nodes);
        
        // Combinar los dos nodos con menor frecuencia
        huffman_node_t *left = nodes[0];
        huffman_node_t *right = nodes[1];
        huffman_node_t *parent = create_node(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        // Mover nodos
        nodes[0] = parent;
        for (int i = 1; i < node_count - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        node_count--;
    }
    
    return nodes[0];
}

static void build_codes(huffman_node_t *root, huffman_code_t codes[], 
                        unsigned char code[], int depth) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        // Nodo hoja
        memcpy(codes[root->symbol].code, code, depth);
        codes[root->symbol].code_length = depth;
    } else {
        if (root->left) {
            code[depth] = 0;
            build_codes(root->left, codes, code, depth + 1);
        }
        if (root->right) {
            code[depth] = 1;
            build_codes(root->right, codes, code, depth + 1);
        }
    }
}

static void write_bit_buffer(int fd, unsigned char *buffer, int *bit_pos, int bit) {
    int byte_pos = *bit_pos / 8;
    int bit_offset = *bit_pos % 8;
    
    if (bit) {
        buffer[byte_pos] |= (1 << (7 - bit_offset));
    }
    
    (*bit_pos)++;
    
    // Escribir buffer cuando esté lleno
    if (byte_pos >= BUFFER_SIZE - 1 && (*bit_pos % 8 == 0)) {
        write(fd, buffer, BUFFER_SIZE);
        memset(buffer, 0, BUFFER_SIZE);
        *bit_pos = 0;
    }
}

static void flush_bit_buffer(int fd, unsigned char *buffer, int bit_pos) {
    int bytes = (bit_pos + 7) / 8;
    if (bytes > 0) {
        write(fd, buffer, bytes);
    }
}

int huffman_compress(const char *input_path, const char *output_path) {
    int fd_in = open(input_path, O_RDONLY);
    if (fd_in < 0) {
        perror("open input");
        return -1;
    }
    
    // Primera pasada: contar frecuencias
    int frequencies[MAX_SYMBOLS] = {0};
    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    off_t file_size = 0;
    
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        file_size += bytes_read;
        for (ssize_t i = 0; i < bytes_read; i++) {
            frequencies[buffer[i]]++;
        }
    }
    
    if (file_size == 0) {
        close(fd_in);
        return 0;
    }
    
    // Construir árbol de Huffman
    huffman_node_t *root = build_huffman_tree(frequencies);
    if (!root) {
        close(fd_in);
        return -1;
    }
    
    // Construir códigos
    huffman_code_t codes[MAX_SYMBOLS] = {0};
    unsigned char code[32] = {0};
    build_codes(root, codes, code, 0);
    
    // Guardar tabla de frecuencias al inicio del archivo
    int fd_out = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("open output");
        free_tree(root);
        close(fd_in);
        return -1;
    }
    
    write(fd_out, frequencies, sizeof(frequencies));
    
    // Segunda pasada: comprimir
    lseek(fd_in, 0, SEEK_SET);
    unsigned char out_buffer[BUFFER_SIZE] = {0};
    int bit_pos = 0;
    
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            unsigned char symbol = buffer[i];
            huffman_code_t *code = &codes[symbol];
            
            for (int j = 0; j < code->code_length; j++) {
                write_bit_buffer(fd_out, out_buffer, &bit_pos, code->code[j]);
            }
        }
    }
    
    flush_bit_buffer(fd_out, out_buffer, bit_pos);
    
    free_tree(root);
    close(fd_in);
    close(fd_out);
    return 0;
}

static int read_bit(int fd, unsigned char *buffer, int *bit_pos, int *buffer_len) {
    if (*bit_pos >= *buffer_len * 8) {
        *buffer_len = read(fd, buffer, BUFFER_SIZE);
        if (*buffer_len <= 0) return -1;
        *bit_pos = 0;
    }
    
    int byte_pos = *bit_pos / 8;
    int bit_offset = *bit_pos % 8;
    int bit = (buffer[byte_pos] >> (7 - bit_offset)) & 1;
    (*bit_pos)++;
    return bit;
}

int huffman_decompress(const char *input_path, const char *output_path) {
    int fd_in = open(input_path, O_RDONLY);
    if (fd_in < 0) {
        perror("open input");
        return -1;
    }
    
    // Leer tabla de frecuencias
    int frequencies[MAX_SYMBOLS];
    if (read(fd_in, frequencies, sizeof(frequencies)) != sizeof(frequencies)) {
        perror("read frequencies");
        close(fd_in);
        return -1;
    }
    
    // Construir árbol de Huffman
    huffman_node_t *root = build_huffman_tree(frequencies);
    if (!root) {
        close(fd_in);
        return -1;
    }
    
    int fd_out = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("open output");
        free_tree(root);
        close(fd_in);
        return -1;
    }
    
    // Descomprimir
    unsigned char in_buffer[BUFFER_SIZE];
    unsigned char out_buffer[BUFFER_SIZE];
    int bit_pos = 0;
    int buffer_len = 0;
    int out_pos = 0;
    
    while (1) {
        huffman_node_t *node = root;
        
        // Recorrer árbol hasta llegar a una hoja
        while (node->left || node->right) {
            int bit = read_bit(fd_in, in_buffer, &bit_pos, &buffer_len);
            if (bit < 0) {
                // Fin de archivo
                if (out_pos > 0) {
                    write(fd_out, out_buffer, out_pos);
                }
                free_tree(root);
                close(fd_in);
                close(fd_out);
                return 0;
            }
            
            node = bit ? node->right : node->left;
            if (!node) break;
        }
        
        if (node && !node->left && !node->right) {
            out_buffer[out_pos++] = node->symbol;
            if (out_pos >= BUFFER_SIZE) {
                write(fd_out, out_buffer, BUFFER_SIZE);
                out_pos = 0;
            }
        }
    }
}

