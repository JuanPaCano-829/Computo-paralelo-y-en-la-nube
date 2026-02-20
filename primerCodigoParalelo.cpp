#include <iostream>
#include <omp.h>

int main(void){
    std::cout << "H" << "\n";
    int thread_id = -1;
    omp_set_num_threads(3);

    #pragma omp parallel private(thread_id)
    {
        thread_id = omp_get_thread_num();

        for(int i=0; i<5; i++){
            std::cout << i << "\n";
        }

        #pragma omp critical
        {
            std::cout << "Soy el hilo " << thread_id << "\n";
            if(thread_id == 0){
                std::cout << "El numero de hilos/procesos es: " << omp_get_num_threads() << "\n";
            }
        }
    }

    // :::::::::::::::::::::::::::: Ejercicio 1::::::::::::::::::::::::::::
    int N = 1000;
    float* a;
    a = new float[N];
    float* b;
    b = new float[N];
    float* c;
    c = new float[N];

    for(int i=0; i < N; i++)
    {
        a[i] = b[i] = i*1.5;
        std::cout <<"En el lugar: " << i <<" Valor de a es: " << a[i] << "\n";
        std::cout <<"En el lugar: " << i <<" Valor de b es: " << b[i] << "\n";
    }

    #pragma omp parallel for
    for(int j=0; j < N; j++)
    {
        c[j] = a[j] + b[j];
        std::cout << "En el lugar: " << j << " Valor de c es: " << c[j] << " con el hilo: " << thread_id << "\n"; 
    }

    std::cout << "En el lugar: " << N-1 << " Valor de c es: " << c[N-1] << "\n"; 
    std::cout << "En el lugar: " << N-(N-1) << " Valor de c es: " << c[N-(N-1)] << "\n"; 

    delete[] a;
    delete[] b;
    delete[] c;
    return 0;
}