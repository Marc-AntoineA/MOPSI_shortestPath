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
    virtual int pi(long u, long t);
};

class Dijkstradouble:public Dijkstra{
public:
    long point_commun;
public:
    Dijkstradouble(Graphe* g):Dijkstra(g){}
    long requete(long s, long t, bool verbose = false);
    pair<long, Chemin> requete_chemin(long s, long t, bool verbose = false);
    ~Dijkstradouble(){}
};
