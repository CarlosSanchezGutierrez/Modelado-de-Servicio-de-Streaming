#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int MAX_VIDEOS = 100;

// Clase base Video
class Video {
protected:
    string titulo;
    string genero;
    double calificacion;
public:
    Video(string titulo, string genero, double calificacion)
        : titulo(titulo), genero(genero), calificacion(calificacion) {}

    virtual void mostrar() {
        cout << "Titulo: " << titulo << ", Genero: " << genero << ", Calificacion: " << calificacion << endl;
    }

    string getTitulo() { return titulo; }
    double getCalificacion() { return calificacion; }
    string getGenero() { return genero; }

    void setCalificacion(double nuevaCalificacion) { calificacion = nuevaCalificacion; }
};

// Clase derivada Pelicula
class Pelicula : public Video {
public:
    Pelicula(string titulo, string genero, double calificacion)
        : Video(titulo, genero, calificacion) {}

    void mostrar() override {
        cout << "Pelicula - ";
        Video::mostrar();
    }
};

// Clase derivada Episodio
class Episodio : public Video {
private:
    string serie;
public:
    Episodio(string titulo, string genero, double calificacion, string serie)
        : Video(titulo, genero, calificacion), serie(serie) {}

    void mostrar() override {
        cout << "Episodio de " << serie << " - ";
        Video::mostrar();
    }

    string getSerie() { return serie; }
};


void cargarDatos(Video* videos[], int &numVideos) {
    ifstream archivo("datos.txt");
    if (!archivo) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    string tipo, titulo, genero, serie;
    double calificacion;
    numVideos = 0;
    while (archivo >> tipo >> titulo >> genero >> calificacion && numVideos < MAX_VIDEOS) {
        if (tipo == "Pelicula") {
            videos[numVideos++] = new Pelicula(titulo, genero, calificacion);
        } else if (tipo == "Episodio") {
            archivo >> serie;
            videos[numVideos++] = new Episodio(titulo, genero, calificacion, serie);
        }
    }

    archivo.close();
    cout << "Datos cargados satisfactoriamente" << endl;
}

void mostrarVideos(Video* videos[], int numVideos, double calificacion, string genero) {
    for (int i = 0; i < numVideos; i++) {
        if ((calificacion == 0 || videos[i]->getCalificacion() >= calificacion) &&
            (genero.empty() || videos[i]->getGenero() == genero)) {
            videos[i]->mostrar();
        }
    }
}

#Esta creo que es una de las partes que más interesante me resultó de como podiamos poner nuestro código o plataforma de servicio de streaming
void mostrarEpisodios(Video* videos[], int numVideos, string serie, double calificacion) {
    for (int i = 0; i < numVideos; i++) {
        Episodio* episodio = dynamic_cast<Episodio*>(videos[i]);
        if (episodio && episodio->getSerie() == serie && episodio->getCalificacion() >= calificacion) {
            episodio->mostrar();
        }
    }
}
#Esta creo que también es una de las partes que más interesante me resultó de como podiamos poner nuestro código o plataforma de servicio de streaming

void mostrarPeliculas(Video* videos[], int numVideos, double calificacion) {
    for (int i = 0; i < numVideos; i++) {
        Pelicula* pelicula = dynamic_cast<Pelicula*>(videos[i]);
        if (pelicula && pelicula->getCalificacion() >= calificacion) {
            pelicula->mostrar();
        }
    }
}

// Función para calificar un video
void calificarVideo(Video* videos[], int numVideos, string titulo, double calificacion) {
    for (int i = 0; i < numVideos; i++) {
        if (videos[i]->getTitulo() == titulo) {
            videos[i]->setCalificacion(calificacion);
            cout << "Calificación actualizada con éxito" << endl;
            return;
        }
    }
    cout << "Contenido multimedia no encontrado" << endl;
}

// Función para validar la entrada del usuario
double validarEntradaCalificacion() {
    double calificacion;
    while (true) {
        cout << "Ingrese la calificación (0.0 - 5.0): ";
        cin >> calificacion;
        if (cin.fail() || calificacion < 0.0 || calificacion > 5.0) {
            cin.clear();
            cin.ignore(10000, '\n'); // Descartar el buffer de entrada
            cout << "Entrada inválida. Intente nuevamente." << endl;
        } else {
            break;
        }
    }
    return calificacion;
}

int main() {
    Video* videos[MAX_VIDEOS];
    int numVideos = 0;
    int opcion;
    string titulo, genero, serie;
    double calificacion;

    while (true) {
        cout << "Menú:" << endl;
        cout << "1. Cargar archivo de datos" << endl;
        cout << "2. Mostrar videos en general con una cierta calificación o de un cierto género" << endl;
        cout << "3. Mostrar episodios de una determinada serie con una calificación determinada" << endl;
        cout << "4. Mostrar películas con cierta calificación" << endl;
        cout << "5. Calificar un video" << endl;
        cout << "6. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cargarDatos(videos, numVideos);
            break;
        case 2:
            cout << "Ingrese el género (o presione Enter para omitir): ";
            cin.ignore();
            getline(cin, genero);
            cout << "Ingrese la calificación mínima (0 para omitir): ";
            calificacion = validarEntradaCalificacion();
            mostrarVideos(videos, numVideos, calificacion, genero);
            break;
        case 3:
            cout << "Ingrese el nombre de la serie: ";
            cin.ignore();
            getline(cin, serie);
            cout << "Ingrese la calificación mínima: ";
            calificacion = validarEntradaCalificacion();
            mostrarEpisodios(videos, numVideos, serie, calificacion);
            break;
        case 4:
            cout << "Ingrese la calificación mínima: ";
            calificacion = validarEntradaCalificacion();
            mostrarPeliculas(videos, numVideos, calificacion);
            break;
        case 5:
            cout << "Ingrese el título del contenido multimedia a calificar: ";
            cin.ignore();
            getline(cin, titulo);
            cout << "Ingrese la nueva calificación: ";
            calificacion = validarEntradaCalificacion();
            calificarVideo(videos, numVideos, titulo, calificacion);
            break;
        case 6:
            for (int i = 0; i < numVideos; i++) {
                delete videos[i];
            }
            numVideos = 0;
            cout << "Saliendo..." << endl;
            return 0;
        default:
            cout << "Opción inválida. Intente nuevamente." << endl;
            break;
        }
    }

    return 0;
}
