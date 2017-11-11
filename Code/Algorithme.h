#pragma once
#include "Graphe.h"
#include "Chemin.h"

class Algorithme{
protected:
    Graphe* G;
    map<long, Sommet>* V;
    map<long, Arc>* A;
    double timer;

public:
    Algorithme(Graphe* g);
    void begin(); // Pour mesurer le temps mis pour réaliser une requete :
    double end();
    virtual long requete(long s, long t)=0;
    Chemin reconstitution_chemin(long s, long t, map<long, long> *distance) const;
    ~Algorithme(){}
};

