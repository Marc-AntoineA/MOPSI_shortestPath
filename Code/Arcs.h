#pragma once

#include <iostream>
using namespace std;

class Arc{
private:
    long id;
    long u, v;    // Identifiants pour les arcs u -> v
    double w;        // poids

public:
    Arc();
    Arc(long identifiant, long src, long dest, double poids);

    long get_id() const{return id;}
    double get_poids() const{return w;}
    long get_u() const{return u;}
    long get_v() const{return v;}
};

ostream& operator<<(ostream& str, const Arc& a);
