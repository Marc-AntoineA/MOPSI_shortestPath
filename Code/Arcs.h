#pragma once

#include <iostream>
using namespace std;

class Arc{
private:
    //int id;
    int u, v;    // Identifiants pour les arcs u -> v
    int w;        // poids

public:
    Arc();
    Arc(int identifiant, int src, int dest, int poids);

    //int get_id() const{return id;}
    double get_poids() const{return w;}
    int get_u() const{return u;}
    int get_v() const{return v;}
};

ostream& operator<<(ostream& str, const Arc& a);
