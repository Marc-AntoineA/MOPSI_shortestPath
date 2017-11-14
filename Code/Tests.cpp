#include "Tests.h"

void test_requete(Graphe &G){
    int nb_tests = 5;
    cout << "test_requete en cours..."<< endl;

    double duration_d = 0;
    double duration_db = 0;
    double duration_asg = 0;
    double duration_asgbd = 0;

    int db = 0; // nombre de fois que Dijkstra bidirectionnel a juste
    int asg = 0; // nombre de fois que A* geographique a juste
    int asgbd = 0; // nombre de fois que A* geographique bidirectionnel a juste

    Dijkstra D(&G);
    DijkstraBidirectionnel DD(&G);
    ASG Asg(&G);
    ASGBD Asgbd(&G);

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

        long distance_asg = Asg.requete(s, t);
        duration_asg += Asg.get_duration();
        if (distance_asg!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur A* geographique : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| A*G : " << distance_asg << endl;
        }else{
            asg ++;
        }

        long distance_asgbd = Asgbd.requete(s, t);
        duration_asgbd += Asgbd.get_duration();
        if (distance_asgbd != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur A* geographique bidirectionnel : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| A*G BD : " << distance_asgbd << endl;
        }else{
            asgbd ++;
        }
    }

    cout << " ...Resultats : " << endl;
    cout << " \t Dijstra Bidirectionnel : " << db << "/" << nb_tests << endl;
    cout << " \t A* geographique        : " << asg << "/" << nb_tests << endl;
    cout << " \t A* Geo Bidirectionnel  : " << asgbd << "/" << nb_tests << endl;
    cout << endl;

    cout << " ...Temps totaux (en sec)  : " << endl;
    cout << " \t Dijstra                : " << duration_d << endl;
    cout << " \t Dijstra Bidirectionnel : " << duration_db << endl;
    cout << " \t A* geographique        : " << duration_asg << endl;
    cout << " \t A* Geo Bidirectionnel  : " << duration_asgbd << endl;
}
