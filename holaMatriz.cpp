#include <iostream>
using namespace std;

int main(void) {

    int rows = 3;
    int cols = 4;
    int** matrix;
    matrix = new int*[rows];

    //reservar memoria
    for(int i=0;i<rows;i++){
        matrix[i] = new int[cols];
    }

    //aquí va mi código donde hago las operaciones 
    //reservar memoria
    //accedemos al puente de la matriz
    cout << *matrix << "\n";
    for(int i=0;i<rows;i++){
        cout << matrix[i] << "\n";
        for(int j=0;j<cols;j++){
            cout << &matrix[i][j] << " ";
        }
        cout << "\n";
    }

    //liberamos la memoria. liberamos primero las alloc de los arreglos.
    for(int i=0;i<rows;i++){
        delete[] matrix[i];
    }
    //borras el arreglo de apuntadores 
    delete[] matrix;


    return 0;
}