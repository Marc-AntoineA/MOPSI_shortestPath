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
    int nV, nA;    // Nombre de sommets et nombre d'arcs


public:
    vector<Sommet>* get_sommets(){return &V;}
    int getReverseArc(const int &a);
    int get_randomSommet();
    vector<Arc>* get_arcs(){return &A;}
    int get_nV() const {return nV;}
    int get_nA() const {return nA;}

    void show();
    Sommet* get_sommet(int id){return &V[id];}
    Graphe(string nomFichierSommets, string nomFichierPoids);
};
