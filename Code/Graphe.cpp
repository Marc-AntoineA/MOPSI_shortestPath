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
    V = map<long, Sommet>();
    A = map<long, Arc>();

    // Ajout des sommets
    ifstream fichierSommets(nomFichierSommets.c_str());
    if (fichierSommets.is_open()){
        string ligne;
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
                V[id] = s;
                nV++;
            }
        }

        fichierSommets.close();
    }
    else cout << "Impossible d'ouvrir le fichier : " << nomFichierSommets << endl;

    // Ajout des arcs
    ifstream fichierArcs(nomFichierPoids.c_str());
    if (fichierArcs.is_open()){
        string ligne;
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

                // Ici un seul arc entre deux mêmes sommets. On bricole un identifiant
                long id = u*nV + v;
                Arc a(id, u, v, w);
                A[id] = a;

                V[u].add_arcP(a);
                V[v].add_arcM(a);
            }
        }

        fichierArcs.close();
    }
    else cout << "Impossible d'ouvrir le fichier : " << nomFichierPoids << endl;
}



void Graphe::show(){
    map<long, Sommet>::iterator it1;
    for(it1 = V.begin(); it1 != V.end(); ++it1){
        cout << it1->second <<endl;
    }

    map<long, Arc>::iterator it;
    for(it = A.begin(); it != A.end(); ++it){
        cout << it->second <<endl;
    }
}


int Graphe::get_randomSommet(){
    map<long, Sommet>::iterator it = V.begin();
    advance(it, rand()%V.size());
    return it->first;
}
