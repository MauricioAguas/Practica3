#include "encriptacion.h"

// Encriptación: Rotación a la izquierda + XOR [cite: 117, 118]
unsigned char encriptarByte(unsigned char dato, int n, unsigned char K) {
    // 1. Rotación circular a la izquierda [cite: 89, 90]
    // Desplazamos n a la izquierda y recuperamos los bits por la derecha
    unsigned char rotado = (dato << n) | (dato >> (8 - n));

    // 2. Operación XOR con la clave K [cite: 118]
    return rotado ^ K;
}

// Desencriptación: XOR + Rotación a la derecha (Inverso) [cite: 119]
unsigned char desencriptarByte(unsigned char dato, int n, unsigned char K) {
    // 1. Aplicar XOR con la misma clave K para revertir [cite: 114]
    unsigned char conXOR = dato ^ K;

    // 2. Invertir la rotación (rotar n posiciones a la derecha) [cite: 119]
    unsigned char original = (conXOR >> n) | (conXOR << (8 - n));

    return original;
}
