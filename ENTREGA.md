# Checklist de Entrega - Proyecto GSEA

## Archivos Incluidos

### Código Fuente
- [x] `src/main.c` - Punto de entrada principal
- [x] `src/utils/cli_parser.c` - Parser de argumentos CLI
- [x] `src/utils/file_utils.c` - Gestión de archivos y concurrencia
- [x] `src/compression/compression.c` - Router de compresión
- [x] `src/compression/rle.c` - Algoritmo RLE
- [x] `src/compression/huffman.c` - Algoritmo Huffman
- [x] `src/compression/lzw.c` - Algoritmo LZW
- [x] `src/encryption/encryption.c` - Router de encriptación
- [x] `src/encryption/vigenere.c` - Algoritmo Vigenère
- [x] `src/encryption/des.c` - Algoritmo DES

### Headers
- [x] `src/include/cli_parser.h`
- [x] `src/include/file_utils.h`
- [x] `src/include/compression.h`
- [x] `src/include/encryption.h`

### Build System
- [x] `Makefile` - Script de compilación

### Documentación
- [x] `README.md` - Documentación principal
- [x] `ESTRUCTURA.md` - Estructura del proyecto
- [x] `COMO_PROBAR.md` - Guía de pruebas
- [x] `tests/README.md` - Documentación de pruebas

### Pruebas
- [x] `tests/test_script.sh` - Script de pruebas automáticas
- [x] `tests/test_files/` - Archivos de prueba

### Configuración
- [x] `.gitignore` - Archivos a ignorar

## Requisitos Cumplidos

### Funcionalidades
- [x] Comprimir/descomprimir archivos y directorios
- [x] Encriptar/desencriptar archivos y directorios
- [x] Operaciones combinadas (ej: comprimir y encriptar)
- [x] Procesamiento concurrente con pthreads
- [x] Llamadas directas al sistema (open, read, write, close)
- [x] Soporte para archivos individuales y directorios completos

### Algoritmos Implementados
- [x] **Compresión**: RLE, Huffman, LZW (desde cero)
- [x] **Encriptación**: Vigenère, DES (desde cero)
- [x] Sin uso de librerías externas para compresión/encriptación

### Calidad del Código
- [x] Código limpio y organizado
- [x] Sin código de debug
- [x] Sin archivos temporales
- [x] Estructura modular y clara
- [x] Comentarios apropiados

## Para Entregar

1. **Comprimir el proyecto** (sin el ejecutable ni archivos temporales):
   ```bash
   make clean
   tar -czf gsea_entrega.tar.gz --exclude='gsea' --exclude='*.o' --exclude='tests/output' .
   ```

2. **O simplemente entregar el directorio** después de ejecutar `make clean`

## Verificación Final

Antes de entregar, verificar:

```bash
# 1. Limpiar
make clean

# 2. Compilar
make

# 3. Probar funcionalidad básica
./gsea -c --comp-alg rle -i tests/test_files/test2.txt -o test.comp
./gsea -d --comp-alg rle -i test.comp -o test_decomp.txt
diff tests/test_files/test2.txt test_decomp.txt  # Debe estar vacío

# 4. Limpiar archivos de prueba
rm -f test.comp test_decomp.txt
make clean
```

## Notas

- El ejecutable `gsea` se genera con `make`
- Todos los algoritmos están implementados desde cero
- El código usa llamadas directas al sistema (no stdio.h para archivos)
- La concurrencia está implementada con pthreads
- El proyecto está listo para compilar y ejecutar en Linux/Unix

