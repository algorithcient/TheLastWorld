#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <windows.h>
#include "Mapa2.h"
#include "Utilidades.h"
using namespace std;

enum class TipoObjeto {
    MOTOR,
    CUERPO,
    CABEZA,
    PROPULSOR,
    ENERGIA
};

static const map<TipoObjeto, vector<string>> ARTE_OBJETO = {
    { TipoObjeto::MOTOR,     { "([  ])", " ||| ", " ||| " } },
    { TipoObjeto::CUERPO,    { "  ___  ", " /   \\", "|_____|" } },
    { TipoObjeto::CABEZA,    { "  /^\\  ", " |   | ", " \\___/ " } },
    { TipoObjeto::PROPULSOR, { "  /=\\  ", " |=+=| ", "  \\=/  " } },
    { TipoObjeto::ENERGIA,   { " (~~~) ", " ~ZZZ~ ", " (~~~) " } }
};

class Objeto {
private:
    int x, y;
    TipoObjeto tipo;
    bool recolectado;
    CHAR_INFO fondoGuardado[15];
    Mapa* mapa; 

public:
    Objeto(int x, int y, TipoObjeto tipo, Mapa* mapa) :
        x(x), y(y), tipo(tipo), recolectado(false), mapa(mapa) {
        guardarFondo(); 
    }

    void guardarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 5, 3 };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT readRegion = {
            (SHORT)x, (SHORT)y,
            (SHORT)(x + 4), (SHORT)(y + 2)
        };
        ReadConsoleOutput(hConsole, fondoGuardado, bufferSize, bufferCoord, &readRegion);
    }

    void restaurarFondo() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { 5, 3 };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT writeRegion = {
            (SHORT)x, (SHORT)y,
            (SHORT)(x + 4), (SHORT)(y + 2)
        };
        WriteConsoleOutput(hConsole, fondoGuardado, bufferSize, bufferCoord, &writeRegion);
    }

    void borrar() {
        restaurarFondo(); 
        if (mapa) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    mapa->dibujarCelda(x + j, y + i);
                }
            }
        }
    }
   
    void dibujar() {
        if (recolectado) return; 

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);

        const auto& arte = ARTE_OBJETO.at(tipo);
        for (size_t i = 0; i < arte.size(); i++) {
            gotoxy(x, y + i);
            cout << arte[i];
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    void marcarRecolectado() {
        if (!recolectado) {
            recolectado = true;
            borrar();

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            gotoxy(x + 1, y + 1);
            cout << "★";
            Sleep(100);
            borrar(); 
        }
    }

    bool estaRecolectado() const {
        return recolectado;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    TipoObjeto getTipo() const { return tipo; }
};