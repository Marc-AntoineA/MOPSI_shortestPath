#include "ALT.h"
#include "Dijkstra.h" // pour le préprocess
#include <cmath>

long ALT::pi(long u, long t, long s){
    long M = 0; // borne maximale (ou son estimation)
    int n = L.size();
    for(int ld = 0; ld < n; ld++){
        M = max(M, d(u, ld) - d(t, ld));
        M = max(M, d(ld, t) - d(ld, u));
    }
    return M;
}

void ALT::preprocess(int n, bool verbose){
    begin();
    if(verbose)
        cout << "Pre-process en cours..." << endl;
    long ld;
    map<long, Sommet>::iterator it1;
    long u; long v;
    for(int k = 0; k < n; k++){
        ld = G->get_randomSommet();
        L.push_back(ld);
        for(it1 = V->begin(); it1 != V->end(); ++it1){
            u = ld;
            v = it1->second.get_id();
            subDist[pp(u, v)] = requete(u, v);
            subDist[pp(v, u)] = requete(v, u);
        }
    }

    if(verbose)
        cout << "\t \t... pre process termine. " << endl;
    end();
}
