#pragma once
#include <map>
#include <string>
#include "Arcs.h"

#include "Sommets.h"

using namespace std;


class Graphe{
private:
    vector<Sommet> V;
    vector<Arc> A;
    long nV, nA;    // Nombre de sommets et nombre d'arcs


public:
    vector<Sommet>* get_sommets(){return &V;}
    long getReverseArc(const long &a);
    long get_randomSommet();
    vector<Arc>* get_arcs(){return &A;}
    long get_nV() const {return nV;}
    long get_nA() const {return nA;}

    void show();
    Sommet* get_sommet(int id){return &V[id];}
    Graphe(string nomFichierSommets, string nomFichierPoids);
};
