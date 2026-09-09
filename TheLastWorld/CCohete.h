#pragma once
#include <iostream>
#include "CMapa3.h"
using namespace std;
using namespace System;

class CCohete {
private:
    int x, y;
    bool activo;
    CMapa3* mapa;

public:
    CCohete(int x, int y, CMapa3* mapa);
    void mover();
    void dibujar();
    void borrar();
    bool estaActivo();
};

CCohete::CCohete(int x, int y, CMapa3* m) {
    this->x = x;
    this->y = y;
    this->activo = true;
    this->mapa = m;
}

void CCohete::mover() {
    if (!activo) return;
    borrar();
    if (y > 1) {
        y--;
        dibujar();
    }
    else {
        borrar();
        activo = false;
    }
}

void CCohete::borrar() {
    for (int dy = 0; dy < 10; dy++) {
        int fila = y + dy;
        if (fila >= 0 && fila < FIL) {
            for (int dx = 0; dx < 7; dx++) { 
                int col = x + dx;
                if (col >= 0 && col < COL) {
                    int valor = mapa->getValor(fila, col);
                    Console::SetCursorPosition(col, fila);
                    switch (valor) {
                    case 0:  Console::ForegroundColor = ConsoleColor::Cyan;       cout << (char)219; break;
                    case 1:  Console::ForegroundColor = ConsoleColor::White;      cout << (char)219; break;
                    case 2:  Console::ForegroundColor = ConsoleColor::Yellow;     cout << (char)179; break;
                    case 3:  Console::ForegroundColor = ConsoleColor::DarkYellow; cout << (char)219; break;
                    case 4:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)218; break;
                    case 5:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)219; break;
                    case 6:  Console::ForegroundColor = ConsoleColor::DarkGray;   cout << (char)254; break; 
                    case 7:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)196; break;
                    case 8:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)191; break;
                    case 9:  Console::ForegroundColor = ConsoleColor::Red;        cout << (char)192; break;
                    case 10: Console::ForegroundColor = ConsoleColor::Red;        cout << (char)217; break;
                    case 11: Console::ForegroundColor = ConsoleColor::Green;      cout << (char)219; break;
                    default: cout << " "; break;
                    }
                }
            }
        }
    }
}

void CCohete::dibujar() {
    Console::ForegroundColor = ConsoleColor::Black;
    Console::BackgroundColor = ConsoleColor::DarkRed;
    Console::SetCursorPosition(x, y);     cout << " / \\";
    Console::SetCursorPosition(x, y + 1); cout << "/   \\";
    Console::ForegroundColor = ConsoleColor::Black;
    Console::BackgroundColor = ConsoleColor::White;
    Console::SetCursorPosition(x, y + 2); cout << "|   |";
    Console::SetCursorPosition(x, y + 3); cout << "|NAS|";
    Console::SetCursorPosition(x, y + 4); cout << "|   |";
    Console::SetCursorPosition(x, y + 5); cout << "'   '";
    Console::ForegroundColor = ConsoleColor::Black;
    Console::BackgroundColor = ConsoleColor::DarkYellow;
    Console::SetCursorPosition(x, y + 6); cout << " / \\";
    Console::SetCursorPosition(x, y + 7); cout << "/   \\";
}

bool CCohete::estaActivo() {
    return activo;
}

