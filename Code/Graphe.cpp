#include "Graphe.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <cstdlib>
#include <ctime>
#include "fonctionsDiverses.h"

using namespace std;

Graphe::Graphe(string nomFichierSommets, string nomFichierPoids)
{
    srand (time(NULL));
    // Initialisations
    nV = 0; nA = 0;

    // Ajout des sommets
    ifstream fichierSommets(nomFichierSommets.c_str());
    if (fichierSommets.is_open()){
        string ligne;
        long id_apriori=1;
        V.push_back(Sommet(0, pair<int, int>(0, 0)));
        while(getline(fichierSommets, ligne)){
            if (ligne.size()> 0 && string(ligne, 0, 1) == "v"){
                size_t k = 2;
                size_t k2 = 2;

                k2 = ligne.find(' ', k);
                long id;
                from_string(string(ligne, k, k2 - k), id);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                int x;
                from_string(string(ligne, k, k2 - k), x);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                int y;
                from_string(string(ligne, k, k2 - k), y);
                k = k2 + 1;
                pair<int, int> coords(x, y);
                Sommet s(id, coords);
                V.push_back(s);
                //V[id] = s;
                nV++;
                if (id!=id_apriori) cerr<<"construction : sommets mal indexes, on a trouve "<<id<<" alors qu'on s'attendait a "<<id_apriori<<endl;
                id_apriori++;
            }
        }

        fichierSommets.close();
    }
    else cout << "Impossible d'ouvrir le fichier : " << nomFichierSommets << endl;

    // Ajout des arcs
    ifstream fichierArcs(nomFichierPoids.c_str());
    if (fichierArcs.is_open()){
        string ligne;
        int current_id = 0;
        while(getline(fichierArcs, ligne)){
            if (ligne.size()> 0 && string(ligne, 0, 1) == "a"){
                size_t k = 2;
                size_t k2 = 2;

                k2 = ligne.find(' ', k);
                long u;
                from_string(string(ligne, k, k2 - k), u);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                int v;
                from_string(string(ligne, k, k2 - k), v);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                int w;
                from_string(string(ligne, k, k2 - k), w);
                k = k2 + 1;

                // Ici un seul arc entre deux mêmes sommets. On bricole un identifiant Impossible !
                long id = current_id;
                current_id ++;
                Arc a(id, u, v, w);
                A.push_back(a);

                V[u].add_arcP(a);
                V[v].add_arcM(a);
                nA++;
            }
        }

        fichierArcs.close();
    }
    else cout << "Impossible d'ouvrir le fichier : " << nomFichierPoids << endl;
}



void Graphe::show(){
    for (int i=1; i<nV+1;i++){
        cout << V[i] << endl;
    }
    for (int i=1; i<nA+1;i++){
        cout << A[i] << endl;
    }
}


long Graphe::get_randomSommet(){
    return rand()%V.size();
}
