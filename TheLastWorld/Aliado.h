#pragma once
#include <iostream>
#include <windows.h>
#include <cmath>
#include "Jugador.h"
#include "Mapa2.h"
using namespace std;

class Aliado {
private:
    int x, y;
    bool yaAyudo;
    int color;
    DWORD tiempoUltimaAyuda;
    CHAR_INFO fondoGuardado[15]; // 5x3 = 15 caracteres (5 de ancho, 3 de alto)
    bool protegiendo;
    Mapa* mapa;

public:
    Aliado(int x, int y, Mapa* mapa) :
        x(x), y(y), yaAyudo(false), color(6), tiempoUltimaAyuda(0),
        protegiendo(false), mapa(mapa) {
    }

    void guardarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 5, 3 };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT readRegion = {
            (SHORT)(x - 2), (SHORT)(y - 1),
            (SHORT)(x + 2), (SHORT)(y + 1)
        };
        ReadConsoleOutput(hConsole, (CHAR_INFO*)fondoGuardado, bufferSize, bufferCoord, &readRegion);
    }

    void restaurarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 5, 3 };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT writeRegion = {
            (SHORT)(x - 2), (SHORT)(y - 1),
            (SHORT)(x + 2), (SHORT)(y + 1)
        };
        WriteConsoleOutput(hConsole, (CHAR_INFO*)fondoGuardado, bufferSize, bufferCoord, &writeRegion);

        // Redibujar el mapa subyacente
        if (mapa) {
            for (int i = -1; i <= 1; i++) {
                for (int j = -2; j <= 2; j++) {
                    mapa->dibujarCelda(x + j, y + i);
                }
            }
        }
    }

    void dibujar() {
        guardarFondo(); // Siempre guardar fondo antes de dibujar

        if (protegiendo) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            gotoxy(x - 1, y - 1); cout << " ^ ";
            gotoxy(x - 1, y);     cout << "(O)";
            gotoxy(x - 1, y + 1); cout << " U ";
        }
        else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
            gotoxy(x - 1, y - 1); cout << " ^ ";
            gotoxy(x - 1, y);     cout << "(o)";
            gotoxy(x - 1, y + 1); cout << " U ";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    void ayudar(Jugador& jugador) {
        DWORD tiempoActual = GetTickCount();

        // Verificar cooldown (5 segundos)
        if (yaAyudo) {
            if (tiempoActual - tiempoUltimaAyuda < 5000) {
                return; // Todavía en cooldown
            }
            yaAyudo = false;
        }

        // Solo ayudar si está cerca (3 casillas) y el jugador necesita vida
        if (protegiendo && jugador.getVidas() < 3) {
            // Animación de ayuda
            for (int i = 0; i < 3; i++) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                gotoxy(x, y - 1); cout << "♥";
                gotoxy(x, y + 1); cout << "♥";
                Sleep(100);
                restaurarFondo();
                dibujar();
                Sleep(100);
            }

            // Dar vida al jugador
            jugador.ganarVida();

            // Activar cooldown
            yaAyudo = true;
            tiempoUltimaAyuda = tiempoActual;

            // Actualizar HUD
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            gotoxy(60, 0);
            cout << "¡Aliado te ayudó!";
            Sleep(500);
        }
    }

    void seguirJugador(Jugador& jugador, int ancho, int alto, int offsetY) {
        const int DISTANCIA_MINIMA = 3;
        const int DISTANCIA_MAXIMA = 5;

        int oldX = x;
        int oldY = y;
        int jugX = jugador.getX();
        int jugY = jugador.getY();

        // Verificar si está protegiendo
        protegiendo = (abs(jugX - x) <= 2 && abs(jugY - y) <= 2);

        // Calcular distancia
        int dx = jugX - x;
        int dy = jugY - y;
        double distancia = sqrt(dx * dx + dy * dy);

        if (distancia < DISTANCIA_MINIMA || distancia > DISTANCIA_MAXIMA) {
            restaurarFondo(); 
            if (abs(dx) > abs(dy)) {
                x += (dx > 0) ? 1 : -1;
                if (abs(jugX - x) <= 1 && abs(jugY - y) <= 1) {
                    x = oldX;
                }
            }
            else {
                y += (dy > 0) ? 1 : -1;
                if (abs(jugX - x) <= 1 && abs(jugY - y) <= 1) {
                    y = oldY;
                }
            }
            x = max(2, min(ancho - 3, x));
            y = max(offsetY + 2, min(offsetY + alto - 3, y));
            if (mapa && mapa->colisionConElemento(x, y)) {
                x = oldX;
                y = oldY;
            }

            if (x != oldX || y != oldY) {
                guardarFondo();
                dibujar();
            }
            else {
                guardarFondo();
                dibujar();
            }
        }
    }

    bool estaProtegiendo() const { return protegiendo; }
    int getX() const { return x; }
    int getY() const { return y; }
};