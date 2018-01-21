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
    //string root = "/home/dekeyser/Documents/MOPSI/MOPSI_shortestPath/Code/Instance/";
    string root = "/home/marc-antoinea/Documents/3-Formation/6 - semestre_3/MOPSI - Projet RO/MOPSI_shortestPath/Code/Instance/";
    string instance = "NewYork/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphe en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t ... Graphe lu" << endl;


    ALT Alt(&G);
    Dijkstra D(&G);
    //Alt.preprocess(5, true);
    //Alt.sauvegarde(root + instance + "output5.pp", instance);
    Alt.preprocess(root + instance + "output15.pp", true);
    //cout << Alt.requete(4088,33180) << endl;
    //cout << D.requete(4088,33180) << endl;
    
    //cout << Alt.get_visites() << endl;
    //Alt.requete(1, 2, true);
    //Alt.requete(1, 160652, true);

    //ArcFlags AF (&G);
    //AF.preprocess(root + instance + "outputAF2.pp", true);
    //AF.preprocess_quadrillage(5, true);
    //AF.sauvegarde(root + instance + "outputAF2.pp", instance, true);
    ///cout<<D.requete(1564, 79615, true)<<endl;//1532, 1
    ///cout<<AF.requete(1532, 1, true)<<endl;

<<<<<<< fbc9e09d9d7f086c4cd78ebcf5ecfecbf5b0472a
    test_requete(G, 100, root + instance + "output5.pp", root + instance + "outputAF2.pp");
=======
    test_requete(G, 50, root + instance + "output5.pp", root + instance + "outputAF2.pp");
>>>>>>> 'Correction' du bug de Alt...

    return 0;
}

