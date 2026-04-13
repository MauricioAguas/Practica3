#include "rle.h"
#include <string>
#include <stdexcept>

using namespace std;

string comprimirRLE(string texto) {
    string resultado = "";
    int n = texto.length();

    for (int i = 0; i < n; i++) {
        // SALTO DE SEGURIDAD: Si el caracter es un salto de línea o retorno, lo ignoramos
        if (texto[i] == '\n' || texto[i] == '\r') continue;

        int conteo = 1;
        while (i < n - 1 && texto[i] == texto[i + 1]) {
            conteo++;
            i++;
        }

        resultado += to_string(conteo) + texto[i];
    }
    return resultado;
}

string descomprimirRLE(string texto) {
    if (texto.empty()) return "";

    string resultado = "";
    for (size_t i = 0; i < texto.length(); ) {
        string numStr = "";

        // 1. Extraer el número de repeticiones (puede ser más de un dígito)
        while (i < texto.length() && isdigit(texto[i])) {
            numStr += texto[i++];
        }

        // Validación: Si no hay un número antes del carácter, el formato es incorrecto
        if (numStr.empty()) {
            throw runtime_error("Error en RLE: Formato inválido (se esperaba un número).");
        }

        // 2. El siguiente carácter es el que debemos repetir
        if (i < texto.length()) {
            int conteo = stoi(numStr);
            char caracter = texto[i++];

            // 3. Reconstruir la cadena original
            resultado.append(conteo, caracter);
        } else {
            throw runtime_error("Error en RLE: Número sin carácter asociado al final.");
        }
    }
    return resultado;
}
