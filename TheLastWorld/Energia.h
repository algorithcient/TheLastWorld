#pragma once
#include <iostream>
#include <windows.h>
#include "Utilidades.h"
using namespace std;

#define SIMBOLO_ENERGIA 'z'

class Energia {
private:
    int x, y;
    int color;
public:
    Energia(int x, int y) {
        this->x = x;
        this->y = y;
        this->color = 10;  // Verde
    }

    void dibujar() {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        gotoxy(x - 1, y); cout << "zzz";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    void borrar() {
        gotoxy(x - 1, y); cout << "   ";
    }

    int getX() { return x; }
    int getY() { return y; }
};
