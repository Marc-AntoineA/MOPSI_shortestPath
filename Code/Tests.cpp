#include "Tests.h"
#include <climits>

void test_requete(Graphe &G, int nb_tests, string pp_alt, string pp_af){
    cout << "test_requete en cours..."<< endl;

    // Durees
    double duration_d = 0;
    double duration_db = 0;
    double duration_asg = 0;
    double duration_asgbd = 0;
    double duration_alt = 0;
    double duration_af = 0;

    // Nombre de fois que l'algo a des resultats exacts (ref = Dijkstra)
    int db = 0;
    int asg = 0;
    int asgbd = 0;
    int alt = 0;
    int af = 0;

    // Nombre de points visites
    long visites_d = 0;
    long visites_db = 0;
    long visites_asg = 0;
    long visites_asgbd = 0;
    long visites_alt = 0;
    long visites_af = 0;

    Dijkstra D(&G);
    ASG Asg(&G);
    ALT Alt(&G);
    Alt.preprocess(pp_alt);
    ArcFlags AF (&G);
    AF.preprocess(pp_af);

    for(int k = 0; k < nb_tests; k++){
        long s = G.get_randomSommet();
        long t = G.get_randomSommet();

        long distance_ref = D.requete(s, t);
        duration_d += D.get_duration();
        visites_d += D.get_visites();

        long distance_db = D.requete_bi(s, t);
        duration_db += D.get_duration();
        visites_db += D.get_visites();
        if (distance_db!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur Dijkstra bidirectionnel : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << " | DD : " << distance_db << endl;
        }else{
            db ++;
        }

        long distance_asg = Asg.requete(s, t);
        duration_asg += Asg.get_duration();
        visites_asg += Asg.get_visites();
        if (distance_asg!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur A* geographique : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| A*G : " << distance_asg << endl;
        }else{
            asg ++;
        }

        long distance_asgbd = Asg.requete_bi(s, t);
        duration_asgbd += Asg.get_duration();
        visites_asgbd += Asg.get_visites();
        if (distance_asgbd != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur A* geographique bidirectionnel : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| A*G BD : " << distance_asgbd << endl;
        }else{
            asgbd ++;
        }

        long distance_alt = Alt.requete(s, t);
        duration_alt += Alt.get_duration();
        visites_alt += Alt.get_visites();
        if (distance_alt != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur ALT: " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| Alt : " << distance_alt << endl;
        }else{
            alt++;
        }

        long distance_af = AF.requete(s, t);
        duration_af += AF.get_duration();
        visites_af += AF.get_visites();
        if (distance_af != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur AF: " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| AF : " << distance_af << endl;
            cerr << "cell de la requete : " << AF.getCell(t) << endl;
        }else{
            af++;
        }
    }

    cout << " ...Resultats : " << endl;
    cout << " \t Dijkstra Bidirectionnel: " << db << "/" << nb_tests << endl;
    cout << " \t A* geographique        : " << asg << "/" << nb_tests << endl;
    cout << " \t A* Geo Bidirectionnel  : " << asgbd << "/" << nb_tests << endl;
    cout << " \t ALT                    : " << alt << "/" << nb_tests << endl;
    cout << " \t AF                     : " << af << "/" << nb_tests << endl;
    cout << endl;

    cout << " ...Temps moyens (en sec)  : " << endl;
    cout << " \t Dijkstra               : " << duration_d /(double) nb_tests<< endl;
    cout << " \t Dijkstra Bidirectionnel: " << duration_db/(double) nb_tests << endl;
    cout << " \t A* geographique        : " << duration_asg/(double) nb_tests << endl;
    cout << " \t A* Geo Bidirectionnel  : " << duration_asgbd/(double) nb_tests << endl;
    cout << " \t ALT                    : " << duration_alt/(double) nb_tests << endl;
    cout << " \t AF                     : " << duration_af/(double) nb_tests << endl;
    cout << endl;

    cout << " ...Espace visite moyen (en nombre de noeuds): " << endl;
    cout << " \t Dijkstra               : " << visites_d /(double) nb_tests<< endl;
    cout << " \t Dijkstra Bidirectionnel: " << visites_db/(double) nb_tests << endl;
    cout << " \t A* geographique        : " << visites_asg/(double) nb_tests << endl;
    cout << " \t A* Geo Bidirectionnel  : " << visites_asgbd/(double) nb_tests << endl;
    cout << " \t ALT                    : " << visites_alt/(double) nb_tests << endl;
    cout << " \t AF                     : " << visites_af/(double) nb_tests << endl;
    cout << endl;

}

