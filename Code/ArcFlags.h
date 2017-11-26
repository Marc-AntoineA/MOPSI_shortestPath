#pragma once
#include "Dijkstra.h"
#include "Chemin.h"
#include <queue>
//#define pp pair<long, long> // paire priorité / id sommet

class ArcFlags: public Dijkstra {
     int K;                 //nombre de cells
     int current_cell;      //cell du t de la requete courante
     map <long, int> affectationCells;     //pour un sommet du ieme cell, Cells[idSommet]=i
     map <pair<long, int>, int> Flags;     //pour chaque arc a, Flags[(a, current_cell)]=0 ou 1
public:
     ArcFlags (Graphe* g):Dijkstra(g){} //K, cells, Flags non initialises
     void initialisation_quadrillage(int racineK); //mettre un simple quadrillage sur la carte
     void initialisation_k_means(int k);
     void initialisationFlags();
     ~ArcFlags(){}
     int getCell(long u) {return affectationCells[u];}
     void depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, long t = 0, long s = 0, bool reverse = false);
     long requete(long s, long t, bool verbose = false);
};
