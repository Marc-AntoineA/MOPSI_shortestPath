#include "ALT.h"
#include "Dijkstra.h" // pour le preprocess
#include <cmath>
#include <fstream>
#include <climits>
#include "fonctionsDiverses.h"
#include <ctime>

long ALT::pi(long u, long t, long s){
    long M = 0; // borne maximale (ou son estimation)
    int n = L.size();
    for(int ld = 0; ld < n; ld++){
        M = max(M, - d(L[ld], t) + d(L[ld], u)); //
        if (d(L[ld], t) - d(L[ld], u) > M){      //meme chose que ds le document "review ..." ?
            M = d(L[ld], t) - d(L[ld], u);       //
        }
    }
    return M;
}

void ALT::preprocess(int n, bool verbose){
    begin();
    if(verbose){
        cout << "Pre-process en cours..." << endl;
        cout << "0% : " << flush;
    }
    L.clear();
    long s; long v;
    double prec = 0;
    for(int k = 0; k < n; k++){
        s = G->get_randomSommet();
        L.push_back(s);

        priority_queue<pp, vector<pp>, priorite> F;
        init_distanceForward();
        distanceForward[s] = 0;

        F.push(pp(0, s));
        while(!F.empty()){
            depileEmpile(F, distanceForward, F.top().second, s);
        }

        // On stocke les distances
        map<long, Sommet>::iterator it1;
        for(it1 = V->begin(); it1 != V->end(); it1++){
            v = it1->first;
            subDist[pp(s, v)] = distanceForward[v];
        }

        if(verbose && (double)k / n - prec > 0.05){
            cout << "="<< flush;
            prec = k/n;
        }
    }
    end();
    if(verbose)
        cout << " 100% " << endl;
        cout << "Duration : " << get_duration() << endl;

}

void ALT::preprocess(string nomInput, bool verbose){
    begin();
    ifstream input(nomInput.c_str());
    if(input.is_open()){
        L.clear();
        subDist.clear();
        string ligne;
        long id1; long id2; long dist;

        size_t k; size_t k2;
        while(getline(input, ligne)){
            if (ligne.size()> 0 && string(ligne, 0, 1) == "l"){
                k = 2;
                k2 = ligne.find(' ', k);

                from_string(string(ligne, k, k2 - k), id1);
                k = k2 + 1;
                L.push_back(id1);
            }
            if(ligne.size() > 0 && string(ligne, 0, 1) == "d"){
                k = 2;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), id1);
                k = k2 + 1;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), id2);
                k = k2 + 1;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), dist);
                subDist[pp(id1, id2)] = dist;
            }
        }
    }else{
        cerr << "ALT::preprocess : " << nomInput
             << "n'a pas pu etre ouvert. Le chargement a echoue" << endl;
    }
    end();
}

void ALT::sauvegarde(string nomOutput, string instance, bool verbose){
    begin();
    if(verbose){
        cout << "Sauvegarde en cours..." << endl;
    }
    ofstream output(nomOutput.c_str());
    if (output.is_open()){
        output << "c Projet MOPSI - Algorithmes de plus courts chemins "<< endl;
        output << "c Donnees de preprocess pour l'algorithme ALT* " << endl;
        output << "c Instance de : " << instance << endl;
        output << "c Format pour les sommets choisis : l <id>" << endl;
        output << "c Format pour les distances : d <id1> <id2> <dist(id1, id2)>" << endl;


        int n = L.size();
        for(int k = 0; k < n; k++){
            output << "l " << L[k] << endl;
        }

        long u; long v;
        map<long, Sommet>::iterator it1;
        for(int k = 0; k < n; k++){
            u = L[k];
            for(it1 = V->begin(); it1 != V->end(); it1++){
                v = it1->second.get_id();
                output << "d " << u << " " << v << " " << d(u, v) << endl;
            }
        }
    }else{
        cerr << "ALT::sauvegarde | Le fichier : "
             << nomOutput << " n'a pas pu être ouvert et la sauvegarde n'a pas ete effectuee" << endl;
    }
    if(verbose)
        cout << "\t\t Sauvegarde terminée" << endl;
    end();
}

