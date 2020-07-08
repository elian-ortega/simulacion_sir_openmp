// SimulacionSIR.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>

#include <vector>
#include <list>
#include <iterator>

#include "Persona.h"

using namespace std;
int main()
{
    vector<int> vertices;

    hola();

    

    int numProcs = omp_get_num_procs();
    cout << "Cant de procs: " << numProcs;
        

    cout << "Hello World!\n";
    

    
}
