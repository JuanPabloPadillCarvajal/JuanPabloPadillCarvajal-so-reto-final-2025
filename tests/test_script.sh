#!/bin/bash

# Script de prueba para GSEA
# Este script prueba las diferentes funcionalidades del proyecto

GSEA="../gsea"
TEST_DIR="test_files"
OUT_DIR="output"

echo "=========================================="
echo "Pruebas de GSEA - Utilidad de Compresión y Encriptación"
echo "=========================================="
echo ""

# Limpiar directorio de salida
rm -rf $OUT_DIR/*
mkdir -p $OUT_DIR

echo "1. Prueba: Comprimir con RLE"
echo "----------------------------"
$GSEA -c --comp-alg rle -i $TEST_DIR/test2.txt -o $OUT_DIR/test2_rle.comp
if [ $? -eq 0 ]; then
    echo "OK: Compresión RLE exitosa"
    ls -lh $OUT_DIR/test2_rle.comp
else
    echo "ERROR: Error en compresión RLE"
fi
echo ""

echo "2. Prueba: Descomprimir RLE"
echo "----------------------------"
$GSEA -d --comp-alg rle -i $OUT_DIR/test2_rle.comp -o $OUT_DIR/test2_rle_decomp.txt
if [ $? -eq 0 ]; then
    echo "OK: Descompresión RLE exitosa"
    if diff $TEST_DIR/test2.txt $OUT_DIR/test2_rle_decomp.txt > /dev/null; then
        echo "OK: Archivo descomprimido coincide con el original"
    else
        echo "ERROR: Archivo descomprimido NO coincide con el original"
    fi
else
    echo "ERROR: Error en descompresión RLE"
fi
echo ""

echo "3. Prueba: Comprimir con Huffman"
echo "--------------------------------"
$GSEA -c --comp-alg huffman -i $TEST_DIR/test1.txt -o $OUT_DIR/test1_huff.comp
if [ $? -eq 0 ]; then
    echo "OK: Compresión Huffman exitosa"
    ls -lh $OUT_DIR/test1_huff.comp
else
    echo "ERROR: Error en compresión Huffman"
fi
echo ""

echo "4. Prueba: Descomprimir Huffman"
echo "--------------------------------"
$GSEA -d --comp-alg huffman -i $OUT_DIR/test1_huff.comp -o $OUT_DIR/test1_huff_decomp.txt
if [ $? -eq 0 ]; then
    echo "OK: Descompresión Huffman exitosa"
    if diff $TEST_DIR/test1.txt $OUT_DIR/test1_huff_decomp.txt > /dev/null; then
        echo "OK: Archivo descomprimido coincide con el original"
    else
        echo "ERROR: Archivo descomprimido NO coincide con el original"
    fi
else
    echo "ERROR: Error en descompresión Huffman"
fi
echo ""

echo "5. Prueba: Comprimir con LZW"
echo "----------------------------"
$GSEA -c --comp-alg lzw -i $TEST_DIR/test3.txt -o $OUT_DIR/test3_lzw.comp
if [ $? -eq 0 ]; then
    echo "OK: Compresión LZW exitosa"
    ls -lh $OUT_DIR/test3_lzw.comp
else
    echo "ERROR: Error en compresión LZW"
fi
echo ""

echo "6. Prueba: Encriptar con Vigenère"
echo "----------------------------------"
$GSEA -e --enc-alg vigenere -i $TEST_DIR/test1.txt -o $OUT_DIR/test1_vig.enc -k "mi_clave_secreta"
if [ $? -eq 0 ]; then
    echo "OK: Encriptación Vigenère exitosa"
    ls -lh $OUT_DIR/test1_vig.enc
else
    echo "ERROR: Error en encriptación Vigenère"
fi
echo ""

echo "7. Prueba: Desencriptar Vigenère"
echo "----------------------------------"
$GSEA -u --enc-alg vigenere -i $OUT_DIR/test1_vig.enc -o $OUT_DIR/test1_vig_dec.txt -k "mi_clave_secreta"
if [ $? -eq 0 ]; then
    echo "OK: Desencriptación Vigenère exitosa"
    if diff $TEST_DIR/test1.txt $OUT_DIR/test1_vig_dec.txt > /dev/null; then
        echo "OK: Archivo desencriptado coincide con el original"
    else
        echo "ERROR: Archivo desencriptado NO coincide con el original"
    fi
else
    echo "ERROR: Error en desencriptación Vigenère"
fi
echo ""

echo "8. Prueba: Encriptar con DES"
echo "----------------------------"
$GSEA -e --enc-alg des -i $TEST_DIR/test2.txt -o $OUT_DIR/test2_des.enc -k "clave123"
if [ $? -eq 0 ]; then
    echo "OK: Encriptación DES exitosa"
    ls -lh $OUT_DIR/test2_des.enc
else
    echo "ERROR: Error en encriptación DES"
fi
echo ""

echo "9. Prueba: Desencriptar DES"
echo "----------------------------"
$GSEA -u --enc-alg des -i $OUT_DIR/test2_des.enc -o $OUT_DIR/test2_des_dec.txt -k "clave123"
if [ $? -eq 0 ]; then
    echo "OK: Desencriptación DES exitosa"
    if diff $TEST_DIR/test2.txt $OUT_DIR/test2_des_dec.txt > /dev/null; then
        echo "OK: Archivo desencriptado coincide con el original"
    else
        echo "ERROR: Archivo desencriptado NO coincide con el original"
    fi
else
    echo "ERROR: Error en desencriptación DES"
fi
echo ""

echo "10. Prueba: Operación combinada (Comprimir y Encriptar)"
echo "--------------------------------------------------------"
$GSEA -ce --comp-alg lzw --enc-alg vigenere -i $TEST_DIR/test3.txt -o $OUT_DIR/test3_comp_enc.bak -k "G3n0m3S3cur1ty!"
if [ $? -eq 0 ]; then
    echo "OK: Compresión y encriptación combinadas exitosas"
    ls -lh $OUT_DIR/test3_comp_enc.bak
else
    echo "ERROR: Error en operación combinada"
fi
echo ""

echo "11. Prueba: Procesar directorio completo (concurrencia)"
echo "--------------------------------------------------------"
$GSEA -c --comp-alg rle -i $TEST_DIR -o $OUT_DIR/dir_compressed
if [ $? -eq 0 ]; then
    echo "OK: Procesamiento de directorio exitoso"
    ls -lh $OUT_DIR/dir_compressed/
else
    echo "ERROR: Error en procesamiento de directorio"
fi
echo ""

echo "=========================================="
echo "Pruebas completadas"
echo "=========================================="

