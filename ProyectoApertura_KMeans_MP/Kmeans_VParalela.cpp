#include <iostream>
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <limits>
#include <string>
#include <cmath>

using namespace std;
struct Punto { // estrucutra para crear un punto 
    double x;
    double y;
    double z; // Si es 2D entonces lo dejamos en 0
    int id_cluster;// Aquí guardamos a qué centro pertenece
};

// no se paraleliza porque puede escribir mal los datos
vector<Punto> leerCSV(string nombreArchivo, int dimensiones) { // función para leer el archvio csv
    vector<Punto> pts; // vector de tipo puntos para almacenar lso puntos
    ifstream f(nombreArchivo); // leer el archvio 
    string linea; // para guardar los puntos en una linea 

    if (!f.is_open()) { std::cerr << "Error abriendo " << nombreArchivo << "\n"; return pts; } // error por is no se puede abrir el arhcivo 

    while (getline(f, linea)) // lee la linea que sigue en f y lo guarda en linea 
    {
       stringstream separador(linea); //lo convierte en un "archivo" para volver a leer
       string valor; // valor de la linea
       Punto pt; // creamos un nuevo punto
       
       pt.id_cluster = -1; // Valor sin asignar
       pt.z = 0; // iniciamos z en 0 por si es 2D
       
       getline(separador, valor, ','); // guarda en valor lo que hay en separador hasta que veas una , 
       pt.x = stod(valor); // stod() convierte el string a double para despues guardarlo en x
       getline(separador, valor, ','); // se guarda el valor hasta la segunda , o cuando ya no hay nada que leer
       pt.y = stod(valor); // mismo proceso para y 

       if(dimensiones == 3){ // agregar z si tiene 3 dimensiones 
            getline(separador, valor, ',');
            pt.z = stod(valor);
       }else // si no dejar en 0 
       {
            pt.z = 0;
       }
       pts.push_back(pt); // meter el punto creado en el vector de puntos
    }

    f.close(); // cerrar el archivo
    return pts; // regresar la lista de puntos 
}


// no se paraleliza porque toma más tiempo abrir los hilos y asignarlos que hacer el ciclo normal 
vector<Punto> inicializarCentroides(const vector<Punto>& pts, int kCentros) { // función para seleccionar los centros
    vector<Punto> centros; // crear una vector de puntos para guardar los centros
    int totalPts = pts.size();// guardar el número total de puntos que se formaron

    for(int i=0;i<kCentros;i++){ // un for para crear los k centros
        int idx = rand() % totalPts; // crea un número aleatorio entre 0 y el total de puntos
        Punto newCentro = pts[idx]; // crear el nuevo centro de tipo punto tomando el punto que hay en ese index
        newCentro.id_cluster = i; // decirle que grupo va a manejar
        centros.push_back(newCentro); // guardar el nuevo centroide
    }
    return centros; // regresar el vector de centros
}


bool asignarPuntosAlMasCercano(vector<Punto>& pts, const vector<Punto>& kCentros) { // asignar los puntos que iran a cada centro y regresar si hubo cambios 
    bool cambios = false; // variable que indicara si hubo cambios 
    

    // omp parallel for porque crea los hilos y reparte automáticamente las iteraciones del ciclo entre ellos
    #pragma omp parallel for schedule(static) reduction(||:cambios)  // static porque tomar la distancia es igual de tardada para todos los puntos
    // reduction(||:cambios) para asignar una varibale booleana a cada hilo para evitar condiciones de carrera 
    for(int i=0;i<pts.size();i++){
        double distancia = 0.0; // variable para guardar la distancia euclidiana
        double minDistancia = numeric_limits<double>::max(); // asignar el maximo valor en c
        int GrupoAnteriror = pts[i].id_cluster;
        int GrupoNuevo = pts[i].id_cluster;
        for (int j=0;j<kCentros.size();j++)
        {
            distancia = sqrt(pow(pts[i].x-kCentros[j].x,2) 
                                + pow(pts[i].y-kCentros[j].y,2) 
                                + pow(pts[i].z-kCentros[j].z,2)); // distancia euclidiana entre puntos
            if(minDistancia > distancia){GrupoNuevo = j;minDistancia = distancia;} // si la distancia nueva es menor ajustamos el id y la distancia min
        }
        if(GrupoAnteriror != GrupoNuevo){pts[i].id_cluster = GrupoNuevo;cambios = true;}
    }
    return cambios;
}

void actualizarPosicionCentroides(const vector<Punto>& pts, vector<Punto>& kCentros) { // recalcular los centros 
    vector<double> sum_x(kCentros.size(),0.0);// vector para guardar la suma de x para cada centro 
    vector<double> sum_y(kCentros.size(),0.0);// vector para guardar la suma de y para cada centro 
    vector<double> sum_z(kCentros.size(),0.0);// vector para guardar la suma de z para cada centro 
    vector<int> numPuntos(kCentros.size(),0);// vector para guardar la suma de los puntos totales para cada centro 
    
    #pragma omp parallel for schedule(static) // un for normal estatico porque todas las sumas tardan lo mismo 
    for(int i=0;i<pts.size();i++) // 
    {
        if (pts[i].id_cluster != -1) {
            #pragma omp atomic // atomico para evitar la condición de carrerar y que no traten de sumar las mismo tiempo
            sum_x[pts[i].id_cluster] += pts[i].x; // usar el id para determinar el centro y despues para sumar su valor x 
            #pragma omp atomic // atomico para evitar la condición de carrerar y que no traten de sumar las mismo tiempo
            sum_y[pts[i].id_cluster] += pts[i].y; // usar el id para determinar el centro y despues para sumar su valor y
            #pragma omp atomic // atomico para evitar la condición de carrerar y que no traten de sumar las mismo tiempo
            sum_z[pts[i].id_cluster] += pts[i].z; // usar el id para determinar el centro y despues para sumar su valor z
            #pragma omp atomic // atomico para evitar la condición de carrerar y que no traten de sumar las mismo tiempo
            numPuntos[pts[i].id_cluster]++; // usar el id para determinar el centro y despues aumentar el número de puntos  
        }
    }
    
    for (int j=0;j<kCentros.size();j++)
    {
        if (numPuntos[j] > 0) {
            kCentros[j].x = sum_x[j] / numPuntos[j]; // ajustar el valor de x al promedio de las x
            kCentros[j].y = sum_y[j] / numPuntos[j]; // ajustar el valor de x al promedio de las y
            kCentros[j].z = sum_z[j] / numPuntos[j]; // ajustar el valor de x al promedio de las z
        }
    }
}

// no se paraleliza porque puede escribir mal los datos
void guardarResultadosCSV(const vector<Punto>& pts, string nombreArchivoSalida) { 
    ofstream CSV(nombreArchivoSalida); // crear el archvio con el nombre 

    // Siempre es buena práctica verificar que el archivo se creó correctamente
    if (!CSV.is_open()) { cerr << "Error: No se pudo crear el archivo " << nombreArchivoSalida << "\n"; return;} // error por si no se crea bien

    for (int i = 0; i < pts.size(); i++) { // recorrer el vector con los puntos 
        CSV << pts[i].x << ","  // por cada punto guardar pts.x,pts.y,pts.z,pts.id_cluster
                << pts[i].y << "," 
                << pts[i].z << "," 
                << pts[i].id_cluster << "\n"; 
    }
    CSV.close(); // cerrar el archvio 
}


int main(int argc, char* argv[]) {
    srand(42); // semilla de aleatoriada 

    if (argc != 5) { // ver si no hay elementos de más
        cerr << "Uso correcto: " << argv[0] << " <.csv> <K_centros> <dimensiones(2 o 3)> <numero_hilos>\n";
        return 1; // Salir con error
    }

    string archivo_Inicial = argv[1]; // leer el archivo
    int K = atoi(argv[2]); // atoi transforma texto en int y guarda en k el número de centros
    int dimensiones = atoi(argv[3]); // atoi transforma texto en int y guarda en dimensiones si es 2D o 3D
    int num_hilos = atoi(argv[4]); // numero de hilos 
    omp_set_num_threads(num_hilos); // decir que use esa cantidad de hilos de ahora en adelante

    cout << "Leyendo CSV" << "\n";
    vector<Punto> mis_pts = leerCSV(archivo_Inicial, dimensiones);
    
    if (mis_pts.empty()) return 1; // ver si el archivo no esta vacío 

    auto tiempo_inicial = chrono::high_resolution_clock::now(); // iniciar el timer 

    cout << "Iniciando K-means con " << num_hilos << " hilos..." << "\n";
    vector<Punto> mis_centros = inicializarCentroides(mis_pts, K); // crear los centroides
    bool continuar = true; 
    int iteracion = 0; // saber cuantas veces actualizo los centros
    int max_iteraciones = 100; // Un límite de seguridad por si acaso

    
    while (continuar && iteracion < max_iteraciones) { // while para el ciclo hasta que no se pueda hacer más cambios
        continuar = asignarPuntosAlMasCercano(mis_pts, mis_centros); // hasta que regrese false para que no haya más cambios en los centros
        if (continuar) {actualizarPosicionCentroides(mis_pts, mis_centros);} // actualizar los centro si es true
        iteracion++;
    }

    auto tiempo_final = chrono::high_resolution_clock::now(); // detener el timer
    chrono::duration<double> duracion = tiempo_final - tiempo_inicial;

    cout << "========================================\n";
    cout << "K-means finalizado en " << iteracion << " iteraciones.\n";
    cout << "Tiempo de ejecucion: " << duracion.count() << " segundos.\n";
    cout << "========================================\n";

    string archivo_salida = "ResultadosP_" + to_string(num_hilos) + "_" + archivo_Inicial; //nombre del archivo con los resultados
    guardarResultadosCSV(mis_pts, archivo_salida); // guardar los datos en el nuevo archvio 
    cout << "Resultados guardados en: " << archivo_salida << "\n";

    return 0;
}