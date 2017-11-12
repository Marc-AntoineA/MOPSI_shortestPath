#include "AStar.h"
#include "queue"

void AStar::depileEmpile(long t, priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, bool reverse){
    long u = F.top().second;
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
            if(dist[v] > dist[u] + a->get_poids()){
                dist[v] = dist[u] + a->get_poids();
                F.push(pp(dist[v] + pi(v, t), v));
            }
        }else{
            long v = a->get_u();
            if(dist[v] > dist[u] + a->get_poids()){
                dist[v] = dist[u] + a->get_poids();
                F.push(pp(dist[v] + pi(v, t), v));// faut surement avoir s pour reverse
            }
        }
    }
}


void AStar::init_distanceForward(long value){
    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        distanceForward[it1->second.get_id()] = value;
    }
}
void AStar::init_distanceBackward(long value){
    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        distanceBackward[it1->second.get_id()] = value;
    }
}
long AStar::requete(long s, long t, bool verbose){
    begin();
    priority_queue<pp, vector<pp>, priorite> F;

    init_distanceForward(LONG_MAX);
    distanceForward[s] = 0;
    F.push(pp(0, s));
    bool stop = false;
    while(!F.empty() && !stop){
        if(F.top().second == t)
            stop = true;
        depileEmpile(t, F, distanceForward);
    }
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;

    return distanceForward[t];
}

pair<long, Chemin> AStar::requete_chemin(long s, long t, bool verbose){
    requete(s, t, verbose);
    return pair<long, Chemin> (distanceForward[t], reconstitution_chemin_forward(s,t, &distanceForward));
}

int AStarGeographique::pi(long u, long t){
    return (*V)[u].distance((*V)[t]);
}
