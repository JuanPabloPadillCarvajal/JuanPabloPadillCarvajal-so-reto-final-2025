# GSEA - Utilidad de Gestión Segura y Eficiente de Archivos

Examen Final Práctico — Sistemas Operativos (Universidad EAFIT)

Este proyecto implementa una utilidad de *línea de comandos en C* para:
- Comprimir / descomprimir archivos o directorios completos.
- Encriptar / desencriptar archivos o directorios completos.
- Procesar archivos de forma *concurrente* (hilos o procesos).
- Usar *llamadas directas al sistema operativo* para manejo de archivos.

## Ejecución general (propuesta)

bash
./gsea [operaciones] --comp-alg <algoritmo_compresion> --enc-alg <algoritmo_encriptacion>            -i <ruta_entrada> -o <ruta_salida> [-k clave_secreta]


Donde:

- Operaciones:
  - -c : comprimir
  - -d : descomprimir
  - -e : encriptar
  - -u : desencriptar
  - Se pueden combinar, por ejemplo -ce para comprimir y luego encriptar.

- Algoritmos de compresión sugeridos: huffman, rle, lzw.
- Algoritmos de encriptación sugeridos: vigenere, des, aes_simple.

## Estructura del repositorio

- src/       — Código fuente en C.
- src/include/ — Archivos de cabecera (.h).
- docs/      — Documentos de diseño, justificación e informe final.
- tests/     — Casos de prueba y archivos de ejemplo.
- video/     — Material relacionado con el video de sustentación.
- Makefile   — Script de compilación para generar el ejecutable gsea.

## Integrantes

- Juan Pablo Padilla Carvajal
- Santiago Ramirez Ramirez
- Sebastian Acosta
- Samuel


## Compilación rápida

En una terminal dentro del proyecto:

bash
make
./gsea
