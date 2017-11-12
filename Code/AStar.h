#pragma once
#include "Algorithme.h"
#include "Chemin.h"
#include <queue>
#define pp pair<long, long> // paire priorité / id sommet

struct priorite{
    bool operator()(const pp &p1, const pp &p2){
        return p1.first > p2.first;
    }
};

class AStar: public Algorithme{
protected:
    map<long, long> distanceForward;
    void init_distanceForward(long value);
    map<long, long> distanceBackward; // pour les variantes bidirectionnelles
    void init_distanceBackward(long value);

public:
    virtual int pi(long u, long t)=0;
    AStar(Graphe* g):Algorithme(g){}
    void depileEmpile(long t, priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, bool reverse = false);
    virtual long requete(long s, long t, bool verbose = false);
    virtual pair<long, Chemin> requete_chemin(long s, long t, bool verbose = false);
    ~AStar(){}
};

class AStarGeographique: public AStar{
public:
    AStarGeographique(Graphe *g):AStar(g){}
    virtual int pi(long u, long t);
};
