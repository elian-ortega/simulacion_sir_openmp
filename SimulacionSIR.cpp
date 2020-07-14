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
    resultado << "Porcentaje de Susceptibles, Cantidad de Suceptibles, Porcentaje de Infectados, Cantidad de Infectados, Porcentaje de Recuperados, Cantidad de Recuperados, Porcentaje de Muertos, Cantidad de Muertos \n";
    // TODO: PEDIRLO PARAM
    int cpr = 100000; // cantidad de personas int ]0,10000000]
    float piv = 0.7; // potencia infecciosa ]0,1[
    float prj = 0.75; // probabilidad de recuperacion jovenes ]0,1[
    float prv = 0.5; // probabilidad de recuperacion mayores ]0,1[
    int poi = 100; // cantidad de personas originalmente infectadas ]0,100]
    float toc = 0.3; // tasa de ocupaci�n ]0,1[ 
    int dmn = 50; // duraci�n minima de la enfermedad ]0,100]
    int dmx = 100; // duraci�n m�xima de la enfermedad ]0,100]
    int rmj = 4; // radio de movilidad de jovenes [0,5]
    int rmm = 2; // radio de movilidad de mayores [0,5]
    int vmj = 3; // velocidad de movimiento jovenes [0,5]
    int vmm = 1; // velocidad de movimiento mayores [0,5]
    int drc = 1000; // duraci�n de la siomulaci�n ]0,100000]
    int tmm = floor(sqrt(cpr / toc)) + 1; // tama�o de la matriz
    int nhilos = 8; // cantidad de hilos
<<<<<<< HEAD

    vector<Persona> personas;
    vector<vector<vector<Persona> > > espacio (tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
    vector<vector<vector<Persona> > > espacionuevo (tmm, vector<vector<Persona>>(tmm, vector<Persona>()));

    vector < vector<list<Persona>>> matriz_listas;
    vector<list<Persona>> incicializacion;

    incicializacion.resize(10);
    matriz_listas.resize(10, incicializacion);




=======
    vector<vector<vector<Persona> > > espacio(tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
    vector<vector<vector<Persona> > > espacionuevo(tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
>>>>>>> 6fa7e2bbf29635e3f0270971f957d918f1419633
    //Variables para recolectar los datos al final de cada dia
    int  suceptiblesGlobal = cpr - poi;
    int  infectadosGlobal = poi;
    int  recuperadosGlobal = 0;
    int  muertosGlobal = 0;

    //Crear personas
    int jovenes = floor(cpr * 0.9045); //cantidad de personas jovenes
    int mayores = cpr - jovenes; //cantidad de personas mayores
    int jovenesi = floor(poi * 0.9045); //cantidad de personas jovenes infectadas
    int mayoresi = poi - jovenesi; //cantidad de personas mayores infectadas
<<<<<<< HEAD

    for (int i = 0; i < jovenes; ++i) {
        if (i < jovenesi) { // jovenes infectados
            Persona persona = Persona(1,  rmj, vmj,  tmm,  poi,  prj,  dmn, dmx);
            personas.push_back(persona);
        }
        else { // jovenes sanos
            Persona persona = Persona(0, rmj, vmj, tmm, poi, prj, dmn, dmx);
            personas.push_back(persona);
=======
    int i, j, k, l;

#pragma omp_set_num_threads(nhilos) 

#pragma omp for schedule(static,2)   
<<<<<<< HEAD
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
>>>>>>> 18088a14524279d77fdfac78661afc5cf3a79fee
        }

<<<<<<< HEAD
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
=======
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
>>>>>>> 6fa7e2bbf29635e3f0270971f957d918f1419633

=======
#pragma omp for  schedule(static,2) 
    for (i = 0; i < mayores; ++i) {
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

    /*for (i = 0; i < tmm; ++i) {
        for (j = 0; j < tmm; ++j) {
            for (k = 0; k < espacio[i][j].size(); ++k) {
                cout << espacio[i][j][k].id << " ";
            }
        }
    }*/

    for (int g = 0; g < drc; ++g) {
#pragma omp for  schedule(static) private (i ,j, k, l)
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
                                    infectadosGlobal--;
                                    espacio[i][j][k].tiempoInmune = rand() % (480 - 200 + 1) + 200;
                                    espacio[i][j][k].estado = 2;
                                }
                                else { // se muere
                                    muertosGlobal++;
                                    infectadosGlobal--;
                                    espacio[i][j][k].estado = 3;
                                }
                            }
                            else {// Es mayor
                                if (proba <= prv) { // se recupera
                                    recuperadosGlobal++;
                                    infectadosGlobal--;
                                    espacio[i][j][k].tiempoInmune = rand() % (480 - 200 + 1) + 200;
                                    espacio[i][j][k].estado = 2;
                                }
                                else { // se muere
                                    muertosGlobal++;
                                    infectadosGlobal--;
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
                                suceptiblesGlobal--;
                                espacio[i][j][k].estado = 1;
                                espacio[i][j][k].duracionEnfermedad = rand() % (dmx - dmn + 1) + dmn;
                            }
                        }
                    }
                    else if (espacio[i][j][k].estado == 2) {
                        if (espacio[i][j][k].tiempoInmune == 0) {
                            suceptiblesGlobal++;
                            recuperadosGlobal--;
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
                    {

                        // cout << "pos nueva: "<< espacio[i][j][k].posicionActual[0] << " " << espacio[i][j][k].posicionActual[1] << endl;
                        espacionuevo[espacio[i][j][k].posicionActual[0]][espacio[i][j][k].posicionActual[1]].push_back(espacio[i][j][k]);
                    }
                }
            }
        }

        espacio = vector(espacionuevo);
        espacionuevo = vector<vector<vector<Persona> > >(tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
        resultado << suceptiblesGlobal / cpr << "," << suceptiblesGlobal << "," << infectadosGlobal / cpr << "," << infectadosGlobal << "," << recuperadosGlobal / cpr << "," << recuperadosGlobal << "," << muertosGlobal / cpr << "," << muertosGlobal << "\n";
        /* cout << "dia:";
         cout << g << endl;
         cout << suceptiblesGlobal << endl;
         cout << infectadosGlobal << endl;
         cout << recuperadosGlobal << endl;
         cout << muertosGlobal << endl;*/

    }
<<<<<<< HEAD
>>>>>>> 18088a14524279d77fdfac78661afc5cf3a79fee
    
=======

>>>>>>> 6fa7e2bbf29635e3f0270971f957d918f1419633
}
