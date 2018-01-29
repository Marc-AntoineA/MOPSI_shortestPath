#pragma once
#include "Algorithme.h"
#include "AStar.h"
#include <queue>
#include <vector>
#include "Chemin.h"


class Dijkstra: public AStar{
// A* où le potentiel est nul --> constructeur
public:
    Dijkstra (Graphe* g):AStar(g){}
    ~Dijkstra(){}
    virtual long pi(int u, int t, int s){return 0;}
};
