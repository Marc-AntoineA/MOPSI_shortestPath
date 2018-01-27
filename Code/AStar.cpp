#include "AStar.h"
#include <climits>
#include <queue>

AStar::AStar(Graphe* g):Algorithme(g){
    long value=LONG_MAX/3;
    distanceBackward0.push_back(value);
    distanceForward0.push_back(value);
    for (int i=1;i<get_V()->size();i++){
        distanceBackward0.push_back(value);
        distanceForward0.push_back(value);
    }
}

void AStar::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, vector<long> &dist, long t, long s, bool reverse){
    long u = F.top().second;
    //long p = F.top().first;
    F.pop();
    vector<long>* deltaP;
    if(reverse)
        deltaP = ((*V)[u]).get_deltaM();
    else
        deltaP = ((*V)[u]).get_deltaP();
    int size=deltaP->size();
    long v;
    Arc* a;
    for(int k = 0; k < size; k++){
         a = &((*A)[(*deltaP)[k]]);
        if(!reverse){
            v = a->get_v();
        }else{
            v = a->get_u();
        }
        if(dist[v] > dist[u] + a->get_poids()){          //modif ici ?
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

void AStar::BD_finish(priority_queue<pp, vector<pp>, priorite> Forward, priority_queue<pp, vector<pp>, priorite> Backward, long& mu){
    long u, v;
    vector<long> *deltaP, *deltaM;
    Arc* a;
    while(!Forward.empty()){
        u = Forward.top().second;
        Forward.pop();
        deltaP = ((*V)[u]).get_deltaP();
        for(int k = 0; k < deltaP->size(); k++){
            a = &((*A)[(*deltaP)[k]]);
            if (distanceBackward[a->get_v()]+distanceForward[u]+a->get_poids() < mu){
                mu = distanceBackward[a->get_v()]+distanceForward[u]+a->get_poids();
                point_commun = u;
                distanceBackward[u] = distanceBackward[a->get_v()]+a->get_poids();
            }
        }
    }
    while(!Backward.empty()){
        v = Backward.top().second;
        Backward.pop();
        deltaM = ((*V)[v]).get_deltaM();
        for(int k = 0; k < deltaM->size(); k++){
            a = &((*A)[(*deltaM)[k]]);
            if (distanceForward[a->get_u()]+distanceBackward[v]+a->get_poids() < mu){
                mu = distanceForward[a->get_u()]+distanceBackward[v]+a->get_poids();
                point_commun = v;
                distanceForward[u] = distanceForward[a->get_u()]+a->get_poids();
            }
        }
    }
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
    bool stop=false;
    while(!Forward.empty() && !Backward.empty() && !stop){

        long u = Forward.top().second;
        long v = Backward.top().second;
        if(distanceForward[u] + distanceBackward[v] > mu){
            stop=true;
        }
        add_visite();
        depileEmpile(Forward, distanceForward, t);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun = u;
        }
        add_visite();
        depileEmpile(Backward, distanceBackward, s, t, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
            point_commun = v;
        }
    }
    BD_finish(Forward, Backward, mu);
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;
    return mu;
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
