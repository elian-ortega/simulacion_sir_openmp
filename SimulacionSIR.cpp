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
int main(int argc, char* argv[]){
    //params
	int cpr = atoi(argv[1]);    // cantidad de personas int ]0,10000000]
	double piv = atof(argv[2]); // potencia infecciosa ]0,1[
	double prj = atof(argv[3]); // probabilidad de recuperacion jovenes ]0,1[
	double prv = atof(argv[4]); // probabilidad de recuperacion mayores ]0,1[
	int poi = atoi(argv[5]);    // cantidad de personas originalmente infectadas ]0,100]
	double toc = atof(argv[6]); // tasa de ocupación ]0,1[ 
	int dmn = atoi(argv[7]);     // duración minima de la enfermedad ]0,100]
	int dmx = atoi(argv[8]);     // duración máxima de la enfermedad ]0,100]
	int rmj = atoi(argv[9]);     // radio de movilidad de jovenes [0,5]
	int rmm = atoi(argv[10]);    // radio de movilidad de mayores [0,5]
	int vmj = atoi(argv[11]);    // velocidad de movimiento jovenes [0,5]
	int vmm = atoi(argv[12]);    // velocidad de movimiento mayores [0,5]
	int drc = atoi(argv[13]);    // duración de la siomulación ]0,100000]
    int nhilos = atoi(argv[14]); // cantidad de hilos
    srand(time(NULL));
    ofstream resultado;
    resultado.open("resultado.csv");
    resultado << "Porcentaje de Susceptibles, Cantidad de Suceptibles, Porcentaje de Infectados, Cantidad de Infectados, Porcentaje de Recuperados, Cantidad de Recuperados, Porcentaje de Muertos, Cantidad de Muertos \n";
    //// TODO: PEDIRLO PARAM
    //int cpr = 4000000; // cantidad de personas int ]0,10000000]
    //double piv = 0.7; // potencia infecciosa ]0,1[
    //double prj = 0.75; // probabilidad de recuperacion jovenes ]0,1[
    //double prv = 0.5; // probabilidad de recuperacion mayores ]0,1[
    //int poi = 4000; // cantidad de personas originalmente infectadas ]0,100]
    //double toc = 0.3; // tasa de ocupación ]0,1[ 
    //int dmn = 50; // duración minima de la enfermedad ]0,100]
    //int dmx = 100; // duración máxima de la enfermedad ]0,100]
    //int rmj = 4; // radio de movilidad de jovenes [0,5]
    //int rmm = 2; // radio de movilidad de mayores [0,5]
    //int vmj = 3; // velocidad de movimiento jovenes [0,5]
    //int vmm = 2; // velocidad de movimiento mayores [0,5]
    //int drc = 365; // duración de la siomulación ]0,100000]
    int tmm = floor(sqrt(cpr / toc)) + 1; // tamaño de la matriz
    //int nhilos = 16; // cantidad de hilos
    vector<vector<vector<Persona> > > espacio(tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
    vector<vector<vector<Persona> > > espacionuevo(tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
    //vector<vector<int>> numInfectados(tmm, vector<int>(tmm));
    //Variables para recolectar los datos al final de cada dia
    int  suceptiblesGlobal = cpr - poi;
    int  infectadosGlobal = poi;
    int  recuperadosGlobal = 0;
    int  muertosGlobal = 0;
    double porcentajeSuc = suceptiblesGlobal;
    double porcentajeInf = infectadosGlobal;
    double porcentajeRec = 0.0;
    double porcentajeMue = 0.0;
    porcentajeSuc = (porcentajeSuc / cpr) * 100;
    porcentajeInf = (porcentajeInf / cpr) * 100;
    resultado << porcentajeSuc << "," << suceptiblesGlobal << "," << porcentajeInf << "," << infectadosGlobal << "," << porcentajeRec << "," << recuperadosGlobal << "," << porcentajeMue << "," << muertosGlobal << "\n";


    //Crear personas
    int jovenes = floor(cpr * 0.9045); //cantidad de personas jovenes
    int mayores = cpr - jovenes; //cantidad de personas mayores
    int jovenesi = floor(poi * 0.9045); //cantidad de personas jovenes infectadas
    int mayoresi = poi - jovenesi; //cantidad de personas mayores infectadas
    int i, j, k, l;
    double inicio, final;
    inicio = omp_get_wtime();

#pragma omp_set_num_threads(nhilos) 

#pragma omp for schedule(static,2)   
    for (i = 0; i < jovenes; ++i) {
        int posx = rand() % tmm;
        int posy = rand() % tmm;
        if (i < jovenesi) { // jovenes infectados
            Persona persona = Persona(i, 1, rand() % (dmx - dmn + 1) + dmn, posx, posy, rmj, vmj, tmm, true);
            espacio[posx][posy].push_back(persona);
        }
        else { // jovenes sanos
            Persona persona = Persona(i, 0, 0, posx, posy, rmj, vmj, tmm, true);
            espacio[posx][posy].push_back(persona);
        }
    }

#pragma omp for  schedule(static,2) 
    for (i = 0; i < mayores; ++i) {
        int posx = rand() % tmm;
        int posy = rand() % tmm;
        if (i < mayoresi) { // mayores infectados
            Persona persona = Persona(i, 1, rand() % (dmx - dmn + 1) + dmn, posx, posy, rmm, vmm, tmm, false);
            espacio[posx][posy].push_back(persona);
        }
        else { // mayores sanos
            Persona persona = Persona(i, 0, 0, posx, posy, rmm, vmm, tmm, false);
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
                            double proba = rand() % 100;
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
                            double proba = rand() % 100;
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
                        if(espacio[i][j][k].estado != 3)
                        espacionuevo[espacio[i][j][k].posicionActual[0]][espacio[i][j][k].posicionActual[1]].push_back(espacio[i][j][k]);
                    }
                }
            }
        }

        espacio = vector(espacionuevo);
        espacionuevo = vector<vector<vector<Persona> > >(tmm, vector<vector<Persona>>(tmm, vector<Persona>()));
        porcentajeSuc = suceptiblesGlobal;
        cout << g << endl;
        porcentajeSuc = (porcentajeSuc / cpr)*100;
        porcentajeInf = infectadosGlobal;
        porcentajeInf = (porcentajeInf / cpr)*100;
        porcentajeRec = recuperadosGlobal;
        porcentajeRec = (porcentajeRec / cpr)*100;
        porcentajeMue = muertosGlobal;
        porcentajeMue = (porcentajeMue / cpr)*100;
        resultado << porcentajeSuc << "," << suceptiblesGlobal << "," << porcentajeInf << "," << infectadosGlobal << "," << porcentajeRec << "," << recuperadosGlobal << "," << porcentajeMue << "," << muertosGlobal << "\n";
        /* cout << "dia:";
         cout << g << endl;
         cout << suceptiblesGlobal << endl;
         cout << infectadosGlobal << endl;
         cout << recuperadosGlobal << endl;
         cout << muertosGlobal << endl;*/

    }
    final = omp_get_wtime();
    printf("tiempo final:%f", final - inicio);
    resultado.close();
}
