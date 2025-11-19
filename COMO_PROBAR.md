# Cómo Usar GSEA - Guía Completa

## Compilación

```bash
make clean
make
```

Esto generará el ejecutable `gsea` en el directorio raíz.

---

## ALGORITMOS DE COMPRESIÓN

### 1. RLE (Run-Length Encoding)

**Mejor para**: Archivos con muchas repeticiones consecutivas

#### Comprimir con RLE
```bash
./gsea -c --comp-alg rle -i archivo.txt -o archivo.comp
```

#### Descomprimir RLE
```bash
./gsea -d --comp-alg rle -i archivo.comp -o archivo_decomp.txt
```

#### Ejemplo Práctico
```bash
# Crear archivo con repeticiones
echo "AAAAABBBBBCCCCCDDDDD" > test.txt

# Comprimir
./gsea -c --comp-alg rle -i test.txt -o test.comp

# Descomprimir
./gsea -d --comp-alg rle -i test.comp -o test_decomp.txt

# Verificar
diff test.txt test_decomp.txt
```

#### Con Archivos Reales
```bash
# Comprimir un archivo de tu PC
./gsea -c --comp-alg rle -i ~/Documentos/mi_archivo.txt -o ~/Documentos/mi_archivo.comp
```

---

### 2. Huffman

**Mejor para**: Archivos de texto normales, compresión basada en frecuencias

#### Comprimir con Huffman
```bash
./gsea -c --comp-alg huffman -i archivo.txt -o archivo.huff
```

#### Descomprimir Huffman
```bash
./gsea -d --comp-alg huffman -i archivo.huff -o archivo_decomp.txt
```

#### Ejemplo Práctico
```bash
# Crear archivo de texto
echo "Este es un archivo de prueba para compresión Huffman" > test.txt

# Comprimir
./gsea -c --comp-alg huffman -i test.txt -o test.huff

# Descomprimir
./gsea -d --comp-alg huffman -i test.huff -o test_decomp.txt

# Verificar
diff test.txt test_decomp.txt
```

#### Con Archivos Reales
```bash
# Comprimir un archivo de texto de tu PC
./gsea -c --comp-alg huffman -i ~/Documentos/informe.txt -o ~/Documentos/informe.huff
```

**Nota**: Huffman funciona bien incluso con archivos sin repeticiones, comprimiendo según frecuencias de caracteres.

---

### 3. LZW (Lempel-Ziv-Welch)

**Mejor para**: Datos repetitivos, archivos genéticos, patrones largos

#### Comprimir con LZW
```bash
./gsea -c --comp-alg lzw -i archivo.txt -o archivo.lzw
```

#### Descomprimir LZW
```bash
./gsea -d --comp-alg lzw -i archivo.lzw -o archivo_decomp.txt
```

#### Ejemplo Práctico
```bash
# Crear archivo con patrones
echo "Hola mundo, este es un archivo de prueba para LZW" > test.txt

# Comprimir
./gsea -c --comp-alg lzw -i test.txt -o test.lzw

# Descomprimir
./gsea -d --comp-alg lzw -i test.lzw -o test_decomp.txt

# Verificar
diff test.txt test_decomp.txt
```

#### Con Archivos Reales
```bash
# Comprimir un archivo de tu PC
./gsea -c --comp-alg lzw -i ~/Documentos/datos_geneticos.txt -o ~/Documentos/datos.lzw
```

---

## ALGORITMOS DE ENCRIPTACIÓN

### 1. Vigenère

**Características**: Cifrado polialfabético, simple y educativo

#### Encriptar con Vigenère
```bash
./gsea -e --enc-alg vigenere -i archivo.txt -o archivo.enc -k "mi_clave_secreta"
```

#### Desencriptar Vigenère
```bash
./gsea -u --enc-alg vigenere -i archivo.enc -o archivo_dec.txt -k "mi_clave_secreta"
```

#### Ejemplo Práctico
```bash
# Crear archivo
echo "Este es un mensaje secreto" > test.txt

# Encriptar
./gsea -e --enc-alg vigenere -i test.txt -o test.enc -k "mi_clave_secreta"

# Desencriptar
./gsea -u --enc-alg vigenere -i test.enc -o test_dec.txt -k "mi_clave_secreta"

# Verificar
diff test.txt test_dec.txt
```

#### Con Archivos Reales
```bash
# Encriptar un archivo confidencial
./gsea -e --enc-alg vigenere -i ~/Documentos/confidencial.txt -o ~/Documentos/confidencial.enc -k "mi_clave_super_secreta"
```

---

### 2. DES (Data Encryption Standard)

**Características**: Algoritmo estándar con S-boxes, permutaciones y rondas de Feistel

#### Encriptar con DES
```bash
./gsea -e --enc-alg des -i archivo.txt -o archivo.des -k "clave123"
```

#### Desencriptar DES
```bash
./gsea -u --enc-alg des -i archivo.des -o archivo_dec.txt -k "clave123"
```

#### Ejemplo Práctico
```bash
# Crear archivo
echo "Mensaje confidencial" > test.txt

# Encriptar
./gsea -e --enc-alg des -i test.txt -o test.des -k "clave123"

# Desencriptar
./gsea -u --enc-alg des -i test.des -o test_dec.txt -k "clave123"

# Verificar
diff test.txt test_dec.txt
```

#### Con Archivos Reales
```bash
# Encriptar un archivo importante
./gsea -e --enc-alg des -i ~/Documentos/importante.pdf -o ~/Documentos/importante.des -k "ClaveSegura123"
```

---

## OPERACIONES COMBINADAS

### Comprimir y Encriptar

**Orden**: Primero comprime, luego encripta

```bash
./gsea -ce --comp-alg lzw --enc-alg des -i archivo.txt -o archivo.bak -k "G3n0m3S3cur1ty!"
```

#### Ejemplo: Caso de Uso del Examen (Biotecnología)

```bash
# Crear directorio de datos genéticos
mkdir -p ~/Resultados/2025-10-15
echo "Datos genéticos 1" > ~/Resultados/2025-10-15/archivo1.txt
echo "Datos genéticos 2" > ~/Resultados/2025-10-15/archivo2.txt

# Comprimir y encriptar (como en el ejemplo de biotecnología)
./gsea -ce --comp-alg lzw --enc-alg des -i ~/Resultados/2025-10-15/ -o ~/Archivados/2025-10-15.bak/ -k "G3n0m3S3cur1ty!"

# Verificar archivos procesados
ls -lh ~/Archivados/2025-10-15.bak/
```

### Desencriptar y Descomprimir

**Orden**: Primero desencripta, luego descomprime (orden inverso)

```bash
./gsea -ud --enc-alg des --comp-alg lzw -i archivo.bak -o archivo_restaurado.txt -k "G3n0m3S3cur1ty!"
```

#### Ejemplo: Restaurar Archivos

```bash
# Desencriptar y descomprimir
mkdir -p ~/Restaurados
./gsea -ud --enc-alg des --comp-alg lzw -i ~/Archivados/2025-10-15.bak/ -o ~/Restaurados/ -k "G3n0m3S3cur1ty!"

# Verificar que los archivos restaurados coinciden
diff ~/Resultados/2025-10-15/archivo1.txt ~/Restaurados/archivo1.txt
diff ~/Resultados/2025-10-15/archivo2.txt ~/Restaurados/archivo2.txt
```

---

## PROCESAMIENTO DE DIRECTORIOS (CONCURRENCIA)

Al procesar un directorio completo, GSEA crea hilos separados para cada archivo, mejorando el rendimiento en sistemas multinúcleo.

### Comprimir Directorio Completo

```bash
./gsea -c --comp-alg rle -i ~/mis_documentos/ -o ~/comprimidos/
```

#### Ejemplo Práctico

```bash
# Crear directorio con múltiples archivos
mkdir -p test_dir
echo "Archivo 1" > test_dir/file1.txt
echo "Archivo 2" > test_dir/file2.txt
echo "Archivo 3" > test_dir/file3.txt

# Comprimir todos los archivos del directorio (procesamiento concurrente)
./gsea -c --comp-alg rle -i test_dir/ -o test_dir_compressed/

# Verificar que se procesaron todos
ls -lh test_dir_compressed/
```

### Encriptar Directorio Completo

```bash
./gsea -e --enc-alg vigenere -i ~/Fotos/vacaciones/ -o ~/Fotos/vacaciones_encrypted/ -k "mi_clave_secreta"
```

---

## RESUMEN DE COMANDOS

### Compresión
```bash
# RLE
./gsea -c --comp-alg rle -i entrada -o salida
./gsea -d --comp-alg rle -i entrada -o salida

# Huffman
./gsea -c --comp-alg huffman -i entrada -o salida
./gsea -d --comp-alg huffman -i entrada -o salida

# LZW
./gsea -c --comp-alg lzw -i entrada -o salida
./gsea -d --comp-alg lzw -i entrada -o salida
```

### Encriptación
```bash
# Vigenère
./gsea -e --enc-alg vigenere -i entrada -o salida -k "clave"
./gsea -u --enc-alg vigenere -i entrada -o salida -k "clave"

# DES
./gsea -e --enc-alg des -i entrada -o salida -k "clave"
./gsea -u --enc-alg des -i entrada -o salida -k "clave"
```

### Operaciones Combinadas
```bash
# Comprimir y encriptar
./gsea -ce --comp-alg lzw --enc-alg des -i entrada -o salida -k "clave"

# Desencriptar y descomprimir
./gsea -ud --enc-alg des --comp-alg lzw -i entrada -o salida -k "clave"
```

---

## PRUEBAS AUTOMÁTICAS

En el directorio `tests/` hay un script de pruebas automáticas:

```bash
cd tests
./test_script.sh
```

Este script ejecuta todas las pruebas y verifica que los algoritmos funcionen correctamente.

---

## VER AYUDA

```bash
./gsea
```

Muestra el mensaje de uso con todas las opciones disponibles.

---

## NOTAS IMPORTANTES

1. **Orden de operaciones combinadas**: 
   - Para comprimir y encriptar: `-ce` (comprime primero, luego encripta)
   - Para desencriptar y descomprimir: `-ud` (desencripta primero, luego descomprime)

2. **Claves**: Las claves son sensibles a mayúsculas/minúsculas y son requeridas para operaciones de encriptación. Es importante guardarlas de forma segura.

3. **Directorios**: Al procesar directorios, se crean hilos separados para cada archivo, mejorando el rendimiento en sistemas multinúcleo.

4. **Algoritmos disponibles**:
   - Compresión: `rle`, `huffman`, `lzw`
   - Encriptación: `vigenere`, `des`

5. **Rutas**: Puedes usar rutas relativas (`./archivo.txt`), absolutas (`/home/usuario/archivo.txt`) o con `~` (`~/Documentos/archivo.txt`).

6. **Archivos ya comprimidos**: Los algoritmos funcionan mejor con archivos sin comprimir. Archivos como PDF, DOCX, JPG ya están comprimidos y pueden no reducir su tamaño.
