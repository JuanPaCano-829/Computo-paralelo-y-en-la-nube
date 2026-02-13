#include <iostream>

int main(void) {
    int numero = 3;
    int* aptNumero = &numero; // Declaramos un puntero a entero y le asignamos la dirección de memoria de la variable numero

    std::cout << "El valor de numero es: " << numero << std::endl; // Imprime el valor de numero
    std::cout << "Numero vive en: " << &numero << std::endl;
    numero = 6;
    std::cout << "Numero vive en: " << aptNumero << std::endl; // Imprime la dirección de memoria almacenada en aptNumero
    *aptNumero = 9; // Modificamos el valor al que apunta aptNumero (que es el valor de numero)
    std::cout << "AptNumero vive en:  " << &aptNumero << std::endl; // Imprime el valor al que apunta aptNumero (que es el valor de numero)
    std::cout << "El valor de numero a traves de aptNumero: " << *aptNumero << std::endl; // Imprime el valor al que apunta aptNumero (que es el valor de numero)
    
    return 0;
}
