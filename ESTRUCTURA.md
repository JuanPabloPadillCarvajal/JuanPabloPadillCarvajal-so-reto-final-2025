# Estructura del Proyecto GSEA

## Organización del Código

```
reto final/
├── Makefile              # Script de compilación
├── README.md             # Documentación principal
├── COMO_PROBAR.md        # Guía de pruebas
├── ESTRUCTURA.md         # Este archivo
├── .gitignore           # Archivos a ignorar en git
├── gsea                 # Ejecutable (generado por make)
│
├── src/                  # Código fuente
│   ├── main.c           # Punto de entrada principal
│   │
│   ├── include/         # Archivos de cabecera (.h)
│   │   ├── cli_parser.h      # Parser de argumentos de línea de comandos
│   │   ├── file_utils.h      # Utilidades para manejo de archivos
│   │   ├── compression.h     # Interfaz de algoritmos de compresión
│   │   └── encryption.h      # Interfaz de algoritmos de encriptación
│   │
│   ├── utils/           # Utilidades generales
│   │   ├── cli_parser.c      # Implementación del parser CLI
│   │   └── file_utils.c      # Implementación de gestión de archivos y concurrencia
│   │
│   ├── compression/     # Algoritmos de compresión
│   │   ├── compression.c    # Router que selecciona el algoritmo
│   │   ├── rle.c            # Run-Length Encoding
│   │   ├── huffman.c        # Codificación de Huffman
│   │   └── lzw.c            # Lempel-Ziv-Welch
│   │
│   └── encryption/      # Algoritmos de encriptación
│       ├── encryption.c      # Router que selecciona el algoritmo
│       ├── vigenere.c        # Cifrado de Vigenère
│       └── des.c            # Data Encryption Standard (simplificado)
│
└── tests/                # Pruebas y archivos de ejemplo
    ├── README.md        # Documentación de pruebas
    ├── test_script.sh   # Script de pruebas automáticas
    └── test_files/      # Archivos de prueba
        ├── test1.txt
        ├── test2.txt
        └── test3.txt
```

## Descripción de Componentes

### Punto de Entrada (`main.c`)
- Valida argumentos de línea de comandos
- Coordina el procesamiento de archivos/directorios
- Maneja la lógica de operaciones combinadas

### Parser CLI (`utils/cli_parser.c`)
- Parsea argumentos de línea de comandos
- Soporta operaciones combinadas (ej: `-ce`)
- Valida que se proporcionen todos los argumentos requeridos

### Gestión de Archivos (`utils/file_utils.c`)
- Detecta si la entrada es archivo o directorio
- Implementa procesamiento concurrente con pthreads
- Usa llamadas directas al sistema (open, read, write, close)
- Maneja operaciones combinadas con archivos temporales

### Algoritmos de Compresión

#### RLE (`compression/rle.c`)
- Run-Length Encoding básico
- Efectivo para datos con muchas repeticiones
- Implementación simple y rápida

#### Huffman (`compression/huffman.c`)
- Codificación basada en frecuencias
- Construye árbol de Huffman
- Eficiente para datos con distribución no uniforme

#### LZW (`compression/lzw.c`)
- Compresión por diccionario
- Muy efectivo para datos repetitivos (como genéticos)
- Implementación con diccionario dinámico

### Algoritmos de Encriptación

#### Vigenère (`encryption/vigenere.c`)
- Cifrado polialfabético
- Usa clave repetida
- Implementación simple

#### DES (`encryption/des.c`)
- Data Encryption Standard (versión simplificada)
- Implementa S-boxes, permutaciones y rondas de Feistel
- Bloque de 8 bytes

## Características Técnicas

### Llamadas al Sistema
- **NO** usa `<stdio.h>` (fopen, fread, fwrite)
- Usa directamente: `open()`, `read()`, `write()`, `close()`
- Usa `opendir()`, `readdir()` para directorios

### Concurrencia
- Implementada con `pthreads`
- Un hilo por archivo al procesar directorios
- Máximo 64 hilos simultáneos
- Gestión adecuada del ciclo de vida (creación y join)

### Algoritmos Propios
- Todos los algoritmos implementados desde cero
- **NO** usa librerías externas (zlib, OpenSSL, etc.)
- Implementaciones educativas pero funcionales

## Compilación

```bash
make clean    # Limpiar archivos compilados
make          # Compilar el proyecto
```

El ejecutable `gsea` se genera en el directorio raíz.

## Requisitos

- Compilador GCC con soporte C11
- Sistema operativo Linux/Unix (POSIX)
- Librería pthread (incluida en sistemas POSIX)

