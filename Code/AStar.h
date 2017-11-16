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
    virtual long pi(long u, long t, long s)=0;
    AStar(Graphe* g):Algorithme(g){}
    void depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, long t = 0, long s = 0, bool reverse = false);
    virtual long requete(long s, long t, bool verbose = false);
    virtual pair<long, Chemin> requete_chemin(long s, long t, bool verbose = false);
    ~AStar(){}
};

class AStarBidirectionnel:public AStar{
protected:
    long point_commun;
public:
    AStarBidirectionnel(Graphe* g):AStar(g){}
    virtual long requete(long s, long t, bool verbose = false);
    virtual pair<long, Chemin> requete_chemin(long s, long t, bool verbose = false);
    ~AStarBidirectionnel(){}
};


class ASG: public AStar{
public:
    ASG(Graphe *g):AStar(g){}
    virtual long pi(long u, long t, long s){return (*V)[u].distance((*V)[t]);}
};

class ASGBD:public AStarBidirectionnel{
public:
    ASGBD(Graphe* g):AStarBidirectionnel(g){}
    virtual long pi(long u, long t, long s){return ((*V)[u].distance((*V)[t]) - (*V)[u].distance((*V)[s]))/2;}
};
