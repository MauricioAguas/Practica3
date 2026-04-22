#include <iostream>
#include <fstream>
#include <string>
#include "rle.h"
#include "lz78.h"
#include "encriptacion.h"
using namespace std;

ResultadoLZ78 miResultadoLZ78 = {nullptr, 0};

void programa(string contenido);

int main()
{
    //LEER ARCHIVO
    fstream file("D:/Mis documentos/U/2026-1/Info2/lab3/prueba/codigos.txt"); //cambiar direccion

    if(!file.is_open()){
        cout<<"Error al ejecutar el archivo";
        return 1;
    }

    string texto;
    string contenido;


    while(getline(file, texto))
        contenido += texto+"\n";
    file.close();

    cout<<"Original:"<<contenido<<endl;

    //INICIAR MENU
    programa(contenido);
    liberarMemoriaLZ78(miResultadoLZ78);
    return 0;
}

void programa(string contenido){
    int opcion=0;
    do{
        //MENU
        cout<<"\n=============MENU==========="<<endl;
        cout<<"Elige una opcion."<<endl;
        cout<<"1. Comprimir RLE"<<endl;
        cout<<"2. Comprimir LZ78"<<endl;
        cout<<"3. Descomprimir RLE"<<endl;
        cout<<"4. Descomprimir LZ78"<<endl;
        cout<<"5.Encriptacion y descriptacion"<<endl;
        cout<<"6. Salir"<<endl;
        cout<<"Opcion: ";


        cin>>opcion;
        switch (opcion) {
        case 1:
            cout << "Comprimiendo en RLE..." << endl;
            // IMPORTANTE: Guardar el resultado en la variable
            contenido = comprimirRLE(contenido);
            cout << "Resultado guardado: " << contenido << endl;
            break;
        case 2:
            cout << "Comprimiendo LZ78..." << endl;
            try {
                liberarMemoriaLZ78(miResultadoLZ78);
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
            miResultadoLZ78 = comprimirLZ78(contenido.c_str(), contenido.length());
            break;
        case 3:
            cout<<"Descomprimiendo RLE..."<<endl;
            try {
                string descompreso = descomprimirRLE(contenido);
                cout << "Texto recuperado: " << descompreso << endl;
            } catch (const exception& e) {
                cout << "ERROR: " << e.what() << endl;
            }
            break;
        case 4:
            cout<<"Descomprimiendo LZ78..."<<endl;
            {
                int longitud = 0;
                char* recuperado = descomprimirLZ78(miResultadoLZ78, longitud);
                if (recuperado) {
                    cout << "Texto recuperado: " << recuperado << endl;
                    delete[] recuperado; // libera memoria dinámica
                }
            }
            break;
        case 5: { // Nueva opción: Encriptar/Desencriptar
            int n;
            char k_char;
            cout << "--- Configuracion de Encriptacion ---" << endl;
            cout << "Ingrese valor de rotacion n (1-7): ";
            cin >> n;

            // Validación con manejo de excepciones [cite: 168]
            if (n <= 0 || n >= 8) {
                cout << "Error: n debe estar entre 1 y 7." << endl;
                break;
            }

            cout << "Ingrese clave K (un caracter): ";
            cin >> k_char;
            unsigned char K = (unsigned char)k_char;

            cout << "1. Encriptar contenido actual" << endl;
            cout << "2. Desencriptar contenido actual" << endl;
            int subOpcion;
            cin >> subOpcion;

            string nuevoContenido = "";
            if (subOpcion == 1) {
                for (char c : contenido)
                    nuevoContenido += (char)encriptarByte((unsigned char)c, n, K);
                cout << "Contenido encriptado." << endl;
            } else {
                for (char c : contenido)
                    nuevoContenido += (char)desencriptarByte((unsigned char)c, n, K);
                cout << "Contenido desencriptado." << endl;
            }

            contenido = nuevoContenido;
            cout << "Vista previa: " << contenido << endl;
            break;
        }
        case 6:
            cout << "Guardando verificacion final..." << endl;
            {
                ofstream archivoSalida("D:/Mis documentos/U/2026-1/Info2/lab3/prueba/verificacion.txt");
                if (archivoSalida.is_open()) {
                    archivoSalida << contenido;
                    archivoSalida.close();
                    cout << "Archivo 'verificacion.txt' generado exitosamente." << endl;
                }
            }
            cout << "Saliendo..." << endl;
            break;

        default:
            cout << "Opcion invalida" << endl;
            break;
        }
    } while (opcion != 6);
}
