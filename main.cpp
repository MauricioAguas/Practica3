#include <iostream>
#include <string>
#include "rle.h"
#include "lz78.h"
#include "encriptacion.h"
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

    // --- Prueba LZ78 completa ---
    string textoLZ = "ABAABABA";
    ResultadoLZ78 res = comprimirLZ78(textoLZ.c_str(), (int)textoLZ.size());

    cout << "LZ78 original  : " << textoLZ << endl;
    cout << "Entradas en diccionario: " << res.tamano << endl;
    descomprimirLZ78(res);
    liberarMemoriaLZ78(res);

    // --- Prueba Encriptacion byte a byte ---
    int n = 3;
    unsigned char K = 42;
    string textoEnc = "Hola";

    cout << "Texto original   : " << textoEnc << endl;
    cout << "Bytes encriptados: ";

    // Encriptar cada byte y guardarlos
    int tam = (int)textoEnc.size();
    unsigned char* encriptado = new unsigned char[tam];
    for (int i = 0; i < tam; i++) {
        encriptado[i] = encriptarByte((unsigned char)textoEnc[i], n, K);
        cout << hex << (int)encriptado[i] << " ";
    }
    cout << dec << endl;

    // Desencriptar y recuperar texto
    string recuperado = "";
    for (int i = 0; i < tam; i++) {
        recuperado += (char)desencriptarByte(encriptado[i], n, K);
    }
    cout << "Texto recuperado : " << recuperado << endl;

    if (textoEnc == recuperado)
        cout << "[OK] Encriptacion correcta." << endl;
    else
        cout << "[ERROR] No coincide." << endl;

    delete[] encriptado;
    return 0;
}
