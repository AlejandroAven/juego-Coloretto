// https://lucid.app/lucidchart/e2965142-c4db-4a07-8190-46939c524b8a/edit?viewport_loc=-1585%2C-1597%2C3928%2C1709%2C0_0&invitationId=inv_ca428d19-f04e-4a8c-816d-cb48a5ad3ed4

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BG_RED "\x1B[41m"
#define BG_GREEN "\x1B[42m"
#define BG_BLUE "\x1B[44m"
#define BG_YELLOW "\x1B[43m"

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

enum ColorCarta {
    AZUL,
    ROJO,
    VERDE,
    AMARILLO,
    MORADO,
    NARANJA,
    MARRON,
};

enum TipoCarta { NORMAL, ULTIMA_RONDA, CARTA_DORADA, CAMBIA_COLOR, COMODIN }; // Add COMODIN

struct Carta {
    ColorCarta color;
    TipoCarta tipo;
};

vector<Carta> crearMazo() {
    vector<Carta> mazo;
    for (int color = AZUL; color <= MARRON; color++) {
        for (int i = 0; i < 4; i++) { // 4 cartas por color
            mazo.push_back({(ColorCarta)color, NORMAL});
        }
    }

    // Asegurarse de que la carta de Última Ronda esté en las últimas 10 cartas
    for (int i = 0; i < 3; i++) { // Rellenar con 3 cartas normales
        mazo.push_back({(ColorCarta)(i % 7), NORMAL});
    }
    mazo.push_back({AZUL, ULTIMA_RONDA}); // Última Ronda

    // Add 3 wildcard cards
    for (int i = 0; i < 3; i++) {
        mazo.push_back({AZUL, COMODIN}); // You can change the color here if needed
    }

    return mazo;
}

void barajarMazo(vector<Carta> &mazo) {
    random_device rd;
    mt19937 g(rd());
    shuffle(mazo.begin(), mazo.end(), g);
}

void imprimirMano(const vector<vector<Carta>> &manoJugador) {
    for (size_t i = 0; i < manoJugador.size(); i++) {
        cout << "Pila " << i + 1 << ":" << endl;
        for (const Carta &carta : manoJugador[i]) {
            cout << " - ";
            switch (carta.color) {
                case AZUL:
                    cout << BG_BLUE;
                    break;
                case ROJO:
                    cout << BG_RED;
                    break;
                case VERDE:
                    cout << BG_GREEN;
                    break;
                case AMARILLO:
                    cout << BG_YELLOW;
                    break;
                case MORADO:
                    cout << MAGENTA;
                    break;
                case NARANJA:
                    cout << CYAN;
                    break;
                case MARRON:
                    cout << BLACK;
                    break;
            }
            cout << "Carta ";
            switch (carta.tipo) {
                case NORMAL:
                    cout << "Normal";
                    break;
                case ULTIMA_RONDA:
                    cout << "Última Ronda";
                    break;
                case CARTA_DORADA:
                    cout << "Carta Dorada";
                    break;
                case CAMBIA_COLOR:
                    cout << "Cambia Color";
                    break;
                case COMODIN:
                    cout << "Comodín"; // Add this case for the wildcard
                    break;
            }
            cout << RESET << endl; // Restablece el color al final
        }
    }
}

bool todasLasPilasEstanVacias(const vector<vector<Carta>> &pilas) {
    return all_of(pilas.begin(), pilas.end(),
                  [](const vector<Carta> &pila) { return pila.empty(); });
}

bool todasLasPilasEstanLlenas(const vector<vector<Carta>> &pilas) {
    return all_of(pilas.begin(), pilas.end(),
                  [](const vector<Carta> &pila) { return pila.size() >= 3; });
}

void jugarRonda(vector<vector<Carta>> &pilas, vector<Carta> &mazo,
                int numJugadores, vector<bool> &jugadoresHanTomadoPila,
                vector<vector<Carta>> &cartasJugadores, int ronda) {
    cout << "\n--- Ronda " << ronda << " ---" << endl; // Mostrar el número de ronda
    bool todosTomaron = true; // Para verificar si todos tomaron cartas/pilas

    // Variable para controlar si todos los jugadores han tomado una pila
    bool todosHanTomadoPila = false; 

    for (int i = 0; i < numJugadores; i++) {
        cout << "Jugador " << i + 1 << ", es tu turno." << endl;

        // Verificar si todas las pilas están llenas
        if (todasLasPilasEstanLlenas(pilas)) {
            cout << "Todas las pilas están llenas. Debes tomar una pila." << endl;
            int pilaElegida;
            cout << "Elige una pila (1-" << numJugadores << "): ";
            cin >> pilaElegida;

            if (pilaElegida > 0 && pilaElegida <= numJugadores) {
                if (!pilas[pilaElegida - 1].empty()) {
                    // Toma todas las cartas de la pila elegida
                    for (const Carta &carta : pilas[pilaElegida - 1]) {
                        cartasJugadores[i].push_back(carta);
                    }
                    pilas[pilaElegida - 1].clear(); // Vacía la pila
                    cout << "Jugador " << i + 1 << " ha tomado la pila " << pilaElegida << "." << endl;
                    jugadoresHanTomadoPila[i] = true; // Marcar que el jugador ha tomado una pila
                } else {
                    cout << "La pila " << pilaElegida << " está vacía." << endl;
                    todosTomaron = false; // Marcar que no todos han tomado una acción válida
                }
            } else {
                cout << "Opción inválida." << endl;
                todosTomaron = false; // Marcar que no todos han tomado una acción válida
            }
            continue; // Pasar al siguiente jugador
        }

        // Verificar si el jugador ya tomó una pila en esta ronda
        if (jugadoresHanTomadoPila[i]) {
            cout << "Jugador " << i + 1 << ", ya has tomado una pila. Debes esperar a que los demás jugadores tomen una pila." << endl;
            continue; // Salta el turno del jugador
        }

        // Preguntar si el jugador desea tomar una carta del mazo o una pila
        int eleccion;
        cout << "Elige una acción: \n1. Tomar una carta del mazo\n2. Tomar una pila" << endl;
        cin >> eleccion;

        if (eleccion == 1) {
            // Si el jugador elige tomar una carta del mazo
            if (!mazo.empty()) {
                Carta cartaTomada = mazo.back();
                mazo.pop_back();

                cout << "Has tomado la carta: " << endl;
                cout << " - ";
                switch (cartaTomada.color) {
                    case AZUL:
                        cout << BLUE << "Azul" << RESET;
                        break;
                    case ROJO:
                        cout << RED << "Rojo" << RESET;
                        break;
                    case VERDE:
                        cout << GREEN << "Verde" << RESET;
                        break;
                    case AMARILLO:
                        cout << YELLOW << "Amarillo" << RESET;
                        break;
                    case MORADO:
                        cout << MAGENTA << "Morado" << RESET;
                        break;
                    case NARANJA:
                        cout << CYAN << "Naranja" << RESET;
                        break;
                    case MARRON:
                        cout << BLACK << "Marrón" << RESET;
                        break;
                }
                cout << " - ";
                switch (cartaTomada.tipo) {
                    case NORMAL:
                        cout << "Normal";
                        break;
                    case ULTIMA_RONDA:
                        cout << "Última Ronda";
                        break;
                    case CARTA_DORADA:
                        cout << "Carta Dorada";
                        break;
                    case CAMBIA_COLOR:
                        cout << "Cambia Color";
                        break;
                    case COMODIN:
                        cout << "Comodín"; // Add this case for the wildcard
                        break;
                }
                cout << endl;

                // Elegir en qué pila colocar la carta tomada
                int pilaElegida;
                while (true) {
                    cout << "Elige en qué pila colocar la carta (1-" << numJugadores << "): ";
                    cin >> pilaElegida;

                    if (pilaElegida > 0 && pilaElegida <= numJugadores) {
                        if (pilas[pilaElegida - 1].size() < 3) {
                            // Pila no está llena, colocar la carta
                            pilas[pilaElegida - 1].push_back(cartaTomada);
                            cout << "Jugador " << i + 1 << " ha colocado la carta en la pila " << pilaElegida << "." << endl;
                            cout << "\nEstado de las pilas:" << endl;
                            imprimirMano(pilas);
                            break; // Salir del bucle
                        } else {
                            cout << "La pila " << pilaElegida << " ya está llena. No puedes colocar la carta ahí." << endl;
                        }
                    } else {
                        cout << "Pila inválida. Elige una pila válida." << endl;
                    }
                }

            } else {
                cout << "El mazo está vacío. No puedes tomar cartas." << endl;
                todosTomaron = false; // Marcar que no todos han tomado una acción válida
            }
        } else if (eleccion == 2) {
            // Si el jugador elige tomar una pila
            int pilaElegida;
            cout << "Elige una pila (1-" << numJugadores << "): ";
            cin >> pilaElegida;

            if (pilaElegida > 0 && pilaElegida <= numJugadores) {
                if (!pilas[pilaElegida - 1].empty()) {
                    // Toma todas las cartas de la pila elegida
                    for (const Carta &carta : pilas[pilaElegida - 1]) {
                        cartasJugadores[i].push_back(carta);
                    }
                    pilas[pilaElegida - 1].clear(); // Vacía la pila
                    cout << "Jugador " << i + 1 << " ha tomado la pila " << pilaElegida << "." << endl;
                    jugadoresHanTomadoPila[i] = true; // Marcar que el jugador ha tomado una pila
                } else {
                    cout << "La pila " << pilaElegida << " está vacía." << endl;
                    todosTomaron = false; // Marcar que no todos han tomado una acción válida
                }
            } else {
                cout << "Opción inválida." << endl;
                todosTomaron = false; // Marcar que no todos han tomado una acción válida
            }
        } else {
            cout << "Opción inválida." << endl;
            todosTomaron = false; // Marcar que no todos han tomado una acción válida
        }

        // Verificar si todos los jugadores han tomado una pila
        todosHanTomadoPila = all_of(jugadoresHanTomadoPila.begin(), jugadoresHanTomadoPila.end(), [](bool tomado) { return tomado; }); 

        // Si todos los jugadores han tomado una pila, reiniciar los marcadores
        if (todosHanTomadoPila) {
            fill(jugadoresHanTomadoPila.begin(), jugadoresHanTomadoPila.end(), false);
        }
    }

    // Verificar si todos los jugadores han tomado una acción válida
    if (todosTomaron) {
        cout << "Todos los jugadores han tomado una acción válida." << endl;
    }
}

void sumarPuntos(int numJugadores, vector<vector<Carta>> &cartasJugadores) {
    for (int i = 0; i < numJugadores; i++) {
        int puntosJugador = 0;
        int contadorColores[7] = {0}; // Contador de colores
        int contadorComodines = 0; // Contador de comodines

        // Contar las cartas por color y comodines
        for (const Carta &carta : cartasJugadores[i]) {
            if (carta.tipo == COMODIN) {
                contadorComodines++;
            } else {
                contadorColores[carta.color]++;
            }
        }

        // Mostrar los colores que tiene el jugador
        cout << "Jugador " << i + 1 << ", tienes las siguientes cartas:" << endl;
        for (int color = AZUL; color <= MARRON; color++) {
            if (contadorColores[color] > 0) {
                switch (color) {
                    case AZUL:
                        cout << BLUE << "Azul" << RESET << " (" << contadorColores[color] << ")" << endl;
                        break;
                    case ROJO:
                        cout << RED << "Rojo" << RESET << " (" << contadorColores[color] << ")" << endl;
                        break;
                    case VERDE:
                        cout << GREEN << "Verde" << RESET << " (" << contadorColores[color] << ")" << endl;
                        break;
                    case AMARILLO:
                        cout << YELLOW << "Amarillo" << RESET << " (" << contadorColores[color] << ")" << endl;
                        break;
                    case MORADO:
                        cout << MAGENTA << "Morado" << RESET << " (" << contadorColores[color] << ")" << endl;
                        break;
                    case NARANJA:
                        cout << CYAN << "Naranja" << RESET << " (" << contadorColores[color] << ")" << endl;
                        break;
                    case MARRON:
                        cout << BLACK << "Marrón" << RESET << " (" << contadorColores[color] << ")" << endl;
                        break;
                }
            }
        }

        // Preguntar al jugador qué colores quiere puntuar
        cout << "Jugador " << i + 1 << ", elige los colores que quieres puntuar (separados por espacios, -1 para terminar): ";
        vector<int> coloresAPuntuar;
        int color;
        while (cin >> color && color != -1) {
            if (color >= AZUL && color <= MARRON) {
                coloresAPuntuar.push_back(color);
            }
        }

        // Calcular puntos para los colores elegidos
        for (int color : coloresAPuntuar) {
            switch (contadorColores[color]) {
                case 1:
                    puntosJugador += 1;
                    break;
                case 2:
                    puntosJugador += 3;
                    break;
                case 3:
                    puntosJugador += 6;
                    break;
                case 4:
                    puntosJugador += 10;
                    break;
                case 5:
                    puntosJugador += 15;
                    break;
                case 6:
                    puntosJugador += 21;
                    break;
            }
        }

        // Calcular puntos para los colores NO elegidos (restan puntos)
        for (int color = AZUL; color <= MARRON; color++) {
            if (find(coloresAPuntuar.begin(), coloresAPuntuar.end(), color) == coloresAPuntuar.end()) {
                switch (contadorColores[color]) {
                    case 1:
                        puntosJugador -= 1;
                        break;
                    case 2:
                        puntosJugador -= 3;
                        break;
                    case 3:
                        puntosJugador -= 6;
                        break;
                    case 4:
                        puntosJugador -= 10;
                        break;
                    case 5:
                        puntosJugador -= 15;
                        break;
                    case 6:
                        puntosJugador -= 21;
                        break;
                }
            }
        }

        // Add points for wildcards
        puntosJugador += contadorComodines * 2;

        cout << "Jugador " << i + 1 << " ha obtenido " << puntosJugador << " puntos." << endl;
    }
}

int main() {
    int numJugadores;

    cout << "Ingrese el número de jugadores (3 a 5): ";
    cin >> numJugadores;

    while (numJugadores < 3 || numJugadores > 5) {
        cout << "Número de jugadores inválido. Ingrese un número entre 3 y 5: ";
        cin >> numJugadores;
    }

    vector<Carta> mazo = crearMazo();
    barajarMazo(mazo);

    vector<vector<Carta>> pilas(numJugadores); // Pilas para cada jugador
    vector<vector<Carta>> cartasJugadores(numJugadores); // Cartas de cada jugador
    vector<bool> jugadoresHanTomadoPila(numJugadores, false); // Para saber si los jugadores han tomado pila

    bool ultimaRondaJugando = false;
    int ronda = 1;

    while (true) {
        if (!ultimaRondaJugando) {
            if (mazo.empty()) {
                cout << "El mazo está vacío. Fin del juego." << endl;
                sumarPuntos(numJugadores, cartasJugadores);
                return 0;
            }
            jugarRonda(pilas, mazo, numJugadores, jugadoresHanTomadoPila, cartasJugadores, ronda);
        }

        // Revisar si alguien ha jugado la carta de Última Ronda
        for (int i = 0; i < numJugadores; i++) {
            if (!cartasJugadores[i].empty() && cartasJugadores[i].back().tipo == ULTIMA_RONDA) {
                cout << "Jugador " << i + 1 << " ha jugado la carta de Última Ronda." << endl;
                ultimaRondaJugando = true; // Activar la última ronda
                cartasJugadores[i].pop_back(); // Quitar la carta de Última Ronda de la mano
                cout << "Se jugará una última ronda." << endl;
                // Reiniciar los marcadores de los jugadores
                fill(jugadoresHanTomadoPila.begin(), jugadoresHanTomadoPila.end(), false);
                jugarRonda(pilas, mazo, numJugadores, jugadoresHanTomadoPila, cartasJugadores, ronda);
                cout << "La última ronda ha terminado." << endl;
                sumarPuntos(numJugadores, cartasJugadores);
                return 0; // Terminar el juego después de la última ronda
            }
        }

        ronda++;
    }

    // Sumar puntos al final del juego
    sumarPuntos(numJugadores, cartasJugadores);

    return 0;
}