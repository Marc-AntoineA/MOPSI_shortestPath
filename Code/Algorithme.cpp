#include "Algorithme.h"
#include <ctime>

Algorithme::Algorithme(Graphe* g){
    G = g;
    V = (*G).get_sommets();
    A = (*G).get_arcs();
}

void Algorithme::begin(){
    timer = double(clock())/CLOCKS_PER_SEC;
}

double Algorithme::end(){
    double now = double(clock())/CLOCKS_PER_SEC;
    return  now - timer;
}


