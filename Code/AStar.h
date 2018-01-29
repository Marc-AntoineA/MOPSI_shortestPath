#pragma once
#include "Algorithme.h"
#include "Chemin.h"
#include <queue>
#define pp pair<long, int> // paire priorité / id sommet

struct priorite{
    bool operator()(const pp &p1, const pp &p2){
        return p1.first > p2.first;
    }    
};

class AStar: public Algorithme{

protected:
    vector<long> distance0;
    void init_distanceForward();
    void init_distanceBackward();
    vector<long> distanceForward;
    vector<long> distanceBackward;
    int point_commun; // pour les variantes bidirectionnelles, point de rencontre

public:
    virtual long pi(int u, int t, int s)=0;
    AStar(Graphe* g);
    void depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, vector<long>& dist, int t = 0, int s = 0, bool reverse = false);
    virtual long requete(int s, int t, bool verbose = false);
    virtual void BD_finish(priority_queue<pp, vector<pp>, priorite> Forward, priority_queue<pp, vector<pp>, priorite> Backward, long &mu);
    virtual long requete_bi(int s, int t, bool verbose = false);

    virtual pair<long, Chemin> chemin(int s, int t, bool verbose = false);
    virtual pair<long, Chemin> chemin_bi(int s, int t, bool verbose = false);
    ~AStar(){}
};



class ASG: public AStar{
public:
    ASG(Graphe *g):AStar(g){}
    virtual long pi(int u, int t, int s){return (*V)[u].distance((*V)[t]);}
};

