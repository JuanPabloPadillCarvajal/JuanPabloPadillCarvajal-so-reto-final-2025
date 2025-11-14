#ifndef CLI_PARSER_H
#define CLI_PARSER_H

typedef struct {
    int do_compress;
    int do_decompress;
    int do_encrypt;
    int do_decrypt;
    const char *input_path;
    const char *output_path;
    const char *comp_alg;
    const char *enc_alg;
    const char *key;
} gsea_options_t;

int parse_arguments(int argc, char *argv[], gsea_options_t *opts);

#endif
