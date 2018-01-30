#pragma once
#include "AStar.h"
#include <map>

class ALT:public AStar{
public:
    vector<int> L; // sous-ensemble de sommets
    //map<pp, long> subDist; // L(u, v) = dist(u, v) où x ou y est un landmark
    vector<long> subDist; //L(u*K+v) = ...
    map<int, int> Lrecip;//
    bool notInPreprocessMode;

public:
    vector<int>* get_L(){return &L;}
    ALT(Graphe *g):AStar(g){notInPreprocessMode=true;}
//    long d(int u, int v){return subDist[pp(u, v)];} // u doit être un landmark
    long d(int u, int v){return subDist[v*L.size()+Lrecip[u]];} // u doit être un landmark
    long d2(int u, int v){return subDist[v*L.size()+u];}

    void preprocess(int n, bool verbose = false);
    void preprocess(string nomInput, bool verbose = false);
    void sauvegarde(string nomOutput, string instance, bool verbose = false);
    virtual long pi(int u, int t, int s);
};
