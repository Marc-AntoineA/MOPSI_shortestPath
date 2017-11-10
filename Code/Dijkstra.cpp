#include "Dijkstra.h"
#include <cassert>
#include <climits>  //Pour LONG_MAX
#include <stdlib.h> //Pour abs


using namespace std;


void Dijkstra::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& distance, map<long, bool>& vus, bool reverse){
    long u = F.top().second;
    vus[u] = true;
    F.pop();
    vector<long>* deltaP;
    if(reverse)
        deltaP = ((*V)[u]).get_deltaM();
    else
        deltaP = ((*V)[u]).get_deltaP();

    for(int k = 0; k < deltaP->size(); k++){
        Arc* a = &((*A)[(*deltaP)[k]]);
        if(!reverse){
            long v = a->get_v();
            if(distance[v] > distance[u] + a->get_poids()){
                distance[v] = distance[u] + a->get_poids();
                F.push(pp(distance[v], v));
            }
        }else{
            long v = a->get_u();
            if(distance[v] > distance[u] + a->get_poids()){
                distance[v] = distance[u] + a->get_poids();
                F.push(pp(distance[v], v));
            }
        }
    }
}

long Dijkstra::requete(long s, long t){
    begin();
    priority_queue<pp, vector<pp>, priorite> F;

    map<long, long> distance; // Couple distance, id sommet
    map<long, bool> vus;

    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        distance[it1->second.get_id()] = LONG_MAX;
        vus[it1->second.get_id()] = false;
    }

    distance[s] = 0;
    F.push(pp(0, s));

    while(!F.empty() && !vus[t]){
        depileEmpile(F, distance, vus);
    }
    cout << "Duration : " << end() << endl;
    return distance[t];

}

pair<long, Chemin> Dijkstra::requete_chemin(long s, long t){//pas prete
    begin();
    priority_queue<pp, vector<pp>, priorite> F;

    map<long, long> distance; // Couple distance, id sommet
    map<long, bool> vus;

    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        distance[it1->second.get_id()] = LONG_MAX;
        vus[it1->second.get_id()] = false;
    }

    distance[s] = 0;
    F.push(pp(0, s));

    while(!F.empty() && !vus[t]){
        depileEmpile(F, distance, vus);
    }
    cout << "Duration : " << end() << endl;

    long sommet_courant = t;
    Chemin C;
    C.push_back(sommet_courant);
    cout<<C<<endl;
    while (sommet_courant != s){bool trouve = false;
        //iteration sur s de delta- : si distance(sommet_courant)-distance(s)==poids(arc(s, sommet_courant)) alors sommet_courant=s et C.push_back(sommet_courant)
        //on suppose que tous les poids des arcs sont un peu differents, donc on peut remonter depuis t jusqu'a s avec seulement les distances
        vector<long> *deltaM = ((*V)[sommet_courant]).get_deltaM();
        for(int k = 0; k < deltaM->size(); k++){
            Arc* a = &((*A)[(*deltaM)[k]]);
            if (sommet_courant != a->get_v()){
                cerr<<"mauvais arc : courant = "<<sommet_courant<<", u = "<<a->get_u()<<" et v = "<<a->get_v()<<endl;//bug... je chercherai samedi
                continue;
            }
            if (abs(distance[sommet_courant] - distance[a->get_u()] - a->get_poids()) < 0.00001){
                sommet_courant=a->get_u();
                C.push_back(sommet_courant);
                trouve = true;cout<<C<<endl;
            }
        }
        if (!trouve){
            cerr<<"pas de chemin"<<endl;
            return pair<long, Chemin> (distance[t], C);
        }
    }
    return pair<long, Chemin> (distance[t], C);
}

long Dijkstradouble::requete(long s, long t){
    begin();
    priority_queue<pp, vector<pp>, priorite> Forward;
    priority_queue<pp, vector<pp>, priorite> Backward;

    map<long, long> distanceForward; // Couple distance, id sommet
    map<long, long> distanceBackward;

    map<long, bool> vusForward;
    map<long, bool> vusBackward;

    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        distanceForward[it1->second.get_id()] = LONG_MAX/3;
        distanceBackward[it1->second.get_id()] = LONG_MAX/3;
        vusForward[it1->second.get_id()] = false;
        vusBackward[it1->second.get_id()] = false;
    }

    distanceForward[s] = 0;
    distanceBackward[t] = 0;

    Forward.push(pp(0, s));
    vusForward[s] = true;
    Backward.push(pp(0, t));
    vusBackward[t] = true;
    long mu = LONG_MAX;
    while(!Forward.empty() && !Backward.empty()){

        if(Forward.top().first + Backward.top().first > mu){
            cout << "Duration : " << end() << endl;
            return mu;
        }
        // On depile l'élément de Forward et on regarde si on l'a déjà vu dans Backward.
        // Sinon on empile dans Forward ses voisins en mettant à jour les distances
        long u = Forward.top().second;
        depileEmpile(Forward, distanceForward, vusForward);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
        }

        long v = Backward.top().second;
        depileEmpile(Backward, distanceBackward,vusBackward, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
        }









            /*if(vusForward[v] && vusBackward[v]){
            cout << "vusForward" << endl;
            cout << "Duration : " << end() << endl;
            cout << v << " | " <<distanceBackward[v] << " | " << distanceForward[v] << endl;
            //return distanceForward[v] + distanceBackward[v];
        }*/



    }
    if(Forward.empty()){
         cout << "forwardEmpty" << endl;
        cout << "Duration : " << end() << endl;
        return distanceForward[t];// normalement jamais atteint
    }
    else{
         cout << "backwardEmpty" << endl;
        cout << "Duration : " << end() << endl;
        return distanceBackward[s];
   }
}

