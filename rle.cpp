#include "rle.h"
#include <string>

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
