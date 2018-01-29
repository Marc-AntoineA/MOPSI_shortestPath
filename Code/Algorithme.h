#pragma once
#include "Graphe.h"
#include "Chemin.h"

class Algorithme{
protected:
    Graphe* G;
    vector<Sommet>* V;
    vector<Arc>* A;
    double timer;
    double duration;
    int visites;
    Chemin reconstitution_chemin_forward(int s, int t, vector<long> *distance) const;
    Chemin reconstitution_chemin_backward(int s, int t, vector<long> *distance) const;

public:
    Algorithme(Graphe* g);
    // MESURE DU TEMPS et du nombre de points visites
    void begin();
    double end();
    double now();
    double get_duration(){return duration;}
    void add_visite();
    int get_visites(){return visites;}

    // REQUETES (les variantes bidirectionnelles ne sont pas partout implémentées
    virtual long requete(int s, int t, bool verbose = false)=0;
    virtual long requete_bi(int s, int t, bool verbose = false)=0;

    virtual pair<long, Chemin> chemin(int s, int t, bool verbose = false)=0;
    virtual pair<long, Chemin> chemin_bi(int s, int t, bool verbose = false)=0;

    vector<Sommet>* get_V(){return V;}
    vector<Arc>* get_A(){return A;}
    ~Algorithme(){}
};

