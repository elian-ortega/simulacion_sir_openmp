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
    float toc = 0.3; // tasa de ocupaci�n ]0,1[ 
    int dmn = 50; // duraci�n minima de la enfermedad ]0,100]
    int dmx = 100; // duraci�n m�xima de la enfermedad ]0,100]
    int rmj = 4; // radio de movilidad de jovenes [0,5]
    int rmm = 2; // radio de movilidad de mayores [0,5]
    int vmj = 5; // velocidad de movimiento jovenes [0,5]
    int vmm = 3; // velocidad de movimiento mayores [0,5]
    int drc = 1000; // duraci�n de la siomulaci�n ]0,100000]
    int tmm = floor(sqrt(cpr / toc)) + 1; // tama�o de la matriz
    int nhilos = 8; // cantidad de hilos
    vector<vector<vector<Persona> > > espacio (tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
    vector<vector<vector<Persona> > > espacionuevo (tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
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
    int i, j, k, l;
   
#pragma omp parallel num_threads(nhilos)
#pragma omp for  schedule(static,2)   
    for (i = 0; i < jovenes; ++i) {
        int posx = rand() % tmm;
        int posy = rand() % tmm;
        if (i < jovenesi) { // jovenes infectados
            Persona persona = Persona(i, 1, posx, posy, rmj, vmj, tmm, true);
            espacio[posx][posy].push_back(persona);
        }
        else { // jovenes sanos
            Persona persona = Persona(i, 0, posx, posy, rmj, vmj, tmm, true);
            espacio[posx][posy].push_back(persona);
        }
    }

#pragma omp for  schedule(static,2) 
    for (i = 0 ; i < mayores; ++i) {
        int posx = rand() % tmm;
        int posy = rand() % tmm;
        if (i < mayoresi) { // mayores infectados
            Persona persona = Persona(i, 1, posx, posy, rmm, vmm, tmm, false);
            espacio[posx][posy].push_back(persona);
        }
        else { // mayores sanos
            Persona persona = Persona(i, 0, posx, posy, rmm, vmm, tmm, false);
            espacio[posx][posy].push_back(persona);
        }
    }

#pragma omp barrier

    for (int g = 0; g < drc; ++g) {
#pragma omp for  schedule(static)   
        for (i = 0; i < tmm; ++i) {
            for (j = 0; j < tmm; ++j) {
                int infectados = 0;
                for (k = 0; k < espacio[i][j].size(); ++k) {// cuenta infectados y los procesa
                    if (espacio[i][j][k].estado == 1) {
                        infectados++;
                        if (espacio[i][j][k].duracionEnfermedad == 0) {//temino enfermedad
                            float proba = rand() % 100;
                            proba = proba / 100;
                            if (espacio[i][j][k].esJoven) { // Es joven
                                if (proba <= prj) { // se recupera
                                    recuperadosGlobal++;
                                    espacio[i][j][k].tiempoInmune = rand() % (480 - 200 + 1) + 200;
                                    espacio[i][j][k].estado = 2;
                                }
                                else { // se muere
                                    muertosGlobal++;
                                    espacio[i][j][k].estado = 3;
                                }
                            }
                            else {// Es mayor
                                if (proba <= prv) { // se recupera
                                    recuperadosGlobal++;
                                    espacio[i][j][k].tiempoInmune = rand() % (480 - 200 + 1) + 200;
                                    espacio[i][j][k].estado = 2;
                                }
                                else { // se muere
                                    muertosGlobal++;
                                    espacio[i][j][k].estado = 3;
                                }
                            }
                        }
                        else { // reduce lo que le queda de enfermedad 
                            espacio[i][j][k].duracionEnfermedad--;
                        }
                    }
                }
                for (k = 0; k < espacio[i][j].size(); ++k) { // procesa los no infectados
                    if (espacio[i][j][k].estado == 0) { // es suceptible
                        for (l = 0; l < infectados && espacio[i][j][k].estado == 0; ++l) { //calcula la proba de contagiarse por cada infectado en la casilla
                            float proba = rand() % 100;
                            proba = proba / 100;
                            if (proba <= piv) {
                                infectadosGlobal++; // tal vez hay que hacerlo critical
                                espacio[i][j][k].estado = 1;
                                espacio[i][j][k].duracionEnfermedad = rand() % (dmx - dmn + 1) + dmn;
                            }
                        }
                    }
                    else if (espacio[i][j][k].estado == 2) {
                        if (espacio[i][j][k].tiempoInmune == 0) {
                            suceptiblesGlobal++;
                            espacio[i][j][k].estado = 0;
                        }
                        else {
                            espacio[i][j][k].tiempoInmune--;
                        }
                    }

                    //Moverse 

                    int posxa = espacio[i][j][k].posicionActual[0];
                    int posya = espacio[i][j][k].posicionActual[1];
                    int maxx = espacio[i][j][k].maxx;
                    int minx = espacio[i][j][k].minx;
                    int maxy = espacio[i][j][k].maxy;
                    int miny = espacio[i][j][k].miny;
                    int direccion = rand() % 3 + 1;
                    int vel = espacio[i][j][k].vel;
                    int rebote = 0;

                    switch (direccion) {
                    case 1: // arriba
                        if (posxa - vel < minx) {
                            rebote = abs((posxa - vel) - minx);
                            posxa = minx + rebote;
                        }
                        else {
                            posxa = posxa - vel;
                        }
                        espacio[i][j][k].posicionActual[0] = posxa;
                        break;
                    case 2: // derecha
                        if (posya + vel > maxy) {
                            rebote = abs((posya + vel) - maxy);
                            posya = maxy - rebote;
                        }
                        else {
                            posya = posya + vel;
                        }
                        espacio[i][j][k].posicionActual[1] = posya;
                        break;
                    case 3: // abajo
                        if (posxa + vel > maxx) {
                            rebote = abs((posxa + vel) - maxx);
                            posxa = maxx - rebote;
                        }
                        else {
                            posxa = posxa + vel;
                        }
                        espacio[i][j][k].posicionActual[0] = posxa;
                        break;
                    case 4: // izquierda
                        if (posya - vel < miny) {
                            rebote = abs((posya - vel) - miny);
                            posya = miny + rebote;
                        }
                        else {
                            posya = posya - vel;
                        }
                        espacio[i][j][k].posicionActual[1] = posya;
                        break;
                    }
#pragma omp critical
                    espacionuevo[espacio[i][j][k].posicionActual[0]][espacio[i][j][k].posicionActual[1]].push_back(espacio[i][j][k]);
                }
            }
        }
        espacio = vector(espacionuevo);
        espacionuevo = vector<vector<vector<Persona> > >(tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
    }
    

    
    
}
