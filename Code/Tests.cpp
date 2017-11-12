#include "Tests.h"

void test_requete(Graphe &G){
    int nb_tests = 30;
    cout << "test_requete en cours..."<< endl;

    double duration_d = 0;
    double duration_db = 0;
    double duration_asg = 0;

    int db = 0; // nombre de fois que Dijkstra bidirectionnel a juste
    int asg = 0; // nombre de fois que A* geographique a juste
    Dijkstra D(&G);
    Dijkstradouble DD(&G);
    AStarGeographique ASG(&G);

    for(int k = 0; k < nb_tests; k++){
        long s = G.get_randomSommet();
        long t = G.get_randomSommet();

        long distance_ref = D.requete(s, t);
        duration_d += D.get_duration();

        long distance_db = DD.requete(s, t);
        duration_db += DD.get_duration();
        if (distance_db!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur Dijkstra bidirectionnel : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << " | DD : " << distance_db << endl;
        }else{
            db ++;
        }

        long distance_asg = ASG.requete(s, t);
        duration_asg += ASG.get_duration();
        if (distance_asg!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur A* geographique : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| A*G : " << distance_asg << endl;
        }else{
            asg ++;
        }


    }

    cout << " ...Resultats : " << endl;
    cout << " \t Dijstra Bidirectionnel : " << db << "/" << nb_tests << endl;
    cout << " \t A* geographique        : " << asg << "/" << nb_tests << endl;

    cout << " ...Temps totaux (en sec)  : " << endl;
    cout << " \t Dijstra                : " << duration_d << endl;
    cout << " \t Dijstra Bidirectionnel : " << duration_db << endl;
    cout << " \t A* geographique        : " << duration_asg << endl;
}
