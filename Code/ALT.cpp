#include "ALT.h"
#include "Dijkstra.h" // pour le preprocess
#include <cmath>
#include <fstream>
#include <climits>
#include "fonctionsDiverses.h"
#include <ctime>

long ALT::pi(int u, int t, int s){
    if (notInPreprocessMode){
    long M = 0; // borne maximale (ou son estimation)
    int n = L.size();
    for(int ld = 0; ld < n; ld++){
        if (abs(d2(ld, t) - d2(ld, u)) > M){      //meme chose que ds le document "review ..." ?
            M = abs(d2(ld, t) - d2(ld, u));       //
        }
    }
    return M;
    }else{return 0;}
}

void ALT::preprocess(int n, bool verbose){
    begin();
    subDist = vector<long>(n*get_V()->size());
    if(verbose){
        cout << "Pre-process en cours..." << endl;
        cout << "0% : " << endl;
    }
    L.clear();
    int s; int v;
    double prec = 0;
    notInPreprocessMode=false;
    for(int k = 0; k < n; k++){
        s = G->get_randomSommet();
        L.push_back(s);
        Lrecip[s] = k;

        priority_queue<pp, vector<pp>, priorite> F;
        init_distanceForward();
//        for (int i=1; i<get_V()->size();i++){
//            if (distanceForward[i]!=LONG_MAX/3)cerr<<"distances mal initialisees : "<<distanceForward[i]<<endl;
//        }
//        for(int j=0;j<100;j++)if (pi(G->get_randomSommet(), G->get_randomSommet(), G->get_randomSommet())!=0)cerr<<"pi non nul"<<endl;
        distanceForward[s] = 0;

        F.push(pp(0, s));
//        int i=0;
        while(!F.empty()){
            depileEmpile(F, distanceForward, F.top().second, s);
//            if (i%1000000==0) cout<<F.size()<<" "<<F.top().first<<endl;
//            i++;
        }

        // On stocke les distances
        for(int v=1;v<get_V()->size();v++){
//            subDist[pp(s, v)] = distanceForward[v];
            subDist[k+n*v] = distanceForward[v];
        }

//        if(verbose && (double)k / n - prec > 0.05){
//            cout << "="<< flush;
//            prec = k/n;
//        }
        if (verbose)cout<<"landmark "<<k<<" effectue"<<endl;
    }
    notInPreprocessMode=true;
    end();
    if(verbose)
        cout << " 100% " << endl;
        cout << "Duration : " << get_duration() << endl;

}

void ALT::preprocess(string nomInput, bool verbose){
    begin();
    if (verbose)cout<<"ALT : Chargement en cours"<<endl;
    ifstream input(nomInput.c_str());
    if(input.is_open()){
        L.clear();
        subDist.clear();
        string ligne;
        int id1; int id2; long dist;

        size_t k; size_t k2;
        bool needToInitSubDist=true;//
        int idL = 0;
        while(getline(input, ligne)){
            if (ligne.size()> 0 && string(ligne, 0, 1) == "l"){
                k = 2;
                k2 = ligne.find(' ', k);

                from_string(string(ligne, k, k2 - k), id1);
                k = k2 + 1;
                L.push_back(id1);
                Lrecip[id1] = idL;
                idL++;
            }
            if(ligne.size() > 0 && string(ligne, 0, 1) == "d"){
                if (needToInitSubDist){//
                    subDist=vector<long>(L.size()*get_V()->size());
                    needToInitSubDist=false;//
                }//
                k = 2;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), id1);
                k = k2 + 1;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), id2);
                k = k2 + 1;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), dist);
                //subDist[pp(id1, id2)] = dist;
                subDist[Lrecip[id1]+L.size()*id2] = dist;
            }
        }
    }else{
        cerr << "ALT::preprocess : " << nomInput
             << "n'a pas pu etre ouvert. Le chargement a echoue" << endl;
    }
    end();cout<<"taille de L : "<<L.size()<<endl;
    if (verbose)cout<<"\t\t ALT : Chargement termine en "<<get_duration()<<"s"<<endl;
}

void ALT::sauvegarde(string nomOutput, string instance, bool verbose){
    begin();
    if(verbose){
        cout << "Sauvegarde en cours..." << endl;
    }
    ofstream output(nomOutput.c_str());
    if (output.is_open()){
        output << "c Projet MOPSI - Algorithmes de plus courts chemins "<< endl;
        output << "c Donnees de preprocess pour l'algorithme ALT* " << endl;
        output << "c Instance de : " << instance << endl;
        output << "c Format pour les sommets choisis : l <id>" << endl;
        output << "c Format pour les distances : d <id1> <id2> <dist(id1, id2)>" << endl;


        int n = L.size();
        for(int k = 0; k < n; k++){
            output << "l " << L[k] << endl;
        }

        int u;
        for(int k = 0; k < n; k++){
            u = L[k];
            for(int v=1;v<get_V()->size();v++){
                output << "d " << u << " " << v << " " << d2(k, v) << endl;
            }
        }
    }else{
        cerr << "ALT::sauvegarde | Le fichier : "
             << nomOutput << " n'a pas pu être ouvert et la sauvegarde n'a pas ete effectuee" << endl;
    }
    if(verbose)
        cout << "\t\t Sauvegarde terminée" << endl;
    end();
}

