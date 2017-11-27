#include <iostream>
#include <vector>
#include "Algorithme.h"
#include "Dijkstra.h"
#include "Tests.h"
#include "Graphe.h"
#include "Chemin.h"
#include "ALT.h"
#include "ArcFlags.h"
#include <string>

using namespace std;


int main()
{//priority_queue<triplet, vector<triplet>, priorite2> F;F.push(triplet(pp(1,1),1));return 0;
    string root = "/home/dekeyser/Documents/MOPSI/MOPSI_shortestPath/Code/Instance/";
    //string root = "d:/Users/User/Documents/3-Formation/6 - semestre_3/MOPSI - Projet RO/MOPSI_ShortestPath/MOPSI_shortestPath/Code/Instance/";

    string instance = "NewYork/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphe en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;


    //ALT Alt(&G);

    //Alt.preprocess(5, true);
    //Alt.sauvegarde(root + instance + "output5.pp", instance);
    //Alt.preprocess(root + instance + "output5.pp", true);//
    //cout << Alt.d(1, 1532) << endl;
    //cout << Alt.requete(1532, 1) << endl;//
    //cout << Alt.get_visites() << endl;//
    //Alt.requete(1, 2, true);
    //Alt.requete(1, 160652, true);

//    ArcFlags AF (&G);
//    AF.preprocess_quadrillage(4, true);
//    //cout<<AF.requete(1532, 1, true)<<endl;
//    AF.montrer_repartition();
//    AF.test_preprocess();

    test_requete(G, 10, root + instance + "output5.pp");


    return 0;
}
