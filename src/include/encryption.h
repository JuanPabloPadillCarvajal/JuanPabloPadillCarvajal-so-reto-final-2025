#ifndef ENCRYPTION_H
#define ENCRYPTION_H

int encrypt_file(const char *input_path, const char *output_path,
                 const char *algorithm, const char *key);
int decrypt_file(const char *input_path, const char *output_path,
                 const char *algorithm, const char *key);

#endif
