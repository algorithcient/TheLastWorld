#pragma once
#include <iostream>
#include <windows.h>
#include <thread>
#include <vector>
#include <random>
#include "Utilidades.h"
using namespace std;
using namespace System;

struct EstrellaFondo {
    int x, y;
    int brillo;
    int contador;
};

struct EfectoLuz {
    int x, y;
    int duracion;
    int color;
};

class Mapa {
private:
    int ancho, alto, offsetY;
    int colorActual;
    bool animacionActiva;
    thread hiloAnimacion;
    thread hiloEfectos;
    vector<EstrellaFondo> estrellas;
    vector<EfectoLuz> efectosLuz;
    vector<vector<int>> rejilla;
    int paletaColores[6] = { 9, 11, 3, 13, 5, 1 };
    int colorBorde;
    int tiempoTranscurrido;

    random_device rd;
    mt19937 gen;

public:
    Mapa() {
        ancho = 78;
        alto = 20;
        offsetY = 2;
        colorActual = 9;
        colorBorde = 11;
        animacionActiva = true;
        tiempoTranscurrido = 0;
        gen = mt19937(rd());

        rejilla.resize(alto, vector<int>(ancho, 0));

        hiloAnimacion = thread(&Mapa::animarColores, this);
        hiloAnimacion.detach();
    }

    ~Mapa() {
        animacionActiva = false;
        if (hiloAnimacion.joinable()) hiloAnimacion.join();
        if (hiloEfectos.joinable()) hiloEfectos.join();
    }

    void dibujar() {
        dibujarInstruccionesDerecha();

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        gotoxy(20, 0); cout << "  ";
        gotoxy(38, 0); cout << "  ";
        gotoxy(63, 0); cout << "  LA ENERGIA ZZZ:";

        dibujarEstrellas();
        cargarMatrizMapa();
        dibujarFondo();

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    void dibujarInstruccionesDerecha() {
        Console::SetCursorPosition(82, 11);
        cout << "----------------------------------" << endl;
        Console::SetCursorPosition(82, 12);
        cout << "|          Instrucciones          |" << endl;
        Console::SetCursorPosition(82, 13);
        cout << "----------------------------------" << endl;
        Console::SetCursorPosition(82, 14);
        cout << "| 1.Recolecta todos las partes del|" << endl;
        Console::SetCursorPosition(82, 15);
        cout << "| Cohete y la energia.Tu aliado te|" << endl;
        Console::SetCursorPosition(82, 16);
        cout << "| da vida cuando vas donde el y pulsas x | " << endl;
        Console::SetCursorPosition(82, 17);
        cout << "| 2.Evita el enemigo,             |" << endl;
        Console::SetCursorPosition(82, 18);
        cout << "| Solo tienes 3 vidas             |" << endl;
        Console::SetCursorPosition(82, 19);
        cout << "----------------------------------" << endl;
    }
    void animarColores() {
        int indiceColor = 0;
        while (animacionActiva) {
            colorBorde = paletaColores[indiceColor];
            indiceColor = (indiceColor + 1) % 6;
            tiempoTranscurrido++;
            if (ancho > 1 && alto > 0) {
                if (tiempoTranscurrido % 5 == 0) {
                    uniform_int_distribution<> disX(1, ancho - 1);
                    uniform_int_distribution<> disY(offsetY + 1, offsetY + alto - 1);
                }
            }

            Sleep(300);
        }
    }


    void dibujarEstrellas() {
        for (auto& estrella : estrellas) {
            gotoxy(estrella.x, estrella.y);
            estrella.contador++;
            if (estrella.contador > 20) {
                estrella.contador = 0;
                estrella.brillo = rand() % 3;
            }

            int color = (estrella.x * estrella.y) % 6;
            if (estrella.brillo == 2) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), paletaColores[color]);
                cout << '*';
            }
            else if (estrella.brillo == 1) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                cout << (char)249;
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                cout << (char)249;
            }
        }
    }

    void cargarMatrizMapa() {
        vector<vector<int>> matriz = {
            {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
            {2,4,0,0,0,0,0,0,2,2,2,2,4,4,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,4,0,0,0,0,0,0,0,2,2,4,2,2,4,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,0,4,4,4,4,4,4,0,0,2,2,4,4,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,0,4,4,4,4,4,0,0,0,2,4,2,2,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,0,0,0,0,0,0,0,0,0,0,2,4,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,0,0,0,0,0,0,0,0,0,2,4,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,2},
            {2,0,4,4,4,4,4,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,0,4,4,4,4,4,4,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,0,0,0,0,0,0,0,0,2,2,5,4,4,5,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,4,0,0,0,0,0,0,0,2,4,5,4,4,5,4,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,4,0,0,0,0,0,0,2,2,5,5,4,4,5,5,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
            {2,2,2,2,2,2,2,2,2,2,4,5,5,5,5,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
        };

        alto = matriz.size();
        ancho = matriz[0].size();
        rejilla = matriz;
    }

    void dibujarFondo() {
        for (int y = 0; y < alto; y++) {
            for (int x = 0; x < ancho; x++) {
                gotoxy(x, y + offsetY);
                switch (rejilla[y][x]) {
                case 0:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // gris
                    cout << " ";
                    break;
                case 1:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // bloque claro
                    cout << (char)219;
                    break;
                case 2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // rojo pared
                    cout << (char)219;
                    break;
                case 3:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // sangre
                    cout << (char)178;
                    break;
                case 4:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // electricidad
                    cout << (char)177;
                    break;
                default:
                    cout << " ";
                    break;
                }
            }
        }
    }


    bool colisionConElemento(int x, int y) {
        // Comprobar si hay colisión con algún elemento del mapa
        int mapaY = y - offsetY;

        if (mapaY >= 0 && mapaY < alto && x >= 0 && x < ancho) {
            return rejilla[mapaY][x] > 0;
        }

        return false;
    }

    bool esMeta(int x, int y) {
        // Comprobar si el jugador ha llegado a la meta
        int mapaY = y - offsetY;
        if (mapaY >= 0 && mapaY < alto && x >= 0 && x < ancho) {
            return rejilla[mapaY][x] == 5; // 5 representa la meta
        }
        return false;
    }

    void dibujarCelda(int x, int y) {
        int fila = y - offsetY;
        if (fila >= 0 && fila < alto && x >= 0 && x < ancho) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); // Gris por defecto
            gotoxy(x, y);

            switch (rejilla[fila][x]) {
            case 1: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << (char)219; break;
            case 3: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); cout << (char)178; break;
            case 4: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); cout << (char)177; break;
            default: cout << " "; break;
            }
        }
    }
    int getValor(int x, int y) {
        int fila = y - offsetY;
        if (fila >= 0 && fila < alto && x >= 0 && x < ancho) {
            return rejilla[fila][x];
        }
        return 0;
    }

    int getAncho() { return ancho; }
    int getAlto() { return alto; }
    int getOffsetY() { return offsetY; }
};