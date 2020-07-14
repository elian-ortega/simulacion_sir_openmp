#pragma once
// my_class.h
#ifndef PERSONA_H // include guard
#define PERSONA_H

#include <iostream>
using namespace std;

class Persona
{
public:
	int estado;

	// Posicion
	int posicionInicial[2];
	int posicionActual[2];
	int rad;
	int vel;

	double potenciaInfecciosa;
	double probRecuperacion;
	int duracionEnfermedad;
	int duracionMinEnfermedad;
	int duracionMaxEnfermedad;
	int tiempoInmune;

	int tmm;

	Persona(int estado, int rad, int vel, int tmm, double potenciaInfecciosa, double probRecuperacion, int duracionMin, int duracionMax);
	
	void seInfecto();
	bool sobreviveEnfermedad();
	bool seContagio();
	void generarDuracionEnfermedad();
	void generarTiempoInmune();
	void pasoDia();

	string toString();

	void actuaizarPosicion();
	void generarPosicion();
	~Persona();

private:

};

Persona::Persona( int estado, int rad, int vel, int tmm, double potenciaInfecciosa, double probRecuperacion, int duracionMin, int duracionMax)
{
	this->estado = estado;// 0: suceptible 1:Infectado 2:Recuperado 3: muerto
	this->tmm = tmm;
	this->rad = rad;
	this->vel = vel;
	this->potenciaInfecciosa = potenciaInfecciosa;
	this->probRecuperacion = probRecuperacion;
	this->duracionMaxEnfermedad;
	this->duracionMinEnfermedad = duracionMin;
	this->duracionMaxEnfermedad = duracionMax;
	this->duracionEnfermedad = 0;

	if (estado == 1) {
		this->generarDuracionEnfermedad();
	}

<<<<<<< HEAD
	this->generarPosicion();
}
=======
		if (posX + rad > tmm - 1) {
			this->maxx = tmm - 1;
		}
		else {
			this->maxx = posX + rad;
		}
>>>>>>> 6fa7e2bbf29635e3f0270971f957d918f1419633

void Persona::seInfecto() {
	if (this->seContagio()) {
		this->estado = 1;
		this->generarDuracionEnfermedad();
	}
}

void Persona::pasoDia() {
	if (this->estado == 1) {
		this->duracionEnfermedad -= 1;
		if (this->duracionEnfermedad <= 0) {
			if (this->sobreviveEnfermedad()) {
				this->estado = 2;
				this->generarTiempoInmune();
			}
			else {
				this->estado = 3;
			}
		}
		else if(this->estado = 2){
			this->tiempoInmune -= 1;
			if (this->tiempoInmune <= 0) {
				this->estado = 0;
			}
		}
	}
}

bool Persona::sobreviveEnfermedad() {
	double prob = (float)rand() / RAND_MAX;
	if (this->probRecuperacion >= prob) {
		return true;
	}
	return false;
}

bool Persona::seContagio() {
	double prob = (float)rand() / RAND_MAX;
	if (this->potenciaInfecciosa <= prob) {
		return true;
	}
	return false;
}

void Persona::generarDuracionEnfermedad() {
	this->duracionEnfermedad = rand() % duracionMaxEnfermedad;
}

void Persona::generarTiempoInmune() {
	this->tiempoInmune = rand() % 60;
}

void Persona::generarPosicion() {
	this->posicionActual[0] = rand() % this->tmm;
	this->posicionActual[1] = rand() % this->tmm;
}

void Persona::actuaizarPosicion() {
	int actualx = this->posicionActual[0];
	int actualy = this->posicionActual[1];

	int nuevox = rand() % this->vel;
	int nuevoy = rand() % this->vel;

	this->posicionActual[0] = nuevox + actualx;
	this->posicionActual[1] = nuevoy + actualy;
}

string Persona::toString() {
	string cadena = "Estado : " + this->estado;
	return cadena;
}


Persona::~Persona()
{
}



#endif /* PERSONA_H */