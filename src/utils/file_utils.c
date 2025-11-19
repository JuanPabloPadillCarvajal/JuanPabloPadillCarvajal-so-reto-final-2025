#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#include "../include/cli_parser.h"
#include "../include/file_utils.h"
#include "../include/compression.h"
#include "../include/encryption.h"

int is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return 0;
    }
    return S_ISDIR(st.st_mode);
}

static void build_output_path(char *buffer, size_t buflen,
                              const char *out_dir, const char *filename) {
    snprintf(buffer, buflen, "%s/%s", out_dir, filename);
}

int ensure_output_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr, "La ruta de salida existe pero no es un directorio\n");
            return -1;
        }
        return 0;
    }
    int res = mkdir(path, 0755);
    if (res != 0) {
        perror("mkdir");
        return -1;
    }
    return 0;
}

static int process_single_file(const char *input_path, const char *output_path,
                               gsea_options_t *opts) {
    char temp_in[512];
    char temp_out[512];
    char temp_intermediate[512];
    strncpy(temp_in, input_path, sizeof(temp_in)-1);
    temp_in[sizeof(temp_in)-1] = '\0';
    strncpy(temp_out, output_path, sizeof(temp_out)-1);
    temp_out[sizeof(temp_out)-1] = '\0';
    
    // Manejo de archivos temporales para operaciones combinadas
    int needs_temp = (opts->do_compress && opts->do_encrypt) || 
                     (opts->do_decompress && opts->do_decrypt);
    
    const char *current_input = temp_in;
    const char *current_output = temp_out;
    
    if (needs_temp) {
        int len = snprintf(temp_intermediate, sizeof(temp_intermediate) - 1, "%s.tmp", temp_out);
        if (len < 0 || len >= (int)sizeof(temp_intermediate)) {
            fprintf(stderr, "Error: ruta de salida demasiado larga\n");
            return -1;
        }
        temp_intermediate[sizeof(temp_intermediate) - 1] = '\0';
        current_output = temp_intermediate;
    }
    
    // Procesar compresión/descompresión primero
    if (opts->do_compress) {
        if (compress_file(current_input, current_output, opts->comp_alg) != 0) {
            fprintf(stderr, "Fallo al comprimir %s\n", current_input);
            return -1;
        }
        current_input = current_output;
        if (opts->do_encrypt) {
            current_output = temp_out;
        }
    } else if (opts->do_decompress) {
        if (decompress_file(current_input, current_output, opts->comp_alg) != 0) {
            fprintf(stderr, "Fallo al descomprimir %s\n", current_input);
            return -1;
        }
        current_input = current_output;
        if (opts->do_decrypt) {
            current_output = temp_out;
        }
    }
    
    // Procesar encriptación/desencriptación
    if (opts->do_encrypt) {
        if (encrypt_file(current_input, current_output, opts->enc_alg, opts->key) != 0) {
            fprintf(stderr, "Fallo al encriptar %s\n", current_input);
            if (needs_temp) unlink(temp_intermediate);
            return -1;
        }
    } else if (opts->do_decrypt) {
        if (decrypt_file(current_input, current_output, opts->enc_alg, opts->key) != 0) {
            fprintf(stderr, "Fallo al desencriptar %s\n", current_input);
            if (needs_temp) unlink(temp_intermediate);
            return -1;
        }
    }
    
    // Eliminar archivo temporal si fue creado
    if (needs_temp && strcmp(current_output, temp_intermediate) != 0) {
        unlink(temp_intermediate);
    }
    
    printf("Procesado: %s -> %s\n", input_path, output_path);
    return 0;
}

typedef struct {
    char input_path[512];
    char output_path[512];
    gsea_options_t opts;
} thread_arg_t;

static void *thread_worker(void *arg) {
    thread_arg_t *t = (thread_arg_t *)arg;
    process_single_file(t->input_path, t->output_path, &t->opts);
    free(t);
    return NULL;
}

int process_path_concurrent(const char *input_path, const char *output_path,
                            gsea_options_t *opts) {
    if (!is_directory(input_path)) {
        return process_single_file(input_path, output_path, opts);
    }
    
    if (ensure_output_directory(output_path) != 0) {
        return -1;
    }
    
    DIR *dir = opendir(input_path);
    if (!dir) {
        perror("opendir");
        return -1;
    }
    
    struct dirent *entry;
    char in_buf[512];
    char out_buf[512];
    pthread_t threads[64];
    int thread_count = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(in_buf, sizeof(in_buf), "%s/%s", input_path, entry->d_name);
        if (is_directory(in_buf)) {
            printf("Omitiendo subdirectorio: %s\n", in_buf);
            continue;
        }
        build_output_path(out_buf, sizeof(out_buf), output_path, entry->d_name);
        
        thread_arg_t *t = malloc(sizeof(thread_arg_t));
        if (!t) {
            perror("malloc");
            continue;
        }
        strncpy(t->input_path, in_buf, sizeof(t->input_path)-1);
        t->input_path[sizeof(t->input_path)-1] = '\0';
        strncpy(t->output_path, out_buf, sizeof(t->output_path)-1);
        t->output_path[sizeof(t->output_path)-1] = '\0';
        t->opts = *opts;
        
        if (pthread_create(&threads[thread_count++], NULL, thread_worker, t) != 0) {
            perror("pthread_create");
            free(t);
            thread_count--;
        }
        
        if (thread_count >= 64) break;
    }
    
    closedir(dir);
    
    for (int i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
