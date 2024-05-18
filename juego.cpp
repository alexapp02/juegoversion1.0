#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct Datos {
    string nombre;
    int Id;
};

struct Jugador {
    Datos datos;
    int puntaje;
    vector<vector<string>> pistas; // Vector de pistas para cada pregunta
};

Datos LeerDatos() {
    Datos ingresarDato;

    cout << "Ingrese su nombre: ";
    getline(cin, ingresarDato.nombre);

    cout << "Ingrese su ID: ";
    cin >> ingresarDato.Id;
    cin.ignore(); // Limpiar el buffer de entrada después de leer el ID

    return ingresarDato;
}

