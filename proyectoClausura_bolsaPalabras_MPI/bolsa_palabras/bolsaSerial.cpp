#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <curl/curl.h>

using namespace std;

// ---------------------------------------------------------------------------
// Callback interno de libcurl: acumula los bytes recibidos en un string
// No modificar
// ---------------------------------------------------------------------------
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}

// ---------------------------------------------------------------------------
// Lee el archivo de URLs línea por línea y retorna un vector con cada URL
// ---------------------------------------------------------------------------
vector<string> readUrls(const string& filename) {
    vector<string> urls;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir " << filename << "\n";
        return urls;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty())
            urls.push_back(line);
    }

    return urls;
}

// ---------------------------------------------------------------------------
// Descarga el contenido de una URL usando libcurl y lo retorna como string
// ---------------------------------------------------------------------------
string downloadBook(const string& url) {
    CURL* curl = curl_easy_init();
    string content;

    if (!curl) {
        cerr << "Error: no se pudo inicializar libcurl\n";
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "Error descargando " << url << ": " << curl_easy_strerror(res) << "\n";
        content = "";
    }

    curl_easy_cleanup(curl);
    return content;
}

// ---------------------------------------------------------------------------
// Elimina todas las etiquetas HTML del texto descargado
// Recibe: string con HTML crudo
// Retorna: string con solo el texto plano (sin <tags>)
// Pista: recorrer caracter por caracter, ignorar todo lo que esté entre < y >
// ---------------------------------------------------------------------------
string stripHtml(const string& html) {
    // TODO:
    // - Crear un string resultado vacío
    // - Recorrer cada caracter del html
    // - Usar una bandera booleana "dentroDeTag"
    // - Si encuentras '<', activar la bandera (no copiar el caracter)
    // - Si encuentras '>', desactivar la bandera (no copiar el caracter)
    // - Si la bandera está en false, copiar el caracter al resultado
    // - Retornar el resultado
    return "";
}

// ---------------------------------------------------------------------------
// Convierte el texto a minúsculas y elimina caracteres no alfabéticos
// Recibe: string con texto plano
// Retorna: string con solo letras minúsculas y espacios
// Pista: usar tolower() y isalpha() de <cctype>
// ---------------------------------------------------------------------------
string cleanText(const string& text) {
    // TODO:
    // - Crear un string resultado vacío
    // - Recorrer cada caracter del texto
    // - Si el caracter es una letra (isalpha), convertirlo a minúscula (tolower) y agregarlo
    // - Si no es letra, agregar un espacio (para separar palabras)
    // - Retornar el resultado
    return "";
}

// ---------------------------------------------------------------------------
// Divide el texto limpio en palabras y cuenta la frecuencia de cada una
// Recibe: string con texto limpio (solo letras minúsculas y espacios)
// Retorna: map<string, int> donde clave=palabra, valor=cantidad de apariciones
// Pista: usar un istringstream para leer palabra por palabra
// ---------------------------------------------------------------------------
map<string, int> countWords(const string& cleanedText) {
    // TODO:
    // - Crear un map<string, int> vacío llamado frecuencias
    // - Crear un istringstream con el texto limpio
    // - Leer palabra por palabra con el operador >>
    // - Para cada palabra, incrementar su contador en el map: frecuencias[palabra]++
    // - Retornar el map
    return {};
}

// ---------------------------------------------------------------------------
// Construye el vocabulario global: conjunto de TODAS las palabras únicas
// de todos los libros combinados
// Recibe: vector de maps, uno por libro (salida de countWords)
// Retorna: vector<string> con todas las palabras únicas ordenadas alfabéticamente
// Pista: usar un set<string> para eliminar duplicados, luego convertir a vector
// ---------------------------------------------------------------------------
vector<string> buildVocabulary(const vector<map<string, int>>& wordCounts) {
    // TODO:
    // - Crear un set<string> vacío llamado vocab
    // - Recorrer cada map del vector
    // - Para cada par (palabra, conteo) del map, insertar la palabra en el set
    // - Convertir el set a un vector<string> (el set ya está ordenado)
    // - Retornar el vector
    return {};
}

// ---------------------------------------------------------------------------
// Construye la matriz de Bolsa de Palabras
// Recibe:
//   - wordCounts: vector de maps (uno por libro)
//   - vocabulary: vector con todas las palabras únicas ordenadas
// Retorna: matriz [num_libros x num_palabras] con las frecuencias
//          representada como vector<vector<int>>
// ---------------------------------------------------------------------------
vector<vector<int>> buildBagOfWords(const vector<map<string, int>>& wordCounts,
                                    const vector<string>& vocabulary) {
    // TODO:
    // - Crear la matriz inicializada en 0 con tamaño [wordCounts.size() x vocabulary.size()]
    // - Para cada libro i:
    //     - Para cada palabra j del vocabulario:
    //         - Si la palabra existe en wordCounts[i], asignar su frecuencia a matriz[i][j]
    //         - Si no existe, dejar en 0 (ya está inicializado)
    // - Retornar la matriz
    return {};
}

// ---------------------------------------------------------------------------
// Guarda la matriz de Bolsa de Palabras en un archivo CSV
// Recibe:
//   - bowMatrix: la matriz de frecuencias
//   - vocabulary: las palabras (serán las columnas del CSV)
//   - urls: las URLs de los libros (serán las filas/etiquetas del CSV)
//   - filename: nombre del archivo de salida
// Formato esperado:
//   libro,palabra1,palabra2,...
//   url1,5,0,3,...
//   url2,0,2,1,...
// ---------------------------------------------------------------------------
void saveCsv(const vector<vector<int>>& bowMatrix,const vector<string>& vocabulary,const vector<string>& urls,const string& filename) {
    // TODO:
    // - Abrir un ofstream con el nombre del archivo
    // - Escribir la primera fila (encabezado): "libro," + todas las palabras separadas por comas
    // - Para cada libro i:
    //     - Escribir la URL como primera columna
    //     - Escribir cada valor de bowMatrix[i][j] separado por comas
    //     - Terminar la línea con "\n"
    // - Cerrar el archivo
}

// ---------------------------------------------------------------------------
// MAIN: orquesta todo el pipeline
// Argumentos: <archivo_urls> <num_libros>
// ---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <archivo_urls> <num_libros>\n";
        return 1;
    }

    string urlFile = argv[1];
    int k = stoi(argv[2]);

    if (k <= 0) {
        cerr << "Error: el número de libros debe ser mayor a 0\n";
        return 1;
    }

    // Paso 1: leer URLs disponibles y seleccionar k con wrap-around
    vector<string> availableUrls = readUrls(urlFile);
    if (availableUrls.empty()) {
        cerr << "Error: no hay URLs en el archivo\n";
        return 1;
    }
    int total = availableUrls.size();
    vector<string> selectedUrls(k);
    for (int i = 0; i < k; i++)
        selectedUrls[i] = availableUrls[i % total];

    cout << "URLs disponibles: " << total << "\n";
    cout << "Libros a procesar: " << k << "\n";

    // Paso 2: descargar cada libro
    // TODO: llamar downloadBook() para cada URL de selectedUrls

    // Paso 3: limpiar HTML y texto de cada libro
    // TODO: para cada libro descargado, llamar stripHtml() y luego cleanText()

    // Paso 4: contar palabras de cada libro
    // TODO: para cada texto limpio, llamar countWords()

    // Paso 5: construir vocabulario global
    // TODO: llamar buildVocabulary() con todos los mapas de frecuencias

    // Paso 6: construir matriz Bolsa de Palabras
    // TODO: llamar buildBagOfWords()

    // Paso 7: guardar resultado en CSV
    // TODO: llamar saveCsv() con el nombre "output.csv"

    return 0;
}
