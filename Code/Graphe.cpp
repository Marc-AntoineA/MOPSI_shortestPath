#include "Graphe.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <cstdlib>
#include <ctime>
#include "fonctionsDiverses.h"
#include <climits>
#include <cmath>
using namespace std;

Graphe::Graphe(string nomFichierSommets, string nomFichierPoids)
{
    // Initialisations
    srand (time(NULL));

    // Ajout des sommets
    ifstream fichierSommets(nomFichierSommets.c_str());
    if (fichierSommets.is_open()){
        string ligne;
        int id_apriori=1;
        while(getline(fichierSommets, ligne)){
            if (ligne.size()> 0 && string(ligne, 0, 1) == "v"){
                size_t k = 2;
                size_t k2 = 2;

                k2 = ligne.find(' ', k);
                int id;
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
                V[id].setCoords(coords);
                V[id].setId(id);
                if (id!=id_apriori) cerr<<"construction : sommets mal indexes, on a trouve "<<id<<" alors qu'on s'attendait a "<<id_apriori<<endl;
                id_apriori++;
            }
            else if (ligne.size()> 0 && string(ligne, 0, 1) == "p"){
                size_t k = 2;
                size_t k2 = 2;

                k2 = ligne.find(' ', k);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), nV);
                nV++;
                V = vector<Sommet> (nV);
            }
            if (id_apriori%1000000==0){
                if (id_apriori>0)
                    cout<<id_apriori<<endl;
            }
        }
        fichierSommets.close();
    }
    else cout << "Impossible d'ouvrir le fichier : " << nomFichierSommets << endl;

    cout<<"\t\t...sommets lus..."<<endl;

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
                int u;
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

                // Ici un seul arc entre deux memes sommets. On bricole un identifiant Impossible !
                int id = current_id;
                current_id ++;
                Arc a(id, u, v, w);
                A[id]=a;
                V[u].add_arcP(id);
                V[v].add_arcM(id);
            }
            else if (ligne.size()> 0 && string(ligne, 0, 1) == "p"){
                size_t k = 2;
                size_t k2 = 2;

                k2 = ligne.find(' ', k);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                k = k2 + 1;

                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), nA);
                A = vector<Arc>(nA);
            }
            if (current_id%1000000==0){
                if (current_id > 0)
                    cout<<current_id<<endl;
            }
        }

        fichierArcs.close();
    }
    else cout << "Impossible d'ouvrir le fichier : " << nomFichierPoids << endl;
}

//int Graphe::getReverseArc(const int &a){
//    Arc ap = A[a];
//    Arc b = A[a-1];
//    if (b.get_u()==ap.get_v() && b.get_v()==ap.get_u())
//        return a-1;
//    else
//        return a+1;
//}

int Graphe::getReverseArc(const int &a){
    if (a%2)
        return a-1;
    else
        return a+1;
}

//void Graphe::show(){
//    for (int i=1; i<nV+1;i++){
//        cout << V[i] << endl;
//    }
//    for (int i=1; i<nA+1;i++){
//        cout << A[i] << endl;
//    }
//}


int Graphe::get_randomSommet(){
    return rand()%(V.size()-1)+1;
}

vector<pair<int, int> >Graphe::getTestSample(int N, bool verbose){
    if (verbose)cout<<"exploration en cours..."<<endl;
    long min=LONG_MAX;
    long max=0;
    long d;
    int u,v;
    for (int i=0;i<10000;i++){
        u = get_randomSommet();
        v = get_randomSommet();
        d = V[u].distance(V[v]);
        if (d < min) min = d;
        if (d > max) max = d;
    }
    if (verbose)cout<<"exploration finie "<<min<<" "<<max<<endl<<"recherche de sommets aux distances bien reparties..."<<endl;

    double a = log(double(min));
    double b = log(double(max));

    vector<pair<int, int> >result;
    vector<int> hist(10, 0);
    int count = 0;
    int k;
    double c;
    int i;
    while (count < N){
        u = get_randomSommet();
        v = get_randomSommet();
        c = log(V[u].distance(V[v]));
        if (c > b || c < a)
            continue;
        k = int(10*(c-a)/(b-a));//cout<<k<<endl;
        if (k < 0 || k > 9) {cerr<<"mauvais k : "<<k<<" "<<c<<" "<<a<<" "<<b<<" distance entre  "<<u<<" et "<<v<<" : "<<V[u].distance(V[v])<<endl;continue;}
        if (hist[k] < N/10){
            hist[k]++;
            count++;
            result.push_back(pair<int, int>(u, v));
        }
        if (verbose && i%100000==0){
            cout<<"histogramme actuel ";
            for (int j=0;j<10;j++)
                cout<<hist[j]<<" ";
            cout<<count<<endl;
        }
        i++;
    }
    if (verbose)cout<<"recherche finie"<<endl;
    if (result.size()!=N)cerr<<"pas assez de resultats"<<endl;
    return result;
}
