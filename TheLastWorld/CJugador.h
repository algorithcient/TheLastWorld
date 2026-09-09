#pragma once
#include <iostream>
#include <conio.h>  
#include <windows.h>
#include "Mapa1.h"
#include "COxigeno.h"
#include "CAgua.h"
#include "CEmpatia.h"
#include "CAlimentos.h"
#include "CEnemigo.h"

using namespace std;
using namespace System;

void gameOver();
void youWin();

class CJugador {
private:
    int x, y;
    int vidas;
    DWORD ultimoDaño;
    const int tiempoEsperaDaño = 1000; 
    COxigeno* oxigeno;
    CAgua* agua;
    CEmpatia* empatia;
    CAlimentos* alimentos;
    CMapa* mapa;
    CEnemigo* enemigo;

    void mostrarVidas();
    bool colisionaConEnemigo();
    bool todasLasMetasCompletadas();

public:
    CJugador(CMapa* m, COxigeno* o, CAgua* a, CEmpatia* e, CAlimentos* l, CEnemigo* enem = nullptr);
    ~CJugador();
    void dibujar();
    void borrar();
    void mover();
    void asignarPosicion(int posX, int posY);
    void perderVida();
    int getX() const { return x; }
    int getY() const { return y; }
    void setEnemigo(CEnemigo* enem) { enemigo = enem; }
};

CJugador::CJugador(CMapa* m, COxigeno* o, CAgua* a, CEmpatia* e, CAlimentos* l, CEnemigo* enem) {
    x = y = 0;
    vidas = 3;
    mapa = m;
    oxigeno = o;
    agua = a;
    empatia = e;
    alimentos = l;
    enemigo = enem;
    ultimoDaño = GetTickCount();
}

CJugador::~CJugador() {}

void CJugador::dibujar() {
    Console::ForegroundColor = ConsoleColor::DarkBlue;
    Console::SetCursorPosition(x, y);     cout << "  ^  ";
    Console::SetCursorPosition(x, y + 1); cout << " /0\\";
    Console::SetCursorPosition(x, y + 2); cout << ">---<";
    mostrarVidas();
}

void CJugador::borrar() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            int fila = y + i;
            int col = x + j;

            Console::SetCursorPosition(col, fila);

            if (fila >= 0 && fila < 30 && col >= 0 && col < 80) {
                int valor = mapa->getValor(fila, col);

                switch (valor) {
                case 0:  Console::ForegroundColor = ConsoleColor::Black;  cout << (char)219; break;
                case 1:  Console::ForegroundColor = ConsoleColor::Blue;   cout << (char)179; break;
                case 2:  Console::ForegroundColor = ConsoleColor::Yellow; cout << (char)177; break;
                case 3:  Console::ForegroundColor = ConsoleColor::Green;  cout << (char)177; break;
                case 4:  Console::ForegroundColor = ConsoleColor::Red;    cout << (char)218; break;
                case 5:  Console::ForegroundColor = ConsoleColor::Green;  cout << (char)177; break;
                case 6:  Console::ForegroundColor = ConsoleColor::Cyan;   cout << (char)254; break;
                case 7:  Console::ForegroundColor = ConsoleColor::Red;    cout << (char)196; break;
                case 8:  Console::ForegroundColor = ConsoleColor::Red;    cout << (char)191; break;
                case 9:  Console::ForegroundColor = ConsoleColor::Red;    cout << (char)192; break;
                case 10: Console::ForegroundColor = ConsoleColor::Red;    cout << (char)217; break;
                default: Console::ForegroundColor = ConsoleColor::Black;  cout << " "; break;
                }
            }
        }
    }
}

void CJugador::asignarPosicion(int posX, int posY) {
    x = posX;
    y = posY;
}

void CJugador::mostrarVidas() {
    Console::SetCursorPosition(84, 10);
    Console::ForegroundColor = ConsoleColor::White;
    cout << "Vidas: " << vidas << "   ";
}

bool CJugador::colisionaConEnemigo() {
    if (!enemigo) return false;
    int jLeft = x;
    int jRight = x + 4;
    int jTop = y;
    int jBottom = y + 2;
    int eLeft = enemigo->getX();
    int eRight = enemigo->getX() + 3;
    int eTop = enemigo->getY();
    int eBottom = enemigo->getY() + 1;  

    bool colision = (jRight >= eLeft && jLeft <= eRight) &&
        (jBottom >= eTop && jTop <= eBottom);

    if (colision) {
        Console::SetCursorPosition(82, 13);
        Console::ForegroundColor = ConsoleColor::Red;
        cout << "¡COLISIÓN DETECTADA!     ";
    }

    return colision;
}

void CJugador::perderVida() {
    vidas--;
    mostrarVidas();
    Beep(600, 150);
    Console::ForegroundColor = ConsoleColor::Red;
    dibujar();  
    Sleep(200);
    Console::ForegroundColor = ConsoleColor::DarkBlue;
    dibujar();
}

void CJugador::mover() {
    while (vidas > 0) {
        if (colisionaConEnemigo()) {
            Console::SetCursorPosition(82, 12);
            Console::ForegroundColor = ConsoleColor::Red;
            cout << "Colisión detectada      ";

            perderVida();
            mostrarVidas();
            Sleep(300);
        }

        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == -32) {
                tecla = _getch();

                int newX = x;
                int newY = y;

                switch (tecla) {
                case 72: newY = y > 0 ? y - 1 : y; break;
                case 80: newY = y < 27 ? y + 1 : y; break;
                case 75: newX = x > 0 ? x - 1 : x; break;
                case 77: newX = x < 75 ? x + 1 : x; break;
                }

                if (mapa->getValor(newY, newX) == 0 &&
                    mapa->getValor(newY + 1, newX) == 0 &&
                    mapa->getValor(newY + 2, newX) == 0) {

                    borrar();
                    x = newX;
                    y = newY;
                    dibujar();

                    bool recolectado = false;

                    if (!recolectado && oxigeno->colisionaConJugador(x, y)) {
                        oxigeno->recolectar(); recolectado = true;
                    }
                    if (!recolectado && agua->colisionaConJugador(x, y)) {
                        agua->recolectar(); recolectado = true;
                    }
                    if (!recolectado && empatia->colisionaConJugador(x, y)) {
                        empatia->recolectar(); recolectado = true;
                    }
                    if (!recolectado && alimentos->colisionaConJugador(x, y)) {
                        alimentos->recolectar(); recolectado = true;
                    }

                    oxigeno->mostrarContador();
                    agua->mostrarContador();
                    empatia->mostrarContador();
                    alimentos->mostrarContador();
                }
               
                if (oxigeno->haCompletadoMeta() &&
                    agua->haCompletadoMeta() &&
                    empatia->haCompletadoMeta() &&
                    alimentos->haCompletadoMeta()) {
                    break;
                }
            }
            else if (tecla == 27) {
                break;
            }
            Sleep(50);
        }
    }
    if (vidas <= 0) {
        Console::ResetColor();    
        Console::Clear();          
        gameOver();                
        Sleep(2000);
        Console::ResetColor();     
        Console::Clear();          
        exit(0);
    }
}

bool CJugador::todasLasMetasCompletadas() {
    return oxigeno->haCompletadoMeta() &&
        agua->haCompletadoMeta() &&
        empatia->haCompletadoMeta() &&
        alimentos->haCompletadoMeta();
}

