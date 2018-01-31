#pragma once

#include "Arcs.h"
#include <vector>
#include <iostream>

using namespace std;

class Sommet{
public:
    int id;
    //pair<int, int> coords;
    int x;
    int y;
    int nP, nM;           //
    int* deltaP,* deltaM;    // delta+ : Arcs incidents, delta- Arcs émergents

public:
    Sommet();
    ~Sommet();
    Sommet(int ID);
    Sommet(int ID, pair<int, int> COORDS);
    Sommet(const Sommet &S);

    void add_arcP(const int &a);
    void add_arcM(const int &a);

    inline int get_id() const{return id;}
    void setId(const int &ID){id = ID;}
    pair<int, int> get_coords() const{return pair<int, int>(x, y);}
    void setCoords(pair<int, int> myCoords){x = myCoords.first; y=myCoords.second;}
    int get_nP(){return nP;}
    int get_nM(){return nM;}
    int* get_deltaP(){return deltaP;}
    int* get_deltaM(){return deltaM;}
    long distance(Sommet& v);

};


ostream& operator<<(ostream& str, const Sommet& s);

bool compareX(Sommet *s1, Sommet *s2);
bool compareY(Sommet *s1, Sommet *s2);
