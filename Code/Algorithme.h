#pragma once
#include "Graphe.h"
#include "Chemin.h"

class Algorithme{
protected:
    Graphe* G;
    map<long, Sommet>* V;
    map<long, Arc>* A;
    double timer;
    double duration;

public:
    Algorithme(Graphe* g);
    void begin(); // Pour mesurer le temps mis pour réaliser une requete :
    double end();
    double get_duration(){return duration;}
    virtual long requete(long s, long t, bool verbose = false)=0;
    virtual pair<long, Chemin> requete_chemin(long s, long t, bool verbose = false)=0;
    Chemin reconstitution_chemin_forward(long s, long t, map<long, long> *distance) const;
    Chemin reconstitution_chemin_backward(long s, long t, map<long, long> *distance) const;
    map<long, Sommet>* get_V(){return V;}
    map<long, Arc>* get_A(){return A;}
    ~Algorithme(){}
};

