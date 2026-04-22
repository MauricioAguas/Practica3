#ifndef LZ78_H
#define LZ78_H

// Cada entrada del diccionario es un par (índice del prefijo, carácter nuevo)
struct EntradaLZ78 {
    int prefijo;   // Apunta a una frase ya existente (0 si es nueva)
    char caracter; // El carácter que se añade a esa frase
};

// Estructura para agrupar el diccionario dinámico y su tamaño actual
struct ResultadoLZ78 {
    EntradaLZ78* diccionario;
    int tamano;
};

// Prototipos de funciones
ResultadoLZ78 comprimirLZ78(const char* texto, int longitud);
void descomprimirLZ78(ResultadoLZ78 res);
void liberarMemoriaLZ78(ResultadoLZ78 &res);
#endif
