\
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../include/compression.h"

    /* Algoritmo DEMO de compresión: RLE muy simple.
       Debe ser revisado y mejorado para la entrega final. */

    int compress_file_demo(const char *input_path, const char *output_path, const char *algorithm) {
        (void)algorithm;
        FILE *in = fopen(input_path, "rb");
        if (!in) { perror("fopen input"); return -1; }
        FILE *out = fopen(output_path, "wb");
        if (!out) { perror("fopen output"); fclose(in); return -1; }

        int prev = fgetc(in);
        if (prev == EOF) {
            fclose(in); fclose(out);
            return 0;
        }
        int count = 1;
        int c;
        while ((c = fgetc(in)) != EOF) {
            if (c == prev && count < 255) {
                count++;
            } else {
                fputc(prev, out);
                fputc(count, out);
                prev = c;
                count = 1;
            }
        }
        fputc(prev, out);
        fputc(count, out);

        fclose(in);
        fclose(out);
        return 0;
    }

    int decompress_file_demo(const char *input_path, const char *output_path, const char *algorithm) {
        (void)algorithm;
        FILE *in = fopen(input_path, "rb");
        if (!in) { perror("fopen input"); return -1; }
        FILE *out = fopen(output_path, "wb");
        if (!out) { perror("fopen output"); fclose(in); return -1; }

        int value, count;
        while ((value = fgetc(in)) != EOF) {
            count = fgetc(in);
            if (count == EOF) break;
            for (int i = 0; i < count; ++i) {
                fputc(value, out);
            }
        }

        fclose(in);
        fclose(out);
        return 0;
    }
