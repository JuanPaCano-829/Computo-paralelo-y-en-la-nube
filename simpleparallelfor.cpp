#include <iostream>
#include <omp.h>

int main(void){
    int size = -1;
    int n_threads = -1;

    std::cout << "Introduce el tamano del vector: \n";
    std::cin >> size;

    std::cout << "Introduce el numero de threads: \n";
    std::cin >> n_threads;

    float* a = new float[size];
    float* b = new float[size];
    float* c = new float[size];

    double start = omp_get_wtime();

    #pragma omp parallel for num_threads(n_threads)
    for(int i=0; i<size; i++){
        std::cout << "Hilo: " << omp_get_thread_num() << " Iteracion: " << i << "\n";
    }

    std::cout << "Tiempo: " << omp_get_wtime() - start << "\n";


    // :::::::::::::::::::::::::::: Ejercicio 1::::::::::::::::::::::::::::
    int thread_id = -1;
    int N = 1000;
    float* a1;
    a1 = new float[N];
    float* b2;
    b2 = new float[N];
    float* c2;
    c2 = new float[N];
    omp_set_num_threads(4);

    for(int i=0; i < N; i++)
    {
        a1[i] = b2[i] = i*1.5;
        std::cout <<"En el lugar: " << i <<" Valor de a es: " << a1[i] << "\n";
        std::cout <<"En el lugar: " << i <<" Valor de b es: " << b2[i] << "\n";
    }

    #pragma omp parallel for
    for(int j=0; j < N; j++)
    {
        thread_id = omp_get_thread_num();
        c2[j] = a1[j] + b2[j];
        std::cout << "En el lugar: " << j << " Valor de c es: " << c2[j] << " con el hilo: " << thread_id << "\n"; 
    } 
    std::cout << "En el lugar: " << N-1 << " Valor de c es: " << c2[N-1] << "\n"; 
    std::cout << "En el lugar: " << N-(N-1) << " Valor de c es: " << c2[N-(N-1)] << "\n"; 

    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}