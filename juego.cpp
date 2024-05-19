#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

struct Datos {
    string nombre;
    int Id;
};

struct Jugador {
    Datos datos;
    int puntaje;
    vector<vector<string>> pistas; // Vector de pistas para cada pregunta
};

void jugarNivel2(const string& archivo, Jugador& jugador);

Datos LeerDatos() {
    Datos ingresarDato;

    cout << "Ingrese su nombre: ";
    getline(cin, ingresarDato.nombre);

    cout << "Ingrese su ID: ";
    cin >> ingresarDato.Id;
    cin.ignore(); // Limpiar el buffer de entrada después de leer el ID

    return ingresarDato;
}

void reglasdejuego3() {
    cout << "-----------------Bienvenido al juego Adivinar la palabra-------------------" << endl;
    cout << "En este juego te vamos a dar la descripcion de los personajes y tu tienes que adivinar el personaje correcto" << endl << endl;
    cout << "Tendras 10 oportunidades por turno. Cada respuesta correcta suma 10 puntos, y por cada incorrecta se restan 5 puntos." << endl;

    cout << "Ademas, tienes comodines que puedes usar para obtener una respuesta correcta automaticamente, pero ten cuidado, solo tienes unos pocos!" << endl;

    cout << "-----------------Listo, empecemos-------------------" << endl << endl;
}

void reglasdejuegoNivel2() {
    cout << "-----------------Bienvenido al juego Adivinar la palabra Nivel 2-------------------" << endl;
    cout << "En este nivel, te daremos cuatro opciones con definiciones de personajes y tendras que adivinar cual es la opcion correcta." << endl;
    cout << "Cada respuesta correcta suma 10 puntos, y por cada incorrecta se restan 5 puntos." << endl;
    cout << "Tendras 10 oportunidades por turno." << endl;
    cout << "Ademas, dispones de comodines que puedes usar para obtener la respuesta correcta automaticamente." << endl;
    cout << "Pero cuidado! Solo tienes unos pocos comodines disponibles." << endl;
    cout << "-----------------Listo, empecemos-------------------" << endl << endl;
}

vector<pair<string, string>> AbrirArchivoPreguntas(const string& filename) {
    vector<pair<string, string>> questions;
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return questions;
    }

    string line;
    while (getline(file, line)) {
        // Buscar el delimitador
        size_t pos = line.find(':');
        if (pos != string::npos && pos + 1 < line.length()) {
            string question = line.substr(0, pos);
            string answer = line.substr(pos + 1);
            questions.push_back({ question, answer });
        }
    }

    file.close();
    return questions;
}

void juego3Adivinarpalabra(Jugador& jugador) {
    reglasdejuego3();

    vector<pair<string, string>> preguntas = AbrirArchivoPreguntas("palabras.txt");

    int puntaje = 0;
    int intentos = 0;
    int comodines = 2; // Sistema de comodines
    for (const auto& pregunta : preguntas) {
        cout << pregunta.first << endl;

        cout << "Ingresa la palabra correcta (o usa un comodin): "<<endl;
        cout <<"Si deseas terminar el juego digita(salir): "<<endl;

        string respuesta;
        getline(cin, respuesta);

        if (respuesta == "salir") {
            cout << "Has decidido salir del juego." << endl;
            break;
        }

        // Convertir respuesta a minúsculas
        transform(respuesta.begin(), respuesta.end(), respuesta.begin(), ::tolower);

        // Si la respuesta es un comodín, usarlo
        if (respuesta == "comodin" && comodines > 0) {
            cout << "Has usado un comodin! La respuesta correcta es: " << pregunta.second << endl;
            respuesta = pregunta.second; // Asignar la respuesta correcta automáticamente
            comodines -= 1; // Reducir el número de comodines disponibles
        }

        if (pregunta.second == respuesta) {
            cout << "La palabra es correcta! Sumas 10 puntos." << endl << endl;
            puntaje += 10;
        } else {
            cout << "La palabra es incorrecta." << endl;
            cout << "La palabra correcta es: " << pregunta.second << endl << endl;
            puntaje -= 5;
        }
        intentos++;
    }
    cout << "Tu puntaje fue: " << puntaje << endl;

    jugador.puntaje = puntaje;
}

void juego3AdivinarpalabraNivel2(Jugador& jugador) {
    reglasdejuegoNivel2();
    jugarNivel2("nivel2.txt", jugador); 
}

void AlmacenarPuntajeNivel1(const Jugador& jugador) {
    ofstream archivoPuntajes("Nombresypuntajes.txt", ios::app);
    if (archivoPuntajes.is_open()) {
        archivoPuntajes << "Puntaje de " << jugador.datos.nombre << " (ID: " << jugador.datos.Id << "): " << jugador.puntaje << endl;
        archivoPuntajes.close();
        cout << "Puntaje almacenado correctamente en el archivo." << endl;
    } else {
        cout << "Error al abrir el archivo para almacenar el puntaje." << endl;
    }
}

void AlmacenarPuntajeNivel2(const Jugador& jugador) {
    ofstream archivoPuntajes("NombresypuntajesNivel2.txt", ios::app);
    if (archivoPuntajes.is_open()) {
        archivoPuntajes << "Puntaje de " << jugador.datos.nombre << " (ID: " << jugador.datos.Id << "): " << jugador.puntaje << endl;
        archivoPuntajes.close();
        cout << "Puntaje almacenado correctamente en el archivo." << endl;
    } else {
        cout << "Error al abrir el archivo para almacenar el puntaje." << endl;
    }
}

void JugarJuegoAdivinarPalabraNivel1() {
    Jugador jugador;
    jugador.datos = LeerDatos();
    juego3Adivinarpalabra(jugador);
    AlmacenarPuntajeNivel1(jugador);
}

void JugarJuegoAdivinarPalabraNivel2() {
    Jugador jugador;
    jugador.datos = LeerDatos();
    juego3AdivinarpalabraNivel2(jugador);
    AlmacenarPuntajeNivel2(jugador);
}

void RadixSort(vector<pair<string, int>>& puntajes) {
    auto getMax = [](const vector<pair<string, int>>& vec) {
        int max = vec[0].second;
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i].second > max) {
                max = vec[i].second;
            }
        }
        return max;
    };

    auto countSort = [&puntajes](int exp) {
        vector<pair<string, int>> output(puntajes.size());
        vector<int> count(10, 0);

        for (size_t i = 0; i < puntajes.size(); ++i) {
            count[(puntajes[i].second / exp) % 10]++;
        }

        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = puntajes.size() - 1; i >= 0; --i) {
            output[count[(puntajes[i].second / exp) % 10] - 1] = puntajes[i];
            count[(puntajes[i].second / exp) % 10]--;
        }

        for (size_t i = 0; i < puntajes.size(); ++i) {
            puntajes[i] = output[i];
        }
    };

    int max = getMax(puntajes);

    for (int exp = 1; max / exp > 0; exp *= 10) {
        countSort(exp);
    }
}

void OrdenarPuntajesMayorAMenor() {
    vector<pair<string, int>> puntajes;

    ifstream archivoPuntajes("Nombresypuntajes.txt");
    if (!archivoPuntajes.is_open()) {
        cout << "Error al abrir el archivo de puntajes." << endl;
        return;
    }

    string linea;
    while (getline(archivoPuntajes, linea)) {
        size_t inicio = linea.find(": ") + 2; // Buscar el inicio del puntaje
        size_t fin = linea.find(")", inicio); // Buscar el fin del puntaje
        if (inicio != string::npos && fin != string::npos) { // Verificar que se encontraron los delimitadores
            string puntaje_str = linea.substr(inicio, fin - inicio);
            int puntaje = stoi(puntaje_str);
            puntajes.push_back({ linea, puntaje });
        }
    }
    archivoPuntajes.close();

    RadixSort(puntajes);

    ofstream archivoOrdenado("Nombresypuntajes_Ordenados.txt");
    if (!archivoOrdenado.is_open()) {
        cout << "Error al abrir el archivo para escribir los puntajes ordenados." << endl;
        return;
    }
    for (auto it = puntajes.rbegin(); it != puntajes.rend(); ++it) {
        archivoOrdenado << it->first << endl;
    }
    archivoOrdenado.close();

    cout << "Puntajes ordenados y almacenados en el archivo 'Nombresypuntajes_Ordenados.txt'." << endl;
}

void OrdenarPuntajesMayorAMenorNivel2() {
    vector<pair<string, int>> puntajes;

    ifstream archivoPuntajes("NombresypuntajesNivel2.txt");
    if (!archivoPuntajes.is_open()) {
        cout << "Error al abrir el archivo de puntajes." << endl;
        return;
    }

    string linea;
    while (getline(archivoPuntajes, linea)) {
        size_t inicio = linea.find(": ") + 2; // Buscar el inicio del puntaje
        size_t fin = linea.find(")", inicio); // Buscar el fin del puntaje
        if (inicio != string::npos && fin != string::npos) { // Verificar que se encontraron los delimitadores
            string puntaje_str = linea.substr(inicio, fin - inicio);
            int puntaje = stoi(puntaje_str);
            puntajes.push_back({ linea, puntaje });
        }
    }
    archivoPuntajes.close();

    RadixSort(puntajes);

    ofstream archivoOrdenado("Nombresypuntajes_OrdenadosNivel2.txt");
    if (!archivoOrdenado.is_open()) {
        cout << "Error al abrir el archivo para escribir los puntajes ordenados." << endl;
        return;
    }
    for (auto it = puntajes.rbegin(); it != puntajes.rend(); ++it) {
        archivoOrdenado << it->first << endl;
    }
    archivoOrdenado.close();

    cout << "Puntajes ordenados y almacenados en el archivo 'Nombresypuntajes_OrdenadosNivel2.txt'." << endl;
}

void CrearArchivoSiNoExiste(const string& nombreArchivo, const string& contenido) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        ofstream nuevoArchivo(nombreArchivo);
        nuevoArchivo << contenido;
        nuevoArchivo.close();
    }
}

void InicializarArchivos() {
    CrearArchivoSiNoExiste("Nombresypuntajes.txt", "");
    CrearArchivoSiNoExiste("NombresypuntajesNivel2.txt", "");
    CrearArchivoSiNoExiste("Nombresypuntajes_Ordenados.txt", "");
    CrearArchivoSiNoExiste("Nombresypuntajes_OrdenadosNivel2.txt", "");
}

void jugarNivel2(const string& archivo, Jugador& jugador) {
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    string linea;
    int puntaje = 0;
    int comodines = 2;

    
    while (getline(file, linea)) {
        string pregunta = linea;
        string opciones[4];
        for (int i = 0; i < 4; ++i) {
            getline(file, opciones[i]);
        }
        string respuestaCorrecta;
        getline(file, respuestaCorrecta);

        cout << pregunta << endl;
        for (int i = 0; i < 4; ++i) {
            cout << char('A' + i) << ") " << opciones[i] << endl;
        }

        cout << "\nIngresa tu respuesta (A, B, C, D) o usa un comodin ingresando el carater 'M': "<<endl;
        cout <<"Si deseas terminar el juego digita(S): "<<endl;
        char respuestaUsuario;
        cin >> respuestaUsuario;
        respuestaUsuario = toupper(respuestaUsuario);

        if(respuestaUsuario == 'S'){
            cout << "Has salido del juego." << endl;
            break;
        }else if (respuestaUsuario == 'M' && comodines > 0) {
            cout << "Has usado un comodin! La respuesta correcta es: " << respuestaCorrecta << endl << "Sumas 10 puntos.\n" << endl;
            respuestaCorrecta = respuestaUsuario; // Asignar la respuesta correcta automaticamente
            comodines -= 1; // Reducir el número de comodines disponibles
            puntaje += 10;
            cin.ignore();
            continue;
        }

        int indiceRespuestaUsuario = respuestaUsuario - 'A';
        if (indiceRespuestaUsuario >= 0 && indiceRespuestaUsuario < 4) {
            if (opciones[indiceRespuestaUsuario] == respuestaCorrecta) {
                cout << "\nRespuesta correcta!. Sumas 10 puntos.\n" << endl;
                puntaje += 10;
            } else {
                cout << "\nRespuesta incorrecta. La respuesta correcta es: " << endl << respuestaCorrecta << endl << "Restas 5 puntos.\n" << endl;
                puntaje -= 5;
            }
        } else {
            cout << "Opcion no valida. Restas 5 puntos." << endl << "La respuesta correcta es: " << respuestaCorrecta << endl << endl;
            puntaje -= 5;
        }
    }

    cout << "\nTu puntaje fue: " << puntaje << endl;
    jugador.puntaje = puntaje;
    file.close();
}

int main() {
    InicializarArchivos(); // Inicializa los archivos necesarios

    int opcion;

    do {
        cout << "\n--- Menu ---" << endl;
        cout << "1. Jugar nivel 1" << endl;
        cout << "2. Jugar nivel 2" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                JugarJuegoAdivinarPalabraNivel1();
                OrdenarPuntajesMayorAMenor();
                break;
            case 2:
                JugarJuegoAdivinarPalabraNivel2();
                OrdenarPuntajesMayorAMenorNivel2();
                break;

            case 0:
                cout << "¡Hasta luego!" << endl;
                break;
            default:
                cout << "Opción no válida. Inténtalo de nuevo." << endl;
                break;
        }
    } while(opcion !=0);
    
    return 0;
}
