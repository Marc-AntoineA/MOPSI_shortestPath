#include "AStar.h"
#include <queue>

void AStar::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, long t, long s, bool reverse){
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
                F.push(pp(dist[v] + pi(v, t, s), v));
            }
        }else{
            long v = a->get_u();
            if(dist[v] > dist[u] + a->get_poids()){
                dist[v] = dist[u] + a->get_poids();
                F.push(pp(dist[v] + pi(v, t, s), v));
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
        depileEmpile(F, distanceForward, t);
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

long AStarBidirectionnel::requete(long s, long t, bool verbose){
    begin();
    priority_queue<pp, vector<pp>, priorite> Forward;
    priority_queue<pp, vector<pp>, priorite> Backward;
    init_distanceForward(LONG_MAX/3);
    init_distanceBackward(LONG_MAX/3);

    distanceForward[s] = 0;
    distanceBackward[t] = 0;

    Forward.push(pp(0, s));
    Backward.push(pp(0, t));
    long mu = LONG_MAX;
    while(!Forward.empty() && !Backward.empty()){

        long u = Forward.top().second;
        long v = Backward.top().second;
        if(distanceForward[u] + distanceBackward[v] > mu){
            end();
            if(verbose)
                cout << "Duration : " << get_duration() << endl;
            return mu;
        }
        depileEmpile(Forward, distanceForward, t);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun = u;
        }

        depileEmpile(Backward, distanceBackward, s, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
            point_commun = v;
        }
    }

    if(Forward.empty()){
        end();
        if(verbose){
            cout << "Forward empty " << endl;
            cout << "Duration : " << get_duration() << endl;

        }
        return distanceForward[t];// normalement jamais atteint
    }
    else{
        end();
        if(verbose){
            cout << "Backward empty " << endl;
            cout << "Duration : " << get_duration() << endl;
        }
        return distanceBackward[s];
   }
}

pair<long, Chemin> AStarBidirectionnel::requete_chemin(long s, long t, bool verbose){
    long mu = requete(s, t, verbose);
    //on retrouve maintenant le chemin
    Chemin C_forward = reconstitution_chemin_forward(s, point_commun, &distanceForward);
    Chemin C_backward = reconstitution_chemin_backward(point_commun, t, &distanceBackward);

    return pair<long, Chemin> (mu, C_forward.join(C_backward));
}
