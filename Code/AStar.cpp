#include "AStar.h"
#include <climits>
#include <queue>

<<<<<<< fbc9e09d9d7f086c4cd78ebcf5ecfecbf5b0472a
AStar::AStar(Graphe* g):Algorithme(g){
    map<long, Sommet>::iterator it1, end;
    long value=LONG_MAX/3;
    end=V->end();
    for(it1 = V->begin(); it1 != end; ++it1){
        distanceForward0[it1->second.get_id()] = value;
        distanceBackward0[it1->second.get_id()] = value;
    }
}

void AStar::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, long t, long s, bool reverse){
=======
void AStar::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F,
 map<long, long>& dist, long t, long s, bool reverse){
>>>>>>> 'Correction' du bug de Alt...
    long u = F.top().second;
    long p = F.top().first;
    F.pop();
    vector<long>* deltaP;
    if(reverse)
        deltaP = ((*V)[u]).get_deltaM();
    else
        deltaP = ((*V)[u]).get_deltaP();

    for(int k = 0; k < deltaP->size(); k++){
        Arc* a = &((*A)[(*deltaP)[k]]);
        long v;
        if(!reverse){
            v = a->get_v();
        }else{
            v = a->get_u();
        }
        if(dist[v] > dist[u] + a->get_poids()){
            dist[v] = dist[u] + a->get_poids();
            F.push(pp(dist[v] + pi(v, t, s), v));
        }
    }
}

void AStar::init_distanceForward(){
    distanceForward=distanceForward0;
}

void AStar::init_distanceBackward(){
    distanceBackward=distanceBackward0;
}

long AStar::requete(long s, long t, bool verbose){
    begin();
    priority_queue<pp, vector<pp>, priorite> F;
    init_distanceForward();/*
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;
    begin();*/

    distanceForward[s] = 0;
    F.push(pp(0, s));
    
    bool stop = false;
    while(!F.empty() && !stop){
        long u = F.top().second;
        if(distanceForward[u] < distanceForward[t]){
            add_visite();
            //cout << u << "\t" << distanceForward[u] << "\t" << distanceForward[t] << endl;
            depileEmpile(F, distanceForward, t);
        }else{
            F.pop();
        }
        if(u == t){
            stop = true;
        }
    }
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;

    return distanceForward[t];
}

pair<long, Chemin> AStar::chemin(long s, long t, bool verbose){
    requete(s, t, verbose);
    return pair<long, Chemin> (distanceForward[t], reconstitution_chemin_forward(s,t, &distanceForward));
}

long AStar::requete_bi(long s, long t, bool verbose){
    begin();
    priority_queue<pp, vector<pp>, priorite> Forward;
    priority_queue<pp, vector<pp>, priorite> Backward;
    init_distanceForward();
    init_distanceBackward();

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
        add_visite();
        depileEmpile(Forward, distanceForward, t);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun = u;
        }
        add_visite();
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
        return distanceForward[t];
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

pair<long, Chemin> AStar::chemin_bi(long s, long t, bool verbose){
    long mu = requete(s, t, verbose);
    //on retrouve maintenant le chemin
    Chemin C_forward = reconstitution_chemin_forward(s, point_commun, &distanceForward);
    Chemin C_backward = reconstitution_chemin_backward(point_commun, t, &distanceBackward);

    return pair<long, Chemin> (mu, C_forward.join(C_backward));
}
