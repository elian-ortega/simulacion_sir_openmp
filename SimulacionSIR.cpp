// SimulacionSIR.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <omp.h>
#include <cmath>
#include <vector>
#include <list>
#include <iterator>
#include <time.h>
#include "Persona.h"

using namespace std;
int main()
{
    srand(time(NULL));
    ofstream resultado;
    resultado.open("resultado.txt");
    resultado << "Promedio Susceptibles, Porcentaje Susceptibles, Cantidad de Suceptibles, Promedio Infectados, Porcentaje Infectados, Cantidad de Infectados, Promedio Resistentes, Porcentaje Resistentes, Cantidad de Resistentes, Promedio Muertos, Porcentaje Muertos, Cantidad de Muertos \n";
    // TODO: PEDIRLO PARAM
    int cpr = 100; // cantidad de personas int ]0,10000000]
    float piv = 0.25; // potencia infecciosa ]0,1[
    float prj = 0.75; // probabilidad de recuperacion jovenes ]0,1[
    float prv = 0.5; // probabilidad de recuperacion mayores ]0,1[
    int poi = 10; // cantidad de personas originalmente infectadas ]0,100]
    float toc = 0.3; // tasa de ocupación ]0,1[ 
    int dmn = 50; // duración minima de la enfermedad ]0,100]
    int dmx = 100; // duración máxima de la enfermedad ]0,100]
    int rmj = 4; // radio de movilidad de jovenes [0,5]
    int rmm = 2; // radio de movilidad de mayores [0,5]
    int vmj = 5; // velocidad de movimiento jovenes [0,5]
    int vmm = 3; // velocidad de movimiento mayores [0,5]
    int drc = 1000; // duración de la siomulación ]0,100000]
    int tmm = floor(sqrt(cpr / toc)) + 1; // tamaño de la matriz
    int nhilos = 8; // cantidad de hilos

    vector<Persona> personas;
    vector<vector<vector<Persona> > > espacio (tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
    vector<vector<vector<Persona> > > espacionuevo (tmm, vector<vector<Persona>>(tmm, vector<Persona>()));

    vector < vector<list<Persona>>> matriz_listas;
    vector<list<Persona>> incicializacion;

    incicializacion.resize(10);
    matriz_listas.resize(10, incicializacion);




    //Variables para recolectar los datos al final de cada dia
    int  suceptiblesGlobal = 0;
    int  infectadosGlobal = 0;
    int  recuperadosGlobal = 0;
    int  muertosGlobal = 0;

    //Crear personas
    int jovenes = floor(cpr * 0.9045); //cantidad de personas jovenes
    int mayores = cpr - jovenes; //cantidad de personas mayores
    int jovenesi = floor(poi * 0.9045); //cantidad de personas jovenes infectadas
    int mayoresi = poi - jovenesi; //cantidad de personas mayores infectadas

    for (int i = 0; i < jovenes; ++i) {
        if (i < jovenesi) { // jovenes infectados
            Persona persona = Persona(1,  rmj, vmj,  tmm,  poi,  prj,  dmn, dmx);
            personas.push_back(persona);
        }
        else { // jovenes sanos
            Persona persona = Persona(0, rmj, vmj, tmm, poi, prj, dmn, dmx);
            personas.push_back(persona);
        }
    }

    for (int i = 0; i < mayores; ++i) {
        if (i < mayoresi) { // mayores infectados
            Persona persona = Persona(1, rmm, vmm, tmm, poi, prv, dmn, dmx);
            personas.push_back(persona);
        }
        else { // mayores sanos
            Persona persona = Persona(1, rmm, vmm, tmm, poi, prv, dmn, dmx);
            personas.push_back(persona);
        }
    }
    
    /*for (i = 0; i < personas.size(); i++) {
       cout << i + 1 << "=> " << personas[i].posicionActual[0] << ":" << personas[i].posicionActual[1] << endl;
    }*/
    
   
    
#pragma omp parallel num_threads(nhilos)
    {
#pragma omp parallel for
        for (int i = 0; i < personas.size(); i++) {
            int x = personas[i].posicionActual[0];
            int y = personas[i].posicionActual[1];
            espacio[x][y].push_back(personas[i]);
        }

#pragma omp master
        {
            cout << matriz_listas[0].size() <<endl ;
            int contadora = 0;
            for (int i = 0; i < matriz_listas[0].size(); i++) {
                for (int j = 0; j < matriz_listas[0][0].size(); j++) {
                    cout << "Pos: " << i << ":" << j << endl;
                }
            }
        }
    }

    
}
