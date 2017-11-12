#include <iostream>
#include <vector>
#include "Algorithme.h"
#include "Dijkstra.h"
#include "Tests.h"
#include "Graphe.h"
#include "Chemin.h"
#include <string>

using namespace std;


int main()
{
    //string root = "/home/dekeyser/Documents/MOPSI/MOPSI_shortestPath/Code/Instance/";
    string root = "d:/Users/User/Documents/3-Formation/6 - semestre_3/MOPSI - Projet RO/MOPSI_ShortestPath/MOPSI_shortestPath/Code/Instance/";

    string instance = "NewYork/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphes en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;

    //test_requete(G);

    Dijkstradouble DD(&G);
    Dijkstra D(&G);
    cout <<"requete Dijkstra :" << endl;
    cout << D.requete(5279, 6937) << endl;
    //cout << D.requete_chemin(5612, 29793).second << endl<<endl;
    //cout << D.requete(1, 48) << endl;

    cout <<"requete double Dijkstra :" << endl;
    cout << DD.requete(5279, 6937) << endl;
    //cout << DD.requete_chemin(5612, 29793).second << endl<<endl;
    /*cout << DD.point_commun << endl;
    //cout << DD.requete(1, 48) << endl;

    cout <<"requete Dijkstra avec chemin :" << endl;
    Chemin C1 = D.requete_chemin(1, 200).second;
    cout << C1 << endl;
    cout <<"La longueur du chemin est de : " << C1.longueur(D.get_A(), D.get_V())<<endl;

    cout <<"requete double Dijkstra avec chemin :" << endl;
    Chemin C2 = DD.requete_chemin(1, 200).second;
    cout << C2 << endl;
    cout <<"La longueur du chemin est de : " << C2.longueur(DD.get_A(), DD.get_V())<<endl;
    */
    //G.show();
    return 0;
}
