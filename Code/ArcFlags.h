#pragma once
#include "Dijkstra.h"
#include "Chemin.h"
#include <queue>
#define triplet pair<pp, long> // triplet priorite / id sommet / arc

struct priorite2{
    bool operator()(const triplet &p1, const triplet &p2){
        return (p1.first).first > (p2.first).first;
    }
};

class ArcFlags: public Dijkstra {
     int K;                 //nombre de cells
     int current_cell;      //cell du t de la requete courante
     map <long, int> affectationCells;     //pour un sommet du ieme cell, Cells[idSommet]=i
     map <pair<long, int>, int> Flags;     //pour chaque arc a, Flags[(a, current_cell)]=1 s'il existe un plus court chemin vers un sommet de C passant par a
public:
     ArcFlags (Graphe* g):Dijkstra(g){}
     void preprocess_quadrillage(int racineK, bool verbose=false);
     void preprocess_k_means(int k, bool verbose=false);
     void empileInitFlags(priority_queue<triplet, vector<triplet>, priorite2> *F, map<long, long>& dist, long u);
     void initialisationFlags(bool verbose=false);
     ~ArcFlags(){}
     int getCell(long u) {return affectationCells[u];}
     void depileEmpile(priority_queue<pp, vector<pp>, priorite> &F, map<long, long>& dist, long t = 0, long s = 0, bool reverse = false);
     long requete(long s, long t, bool verbose = false);
     long requete_bi(long s, long t, bool verbose = false);
};
