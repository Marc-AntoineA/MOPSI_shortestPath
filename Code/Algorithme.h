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
    Chemin reconstitution_chemin_forward(long s, long t, map<long, long> *distance) const;
    Chemin reconstitution_chemin_backward(long s, long t, map<long, long> *distance) const;

public:
    Algorithme(Graphe* g);
    // MESURE DU TEMPS :
    void begin();
    double end();
    double now();
    double get_duration(){return duration;}

    // REQUETES (les variantes bidirectionnelles ne sont pas partout implémentées
    virtual long requete(long s, long t, bool verbose = false)=0;
    virtual long requete_bi(long s, long t, bool verbose = false)=0;

    virtual pair<long, Chemin> chemin(long s, long t, bool verbose = false)=0;
    virtual pair<long, Chemin> chemin_bi(long s, long t, bool verbose = false)=0;

    map<long, Sommet>* get_V(){return V;}
    map<long, Arc>* get_A(){return A;}
    ~Algorithme(){}
};

