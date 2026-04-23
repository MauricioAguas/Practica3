#include <iostream>
#include <fstream>
#include <string>
#include "rle.h"
#include "lz78.h"
#include "encriptacion.h"
using namespace std;

ResultadoLZ78 miResultadoLZ78 = {nullptr, 0};

void programa(string contenido, string rutaArchivo);

int main()
{
    //LEER ARCHIVO
    string rutaArchivo="D:/Mis documentos/U/2026-1/Info2/lab3/prueba/codigos.txt"; //cambiar direccion
    string texto;
    string contenido;

    try {
    //   fstream file(rutaArchivo);
        fstream file(rutaArchivo, ios::in | ios::binary);
        if (!file.is_open())
            throw runtime_error("No se pudo abrir el archivo: " + rutaArchivo);

        while (getline(file, texto))
            contenido += texto + "\n";
        file.close();
    } catch (const exception& e) {
        cout << "ERROR: " << e.what() << endl;
        return 1;
    }

    cout << "Original: " << contenido << endl;

    //INICIAR MENU
    programa(contenido,rutaArchivo);
    liberarMemoriaLZ78(miResultadoLZ78);
    return 0;
}

void programa(string contenido, string rutaArchivo){
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
        cout<<"6. Allin1"<<endl;
        cout<<"7. Salir"<<endl;
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

            // Validación con manejo de excepciones
            if (n <= 0 || n >= 8) {
                cout << "Error: n debe estar entre 1 y 7." << endl;
                break;
            }

            cout << "Ingrese clave K (un caracter): ";
            cin >> k_char;
            cin.ignore();
            unsigned char K = (unsigned char)k_char;

            int subOpcion=0;
            cout << "1. Encriptar contenido actual" << endl;
            cout << "2. Desencriptar contenido actual" << endl;
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
        case 6: { // NUEVO — Flujo integrado completo (sección 5.4)
            int metodo, n;
            char k_char;

            cout << "\n--- Flujo Integrado Completo ---" << endl;

            // 1. Elegir método de compresión
            cout << "Metodo de compresion (1=RLE, 2=LZ78): ";
            cin >> metodo;

            // 2. Parámetros de encriptación
            cout << "Ingrese valor de rotacion n (1-7): ";
            cin >> n;
            if (n <= 0 || n >= 8) {
                cout << "Error: n debe estar entre 1 y 7." << endl;
                break;
            }
            cout << "Ingrese clave K (un caracter): ";
            cin >> k_char;
            unsigned char K = (unsigned char)k_char;

            // Releer el texto original desde rutaArchivo para tener referencia limpia
            string original = "";
            try {
                ifstream fileOrig(rutaArchivo);
                if (!fileOrig.is_open())
                    throw runtime_error("No se pudo releer el archivo: " + rutaArchivo);
                string linea;
                while (getline(fileOrig, linea))
                    original += linea + "\n";
                fileOrig.close();
            } catch (const exception& e) {
                cout << "ERROR: " << e.what() << endl;
                break;
            }

            // PASO 1: Comprimir
            string comprimidoRLE = "";
            ResultadoLZ78 comprimidoLZ78 = {nullptr, 0};

            if (metodo == 1) {
                cout << "\n[1/4] Comprimiendo con RLE..." << endl;
                comprimidoRLE = comprimirRLE(original);
                cout << "Comprimido: " << comprimidoRLE << endl;
            } else {
                cout << "\n[1/4] Comprimiendo con LZ78..." << endl;
                comprimidoLZ78 = comprimirLZ78(original.c_str(), original.length());
            }

            // PASO 2: Encriptar byte a byte
            cout << "[2/4] Encriptando..." << endl;
            string encriptado = "";
            if (metodo == 1) {
                for (char c : comprimidoRLE)
                    encriptado += (char)encriptarByte((unsigned char)c, n, K);
            } else {
                // Encriptar el carácter de cada entrada del diccionario LZ78
                for (int i = 0; i < comprimidoLZ78.tamano; i++)
                    comprimidoLZ78.diccionario[i].caracter =
                        (char)encriptarByte((unsigned char)comprimidoLZ78.diccionario[i].caracter, n, K);
            }
            cout << "Encriptado correctamente." << endl;

            // PASO 3: Desencriptar
            cout << "[3/4] Desencriptando..." << endl;
            if (metodo == 1) {
                string desencriptado = "";
                for (char c : encriptado)
                    desencriptado += (char)desencriptarByte((unsigned char)c, n, K);
                encriptado = desencriptado;
            } else {
                // Desencriptar el carácter de cada entrada del diccionario LZ78
                for (int i = 0; i < comprimidoLZ78.tamano; i++)
                    comprimidoLZ78.diccionario[i].caracter =
                        (char)desencriptarByte((unsigned char)comprimidoLZ78.diccionario[i].caracter, n, K);
            }

            // PASO 4: Descomprimir y recuperar texto
            cout << "[4/4] Descomprimiendo..." << endl;
            string recuperado = "";
            if (metodo == 1) {
                try {
                    recuperado = descomprimirRLE(encriptado);
                } catch (const exception& e) {
                    cout << "ERROR al descomprimir RLE: " << e.what() << endl;
                    break;
                }
            } else {
                int longSalida = 0;
                char* recChar = descomprimirLZ78(comprimidoLZ78, longSalida);
                if (recChar) {
                    // Construir string solo para comparar e imprimir — único uso permitido
                    for (int i = 0; i < longSalida; i++)
                        recuperado += recChar[i];
                    delete[] recChar;
                }
                liberarMemoriaLZ78(comprimidoLZ78);
            }

            // PASO 5: Escribir resultado en archivo de salida
            try {
                ofstream archivoSalida("verificacion.txt");
                if (!archivoSalida.is_open())
                    throw runtime_error("No se pudo crear el archivo verificacion.txt");
                archivoSalida << recuperado;
                archivoSalida.close();
                cout << "Archivo 'verificacion.txt' generado." << endl;
            } catch (const exception& e) {
                cout << "ERROR: " << e.what() << endl;
                break;
            }

            // PASO 6: Verificar que el texto recuperado == original
            cout << "\n--- Verificacion ---" << endl;
            if (recuperado == original) {
                cout << "EXITO: El texto recuperado es identico al original." << endl;
            } else {
                cout << "ERROR: El texto recuperado NO coincide con el original." << endl;
                cout << "Longitud original:   " << original.length()   << " chars" << endl;
                cout << "Longitud recuperado: " << recuperado.length() << " chars" << endl;
            }
            break;
        }

        case 7:
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
    } while (opcion != 7);
}
