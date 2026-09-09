#pragma once
#include <mutex>
#include "CRecursoBase.h"
#include "Mapa1.h"
#include <iostream>
using namespace std;
using namespace System;

class CEmpatia : public CRecursoBase {
private:
    int meta;
    int recolectados;
    bool activo;
public:
    CEmpatia();

    void asignarPosicion(int posX, int posY);
    void dibujar() override;
    void borrar(CMapa* mapa);
    bool colisionaConJugador(int jugadorX, int jugadorY);
    void recolectar();
    void mostrarContador();
    bool haCompletadoMeta() const;
};

// === IMPLEMENTACIÓN ===

CEmpatia::CEmpatia() {
    x = y = 0;
    recolectados = 0;
    activo = false;
    meta = 1;
}

void CEmpatia::asignarPosicion(int posX, int posY) {
    x = posX;
    y = posY;
    activo = true;
}

void CEmpatia::dibujar() {
    if (!activo) return;
    Console::SetCursorPosition(x, y);
    Console::ForegroundColor = ConsoleColor::DarkRed;
    cout << "[E+]";
}

void CEmpatia::borrar(CMapa* mapa) {
    if (!activo) return;
    activo = false;

    for (int dx = 0; dx <= 4; dx++) {
        int col = x + dx;
        int valor = mapa->getValor(y, col);
        Console::SetCursorPosition(col, y);

        switch (valor) {
        case 0:  Console::ForegroundColor = ConsoleColor::Black; cout << (char)219; break;
        case 1:  Console::ForegroundColor = ConsoleColor::Blue;       cout << (char)179; break;
        case 2:  Console::ForegroundColor = ConsoleColor::Yellow;     cout << (char)177; break;
        case 3:  Console::ForegroundColor = ConsoleColor::Green;      cout << (char)177; break;
        case 4:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)218; break;
        case 5:  Console::ForegroundColor = ConsoleColor::Green;      cout << (char)177; break;
        case 6:  Console::ForegroundColor = ConsoleColor::Cyan;       cout << (char)254; break;
        case 7:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)196; break;
        case 8:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)191; break;
        case 9:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)192; break;
        case 10: Console::ForegroundColor = ConsoleColor::Red;        cout << (char)217; break;
        default: Console::ForegroundColor = ConsoleColor::Black;      cout << " "; break;
        }
    }
}

bool CEmpatia::colisionaConJugador(int jugadorX, int jugadorY) {
    if (!activo) return false;
    return (jugadorX <= x && x <= jugadorX + 4) && (jugadorY <= y && y <= jugadorY + 2);
}

void CEmpatia::recolectar() {
    if (activo) {
        recolectados++;
        activo = false;
    }
}

void CEmpatia::mostrarContador() {
    Console::SetCursorPosition(84, 8);       
    Console::ForegroundColor = ConsoleColor::White;
    cout << string(38, ' ');                
    Console::SetCursorPosition(84, 8);
    cout << "Empatia: " << recolectados << " / " << meta;
}

bool CEmpatia::haCompletadoMeta() const {
    return recolectados >= meta;
}