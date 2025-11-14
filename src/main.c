\
    #include <stdio.h>
    #include "include/cli_parser.h"
    #include "include/file_utils.h"

    int main(int argc, char *argv[]) {
        gsea_options_t opts;

        if (parse_arguments(argc, argv, &opts) != 0) {
            return 1;
        }

        printf("GSEA DEMO - opciones recibidas:\\n");
        printf("  Entrada: %s\\n", opts.input_path);
        printf("  Salida:  %s\\n", opts.output_path);
        printf("  Compresión: c=%d d=%d alg=%s\\n",
               opts.do_compress, opts.do_decompress,
               opts.comp_alg ? opts.comp_alg : "(ninguno)");
        printf("  Encriptación: e=%d u=%d alg=%s\\n",
               opts.do_encrypt, opts.do_decrypt,
               opts.enc_alg ? opts.enc_alg : "(ninguno)");
        printf("  Clave: %s\\n", opts.key ? opts.key : "(no aplica)");

        if (process_path_concurrent(opts.input_path, opts.output_path, &opts) != 0) {
            fprintf(stderr, "Error procesando ruta\\n");
            return 1;
        }

        printf("Procesamiento DEMO finalizado.\\n");
        return 0;
    }
