#pragma once
#include "Dijkstra.h"
#include "Chemin.h"
#include <queue>
#define triplet pair<pp, int> // triplet priorite / id sommet / arc

struct priorite2{
    bool operator()(const triplet &p1, const triplet &p2){
        return (p1.first).first > (p2.first).first;
    }
};

class ArcFlags: public Dijkstra {
     int K;                 //nombre de cells
     int target_cell;      //cell du t de la requete courante
     int start_cell;
     vector<vector<int> > frontieres;
     vector<int> affectationCells;     //pour un sommet du ieme cell, Cells[idSommet]=i
     vector<vector<bool> > Flags;                  //Flags[C][a]
public:
     ArcFlags (Graphe* g):Dijkstra(g){affectationCells=vector<int>(g->get_sommets()->size() + 1);}
     vector<vector<bool> >* getFlags(){return &Flags;}
     void initFlagsFalse();
     void initFrontieres(bool verbose=false);
     void initCellsQuadrillage(int racineK, bool verbose=false);
     void init_cells_kd_tree(int k, bool verbose=false);
     void preprocess_base(bool verbose=false);
     void preprocess_quadrillage(int racineK, bool verbose=false);
     void preprocess_kd_tree(int k, bool verbose=false);

     void empileInitFlags(priority_queue<triplet, vector<triplet>, priorite2> &F, vector<long>& dist, int u);
     void initialisationFlags(int k1, int k2, bool verbose=false);
     void montrer_repartition();
     ~ArcFlags(){}
     int getCell(int u) {return affectationCells[u];}
     void depileEmpile(priority_queue<pp, vector<pp>, priorite> &F, vector<long>& dist, int t = 0, int s = 0, bool reverse = false);
     long requete(int s, int t, bool verbose = false);
     virtual void BD_finish(priority_queue<pp, vector<pp>, priorite> Forward, priority_queue<pp, vector<pp>, priorite> Backward, long &mu);
     long requete_bi(int s, int t, bool verbose = false);
     void test_preprocess();
     void preprocess(string nomInput, bool verbose = false);
     void sauvegarde(string nomOutput, string instance, bool verbose = false);
};

struct thread_data{
    int K;
    bool* doneCells;
    int k1;
    int k2;
    bool verbose;
    ArcFlags* AF;
};

class Node{
    //Node *child1, *child2;
    vector<Sommet*> elements;
public:
    void add(Sommet* s){elements.push_back(s);}
    vector<Sommet*>* getEls(){return &elements;}
};

//class Node{
//    //Node *child1, *child2;
//    vector<int> elements;
//public:
//    void add(int s){elements.push_back(s);}
//    vector<int>* getEls(){return &elements;}
//};
