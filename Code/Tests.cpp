#include "Tests.h"
#include <climits>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

void test_requete(Graphe &G, int nb_tests, string instance, string pp_alt, string pp_af, string output){
    cout << "test_requete en cours..."<< endl;

    // Durees
    double duration_d = 0;
    double duration_db = 0;
    double duration_asg = 0;
    double duration_asgbd = 0;
    double duration_alt = 0;
    double duration_altbd = 0;
    double duration_af = 0;
    double duration_afbd = 0;

    // Nombre de fois que l'algo a des resultats exacts (ref = Dijkstra)
    int db = 0;
    int asg = 0;
    int asgbd = 0;
    int alt = 0;
    int altbd = 0;
    int af = 0;
    int afbd = 0;

    // Nombre de points visites
    int visites_d = 0;
    int visites_db = 0;
    int visites_asg = 0;
    int visites_asgbd = 0;
    int visites_alt = 0;
    int visites_altbd = 0;
    int visites_af = 0;
    int visites_afbd = 0;

    Dijkstra D(&G);
    ASG Asg(&G);
    ALT Alt(&G);
    Alt.preprocess(pp_alt, true);
    ArcFlags AF (&G);
    AF.preprocess(pp_af, true);
    ofstream write(output.c_str());
    write << "DijkstraRank, Dij_dur, Dbd, Dbd_dur, ASG, ASG_dur, ASGBd, ASGBd_dur,";
    write << "AF_" << pp_af << ",";
    write << "AF_dur_" << pp_af << ",";
    write << "AFBD_" << pp_af << ",";
    write << "AFBD_dur_" << pp_af << ",";
    write << "ALT_" << pp_alt << ",";
    write << "ALT_dur" << pp_alt << ",";
    write << "ALTBD_" << pp_alt << ",";
    write << "ALTBD_dur" << pp_alt << endl;

    vector<pair<int, int> > testSample = G.getTestSample(nb_tests, true);

    for(int k = 0; k < nb_tests; k++){
//        int s = G.get_randomSommet();
//        int t = G.get_randomSommet();
        int s = testSample[k].first;
        int t = testSample[k].second;

        long distance_ref = D.requete(s, t);
        duration_d += D.get_duration();
        visites_d += D.get_visites();
        write << D.get_visites() << "," << D.get_duration();

        long distance_db = D.requete_bi(s, t);
        duration_db += D.get_duration();
        visites_db += D.get_visites();
        if (distance_db!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur Dijkstra bidirectionnel : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << " | DD : " << distance_db << endl;
        }else{
            write << "," << D.get_visites() << "," << D.get_duration();
            db ++;
        }

        long distance_asg = Asg.requete(s, t);
        duration_asg += Asg.get_duration();
        visites_asg += Asg.get_visites();
        if (distance_asg!= distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur A* geographique : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| A*G : " << distance_asg << endl;
        }else{
            write << "," << Asg.get_visites() << "," << Asg.get_duration();
            asg ++;
        }

        long distance_asgbd = Asg.requete_bi(s, t);
        duration_asgbd += Asg.get_duration();
        visites_asgbd += Asg.get_visites();
        if (distance_asgbd != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur A* geographique bidirectionnel : " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| A*G BD : " << distance_asgbd << endl;
        }else{
            write << "," << Asg.get_visites() << "," << Asg.get_duration();
            asgbd ++;
        }

        long distance_af = AF.requete(s, t);
        duration_af += AF.get_duration();
        visites_af += AF.get_visites();
        if (distance_af != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur AF: " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| AF : " << distance_af << endl;
            cerr << "cell de la requete : " << AF.getCell(t) << endl;
        }else{
            write << "," << AF.get_visites() << "," << AF.get_duration();
            af++;
        }

        long distance_afbd = AF.requete_bi(s, t);
        duration_afbd += AF.get_duration();
        visites_afbd += AF.get_visites();
        if (distance_afbd != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur AF Bidirectionnel: " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| AF BD : " << distance_afbd << endl;
            cerr << "cells de la requete : " << AF.getCell(s) << " " << AF.getCell(t) << endl;
        }else{
            write << "," << AF.get_visites() << "," << AF.get_duration();
            afbd++;
        }

        long distance_alt = Alt.requete(s, t);
        duration_alt += Alt.get_duration();
        visites_alt += Alt.get_visites();
        if (distance_alt != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur ALT: " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| Alt : " << distance_alt << endl;
        }else{
            write << "," << Alt.get_visites() << "," << Alt.get_duration();
            alt++;
        }

        long distance_altbd = distance_ref;//Alt.requete_bi(s, t);
        duration_altbd += Alt.get_duration();
        visites_altbd += Alt.get_visites();
        if (distance_altbd != distance_ref && distance_ref < LONG_MAX/3){
            cerr << " Erreur ALT: " << s << "-->" << t << endl;
            cerr << "\t Ref : " << distance_ref << "| Alt : " << distance_altbd << endl;
        }else{
            write << "," << Alt.get_visites() << "," << Alt.get_duration();
            altbd++;
        }
        write << endl;
        cout << k << endl;
    }

    cout << " ...Resultats : " << endl;
    cout << " \t Dijkstra Bidirectionnel: " << db << "/" << nb_tests << endl;
    cout << " \t A* geographique        : " << asg << "/" << nb_tests << endl;
    cout << " \t A* Geo Bidirectionnel  : " << asgbd << "/" << nb_tests << endl;
    cout << " \t ALT                    : " << alt << "/" << nb_tests << endl;
    cout << " \t ALT Bidirectionnel     : " << altbd << "/" << nb_tests << endl;
    cout << " \t AF                     : " << af << "/" << nb_tests << endl;
    cout << " \t AF Bidirectionnel      : " << afbd << "/" << nb_tests << endl;
    cout << endl;

    cout << " ...Temps moyens (en sec)  : " << endl;
    cout << " \t Dijkstra               : " << duration_d /(double) nb_tests<< endl;
    cout << " \t Dijkstra Bidirectionnel: " << duration_db/(double) nb_tests << endl;
    cout << " \t A* geographique        : " << duration_asg/(double) nb_tests << endl;
    cout << " \t A* Geo Bidirectionnel  : " << duration_asgbd/(double) nb_tests << endl;
    cout << " \t ALT                    : " << duration_alt/(double) nb_tests << endl;
    cout << " \t ALT Birectionnel       : " << duration_altbd/(double) nb_tests << endl;
    cout << " \t AF                     : " << duration_af/(double) nb_tests << endl;
    cout << " \t AF Bidirectionnel      : " << duration_afbd/(double) nb_tests << endl;
    cout << endl;

    cout << " ...Espace visite moyen (en nombre de noeuds): " << endl;
    cout << " \t Dijkstra               : " << visites_d /(double) nb_tests<< endl;
    cout << " \t Dijkstra Bidirectionnel: " << visites_db/(double) nb_tests << endl;
    cout << " \t A* geographique        : " << visites_asg/(double) nb_tests << endl;
    cout << " \t A* Geo Bidirectionnel  : " << visites_asgbd/(double) nb_tests << endl;
    cout << " \t ALT                    : " << visites_alt/(double) nb_tests << endl;
    cout << " \t ALT Bidirectionnel     : " << visites_altbd/(double) nb_tests << endl;
    cout << " \t AF                     : " << visites_af/(double) nb_tests << endl;
    cout << " \t AF Bidirectionnel      : " << visites_afbd/(double) nb_tests << endl;
    cout << endl;

}
