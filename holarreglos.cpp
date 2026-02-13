#include <iostream>

int main(void) {

    int arregloEstatico[5];
    arregloEstatico[0] = 10;
    arregloEstatico[1] = 11;
    //arregloEstatico[2] = 12;
    arregloEstatico[3] = 13;
    arregloEstatico[4] = 14;

    for(int dato : arregloEstatico) 
    {
        std::cout << dato << std::endl;
    }

    for(int i = 0; i < sizeof(arregloEstatico)/sizeof(int); i++) 
    {
        std::cout << arregloEstatico[i] << std::endl;
    }

    // :::::::::::::::::::::::::::ARREGLO DINAMICO:::::::::::::::::::::::::::::::::::::::::::::::::::
    int size = 5;
    int* arregloDinamico;
    arregloDinamico = new int[size];
    arregloDinamico[0] = 1111;
    arregloDinamico[4] = 4444;

    for (int i = 0; i < size; i++)
    {
        std::cout << " Valor " << i << " " << arregloDinamico[i] << "\n";
    }


    delete[] arregloDinamico;
    return 0;
}
