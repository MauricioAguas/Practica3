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
        //if (c == '\n' || c == '\r') continue; // Ignorar saltos de línea del archivo

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
//Retorna char* con memoria dinámica, sin usar string
char* descomprimirLZ78(ResultadoLZ78 res, int &longitudSalida) {
    if (!res.diccionario || res.tamano == 0) {
        longitudSalida = 0;
        return nullptr;
    }

    // Primero calculamos cuántos caracteres va a tener el resultado
    int totalChars = 0;
    for (int i = 0; i < res.tamano; i++) {
        int p = i;
        if (res.diccionario[p].caracter != '\0') totalChars++;
        int sig = res.diccionario[p].prefijo;
        while (sig != 0) {
            p = sig - 1;
            totalChars++;
            sig = res.diccionario[p].prefijo;
        }
    }

    // Reservar memoria dinámica para el resultado
    char* resultado = new (nothrow) char[totalChars + 1];
    if (!resultado) throw runtime_error("Error: No hay memoria para descomprimir.");

    int pos = 0;
    char temporal[256];

    for (int i = 0; i < res.tamano; i++) {
        int tope = 0;
        int p = i;

        if (res.diccionario[p].caracter != '\0')
            temporal[tope++] = res.diccionario[p].caracter;

        int sig = res.diccionario[p].prefijo;
        while (sig != 0) {
            p = sig - 1;
            temporal[tope++] = res.diccionario[p].caracter;
            sig = res.diccionario[p].prefijo;
        }

        while (tope > 0) {
            resultado[pos++] = temporal[--tope];
        }
    }

    resultado[pos] = '\0'; // null-terminator
    longitudSalida = pos;
    return resultado;
}
void liberarMemoriaLZ78(ResultadoLZ78 &res) {
    if (res.diccionario) {
        delete[] res.diccionario;
        res.diccionario = nullptr;
        res.tamano = 0;
    }
}
