#include "Dijkstra.h"
#include <cassert>
#include <climits>  //Pour LONG_MAX
#include <stdlib.h> //Pour abs


using namespace std;

void Dijkstra::init_distance(long value)
{
    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        distance[it1->second.get_id()] = value;
    }
}

void Dijkstra::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, bool reverse){
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
                F.push(pp(dist[v], v));
            }
        }else{
            long v = a->get_u();
            if(dist[v] > dist[u] + a->get_poids()){
                dist[v] = dist[u] + a->get_poids();
                F.push(pp(dist[v], v));
            }
        }
    }
}

long Dijkstra::requete(long s, long t, bool verbose){
    begin();
    priority_queue<pp, vector<pp>, priorite> F;

    init_distance(LONG_MAX);
    distance[s] = 0;
    F.push(pp(0, s));
    bool stop = false;
    while(!F.empty() && !stop){
        if(F.top().second == t)
            stop = true;
        depileEmpile(F, distance);
    }

    if(verbose)
        cout << "Duration : " << end() << endl;

    return distance[t];
}

pair<long, Chemin> Dijkstra::requete_chemin(long s, long t, bool verbose){
    requete(s, t, verbose);
    return pair<long, Chemin> (distance[t], reconstitution_chemin_forward(s,t, &distance));
}

void Dijkstradouble::init_distances(long value){
    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        distanceForward[it1->second.get_id()] = value;
        distanceBackward[it1->second.get_id()] = value;
    }
}

long Dijkstradouble::requete(long s, long t, bool verbose){
    begin();
    priority_queue<pp, vector<pp>, priorite> Forward;
    priority_queue<pp, vector<pp>, priorite> Backward;
    init_distances(LONG_MAX/3);

    distanceForward[s] = 0;
    distanceBackward[t] = 0;

    Forward.push(pp(0, s));
    Backward.push(pp(0, t));
    long mu = LONG_MAX;
    while(!Forward.empty() && !Backward.empty()){
        if(Forward.top().first + Backward.top().first > mu){
            cout << "ici";
            if(verbose)
                cout << "Duration : " << end() << endl;
            return mu;
        }
        long u = Forward.top().second;
        depileEmpile(Forward, distanceForward);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun = u;
        }

        long v = Backward.top().second;
        depileEmpile(Backward, distanceBackward, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
            point_commun = v;
        }
    }

    if(Forward.empty()){
        cout << "forwardempty" << endl;
        if(verbose){
            cout << "Duration : " << end() << endl;

        }
        return distanceForward[t];// normalement jamais atteint
    }
    else{
        cout << "bacrdempty" << endl;
        if(verbose)
            cout << "Duration : " << end() << endl;
        return distanceBackward[s];
   }
}

pair<long, Chemin> Dijkstradouble::requete_chemin(long s, long t, bool verbose){
    long mu = requete(s, t, verbose);
    //on retrouve maintenant le chemin
    Chemin C_forward = reconstitution_chemin_forward(s, point_commun, &distanceForward);
    Chemin C_backward = reconstitution_chemin_backward(point_commun, t, &distanceBackward);
    cout << endl << "Cfw" << endl << C_forward << endl;
    C_backward.inversion();
    cout << endl << "Cbw" << endl << C_backward << endl;
    return pair<long, Chemin> (mu, C_forward.join(C_backward));
}



