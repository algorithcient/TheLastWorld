#pragma once
#include <iostream>
#include <windows.h>

class CRecursoBase {
protected:
    int x, y;

public:
    virtual void asignarPosicion(int px, int py) {
        x = px;
        y = py;
    }

    virtual void dibujar() = 0;

    virtual ~CRecursoBase() = default;
};
