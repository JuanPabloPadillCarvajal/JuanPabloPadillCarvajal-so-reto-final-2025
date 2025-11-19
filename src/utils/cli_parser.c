#include <stdio.h>
#include <string.h>
#include "../include/cli_parser.h"

static void print_usage(const char *prog_name) {
    printf("Uso:\n");
    printf("  %s [opciones] -i <entrada> -o <salida> [-k clave]\n\n", prog_name);
    printf("Operaciones:\n");
    printf("  -c               Comprimir\n");
    printf("  -d               Descomprimir\n");
    printf("  -e               Encriptar\n");
    printf("  -u               Desencriptar\n");
    printf("  Se pueden combinar (ej: -ce para comprimir y encriptar)\n\n");
    printf("Algoritmos:\n");
    printf("  --comp-alg ALG   Algoritmo de compresión: rle, huffman, lzw\n");
    printf("  --enc-alg  ALG   Algoritmo de encriptación: vigenere, des\n");
    printf("  -i RUTA          Archivo o directorio de entrada\n");
    printf("  -o RUTA          Archivo o directorio de salida\n");
    printf("  -k CLAVE         Clave secreta (requerida para encriptación)\n");
}

int parse_arguments(int argc, char *argv[], gsea_options_t *opts) {
    memset(opts, 0, sizeof(*opts));

    if (argc < 2) {
        print_usage(argv[0]);
        return -1;
    }

    int i = 1;
    while (i < argc) {
        char *arg = argv[i];

        // Verificar opciones con argumentos primero
        if (strcmp(arg, "--comp-alg") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "ERROR: --comp-alg requiere un argumento\n");
                return -1;
            }
            opts->comp_alg = argv[i + 1];
            i += 2;
            continue;
        } else if (strcmp(arg, "--enc-alg") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "ERROR: --enc-alg requiere un argumento\n");
                return -1;
            }
            opts->enc_alg = argv[i + 1];
            i += 2;
            continue;
        } else if (strcmp(arg, "-i") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "ERROR: -i requiere un argumento\n");
                return -1;
            }
            opts->input_path = argv[i + 1];
            i += 2;
            continue;
        } else if (strcmp(arg, "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "ERROR: -o requiere un argumento\n");
                return -1;
            }
            opts->output_path = argv[i + 1];
            i += 2;
            continue;
        } else if (strcmp(arg, "-k") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "ERROR: -k requiere un argumento\n");
                return -1;
            }
            opts->key = argv[i + 1];
            i += 2;
            continue;
        } else if (strcmp(arg, "-c") == 0) {
            opts->do_compress = 1;
            i++;
        } else if (strcmp(arg, "-d") == 0) {
            opts->do_decompress = 1;
            i++;
        } else if (strcmp(arg, "-e") == 0) {
            opts->do_encrypt = 1;
            i++;
        } else if (strcmp(arg, "-u") == 0) {
            opts->do_decrypt = 1;
            i++;
        } else if (arg[0] == '-' && arg[1] != '-' && strlen(arg) > 1) {
            // Manejar operaciones combinadas como -ce, -du, etc.
            for (int j = 1; arg[j] != '\0'; j++) {
                if (arg[j] == 'c') {
                    opts->do_compress = 1;
                } else if (arg[j] == 'd') {
                    opts->do_decompress = 1;
                } else if (arg[j] == 'e') {
                    opts->do_encrypt = 1;
                } else if (arg[j] == 'u') {
                    opts->do_decrypt = 1;
                }
            }
            i++;
        } else {
            fprintf(stderr, "ERROR: opción desconocida: %s\n", arg);
            print_usage(argv[0]);
            return -1;
        }
    }

    if (!opts->input_path || !opts->output_path) {
        fprintf(stderr, "ERROR: debes especificar -i y -o\n");
        return -1;
    }

    return 0;
}
