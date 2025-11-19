#include <stdio.h>
#include "include/cli_parser.h"
#include "include/file_utils.h"

int main(int argc, char *argv[]) {
    gsea_options_t opts;

    if (parse_arguments(argc, argv, &opts) != 0) {
        return 1;
    }

    // Validar que se especifiquen operaciones
    if (!opts.do_compress && !opts.do_decompress && 
        !opts.do_encrypt && !opts.do_decrypt) {
        fprintf(stderr, "Error: debe especificar al menos una operación (-c, -d, -e, -u)\n");
        return 1;
    }

    // Validar algoritmos
    if ((opts.do_compress || opts.do_decompress) && !opts.comp_alg) {
        fprintf(stderr, "Error: debe especificar --comp-alg para operaciones de compresión\n");
        return 1;
    }

    if ((opts.do_encrypt || opts.do_decrypt) && !opts.enc_alg) {
        fprintf(stderr, "Error: debe especificar --enc-alg para operaciones de encriptación\n");
        return 1;
    }

    if ((opts.do_encrypt || opts.do_decrypt) && !opts.key) {
        fprintf(stderr, "Error: debe especificar -k para operaciones de encriptación\n");
        return 1;
    }

    printf("GSEA - Procesando archivos...\n");
    printf("  Entrada: %s\n", opts.input_path);
    printf("  Salida:  %s\n", opts.output_path);
    if (opts.do_compress || opts.do_decompress) {
        printf("  Compresión: %s (%s)\n",
               opts.do_compress ? "comprimir" : "descomprimir",
               opts.comp_alg);
    }
    if (opts.do_encrypt || opts.do_decrypt) {
        printf("  Encriptación: %s (%s)\n",
               opts.do_encrypt ? "encriptar" : "desencriptar",
               opts.enc_alg);
    }

    if (process_path_concurrent(opts.input_path, opts.output_path, &opts) != 0) {
        fprintf(stderr, "Error procesando ruta\n");
        return 1;
    }

    printf("Procesamiento finalizado exitosamente.\n");
    return 0;
}
