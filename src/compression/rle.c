#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 4096

int rle_compress(const char *input_path, const char *output_path) {
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
    
    unsigned char buffer[BUFFER_SIZE];
    unsigned char prev = 0;
    int count = 0;
    ssize_t bytes_read;
    int first_byte = 1;
    
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (first_byte) {
                prev = buffer[i];
                count = 1;
                first_byte = 0;
            } else if (buffer[i] == prev && count < 255) {
                count++;
            } else {
                // Escribir el byte y su conteo
                write(fd_out, &prev, 1);
                write(fd_out, &count, 1);
                prev = buffer[i];
                count = 1;
            }
        }
    }
    
    // Escribir el último byte
    if (!first_byte) {
        write(fd_out, &prev, 1);
        write(fd_out, &count, 1);
    }
    
    close(fd_in);
    close(fd_out);
    return 0;
}

int rle_decompress(const char *input_path, const char *output_path) {
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
    
    unsigned char value, count;
    unsigned char output_buffer[256];
    
    while (read(fd_in, &value, 1) == 1) {
        if (read(fd_in, &count, 1) != 1) {
            break;
        }
        
        // Llenar buffer con el valor repetido
        memset(output_buffer, value, count);
        write(fd_out, output_buffer, count);
    }
    
    close(fd_in);
    close(fd_out);
    return 0;
}

