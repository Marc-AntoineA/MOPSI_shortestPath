#include "Dijkstra.h"
#include <cassert>
#include <climits>  //Pour LONG_MAX
#include <stdlib.h> //Pour abs


using namespace std;

int Dijkstra::pi(long u, long t){
    return 0;
}


long Dijkstradouble::requete(long s, long t, bool verbose){
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
        if(Forward.top().first + Backward.top().first > mu){
            end();
            if(verbose)
                cout << "Duration : " << get_duration() << endl;
            return mu;
        }
        long u = Forward.top().second;
        depileEmpile(t, Forward, distanceForward);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun = u;
        }

        long v = Backward.top().second;
        depileEmpile(t, Backward, distanceBackward, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
            point_commun = v;
        }
    }

    if(Forward.empty()){
        cout << "forwardempty" << endl;
        end();
        if(verbose){
            cout << "Duration : " << get_duration() << endl;

        }
        return distanceForward[t];// normalement jamais atteint
    }
    else{
        end();
        cout << "bacrdempty" << endl;
        if(verbose)
            cout << "Duration : " << get_duration() << endl;
        return distanceBackward[s];
   }
}

pair<long, Chemin> Dijkstradouble::requete_chemin(long s, long t, bool verbose){
    long mu = requete(s, t, verbose);
    //on retrouve maintenant le chemin
    Chemin C_forward = reconstitution_chemin_forward(s, point_commun, &distanceForward);
    Chemin C_backward = reconstitution_chemin_backward(point_commun, t, &distanceBackward);

    return pair<long, Chemin> (mu, C_forward.join(C_backward));
}



