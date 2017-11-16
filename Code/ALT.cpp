#include "ALT.h"
#include "Dijkstra.h" // pour le préprocess
#include <cmath>
#include <fstream>
#include "fonctionsDiverses.h"

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
                output << "d " << v << " " << u << " " << d(v, u) << endl;
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

