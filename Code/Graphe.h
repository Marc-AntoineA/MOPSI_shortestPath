#pragma once
#include <map>
#include <string>
#include "Arcs.h"

#include "Sommets.h"

using namespace std;


class Graphe{
private:
    map<long, Sommet> V;
    map<long, Arc> A;
    long nV, nA;    // Nombre de sommets et nombre d'arcs


public:
    map<long, Sommet>* get_sommets(){return &V;}
    map<long, Arc>* get_arcs(){return &A;}
    long get_nV() const {return nV;}
    long gett_nA() const {return nA;}

    void show();
    Sommet* get_sommet(int id) const;
    Graphe(char* nomFichierSommets, char *nomFichierPoids);
};



