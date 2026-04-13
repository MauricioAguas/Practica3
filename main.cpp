#include <iostream>
#include <string>
#include "rle.h"


using namespace std;

int main() {
    // Prueba basica de compresion y descompresion RLE
    string texto = "aaaaabbbbbbccccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrsssstttt";

    cout << "Texto original   : " << texto << endl;

    string comprimido = comprimirRLE(texto);
    cout << "Texto comprimido : " << comprimido << endl;

    return 0;
}
