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

    string instance = "Petite/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphes en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;

    ALT Alt(&G);
    //Alt.preprocess(5, true);
    //Alt.sauvegarde(root + "output.pp", instance);
    Alt.preprocess(root + "output.pp");
    cout << Alt.requete(12, 2) << endl;
    cout << "Preprocess duration : " << Alt.get_duration() << endl;
    cout << Alt.requete(12, 2) << endl;
    //test_requete(G);

    return 0;
}
