#pragma once
#include "CCohete.h"

class CControladorCohetes {
private:
    CCohete* cohetes[3];

public:
    CControladorCohetes(CMapa3* mapa); 
    void dibujarCohetes();
    void actualizarCohetes();
    void borrarCohetes();
};

CControladorCohetes::CControladorCohetes(CMapa3* mapa) {
    cohetes[0] = new CCohete(30, 19, mapa);
    cohetes[1] = new CCohete(60, 19, mapa);
    cohetes[2] = new CCohete(90, 19, mapa);
}

void CControladorCohetes::dibujarCohetes() {
    for (int i = 0; i < 3; ++i) {
        cohetes[i]->dibujar();
    }
}

void CControladorCohetes::actualizarCohetes() {
    for (int i = 0; i < 3; ++i) {
        if (cohetes[i]->estaActivo()) {
            cohetes[i]->mover();
        }
    }
}

void CControladorCohetes::borrarCohetes() {
    for (int i = 0; i < 3; ++i) {
        cohetes[i]->borrar();
    }
}