#include <iostream>
#include <vector>
#include "Algorithme.h"
#include "Dijkstra.h"

#include "Graphe.h"
#include "Chemin.h"
#include <string>

using namespace std;


int main()
{
    string root = "/home/dekeyser/Documents/MOPSI/MOPSI_shortestPath/Code/Instance/";
    string instance = "NewYork/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphes en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;

    Dijkstra D(&G);
    cout <<"requete Dijkstra :" << endl;
    cout << D.requete(1, 200) << endl;
    //cout << D.requete(1, 48) << endl;

    Dijkstradouble DD(&G);
    cout <<"requete double Dijkstra :" << endl;
    cout << DD.requete(1, 200) << endl;
    //cout << DD.requete(1, 48) << endl;

    cout <<"requete Dijkstra avec chemin :" << endl;
    Chemin C1 = D.requete_chemin(1, 200).second;
    cout << C1 << endl;
    cout <<"La longueur du chemin est de : " << C1.longueur(D.get_A(), D.get_V())<<endl;

    cout <<"requete double Dijkstra avec chemin :" << endl;
    Chemin C2 = DD.requete_chemin(1, 200).second;
    cout << C2 << endl;
    cout <<"La longueur du chemin est de : " << C2.longueur(DD.get_A(), DD.get_V())<<endl;

    //G.show();
    return 0;
}
