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

    string instance = "ChamberySmall/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphes en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;
    test_requete(G, 1000, root + instance + "output10N.pp");

    ALT Alt(&G);

    //Alt.preprocess(10, true);
    //Alt.sauvegarde(root + instance + "output10N.pp", instance);
    //Alt.preprocess(root + instance + "output.pp");
    //cout << D.requete(7294, 36510) << endl;
    //cout << "Preprocess duration : " << Alt.get_duration() << endl;
    //cout << Alt.requete(13636, 13637) << endl;


    return 0;
}
