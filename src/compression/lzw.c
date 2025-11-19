#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096
#define DICT_SIZE 4096
#define INITIAL_DICT_SIZE 256

typedef struct {
    unsigned char *data;
    int length;
    int code;
} dict_entry_t;

static dict_entry_t dictionary[DICT_SIZE];
static int dict_size;

static void init_dictionary() {
    dict_size = INITIAL_DICT_SIZE;
    for (int i = 0; i < INITIAL_DICT_SIZE; i++) {
        dictionary[i].data = malloc(1);
        dictionary[i].data[0] = i;
        dictionary[i].length = 1;
        dictionary[i].code = i;
    }
}

static void free_dictionary() {
    for (int i = 0; i < dict_size; i++) {
        free(dictionary[i].data);
    }
}

static int find_in_dict(const unsigned char *seq, int len) {
    for (int i = 0; i < dict_size; i++) {
        if (dictionary[i].length == len && 
            memcmp(dictionary[i].data, seq, len) == 0) {
            return i;
        }
    }
    return -1;
}

static int add_to_dict(const unsigned char *seq, int len) {
    if (dict_size >= DICT_SIZE) {
        return -1;
    }
    
    dictionary[dict_size].data = malloc(len);
    if (!dictionary[dict_size].data) {
        return -1;
    }
    memcpy(dictionary[dict_size].data, seq, len);
    dictionary[dict_size].length = len;
    dictionary[dict_size].code = dict_size;
    dict_size++;
    return dict_size - 1;
}

static void write_code(int fd, int code) {
    // Escribir código como 2 bytes (big-endian)
    unsigned char bytes[2];
    bytes[0] = (code >> 8) & 0xFF;
    bytes[1] = code & 0xFF;
    write(fd, bytes, 2);
}

static int read_code(int fd) {
    unsigned char bytes[2];
    if (read(fd, bytes, 2) != 2) {
        return -1;
    }
    return (bytes[0] << 8) | bytes[1];
}

int lzw_compress(const char *input_path, const char *output_path) {
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
    
    init_dictionary();
    
    unsigned char buffer[BUFFER_SIZE];
    unsigned char *current_seq = malloc(BUFFER_SIZE);
    int seq_len = 0;
    ssize_t bytes_read;
    int pos = 0;
    int remaining = 0;
    
    while (1) {
        if (pos >= remaining) {
            bytes_read = read(fd_in, buffer, BUFFER_SIZE);
            if (bytes_read <= 0) break;
            remaining = bytes_read;
            pos = 0;
        }
        
        current_seq[seq_len] = buffer[pos++];
        seq_len++;
        
        int code = find_in_dict(current_seq, seq_len);
        if (code == -1) {
            // Secuencia no encontrada, escribir código anterior
            if (seq_len > 1) {
                int prev_code = find_in_dict(current_seq, seq_len - 1);
                write_code(fd_out, prev_code);
                
                // Agregar nueva secuencia al diccionario
                if (dict_size < DICT_SIZE) {
                    add_to_dict(current_seq, seq_len);
                }
                
                // Empezar nueva secuencia con el último byte
                current_seq[0] = current_seq[seq_len - 1];
                seq_len = 1;
            }
        }
    }
    
    // Escribir último código
    if (seq_len > 0) {
        int code = find_in_dict(current_seq, seq_len);
        if (code >= 0) {
            write_code(fd_out, code);
        }
    }
    
    free(current_seq);
    free_dictionary();
    close(fd_in);
    close(fd_out);
    return 0;
}

int lzw_decompress(const char *input_path, const char *output_path) {
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
    
    init_dictionary();
    
    int prev_code = read_code(fd_in);
    if (prev_code < 0 || prev_code >= dict_size) {
        close(fd_in);
        close(fd_out);
        free_dictionary();
        return -1;
    }
    
    write(fd_out, dictionary[prev_code].data, dictionary[prev_code].length);
    
    unsigned char *prev_seq = malloc(BUFFER_SIZE);
    int prev_len = dictionary[prev_code].length;
    memcpy(prev_seq, dictionary[prev_code].data, prev_len);
    
    while (1) {
        int code = read_code(fd_in);
        if (code < 0) break;
        
        if (code < dict_size) {
            write(fd_out, dictionary[code].data, dictionary[code].length);
            
            // Agregar nueva secuencia: prev_seq + primer byte de code
            if (dict_size < DICT_SIZE) {
                unsigned char *new_seq = malloc(prev_len + 1);
                memcpy(new_seq, prev_seq, prev_len);
                new_seq[prev_len] = dictionary[code].data[0];
                add_to_dict(new_seq, prev_len + 1);
                free(new_seq);
            }
            
            // Actualizar prev_seq
            free(prev_seq);
            prev_len = dictionary[code].length;
            prev_seq = malloc(prev_len);
            memcpy(prev_seq, dictionary[code].data, prev_len);
        } else {
            // Caso especial: código no está en diccionario aún
            unsigned char *new_seq = malloc(prev_len + 1);
            memcpy(new_seq, prev_seq, prev_len);
            new_seq[prev_len] = prev_seq[0];
            write(fd_out, new_seq, prev_len + 1);
            
            if (dict_size < DICT_SIZE) {
                add_to_dict(new_seq, prev_len + 1);
            }
            
            free(prev_seq);
            prev_len = prev_len + 1;
            prev_seq = new_seq;
        }
    }
    
    free(prev_seq);
    free_dictionary();
    close(fd_in);
    close(fd_out);
    return 0;
}

