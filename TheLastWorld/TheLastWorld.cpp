#include "pch.h"
#include "iostream"
#include "Juego.h"
#include "CJuego.h"
#include "CMapa3.h"
#include "Mapa2.h"
#include "Mapa1.h"
#include "CControladorCohetes.h"
#include <cstring>    
#include <algorithm>  
#include <string>     

using namespace System;
using namespace std;

int menu() {
	short opcion;
	int x = 30;
	int y = 10;
	Console::ForegroundColor = ConsoleColor::Blue;
	Console::BackgroundColor = ConsoleColor::Black;
	Console::Clear();
	Console::ForegroundColor = ConsoleColor::Blue;
	Console::SetCursorPosition(x, y);
	cout << " _____ _  _ ___   _      _   ___ _____  __      _____  ___ _    ___  ";
	Console::SetCursorPosition(x, y + 1);
	cout << "|_   _| || | __| | |    /_\\ / __|_   _| \\ \\    / / _ \\| _ \\ |  |   \\ ";
	Console::SetCursorPosition(x, y + 2);
	cout << "  | | | __ | _|  | |__ / _ \\\\__ \\ | |    \\ \\/\\/ / (_) |   / |__| |) |";
	Console::SetCursorPosition(x, y + 3);
	cout << "  |_| |_||_|___| |____/_/ \\_\\___/ |_|     \\_/\\_/ \\___/|_|_\\____|___/ ";
	Console::ForegroundColor = ConsoleColor::White;
	Console::SetCursorPosition(x, y + 4);
	cout << "___________________________________________________________________________";
	Console::SetCursorPosition(x + 3, y + 6); cout << " [1] INICIAR JUEGO";
	Console::SetCursorPosition(x + 3, y + 7); cout << " [2] PRIMER MAPA";
	Console::SetCursorPosition(x + 3, y + 8); cout << " [3] SEGUNDO MAPA";
	Console::SetCursorPosition(x + 3, y + 9); cout << " [4] TERCER MAPA";
	Console::SetCursorPosition(x + 3, y + 10); cout << " [5] SALIR";
	Console::SetCursorPosition(x + 3, y + 11); cout << "INGRESE UNA OPCION NUMERICA: "; cin >> opcion;
	return opcion;
}

void youWin() {
	short opcion;
	int x = 30;
	int y = 10;
	Console::ForegroundColor = ConsoleColor::Blue;
	Console::BackgroundColor = ConsoleColor::Black; 
	Console::Clear();
	Console::ForegroundColor = ConsoleColor::Blue;
	Console::SetCursorPosition(x, y);
	cout << "__   __                     _       ";
	Console::SetCursorPosition(x, y + 1);
	cout << "\\";
	Console::SetCursorPosition(x, y + 2);
	cout << "\\ \\ / /__  _   _  __      _(_)_ __ ";
	Console::SetCursorPosition(x, y + 3);
	cout << " \\ V / _ \\| | | | \\ \\ /\\ / / | '_ \\ ";
	Console::SetCursorPosition(x, y + 4);
	cout << "  | | (_) | |_| |  \\ V  V /| | | | |";
	Console::SetCursorPosition(x, y + 5);
	cout << "  |_|\\___/ \\__,_|   \\_/\\_/ |_|_| |_|";
}

void gameOver() {
	short opcion;
	int x = 30;
	int y = 10;
	Console::ForegroundColor = ConsoleColor::Blue;
	Console::SetCursorPosition(x, y);
	cout << "  ____                         ___                 ";
	Console::SetCursorPosition(x, y + 1);
	cout << " / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __ ";
	Console::SetCursorPosition(x, y + 2);
	cout << "| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|";
	Console::SetCursorPosition(x, y + 3);
	cout << "| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |";
	Console::SetCursorPosition(x, y + 4);
	cout << " \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|   ";
}

int main() {
	Console::SetWindowSize(110, 30);
	int x = 10;
	int y = 5;
	int opcion;
	while (true) {
		Console::Clear();
		opcion = menu();
		switch (opcion) {
		case 1: {
			Console::Clear();
			CJuego* Mapa1 = new CJuego();
			Mapa1->iniciar();
			delete Mapa1;

			Console::Clear();
			Console::SetCursorPosition(35, 14);
			Console::ForegroundColor = ConsoleColor::Yellow;
			cout << " Nivel completado! Cargando siguiente mapa...";
			Sleep(2000);

			Juego* Mapa2 = new Juego();
			Mapa2->iniciar();
			delete Mapa2;

			Console::Clear();
			Console::SetCursorPosition(35, 14);
			Console::ForegroundColor = ConsoleColor::Yellow;
			cout << " Nivel completado! Cargando siguiente mapa...";
			Sleep(2000);

			CMapa3* mapa = new CMapa3();
			mapa->imprimir();

			CControladorCohetes* controlador = new CControladorCohetes(mapa);
			controlador->dibujarCohetes();

			for (int i = 0; i < 40; ++i) {
				controlador->actualizarCohetes();
				Sleep(100);
			}

			delete controlador;
			delete mapa;

			Console::Clear();  
			youWin();          
			Sleep(5000);     
			break;             
		}

		case 2: {
			Console::Clear();
			CJuego* Mapa1 = new CJuego();
			Mapa1->iniciar();
			delete Mapa1;

			Console::Clear();
			Console::SetCursorPosition(35, 14);
			Console::ForegroundColor = ConsoleColor::Yellow;
			cout << " Nivel completado!";
			Sleep(2000);
			break;
		}
		case 3: {
			Juego* Mapa2 = new Juego();
			Mapa2->iniciar();
			delete Mapa2;

			Console::Clear();
			Console::SetCursorPosition(35, 14);
			Console::ForegroundColor = ConsoleColor::Yellow;
			cout << " Nivel completado!";
			Sleep(2000);
			break;
		}
		case 4: {
			CMapa3* mapa = new CMapa3();
			mapa->imprimir();

			CControladorCohetes* controlador = new CControladorCohetes(mapa);
			controlador->dibujarCohetes();

			for (int i = 0; i < 40; ++i) {
				controlador->actualizarCohetes();
				Sleep(100);
			}

			delete controlador;
			delete mapa;

			Console::Clear();
			youWin();
			Sleep(5000);
			break;
		}
		case 5: {
			Console::Clear();
			Console::SetCursorPosition(40, 14);
			Console::ForegroundColor = ConsoleColor::Red;
			cout << "Saliendo del juego...";
			Sleep(1500);
			return 0; 
		}
		default:
			cout << "Opcion no valida" << endl;
		}
		system("pause>0");
	}
}