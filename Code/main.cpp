#include <iostream>
#include <vector>
#include "Algorithme.h"
#include "Dijkstra.h"

#include "Graphe.h"
using namespace std;

int main()
{
    char* dossierCourant = "d:/Users/User/Documents/3-Formation/6 - semestre_3/MOPSI - Projet RO/Code/";
    char* sommets = dossierCourant + "instancePetite.txt";
    char* distances = dossierCourant + "instancePetiteDistance.txt";
    Graphe G(sommets, distances);
    Dijkstra D(&G);
    cout << "Graphe lu" << endl;
    cout << D.requete(1, 2) << endl;
    cout << D.requete(1, 48) << endl;

    //G.show();
    return 0;
}
