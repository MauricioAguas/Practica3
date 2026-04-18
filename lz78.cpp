#include "lz78.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// Función para agrandar el arreglo en memoria dinámica cuando se llena
EntradaLZ78* redimensionarDiccionario(EntradaLZ78* viejo, int &capacidad) {
    int nuevaCapacidad = capacidad * 2;
    EntradaLZ78* nuevo = new (nothrow) EntradaLZ78[nuevaCapacidad];

    if (!nuevo) throw runtime_error("Error: No hay memoria suficiente para expandir el diccionario.");

    for (int i = 0; i < capacidad; i++) {
        nuevo[i] = viejo[i];
    }

    delete[] viejo; // Liberamos la memoria anterior
    capacidad = nuevaCapacidad;
    return nuevo;
}

ResultadoLZ78 comprimirLZ78(const char* texto, int longitud) {
    int capacidad = 10; // Capacidad inicial pequeña para probar el redimensionamiento
    int tamanoActual = 0;
    EntradaLZ78* diccionario = new (nothrow) EntradaLZ78[capacidad];

    if (!diccionario) throw runtime_error("Error: No se pudo asignar memoria inicial.");

    int indicePrefijoActual = 0; // 0 representa la "frase vacía"
    cout << "Pares (i, c) generados: ";

    for (int i = 0; i < longitud; i++) {
        char c = texto[i];
        if (c == '\n' || c == '\r') continue; // Ignorar saltos de línea del archivo

        int encontrado = -1;
        // Buscar si el par (prefijo, caracter) ya existe en nuestro diccionario
        for (int j = 0; j < tamanoActual; j++) {
            if (diccionario[j].prefijo == indicePrefijoActual && diccionario[j].caracter == c) {
                encontrado = j + 1; // El índice en LZ78 es base 1
                break;
            }
        }

        if (encontrado != -1) {
            // Si existe, el prefijo para la siguiente vuelta será esta entrada
            indicePrefijoActual = encontrado;
        } else {
            // Si no existe, imprimimos el par y lo guardamos
            cout << "(" << indicePrefijoActual << "," << c << ") ";

            if (tamanoActual >= capacidad) {
                diccionario = redimensionarDiccionario(diccionario, capacidad);
            }

            diccionario[tamanoActual].prefijo = indicePrefijoActual;
            diccionario[tamanoActual].caracter = c;
            tamanoActual++;

            indicePrefijoActual = 0; // Reiniciamos para buscar una nueva frase
        }
    }
    cout << endl;
    return {diccionario, tamanoActual};
}
