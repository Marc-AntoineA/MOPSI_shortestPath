#pragma once
#include "AStar.h"
#include <map>

class ALT:public AStar{
protected:
    vector<long> L; // sous-ensemble de sommets
    map<pp, long> subDist; // L(u, v) = dist(u, v) où x ou y est un landmark

public:
    ALT(Graphe *g):AStar(g){}
    long d(long u, long v){return subDist[pp(u, v)];} // u ou v doit être un landmark
    void preprocess(int n, bool verbose = false);
    virtual long pi(long u, long t, long s);
};
