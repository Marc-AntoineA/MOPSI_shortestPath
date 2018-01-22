#pragma once

#include <iostream>
#include <vector>
#include<map>
#include"Arcs.h"
#include"Sommets.h"
using namespace std;

class Chemin{
    vector<long> listeId;  //liste des indices des sommets formant le chemin
public:
    void push_back(long id);
    void inversion();   //inverser le sens du chemin
    Chemin join(Chemin C);
    string chaine() const;
    double longueur(vector<Arc> *A, vector<Sommet>* V) const;
};
ostream& operator<<(ostream&str,const Chemin &C);
