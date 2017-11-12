#include "Tests.h"

void test_requete(Graphe &G){
    int nb_tests = 30;
    cout << "test_requete en cours..."<< endl;

    int db = 0; // nombre de fois que Dijkstra bidirectionnel a juste
    Dijkstradouble DD(&G);

    for(int k = 0; k < nb_tests; k++){
        long s = G.get_randomSommet();
        long t = G.get_randomSommet();
        Dijkstra D(&G);
        long distance_ref = D.requete(s, t);

        long distance_db = DD.requete(s, t);
        if (distance_db!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur Dijkstra bidirectionnel : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << " DD : " << distance_db << endl;
        }else{
            db ++;
        }
    }

    if(db == nb_tests){
        cout << " \t \t ...test_requete Ok " << endl;
    }
}
