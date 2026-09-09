#pragma once
#include <conio.h>
#include <windows.h>
#include <iostream>
#include "Jugador.h"
#include "Mapa2.h"
#include "Objeto.h"
#include "Enemigo.h"
#include "Aliado.h"

using namespace std;

#define MAX_OBJETOS 10
#define MAX_ENEMIGOS 1

void gameOver();
void youWin();
class Juego {
private:
    Jugador* jugador;
    Mapa* mapa;
    Objeto* objetos[MAX_OBJETOS];
    int totalObjetos = 6;
    Enemigo* enemigo;
    Aliado* aliado;
    int recolectados = 0;
    bool enJuego = true;

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void ocultarCursor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }

public:
    Juego() {
        srand(static_cast<unsigned>(time(nullptr)));

        // Inicialización segura de punteros
        for (int i = 0; i < MAX_OBJETOS; i++) {
            objetos[i] = nullptr;
        }
        enemigo = nullptr;
        aliado = nullptr;

        // Crear instancias principales
        mapa = new Mapa();
        jugador = new Jugador();

        // Inicializar componentes
        inicializarAliado();
        inicializarObjetos();
        inicializarEnemigo();
    }

    ~Juego() {
        // Limpieza segura de memoria
        if (jugador) delete jugador;
        if (mapa) delete mapa;

        for (int i = 0; i < totalObjetos; i++) {
            if (objetos[i]) delete objetos[i];
        }

        if (enemigo) delete enemigo;
        if (aliado) delete aliado;
    }

    void iniciar() {
        system("cls");
        ocultarCursor();

        // Dibujo inicial
        mapa->dibujar();
        if (enemigo) enemigo->guardarFondo();
        if (aliado) aliado->guardarFondo();
        jugador->guardarFondo();
        dibujarTodo();

        // Bucle principal del juego
        while (enJuego) {
            if (_kbhit()) {
                char tecla = _getch();

                if (tecla == 27) break; // Salir con ESC

                if (tecla == 'x' || tecla == 'X') {
                    if (aliado) aliado->ayudar(*jugador);
                }
                else {
                    jugador->mover(tecla, mapa->getOffsetY(), mapa->getAncho(), mapa->getAlto(), *mapa);
                }
                verificarColisiones();
            }

            // Actualización del juego
            moverEnemigo();
            moverAliado();
            redibujarJuego();

            // Verificar victoria
            if (mapa->esMeta(jugador->getX(), jugador->getY()) && recolectados >= totalObjetos) {
                finDelJuego("¡HAS GANADO! RECOLECTASTE TODAS LAS PIEZAS");
            }
            Sleep(30);
        }
    }

    void dibujarTodo() {
        // Dibujar objetos
        for (int i = 0; i < totalObjetos; i++) {
            if (objetos[i] && !objetos[i]->estaRecolectado()) {
                objetos[i]->guardarFondo();
                objetos[i]->dibujar();
            }
        }

        // Dibujar personajes
        if (enemigo) {
            enemigo->guardarFondo();
            enemigo->dibujar();
        }

        if (aliado) {
            aliado->guardarFondo();
            aliado->dibujar();
        }

        jugador->guardarFondo();
        jugador->dibujar();

        actualizarHUD();
    }

    void redibujarJuego() {
        // Restaurar fondos
        jugador->restaurarFondo();
        if (aliado) aliado->restaurarFondo();
        if (enemigo) enemigo->restaurarFondo();

        // Redibujar objetos
        for (int i = 0; i < totalObjetos; i++) {
            if (objetos[i] && !objetos[i]->estaRecolectado()) {
                objetos[i]->dibujar();
            }
        }

        // Redibujar personajes
        if (enemigo) {
            enemigo->guardarFondo();
            enemigo->dibujar();
        }

        if (aliado) {
            aliado->guardarFondo();
            aliado->dibujar();
        }

        jugador->guardarFondo();
        jugador->dibujar();

        actualizarHUD();
    }

    void moverEnemigo() {
        if (enemigo) {
            enemigo->moverAleatorio(
                mapa->getOffsetY(),
                mapa->getAncho(),
                mapa->getAlto(),
                *mapa
            );
        }
    }

    void moverAliado() {
        if (aliado) {
            aliado->restaurarFondo();
            aliado->seguirJugador(
                *jugador,
                mapa->getAncho(),
                mapa->getAlto(),
                mapa->getOffsetY()
            );
        }
    }

    void actualizarHUD() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Mostrar vidas
        SetConsoleTextAttribute(hConsole, 12);
        gotoxy(28, 0);
        cout << "VIDAS: " << jugador->getVidas() << " ";

        // Mostrar piezas recolectadas
        SetConsoleTextAttribute(hConsole, 10);
        gotoxy(50, 0);
        cout << "PIEZAS: " << recolectados << "/" << totalObjetos << " ";

        SetConsoleTextAttribute(hConsole, 7);
    }

    void inicializarObjetos() {
        objetos[0] = new Objeto(10, 5, TipoObjeto::CABEZA, mapa);
        objetos[1] = new Objeto(20, 12, TipoObjeto::CUERPO, mapa);
        objetos[2] = new Objeto(35, 6, TipoObjeto::MOTOR, mapa);
        objetos[3] = new Objeto(50, 10, TipoObjeto::MOTOR, mapa);
        objetos[4] = new Objeto(60, 15, TipoObjeto::PROPULSOR, mapa);
        objetos[5] = new Objeto(70, 7, TipoObjeto::ENERGIA, mapa);
    }

    void inicializarEnemigo() {
        enemigo = new Enemigo(25, 4);
    }

    void inicializarAliado() {
        if (aliado) {
            aliado->restaurarFondo();
            delete aliado;
        }

        try {
            aliado = new Aliado(5, 17, mapa);
            aliado->guardarFondo();
            aliado->dibujar();
        }
        catch (...) {
            aliado = nullptr;
            cerr << "Error al crear aliado\n";
        }
    }

    void verificarColisiones() {
        
        for (int i = 0; i < totalObjetos; i++) {
            if (objetos[i] && !objetos[i]->estaRecolectado() &&
                abs(jugador->getX() - objetos[i]->getX()) <= 1 &&
                abs(jugador->getY() - objetos[i]->getY()) <= 1) {

                objetos[i]->marcarRecolectado();
                objetos[i]->borrar();
                recolectados++;
                actualizarHUD();

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                gotoxy(objetos[i]->getX() + 2, objetos[i]->getY() + 1);
                cout << "★";
                Sleep(150);
                objetos[i]->borrar();

                if (recolectados >= totalObjetos) {
                    finDelJuego("¡RECOLECTASTE TODAS LAS PIEZAS!");
                }
            }
        }
        if (enemigo && abs(jugador->getX() - enemigo->getX()) <= 1 &&
            abs(jugador->getY() - enemigo->getY()) <= 1) {

            enemigo->restaurarFondo();

            if (!aliado || !aliado->estaProtegiendo()) {
                jugador->perderVida();
                actualizarHUD();

                if (jugador->getVidas() <= 0) {
                    finDelJuego("¡PERDISTE TODAS TUS VIDAS!");
                }

                for (int j = 0; j < 3; j++) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    gotoxy(jugador->getX(), jugador->getY());
                    cout << "X";
                    Sleep(80);
                    jugador->restaurarFondo();
                    jugador->dibujar();
                    Sleep(80);
                }
            }

            enemigo->moverAleatorio(mapa->getOffsetY(), mapa->getAncho(), mapa->getAlto(), *mapa);
        }

        if (enemigo && aliado &&
            abs(enemigo->getX() - aliado->getX()) <= 1 &&
            abs(enemigo->getY() - aliado->getY()) <= 1) {

            enemigo->restaurarFondo();
            aliado->restaurarFondo();

            enemigo->moverAleatorio(
                mapa->getOffsetY(),
                mapa->getAncho(),
                mapa->getAlto(),
                *mapa
            );

            aliado->guardarFondo();
            aliado->dibujar();

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            gotoxy(aliado->getX(), aliado->getY());
            cout << "★";
            Sleep(100);

            aliado->restaurarFondo();
            aliado->dibujar();
        }
    }

    void finDelJuego(string mensaje) {
        system("cls");

        bool perdio = mensaje.find("PERDISTE") != string::npos;

        if (perdio) {
            Console::Clear();
            gameOver();
            Sleep(2000);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            gotoxy(40 - mensaje.length() / 2, 12);
            cout << mensaje;

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            gotoxy(35, 14);
            cout << "Piezas recolectadas: " << recolectados << "/" << totalObjetos;

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            gotoxy(32, 18);
            cout << "Presiona cualquier tecla para salir";

            _getch();
            exit(0); 
        }
        else {
            enJuego = false; 
        }
    }

};