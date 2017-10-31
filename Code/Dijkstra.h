#pragma once
#include "Algorithme.h"

class Dijkstra:Algorithme{
public:
    Dijkstra (Graphe* g);

    long requete(long s, long t);

};
