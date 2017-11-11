#include "Algorithme.h"
#include <ctime>
#include "Chemin.h"
#include <stdlib.h> //Pour abs
#include <cmath>

Algorithme::Algorithme(Graphe* g){
    G = g;
    V = (*G).get_sommets();
    A = (*G).get_arcs();
}

void Algorithme::begin(){
    timer = double(clock())/CLOCKS_PER_SEC;
}

double Algorithme::end(){
    double now = double(clock())/CLOCKS_PER_SEC;
    return  now - timer;
}

//distance contient les distances a s calculees par Dijkstra
Chemin Algorithme::reconstitution_chemin(long s, long t, map<long, long> *distance) const{
    long sommet_courant = t;
    Chemin C;
    C.push_back(sommet_courant);
    while (sommet_courant != s){
        bool trouve = false;
        //iteration sur s de delta- : si distance(sommet_courant)-distance(s)==poids(arc(s, sommet_courant)) alors sommet_courant=s et C.push_back(sommet_courant)
        //on suppose que tous les poids des arcs sont un peu differents, donc on peut remonter depuis t jusqu'a s avec seulement les distances
        vector<long> *deltaM = ((*V)[sommet_courant]).get_deltaM();
        for(int k = 0; (k < deltaM->size()) && !trouve; k++){
            Arc* a = &((*A)[(*deltaM)[k]]);
            if (pow((*distance)[sommet_courant] - (*distance)[a->get_u()] - a->get_poids(),2) < 0.00001){//cout<<"distance a "<<s<<" de "<<sommet_courant<<" : "<<(*distance)[sommet_courant]<<" et distance de "<<a->get_u()<<" : "<<(*distance)[a->get_u()]<<" et abs = "<<abs((*distance)[sommet_courant] - (*distance)[a->get_u()] - a->get_poids())<<endl;compteur+=1;
                sommet_courant=a->get_u();
                C.push_back(sommet_courant);
                trouve=true;
            }
        }
        if (!trouve){
            cerr<<"pas de chemin"<<endl;
            C.inversion();
            return C;
        }
    }
    C.inversion();
    return C;
}
