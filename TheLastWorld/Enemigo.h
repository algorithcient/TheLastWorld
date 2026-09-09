#pragma once
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include "Utilidades.h"
#include "Mapa2.h"
#include "Aliado.h"
#include "Jugador.h"
using namespace std;

class Enemigo {
private:
    int x, y;
    int color;
    int direccionX;
    int direccionY;
    DWORD tiempoUltimoCambio;
    DWORD tiempoUltimoMovimiento;
    CHAR_INFO fondoGuardado[3][3];
    bool chocando;

    void escribirEn(int x, int y, string texto, int color = 7) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD charsEscritos;
        SetConsoleTextAttribute(hConsole, color);
        WriteConsoleOutputCharacterA(hConsole, texto.c_str(), texto.length(), { (SHORT)x, (SHORT)y }, &charsEscritos);
    }

public:
    Enemigo(int x = 10, int y = 10) : x(x), y(y), color(4), chocando(false) {
        static bool semillaInicializada = false;
        if (!semillaInicializada) {
            srand(static_cast<unsigned>(time(nullptr)));
            semillaInicializada = true;
        }

        do {
            direccionX = (rand() % 3) - 1; // -1, 0, 1
            direccionY = (rand() % 3) - 1;
        } while (direccionX == 0 && direccionY == 0);

        tiempoUltimoCambio = GetTickCount();
        tiempoUltimoMovimiento = GetTickCount();
    }

    void guardarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 3, 3 };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT readRegion = {
            (SHORT)(x - 1),
            (SHORT)(y - 1),
            (SHORT)(x + 1),
            (SHORT)(y + 1)
        };
        ReadConsoleOutput(hConsole, (CHAR_INFO*)fondoGuardado, bufferSize, bufferCoord, &readRegion);
    }

    void restaurarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 3, 3 };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT writeRegion = {
            (SHORT)(x - 1),
            (SHORT)(y - 1),
            (SHORT)(x + 1),
            (SHORT)(y + 1)
        };
        WriteConsoleOutput(hConsole, (CHAR_INFO*)fondoGuardado, bufferSize, bufferCoord, &writeRegion);
    }

    void dibujar() {
        if (chocando) {
            escribirEn(x - 1, y - 1, "[X]", 12);
            escribirEn(x - 1, y, "|X|", 12);
            escribirEn(x - 1, y + 1, "\\_/", 12);
        }
        else {
            escribirEn(x - 1, y - 1, "[=]", color);
            escribirEn(x - 1, y, "|#|", color);
            escribirEn(x - 1, y + 1, "/_\\", color);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    void moverAleatorio(int offsetY, int ancho, int alto, Mapa& mapa) {
        int oldX = x;
        int oldY = y;
        restaurarFondo();
        if (rand() % 100 < 15) {
            do {
                direccionX = (rand() % 3) - 1;
                direccionY = (rand() % 3) - 1;
            } while (direccionX == 0 && direccionY == 0);
        }
        int nuevoX = x + direccionX;
        int nuevoY = y + direccionY;

        if (nuevoX <= 1 || nuevoX >= ancho - 2 ||
            nuevoY <= offsetY + 1 || nuevoY >= offsetY + alto - 2 ||
            mapa.colisionConElemento(nuevoX, nuevoY)) {
            do {
                direccionX = (rand() % 3) - 1;
                direccionY = (rand() % 3) - 1;
            } while (direccionX == 0 && direccionY == 0);
            guardarFondo();
            dibujar();
            return;
        }
        x = nuevoX;
        y = nuevoY;

        guardarFondo();
        dibujar();
    }
    void reiniciar(int startX, int startY) {
        restaurarFondo();
        x = startX;
        y = startY;
        chocando = false;
        do {
            direccionX = (rand() % 3) - 1;
            direccionY = (rand() % 3) - 1;
        } while (direccionX == 0 && direccionY == 0);

        guardarFondo();
        dibujar();
    }

    int getX() const { return x; }
    int getY() const { return y; }
};