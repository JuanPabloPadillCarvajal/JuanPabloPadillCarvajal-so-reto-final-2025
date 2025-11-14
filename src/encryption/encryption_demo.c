\
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../include/encryption.h"

    /* Algoritmo DEMO de encriptación: XOR con clave de texto.
       Es un ejemplo sencillo y débil, ideal para pruebas pero NO para producción. */

    static void xor_buffer(unsigned char *buf, size_t len, const char *key) {
        size_t key_len = strlen(key);
        for (size_t i = 0; i < len; ++i) {
            buf[i] ^= key[i % key_len];
        }
    }

    static int xor_file(const char *input_path, const char *output_path, const char *key) {
        FILE *in = fopen(input_path, "rb");
        if (!in) { perror("fopen input"); return -1; }
        FILE *out = fopen(output_path, "wb");
        if (!out) { perror("fopen output"); fclose(in); return -1; }

        unsigned char buffer[4096];
        size_t n;
        while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0) {
            xor_buffer(buffer, n, key);
            fwrite(buffer, 1, n, out);
        }
 
        fclose(in);
        fclose(out);
        return 0;
    }

    int encrypt_file_demo(const char *input_path, const char *output_path,
                          const char *algorithm, const char *key) {
        (void)algorithm;
        return xor_file(input_path, output_path, key);
    }

    int decrypt_file_demo(const char *input_path, const char *output_path,
                          const char *algorithm, const char *key) {
        (void)algorithm;
        return xor_file(input_path, output_path, key);
    }
