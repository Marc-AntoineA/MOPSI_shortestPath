#include <iostream>
#include <vector>
#include "Algorithme.h"
#include "Dijkstra.h"
#include "Tests.h"
#include "Graphe.h"
#include "Chemin.h"
#include "ALT.h"
#include <string>

using namespace std;


int main()
{
    //string root = "/home/dekeyser/Documents/MOPSI/MOPSI_shortestPath/Code/Instance/";
    string root = "d:/Users/User/Documents/3-Formation/6 - semestre_3/MOPSI - Projet RO/MOPSI_ShortestPath/MOPSI_shortestPath/Code/Instance/";

    string instance = "NewYork/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphe en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;


    ALT Alt(&G);

    Alt.preprocess(5, true);
    Alt.sauvegarde(root + instance + "output5.pp", instance);
    test_requete(G, 15, root + instance + "output5.pp");


    return 0;
}
