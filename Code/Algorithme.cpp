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
    duration = now - timer;
    return  now - timer;
}

//distance contient les distances a s calculees par Dijkstra
Chemin Algorithme::reconstitution_chemin_forward(long s, long t, map<long, long> *distance) const{
    long sommet_courant = t;
    Chemin C;
    C.push_back(sommet_courant);
    Arc* a;
    long u;
    while (sommet_courant != s){
        bool trouve = false;
        //iteration sur u de delta- : si distance(sommet_courant)-distance(u)==poids(arc(u, sommet_courant)) alors sommet_courant=u et C.push_back(sommet_courant)
        //on suppose que tous les poids des arcs sont un peu differents, donc on peut remonter depuis t jusqu'a s avec seulement les distances
        vector<long> *deltaM = ((*V)[sommet_courant]).get_deltaM();
        for(int k = 0; (k < deltaM->size()) && !trouve; k++){
            a = &((*A)[(*deltaM)[k]]);
            u = a->get_u();
            if (pow((*distance)[sommet_courant] - (*distance)[u] - a->get_poids(),2) < 0.00001){//cout<<"distance a "<<s<<" de "<<sommet_courant<<" : "<<(*distance)[sommet_courant]<<" et distance de "<<a->get_u()<<" : "<<(*distance)[a->get_u()]<<" et abs = "<<abs((*distance)[sommet_courant] - (*distance)[a->get_u()] - a->get_poids())<<endl;compteur+=1;
                sommet_courant=u;
                C.push_back(sommet_courant);
                trouve=true;
            }
        }
        if (!trouve){
            cerr<<"Algorithme::reconstitution_chemin : pas de chemin"<<endl;
            C.inversion();
            return C;
        }
    }
    C.inversion();//le chemin a ete reconstitue a l'envers donc on echange son sens
    return C;
}

//distance contient les distances a t calculees par Dijkstra
Chemin Algorithme::reconstitution_chemin_backward(long s, long t, map<long, long> *distance) const{
    long sommet_courant = s;
    Chemin C;
    C.push_back(sommet_courant);
    Arc* a;
    long v;
    while (sommet_courant != t){
        bool trouve = false;
        //iteration sur v de delta+ : si distance(sommet_courant)-distance(v)==poids(arc(sommet_courant, v)) alors sommet_courant=v et C.push_back(sommet_courant)
        //on suppose que tous les poids des arcs sont un peu differents, donc on peut remonter depuis s jusqu'a t avec seulement les distances
        vector<long> *deltaP = ((*V)[sommet_courant]).get_deltaP();
        for(int k = 0; (k < deltaP->size()) && !trouve; k++){
            a = &((*A)[(*deltaP)[k]]);
            v = a->get_v();
            if (pow((*distance)[sommet_courant] - (*distance)[v] - a->get_poids(),2) < 0.00001){//cout<<"distance a "<<s<<" de "<<sommet_courant<<" : "<<(*distance)[sommet_courant]<<" et distance de "<<a->get_u()<<" : "<<(*distance)[a->get_u()]<<" et abs = "<<abs((*distance)[sommet_courant] - (*distance)[a->get_u()] - a->get_poids())<<endl;compteur+=1;
                sommet_courant=v;
                C.push_back(sommet_courant);
                trouve=true;
            }
        }
        if (!trouve){
            cerr<<"Algorithme::reconstitution_chemin : pas de chemin"<<endl;
            C.inversion();
            return C;
        }
    }
    return C;
}
