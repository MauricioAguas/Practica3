#include <iostream>
#include <string>
#include "rle.h"
#include "lz78.h"
using namespace std;

int main() {
    // Prueba basica de compresion y descompresion RLE
    string texto = "AAABBBCC";

    cout << "Texto original   : " << texto << endl;

    string comprimido = comprimirRLE(texto);
    cout << "Texto comprimido : " << comprimido << endl;

    string restaurado = descomprimirRLE(comprimido);
    cout << "Texto restaurado : " << restaurado << endl;

    if (texto == restaurado) {
        cout << "[OK] La descompresion RLE es correcta." << endl;
    } else {
        cout << "[ERROR] La descompresion no coincide con el original." << endl;
    }

    // --- Prueba LZ78 compresion ---
    string textoLZ = "ABAABABA";
    ResultadoLZ78 res = comprimirLZ78(textoLZ.c_str(), (int)textoLZ.size());

    cout << "LZ78 original  : " << textoLZ << endl;
    cout << "Entradas en diccionario: " << res.tamano << endl;

    liberarMemoriaLZ78(res);
    return 0;
}
