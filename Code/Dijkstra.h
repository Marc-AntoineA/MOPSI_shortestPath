#pragma once
#include "Algorithme.h"
#include <queue>
#include <vector>
#include "Chemin.h"
#define pp pair<long, long> // paire priorité / id sommet

struct priorite{
    bool operator()(const pp &p1, const pp &p2){
        return p1.first > p2.first;
    }
};

class Dijkstra: public Algorithme{
 private:
    map<long, long> distance;
    void init_distance(long value);
public:
    Dijkstra (Graphe* g):Algorithme(g){}
    void depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, bool reverse = false);
    virtual long requete(long s, long t, bool verbose = false);
    virtual pair<long, Chemin> requete_chemin(long s, long t, bool verbose = false);
    ~Dijkstra(){}
};

class Dijkstradouble:public Dijkstra{
public:
    map<long, long> distanceForward;
    map<long, long> distanceBackward;
    long point_commun;
    void init_distances(long value);
public:
    Dijkstradouble(Graphe* g):Dijkstra(g){}
    long requete(long s, long t, bool verbose = false);
    pair<long, Chemin> requete_chemin(long s, long t, bool verbose = false);
    ~Dijkstradouble(){}
};
