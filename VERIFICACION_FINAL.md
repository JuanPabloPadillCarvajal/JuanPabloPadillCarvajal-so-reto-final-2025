# Verificación Final - Proyecto GSEA

## Checklist de Requisitos del Examen

### Funcionalidades Implementadas

- [x] Comprimir/descomprimir archivos o directorios
  - Implementado con 3 algoritmos: RLE, Huffman, LZW
  - Soporta archivos individuales y directorios completos

- [x] Encriptar/desencriptar archivos o directorios
  - Implementado con 2 algoritmos: Vigenère, DES
  - Soporta archivos individuales y directorios completos

- [x] Procesamiento concurrente
  - Implementado con pthreads
  - Un hilo por archivo al procesar directorios
  - Gestión adecuada del ciclo de vida de hilos

- [x] Llamadas directas al sistema
  - NO usa `<stdio.h>` para archivos (fopen, fread, fwrite, fclose)
  - Usa `open()`, `read()`, `write()`, `close()` directamente
  - Usa `opendir()`, `readdir()` para directorios

- [x] Algoritmos propios implementados desde cero
  - Sin librerías externas (zlib, OpenSSL, etc.)
  - Todos los algoritmos implementados manualmente

### Algoritmos de Compresión (3/3 sugeridos)

- [x] Run-Length Encoding (RLE) - Implementado
- [x] Codificación Huffman - Implementado
- [x] Lempel-Ziv-Welch (LZW) - Implementado

### Algoritmos de Encriptación (2/3 sugeridos)

- [x] Cifrado Vigenère - Implementado
- [x] DES - Implementado (con S-boxes, permutaciones, rondas de Feistel)
- [ ] AES simplificado - No implementado (opcional)

### Operaciones Combinadas

- [x] Soporta combinaciones como `-ce` (comprimir y encriptar)
- [x] Manejo correcto de orden: comprimir → encriptar / desencriptar → descomprimir

### Parser CLI

- [x] Soporta operaciones combinadas (`-ce`, `-du`, etc.)
- [x] Validación de argumentos
- [x] Mensajes de error claros

## Estructura del Proyecto

```
reto final/
├── Makefile              Script de compilación
├── README.md             Documentación principal
├── COMO_PROBAR.md        Guía de uso completa
├── ESTRUCTURA.md         Estructura del proyecto
├── ENTREGA.md            Checklist de entrega
├── .gitignore           Archivos a ignorar
├── src/                  Código fuente (14 archivos)
│   ├── main.c
│   ├── include/         (4 headers)
│   ├── utils/           (2 archivos)
│   ├── compression/     (4 archivos: 3 algoritmos + router)
│   └── encryption/       (3 archivos: 2 algoritmos + router)
└── tests/                Pruebas y archivos de ejemplo
```

## Verificación Técnica

### Compilación
- Compila sin errores ni warnings
- Makefile correcto
- Flags apropiados (-Wall -Wextra -pthread)

### Código
- Sin código de debug
- Sin funciones no utilizadas
- Sin includes innecesarios
- Sin código comentado innecesario
- Estructura modular y clara

### Pruebas
- RLE funciona correctamente
- Huffman funciona correctamente
- LZW funciona correctamente
- Vigenère funciona correctamente
- DES funciona correctamente
- Operaciones combinadas funcionan
- Procesamiento de directorios funciona

### Documentación
- README.md completo
- Guía de uso detallada
- Estructura documentada
- Ejemplos de uso

## Estado Final

PROYECTO LISTO PARA ENTREGAR

Todos los requisitos del examen están implementados y verificados.

