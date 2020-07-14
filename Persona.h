#pragma once
// my_class.h
#ifndef PERSONA_H // include guard
#define PERSONA_H

#include <iostream>
using namespace std;

class Persona
{
public:
	int id;

	/*bool esSuceptioble;
	bool estaInfectado;
	bool estaRecuperado;
	bool estaMuerto;*/

	int estado; // 0 = suceptible, 1 = infectado, 2 = inmune, 3 muerto.

	int posicionInicial[2];
	int posicionActual[2];

	int tiempoInmune;
	int duracionEnfermedad;

	int rad, vel;

	int maxx, minx, maxy, miny;

	bool esJoven;

	Persona(int nId, bool nEstaInfectado, int posX, int posY, int rad, int vel, int tmm, bool edad);


	~Persona();

private:

};

Persona::Persona(int nId, bool estado, int posX, int posY, int rad, int vel, int tmm, bool edad)
{
	this->id = nId;
	this->esJoven = edad;
	this->estado = estado;
	this->posicionInicial[0] = posX;
	this->posicionInicial[1] = posY;
	this->posicionActual[0] = posX;
	this->posicionActual[1] = posY;
	this->rad = rad;
	this->vel = vel;
	if (rad == 0) {
		this->maxx = tmm - 1;
		this->maxy = tmm - 1;
		this->minx = 0;
		this->miny = 0;
	}
	else {
		if (posX - rad < 0) {
			this->minx = 0;
		}
		else {
			this->minx = posX - rad;
		}

		if (posX + rad > tmm-1) {
			this->maxx = tmm - 1;
		}
		else {
			this->maxx = posX + rad;
		}

		if (posY - rad < 0) {
			this->miny = 0;
		}
		else {
			this->miny = posY - rad;
		}

		if (posY + rad > tmm - 1) {
			this->maxy = tmm - 1;
		}
		else {
			this->maxy = posY + rad;
		}
	}
}

Persona::~Persona()
{
}



#endif /* PERSONA_H */