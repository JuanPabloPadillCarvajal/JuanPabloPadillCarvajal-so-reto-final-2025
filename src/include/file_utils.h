#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "cli_parser.h"

int is_directory(const char *path);
int ensure_output_directory(const char *path);
int process_path_concurrent(const char *input_path, const char *output_path,
                            gsea_options_t *opts);

#endif
