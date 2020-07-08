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

	bool esSuceptioble;
	bool estaInfectado;
	bool estaRecuperado;
	bool estaMuerto;

	int posicionInicial[2];
	int posicionActual[2];

	int tiempoInmune;
	int duracionEnfermedad;

	Persona(int nId, bool nEstaInfectado, int posX, int posY, );


	~Persona();

private:

};

Persona::Persona()
{
}

Persona::~Persona()
{
}



#endif /* PERSONA_H */