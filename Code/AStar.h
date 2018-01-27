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
    vector<long> distanceForward0;
    void init_distanceForward();
    vector<long> distanceBackward0; // pour les variantes bidirectionnelles
    vector<long> distanceForward;
    vector<long> distanceBackward;
    void init_distanceBackward();
    long point_commun; // pour les variantes bidirectionnelles, point de rencontre

public:
    virtual long pi(long u, long t, long s)=0;
    AStar(Graphe* g);
    void depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, vector<long>& dist, long t = 0, long s = 0, bool reverse = false);
    virtual long requete(long s, long t, bool verbose = false);
    void BD_finish(priority_queue<pp, vector<pp>, priorite> Forward, priority_queue<pp, vector<pp>, priorite> Backward, long& mu);
    virtual long requete_bi(long s, long t, bool verbose = false);

    virtual pair<long, Chemin> chemin(long s, long t, bool verbose = false);
    virtual pair<long, Chemin> chemin_bi(long s, long t, bool verbose = false);
    ~AStar(){}
};



class ASG: public AStar{
public:
    ASG(Graphe *g):AStar(g){}
    virtual long pi(long u, long t, long s){return (*V)[u].distance((*V)[t]);}
};

