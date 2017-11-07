#include <iostream>
#include <vector>
#include "Algorithme.h"
#include "Dijkstra.h"

#include "Graphe.h"
#include <string>

using namespace std;

int main()
{
    string root = "d:/Users/User/Documents/3-Formation/6 - semestre_3/MOPSI - Projet RO/MOPSI_ShortestPath/MOPSI_ShortestPath/Code/Instance/";
    string instance = "NewYork/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphes en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;

    Dijkstra D(&G);
    cout << D.requete(1, 200) << endl;
    //cout << D.requete(1, 48) << endl;

    Dijkstradouble DD(&G);
    cout << DD.requete(1, 200) << endl;
    //cout << DD.requete(1, 48) << endl;
    //G.show();
    return 0;
}
