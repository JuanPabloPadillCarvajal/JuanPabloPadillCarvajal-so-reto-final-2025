# Guía de Pruebas para GSEA

Este directorio contiene archivos y scripts para probar la funcionalidad de GSEA.

## Archivos de Prueba

Los archivos de prueba están en `test_files/`:
- `test1.txt`: Archivo de texto simple
- `test2.txt`: Archivo con repeticiones (ideal para RLE)
- `test3.txt`: Archivo de texto más largo

## Ejecutar Pruebas Automáticas

Para ejecutar todas las pruebas automáticamente:

```bash
cd tests
./test_script.sh
```

Este script prueba:
1. Compresión y descompresión con RLE
2. Compresión y descompresión con Huffman
3. Compresión y descompresión con LZW
4. Encriptación y desencriptación con Vigenère
5. Encriptación y desencriptación con DES
6. Operaciones combinadas (comprimir + encriptar)
7. Procesamiento de directorios completos

## Pruebas Manuales

### 1. Compresión Básica

```bash
# Comprimir con RLE
../gsea -c --comp-alg rle -i test_files/test2.txt -o output/test2_rle.comp

# Descomprimir
../gsea -d --comp-alg rle -i output/test2_rle.comp -o output/test2_decomp.txt

# Verificar que coinciden
diff test_files/test2.txt output/test2_decomp.txt
```

### 2. Compresión con Huffman

```bash
../gsea -c --comp-alg huffman -i test_files/test1.txt -o output/test1_huff.comp
../gsea -d --comp-alg huffman -i output/test1_huff.comp -o output/test1_huff_decomp.txt
diff test_files/test1.txt output/test1_huff_decomp.txt
```

### 3. Compresión con LZW

```bash
../gsea -c --comp-alg lzw -i test_files/test3.txt -o output/test3_lzw.comp
../gsea -d --comp-alg lzw -i output/test3_lzw.comp -o output/test3_lzw_decomp.txt
diff test_files/test3.txt output/test3_lzw_decomp.txt
```

### 4. Encriptación con Vigenère

```bash
../gsea -e --enc-alg vigenere -i test_files/test1.txt -o output/test1_vig.enc -k "mi_clave"
../gsea -u --enc-alg vigenere -i output/test1_vig.enc -o output/test1_vig_dec.txt -k "mi_clave"
diff test_files/test1.txt output/test1_vig_dec.txt
```

### 5. Encriptación con DES

```bash
../gsea -e --enc-alg des -i test_files/test2.txt -o output/test2_des.enc -k "clave123"
../gsea -u --enc-alg des -i output/test2_des.enc -o output/test2_des_dec.txt -k "clave123"
diff test_files/test2.txt output/test2_des_dec.txt
```

### 6. Operación Combinada (Caso de Uso del Examen)

```bash
# Comprimir y encriptar (como en el ejemplo de biotecnología)
../gsea -ce --comp-alg lzw --enc-alg des -i test_files/ -o output/archivados.bak -k "G3n0m3S3cur1ty!"

# Desencriptar y descomprimir (orden inverso)
../gsea -ud --enc-alg des --comp-alg lzw -i output/archivados.bak/ -o output/restaurados/ -k "G3n0m3S3cur1ty!"
```

### 7. Procesar Directorio Completo (Concurrencia)

```bash
# Comprimir todos los archivos de un directorio
../gsea -c --comp-alg rle -i test_files/ -o output/dir_compressed/

# Verificar que se procesaron todos los archivos
ls -lh output/dir_compressed/
```

## Verificar Resultados

Los archivos procesados se guardan en `output/`. Puedes comparar:

```bash
# Ver tamaño de archivos originales vs comprimidos
ls -lh test_files/
ls -lh output/*.comp

# Verificar integridad comparando archivos originales y descomprimidos
diff test_files/test1.txt output/test1_decomp.txt
```

## Notas

- Los archivos de salida se guardan en `output/`
- Asegúrate de que el ejecutable `gsea` esté en el directorio raíz del proyecto
- Las claves para encriptación son sensibles a mayúsculas/minúsculas
- Para operaciones combinadas, el orden es: comprimir → encriptar (al comprimir y encriptar)
- Para deshacer: desencriptar → descomprimir (al desencriptar y descomprimir)
