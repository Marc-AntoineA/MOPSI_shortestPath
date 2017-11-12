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

pair<long, Chemin> Dijkstra::requete_chemin(long s, long t){
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
    return pair<long, Chemin> (distance[t], reconstitution_chemin_forward(s,t,&distance));
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

pair<long, Chemin> Dijkstradouble::requete_chemin(long s, long t){
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
    bool fini = false;
    long point_commun;
    while(!Forward.empty() && !Backward.empty() && !fini){

        if(Forward.top().first + Backward.top().first > mu){
            cout << "Duration : " << end() << endl;
            fini = true;
        }
        // On depile l'élément de Forward et on regarde si on l'a déjà vu dans Backward.
        // Sinon on empile dans Forward ses voisins en mettant à jour les distances
        long u = Forward.top().second;
        depileEmpile(Forward, distanceForward, vusForward);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun=u;
        }

        long v = Backward.top().second;
        depileEmpile(Backward, distanceBackward,vusBackward, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
            point_commun=v;
        }
    }

    //on retrouve maintenant le chemin
    Chemin C_forward = reconstitution_chemin_forward(s, point_commun, &distanceForward);
    Chemin C_backward = reconstitution_chemin_backward(point_commun, t, &distanceBackward);
    return pair<long, Chemin> (mu, C_forward.join(C_backward));
}



