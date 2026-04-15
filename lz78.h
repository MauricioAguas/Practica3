#ifndef LZ78_H
#define LZ78_H

// Cada entrada del diccionario es un par (índice del prefijo, carácter nuevo)
struct EntradaLZ78 {
    int prefijo;   // Apunta a una frase ya existente (0 si es nueva)
    char caracter; // El carácter que se añade a esa frase
};


#endif
