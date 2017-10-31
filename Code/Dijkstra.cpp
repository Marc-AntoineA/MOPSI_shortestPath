#include "Dijkstra.h"
#include <queue>
#include <vector>

#define pp pair<long, long> // paire priorité / id sommet

using namespace std;

struct priorite{
    bool operator()(const pp &p1, const pp &p2){
        return p1.first < p2.first;
    }
};

Dijkstra::Dijkstra(Graphe* g):Algorithme(g){}

long Dijkstra::requete(long s, long t){

    map<long, Sommet>* V = (*G).get_sommets();
    map<long, Arc>* A = (*G).get_arcs();

    priority_queue<pp, vector<pp>, priorite> F;

    map<long, long> distance; // Couple distance, id sommet
    map<long, Sommet>::iterator it1;
    for(it1 = (*V).begin(); it1 != (*V).end(); ++it1){
        distance[it1->second.get_id()] = LONG_MAX;
    }

    distance[s] = 0;
    F.push(pp(0, s));

    while(!F.empty()){
        long u = F.top().second;
        F.pop();
        vector<long>* deltaP = ((*V)[u]).get_deltaP();

        for(int k = 0; k < (*deltaP).size(); k++){
            Arc* a = &((*A)[(*deltaP)[k]]);
            long v = (*a).get_v();
            if(distance[v] > distance[u] + (*a).get_poids()){
                distance[v] = distance[u] + (*a).get_poids();
                F.push(pp(distance[v], v));
            }
        }

    }
    return distance[t];

}
