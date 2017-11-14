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
    virtual long pi(long u, long t){return 0;}
};

class DijkstraBidirectionnel:public AStarBidirectionnel{
public:
public:
    DijkstraBidirectionnel(Graphe* g):AStarBidirectionnel(g){}
    virtual long pi(long u, long t){return 0;}
    ~DijkstraBidirectionnel(){}
};
