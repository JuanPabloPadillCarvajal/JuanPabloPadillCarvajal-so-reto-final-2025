\
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
        printf("  --comp-alg ALG   Algoritmo de compresión (ej: rle_demo)\n");
        printf("  --enc-alg  ALG   Algoritmo de encriptación (ej: xor_demo)\n");
        printf("  -i RUTA          Archivo o directorio de entrada\n");
        printf("  -o RUTA          Archivo o directorio de salida\n");
        printf("  -k CLAVE         Clave secreta\n");
    }

    int parse_arguments(int argc, char *argv[], gsea_options_t *opts) {
        memset(opts, 0, sizeof(*opts));

        if (argc < 2) {
            print_usage(argv[0]);
            return -1;
        }

        for (int i = 1; i < argc; ++i) {
            char *arg = argv[i];

            if (strcmp(arg, "-c") == 0) {
                opts->do_compress = 1;
            } else if (strcmp(arg, "-d") == 0) {
                opts->do_decompress = 1;
            } else if (strcmp(arg, "-e") == 0) {
                opts->do_encrypt = 1;
            } else if (strcmp(arg, "-u") == 0) {
                opts->do_decrypt = 1;
            } else if (strcmp(arg, "--comp-alg") == 0) {
                opts->comp_alg = argv[++i];
            } else if (strcmp(arg, "--enc-alg") == 0) {
                opts->enc_alg = argv[++i];
            } else if (strcmp(arg, "-i") == 0) {
                opts->input_path = argv[++i];
            } else if (strcmp(arg, "-o") == 0) {
                opts->output_path = argv[++i];
            } else if (strcmp(arg, "-k") == 0) {
                opts->key = argv[++i];
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
