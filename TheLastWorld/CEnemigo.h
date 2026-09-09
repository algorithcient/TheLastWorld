#pragma once
#include "CRecursoBase.h"
#include "Mapa1.h"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace System;

class CEnemigo : public CRecursoBase {
private:
    bool activo;
    int direccion;
    int velocidad;
public:
    CEnemigo();
    void asignarPosicion(int posX, int posY);
    void dibujar() override;
    void borrar();
    void mover(CMapa* mapa);
    int getX() const { return x; }
    int getY() const { return y; }

};
CEnemigo::CEnemigo() {
    x = y = 0;
    activo = false;
    direccion = rand() % 4; 
    velocidad = 1;
}

void CEnemigo::asignarPosicion(int posX, int posY) {
    x = posX;
    y = posY;
    activo = true;
}

void CEnemigo::dibujar() {
    if (x >= 0 && x + 3 < COL && y >= 0 && y < FIL) {
        Console::SetCursorPosition(x, y);
        Console::ForegroundColor = ConsoleColor::DarkCyan;
        cout << "[XX]";
    }
}

void CEnemigo::borrar() {
    if (x >= 0 && x + 3 < COL && y >= 0 && y < FIL) {
        Console::SetCursorPosition(x, y);
        cout << "    ";
    }
}


void CEnemigo::mover(CMapa* mapa) {
    if (!activo) return;
    borrar();
    int dx = 0, dy = 0;
    switch (direccion) {
    case 0: dy = -1; break; 
    case 1: dy = 1; break; 
    case 2: dx = -1; break; 
    case 3: dx = 1; break; 
    }

    int nuevoX = x + dx;
    int nuevoY = y + dy;

    bool puedeMover = true;
    for (int i = 0; i < 4; ++i) {
        int col = nuevoX + i;
        if (col < 0 || col >= COL ||
            nuevoY < 0 || nuevoY >= FIL ||
            mapa->getValor(nuevoY, col) != 0) {
            puedeMover = false;
            break;
        }
    }
    if (puedeMover) {
        x = nuevoX;
        y = nuevoY;
    }
    else {

        int nuevaDireccion;
        do {
            nuevaDireccion = rand() % 4;
        } while (nuevaDireccion == direccion);
        direccion = nuevaDireccion;
    }
    dibujar();
}

