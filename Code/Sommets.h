#pragma once

#include "Arcs.h"
#include <vector>
#include <iostream>

using namespace std;


class Sommet{
public:
    long id;
    pair<int, int> coords;
    vector<long> deltaP, deltaM;    // delta+ : Arcs incidents, delta- Arcs émergents

public:
    Sommet();
    Sommet(long ID);
    Sommet(long ID, pair<int, int> COORDS);

    void add_arcP(Arc a);
    void add_arcM(Arc a);

    long get_id() const{return id;}
    pair<int, int> get_coords() const{return coords;}
    vector<long>* get_deltaP(){return &deltaP;}
    vector<long>* get_deltaM(){return &deltaM;}


};


ostream& operator<<(ostream& str, const Sommet& s);
