#pragma once
#include"conio.h"
#include"iostream"
using namespace std;
using namespace System;

void mostrarInstrucciones() {
    Console::ForegroundColor = ConsoleColor::White;
    Console::SetCursorPosition(82, 15);
    cout << "----------------------------------" << endl;
    Console::SetCursorPosition(82, 16);
    cout << "|          Instrucciones          |" << endl;
    Console::SetCursorPosition(82, 17);
    cout << "----------------------------------" << endl;
    Console::SetCursorPosition(82, 18);
    cout << "| 1.Recolecta todos los elementos |" << endl;
    Console::SetCursorPosition(82, 19);
    cout << "| Empatia->[E+] | Alimentos->[oo] |" << endl;
    Console::SetCursorPosition(82, 20);
    cout << "| Oxigeno->o2    |Agua ->h2o      |" << endl;
    Console::SetCursorPosition(82, 21);
    cout << "| 2.Evita el enemigo -> xx        |" << endl;
    Console::SetCursorPosition(82, 22);
    cout << "| Solo tienes 3 vidas             |" << endl;
    Console::SetCursorPosition(82, 23);
    cout << "----------------------------------" << endl;
}