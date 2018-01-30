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
#include <unistd.h>

using namespace std;

int main()
{//int n=50;int* a = new int [n];for(int i=0; i<n; i++) a[n] = rand();cout<<a[rand()%n]*a[rand()%n]<<endl;delete[] a;return 0;//int* deltaM = new int[2]; while(true){int* old = deltaM; deltaM = new int [2]; delete [] old;}
    //string root = "/home/dekeyser/Documents/MOPSI/MOPSI_shortestPath/Code/Instance/";
    string root = "/home/marc-antoinea/Documents/3-Formation/6 - semestre_3/MOPSI - Projet RO/MOPSI_shortestPath/Code/Instance/";
    //string instance = "GrandsLacs/";
    string instance = "NewYork/";
    //string instance = "USA/";
    string sommets = root + instance + "coordsGraphe.co";
    string distances = root + instance + "distancesGraphe.gr";
    cout << "Graphe en cours de lecture..." << endl;
    Graphe G(sommets, distances);
    cout << "\t \t \t... Graphe lu" << endl;


    //ALT Alt(&G);
    //Dijkstra D(&G);
    //ArcFlags AF (&G);

    //Alt.preprocess(5, true);
    //Alt.sauvegarde(root + instance + "output5.pp", instance);
    //Alt.preprocess(root + instance + "output5.pp", true);//
    //cout << Alt.d(1, 1532) << endl;
    //cout << Alt.requete(2032000, 1, true) << endl;//
    //cout << Alt.get_visites() << endl;//
    //Alt.requete(1, 2, true);
    //cout<<"ALT :"<<endl;
    //cout<<Alt.requete(1600, 1606, true)<<endl;
    //cout<<Alt.get_visites()<<endl;
    //cout<<"pi : "<<Alt.pi(1600, 1606, 160652)<<endl;

//    AF.init_cells_kd_tree(10);
//    AF.initFrontieres(true);
//    AF.montrer_repartition();
    //AF.preprocess_kd_tree(2, true);
    //AF.preprocess(root + instance + "outputAF1.pp", true);
    //AF.preprocess_quadrillage(10, true);
    //AF.sauvegarde(root + instance + "outputAFkdTree0.pp", instance, true);
    //cout<< "D :"<<endl;
    //cout<<D.requete(2032000, 1, true)<<endl;
    //cout<<D.requete_bi(88973, 90345, true)<<endl;
    //cout<<D.get_visites()<<endl;
    //cout<<AF.requete(88, 90345, true)<<endl;
    //cout<<AF.requete_bi(88973, 90345, true)<<endl;
    //usleep(3000000);

    //ASG Asg(&G);
    //cout<<Asg.requete_bi(129294, 254505, true)<<endl;

    test_requete(G, 1000, instance, root + instance + "output5.pp",
        root + instance + "outputAF1.pp", root + instance + "output2.csv");

    return 0;
}

