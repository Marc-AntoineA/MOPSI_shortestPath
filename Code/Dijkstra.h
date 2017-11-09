#pragma once
#include "Algorithme.h"
#include <queue>
#include <vector>
#define pp pair<long, long> // paire priorité / id sommet

struct priorite{
    bool operator()(const pp &p1, const pp &p2){
        return p1.first > p2.first;
    }
};

class Dijkstra: public Algorithme{
public:
    Dijkstra (Graphe* g):Algorithme(g){}
    void depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& distance, map<long, bool>& vus, bool reverse = false);
    virtual long requete(long s, long t);
    ~Dijkstra(){}

};

class Dijkstradouble:public Dijkstra{
public:
    Dijkstradouble(Graphe* g):Dijkstra(g){}
    long requete(long s, long t);
    ~Dijkstradouble(){}
};
