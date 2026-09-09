#pragma once
#include "COxigeno.h"
#include "CAgua.h"
#include "CAlimentos.h"
#include "CEmpatia.h"
#include "CRecursoBase.h"
#include"Mapa1.h"
#include "CJugador.h"
#include <ctime>
#include <thread>
#include <chrono>
#include "CEnemigo.h"
#include "CInstrucciones.h"
#include "CEnemigo.h"
using namespace std;
using namespace System;

#define NUM_RECURSOS 4
#define NUM_ENEMIGOS 1

class CJuego {
private:
    CRecursoBase* recursos[NUM_RECURSOS];
    COxigeno* oxigeno;
    CAgua* agua;
    CEmpatia* empatia;
    CAlimentos* alimentos;
    CEnemigo* enemigo;
    bool juegoActivo;

public:
    CJuego();
    ~CJuego();
    void iniciar();
    void dibujarRecursos();
    void InicializarEnemigo();
    void mostrarHUD();
};


CJuego::CJuego() {
    recursos[0] = new COxigeno();
    recursos[1] = new CAgua();
    recursos[2] = new CAlimentos();
    recursos[3] = new CEmpatia();
    oxigeno = new COxigeno(); 
    agua = new CAgua();
    empatia = new CEmpatia();
    alimentos = new CAlimentos();
    juegoActivo = true;


}

CJuego::~CJuego() {
    for (int i = 0; i < NUM_RECURSOS; ++i) {
        delete recursos[i];
        recursos[i] = nullptr;
    }
    delete oxigeno;
    oxigeno = nullptr;

    delete agua;
    agua = nullptr;

    delete empatia;
    empatia = nullptr;

    delete alimentos;
    alimentos = nullptr;

}
void CJuego::iniciar() {
    srand((unsigned)time(0));
    Console::SetWindowSize(110, 30);


    CMapa* mapa1 = new CMapa();
    mapa1->imprimir();

    enemigo = new CEnemigo();
    enemigo->asignarPosicion(10, 10);
    enemigo->dibujar();

    CJugador* jugador = new CJugador(mapa1, oxigeno, agua, empatia, alimentos, enemigo);
    jugador->asignarPosicion(3, 2);
    jugador->dibujar();

    std::thread enemigoThread;
    std::thread oxigenoThread;
    std::thread aguaThread;
    std::thread alimentosThread;
    std::thread empatiaThread;

    enemigoThread = std::thread([=]() {
        while (juegoActivo) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            enemigo->mover(mapa1);
        }
        });

    mostrarInstrucciones();
    oxigenoThread = std::thread([=]() {
        while (!oxigeno->haCompletadoMeta()) {
            std::this_thread::sleep_for(std::chrono::seconds(6));
            int x, y;
            do {
                x = rand() % COL;
                y = rand() % FIL;
            } while (
                mapa1->getValor(y, x) != 0 ||
                agua->colisionaConJugador(x, y) ||
                alimentos->colisionaConJugador(x, y) ||
                empatia->colisionaConJugador(x, y)
                );
            oxigeno->asignarPosicion(x, y);
            oxigeno->dibujar();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            oxigeno->borrar(mapa1);
        }
        Console::SetCursorPosition(84, 3);
        Console::ForegroundColor = ConsoleColor::DarkYellow;
        cout << "¡Meta completada!";
        });
    aguaThread = std::thread([=]() {
        while (!agua->haCompletadoMeta()) {
            std::this_thread::sleep_for(std::chrono::seconds(9));
            int x, y;
            do {
                x = rand() % COL;
                y = rand() % FIL;
            } while (
                mapa1->getValor(y, x) != 0 ||
                oxigeno->colisionaConJugador(x, y) ||
                alimentos->colisionaConJugador(x, y) ||
                empatia->colisionaConJugador(x, y)
                );
            agua->asignarPosicion(x, y);
            agua->dibujar();
            std::this_thread::sleep_for(std::chrono::seconds(6));
            agua->borrar(mapa1);
        }
        Console::SetCursorPosition(84, 5);
        Console::ForegroundColor = ConsoleColor::DarkCyan;
        cout << "¡Meta completada!";
        });
    alimentosThread = std::thread([=]() {
        while (!alimentos->haCompletadoMeta()) {
            std::this_thread::sleep_for(std::chrono::seconds(6));
            int x, y;
            do {
                x = rand() % COL;
                y = rand() % FIL;
            } while (
                mapa1->getValor(y, x) != 0 ||
                agua->colisionaConJugador(x, y) ||
                oxigeno->colisionaConJugador(x, y) ||
                empatia->colisionaConJugador(x, y)
                );
            alimentos->asignarPosicion(x, y);
            alimentos->dibujar();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            alimentos->borrar(mapa1);
        }
        Console::SetCursorPosition(84, 7);
        Console::ForegroundColor = ConsoleColor::DarkGray;
        cout << "¡Meta completada!";
        });

    empatiaThread = std::thread([=]() {
        while (!empatia->haCompletadoMeta()) {
            std::this_thread::sleep_for(std::chrono::seconds(6));
            int x, y;
            do {
                x = rand() % COL;
                y = rand() % FIL;
            } while (
                mapa1->getValor(y, x) != 0 ||
                agua->colisionaConJugador(x, y) ||
                oxigeno->colisionaConJugador(x, y) ||
                alimentos->colisionaConJugador(x, y)
                );
            empatia->asignarPosicion(x, y);
            empatia->dibujar();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            empatia->borrar(mapa1);
        }
        Console::SetCursorPosition(84, 9);
        Console::ForegroundColor = ConsoleColor::DarkRed;
        cout << "¡Meta completada!";
        });
    jugador->mover(); 

    juegoActivo = false;

    enemigoThread.join();
    oxigenoThread.join();
    aguaThread.join();
    alimentosThread.join();
    empatiaThread.join();

    
}


void CJuego::dibujarRecursos() {

    for (int i = 0; i < NUM_RECURSOS; ++i) {
        if (recursos[i]) {
            recursos[i]->dibujar();
        }
    }
}
