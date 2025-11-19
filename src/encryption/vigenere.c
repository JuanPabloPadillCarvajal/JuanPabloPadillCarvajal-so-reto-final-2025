#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

static void prepare_key(unsigned char *key_buffer, const char *key, int key_len) {
    for (int i = 0; i < key_len; i++) {
        key_buffer[i] = (unsigned char)key[i];
    }
}

int vigenere_encrypt(const char *input_path, const char *output_path, const char *key) {
    if (!key || strlen(key) == 0) {
        fprintf(stderr, "Error: clave requerida para encriptación Vigenère\n");
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
    
    int key_len = strlen(key);
    unsigned char *key_buffer = malloc(key_len);
    prepare_key(key_buffer, key, key_len);
    
    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int key_pos = 0;
    
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            buffer[i] = (buffer[i] + key_buffer[key_pos]) % 256;
            key_pos = (key_pos + 1) % key_len;
        }
        write(fd_out, buffer, bytes_read);
    }
    
    free(key_buffer);
    close(fd_in);
    close(fd_out);
    return 0;
}

int vigenere_decrypt(const char *input_path, const char *output_path, const char *key) {
    if (!key || strlen(key) == 0) {
        fprintf(stderr, "Error: clave requerida para desencriptación Vigenère\n");
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
    
    int key_len = strlen(key);
    unsigned char *key_buffer = malloc(key_len);
    prepare_key(key_buffer, key, key_len);
    
    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int key_pos = 0;
    
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            buffer[i] = (buffer[i] - key_buffer[key_pos] + 256) % 256;
            key_pos = (key_pos + 1) % key_len;
        }
        write(fd_out, buffer, bytes_read);
    }
    
    free(key_buffer);
    close(fd_in);
    close(fd_out);
    return 0;
}

