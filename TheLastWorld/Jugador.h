#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include "Utilidades.h"
#include "Mapa2.h"

using namespace std;

class Jugador {
private:
    int x, y;
    int vidas;
    int energia;
    int color;
    char direccion;
    bool velocidadExtra;
    int velocidadTick;

    CHAR_INFO fondoGuardado[5][5]; 

    void escribirEn(int x, int y, const string& texto, int color = 7) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD charsEscritos;
        SetConsoleTextAttribute(hConsole, color);
        WriteConsoleOutputCharacterA(hConsole, texto.c_str(), texto.length(), { (SHORT)x, (SHORT)y }, &charsEscritos);
    }

public:
    Jugador(int offsetY = 2) {
        x = 9;
        y = offsetY + 4;
        vidas = 3;
        energia = 100;
        color = 11;
        direccion = '^';
        velocidadExtra = false;
        velocidadTick = 0;
    }

    void guardarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 5, 3 }; 
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT readRegion = {
            (SHORT)(x - 2),  // 2 espacios a la izquierda
            (SHORT)(y - 1),  // 1 línea arriba
            (SHORT)(x + 2),  // 2 espacios a la derecha
            (SHORT)(y + 1)   // 1 línea abajo
        };
        ReadConsoleOutput(hConsole, (CHAR_INFO*)fondoGuardado, bufferSize, bufferCoord, &readRegion);
    }

    void restaurarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 5, 3 };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT writeRegion = {
            (SHORT)(x - 2),
            (SHORT)(y - 1),
            (SHORT)(x + 2),
            (SHORT)(y + 1)
        };
        WriteConsoleOutput(hConsole, (CHAR_INFO*)fondoGuardado, bufferSize, bufferCoord, &writeRegion);
    }

    void dibujar() {
        escribirEn(x - 1, y - 1, " O ", color);  // Centrado en X (usa x-1)
        escribirEn(x - 1, y, "/#\\", color);  // Centrado en X
        escribirEn(x - 1, y + 1, "/ \\", color);  // Centrado en X
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    void mover(char tecla, int offsetY, int ancho, int alto, Mapa& mapa) {
        restaurarFondo(); // limpia sprite anterior

        int antiguoX = x;
        int antiguoY = y;

        switch (tecla) {
        case 'w': case 'W': if (y > offsetY + 1) y--; direccion = '^'; break;
        case 's': case 'S': if (y < offsetY + alto - 2) y++; direccion = 'v'; break;
        case 'a': case 'A': if (x > 1) x--; direccion = '<'; break;
        case 'd': case 'D': if (x < ancho - 4) x++; direccion = '>'; break;
        case 'c': case 'C': color = (color == 11) ? 9 : 11; return;
        }

        if (x == antiguoX && y == antiguoY) {
            guardarFondo();
            dibujar();
            return;
        }

        Sleep(velocidadExtra ? 20 : 50);
        guardarFondo();
        dibujar();

        if (velocidadExtra) {
            velocidadTick++;
            if (velocidadTick > 20) {
                velocidadExtra = false;
                velocidadTick = 0;
                escribirEn(63, 0, "< BOOST OFF >", 7);
            }
        }
    }

    void reiniciar(int startX, int startY) {
        restaurarFondo();
        x = startX;
        y = startY;
        direccion = '^';
        guardarFondo();
        dibujar();
    }

    void perderVida() {
        if (vidas > 0) {
            vidas--;
            for (int i = 0; i < 3; i++) {
                escribirEn(x - 1, y - 1, "X X", 12);
                escribirEn(x - 1, y, "X#X", 12);
                escribirEn(x - 1, y + 1, "X X", 12);
                Sleep(100);
                restaurarFondo();
                dibujar();
                Sleep(100);
            }
        }
    }
    void ganarVida() {
        if (vidas < 3) {
            vidas++;
            restaurarFondo();
            escribirEn(x, y - 1, "\\|/", 10);
            escribirEn(x, y, "-O-", 10);
            escribirEn(x, y + 1, "/ \\", 10);
            Sleep(150);
            guardarFondo();
            dibujar();
        }
    }

    void consumirEnergia(int cantidad) {
        energia -= cantidad;
        if (energia < 0) energia = 0;
        escribirEn(63, 0, "< ENERGY " + to_string(energia) + "% >", 8);
    }

    void recargarEnergia(int cantidad) {
        energia += cantidad;
        if (energia > 100) energia = 100;
        escribirEn(63, 0, "< ENERGY " + to_string(energia) + "% >", 10);
    }

    void activarVelocidadExtra() {
        velocidadExtra = true;
        velocidadTick = 0;
        escribirEn(63, 0, "< BOOST ON!  >", 10);
    }

    int getX() { return x; }
    int getY() { return y; }
    int getVidas() { return vidas; }
    int getEnergia() { return energia; }
};
