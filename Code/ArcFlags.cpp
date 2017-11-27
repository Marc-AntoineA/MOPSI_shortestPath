#include "ArcFlags.h"
#include <climits>


void ArcFlags::empileInitFlags(priority_queue<triplet, vector<triplet>, priorite2> &F, map<long, long>& dist, long u){
    vector<long>* deltaM;
    deltaM = ((*V)[u]).get_deltaM();
    for(int j = 0; j < deltaM->size(); j++){
        Arc* a = &((*A)[(*deltaM)[j]]);
        long v = a->get_u();
        if(dist[v] > dist[u] + a->get_poids()){
            dist[v] = dist[u] + a->get_poids();
            F.push(triplet(pp(dist[v],v),a->get_id()));
        }
    }
}

void ArcFlags::initialisationFlags(bool verbose){
    //initialiser Flags[a, C]=0 pour tout arc a, cell C
    //d'abord, on regroupe dans des vector les sommets de chaque cell

    if (verbose){
        cout<<"initialising flags"<<endl;
    }
    vector<vector<long> > Cells(K);
    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        Cells[getCell(it1->first)].push_back(it1->first);
    }

    for (int k=0; k<K;k++){
        map<long, Arc>::iterator it2;
        for(it2 = A->begin(); it2 != A->end(); ++it2){
            Flags[pair<long, int>(it2->first, k)]=0;
        }
    }

    //puis on recherche les frontieres de chaque cell

    vector<vector<long> > frontieres(K);
    vector<long>* deltaM;
    bool b1, b2;
    for(int k=0; k<K;k++){
        for(int i=0; i < Cells[k].size(); i++){
            deltaM = ((*V)[Cells[k][i]]).get_deltaM();
            b1=false;
            b2=false;
            for(int j=0; j < deltaM->size(); j++){
                Arc* a = &((*A)[(*deltaM)[j]]);
                Flags[pair<long, int>(a->get_id(), k)]=1;                        //Ici on initialise les "k-flags" des aretes interieures au cell
                long v = a->get_u();
                if (affectationCells[v]!=k){
                    b1=true;                          //il est sur la frontiere car relie a qq qui n'est pas ds le cell k
                }
                else{
                    b2=true;                          // il est relie a qq du cell k (ceux qui ne le sont pas ne sont pas inclus a la frontiere)
                }
                if (b1 && b2)
                    frontieres[k].push_back(Cells[k][i]);
            }
        }
    }

    //enfin on fait une recherche backward de plus court chemin avec chaque sommet de la frontiere du cell C.
    //chaque fois qu'on depile un sommet de la file de priorite, l'arc via lequel on a ajoute le sommet
    //est sur un plus court chemin vers un sommet de C donc on le "flag" : Flags[a, C]=1
    map<long, int> visite;                           //il est essentiel de retenir quels sommets ont deja ete visites pour ne pas les depiler plusieurs fois
    priority_queue<triplet, vector<triplet>, priorite2> F;
    long u;
    for (int k=0;k<K;k++){
        for (int i=0; i<frontieres[k].size();i++){
            for(it1 = V->begin(); it1 != V->end(); ++it1){
                visite[it1->second.get_id()] = 0;
            }
            init_distanceBackward(LONG_MAX);

            u = frontieres[k][i];
            distanceBackward[u] = 0;
            empileInitFlags(F, distanceBackward, u);

            while(!F.empty()){
                u = F.top().first.second;
                if (visite[u]){
                    F.pop();
                    continue;
                }
                visite[u]=1;
                Flags[pair<long, int>(F.top().second, k)]=1;
                F.pop();
                empileInitFlags(F, distanceBackward, u);
            }

            int compteur = 0;
            for(it1 = V->begin(); it1 != V->end(); ++it1){
                if(!visite[it1->first] && it1->first != frontieres[k][i])
                    compteur+=1;
            }
            if (compteur >0)
                cerr <<"Dans ArcFlags::InitialisationFlags, " << compteur <<" sommets n'ont pas ete visites pour le cell " << k << endl;
        }
        if (verbose)
            cout<<"cell "<< k <<" fini"<<endl;
    }
    if (verbose){
        cout<<"flags initialised"<<endl;
    }
}

void ArcFlags::preprocess_quadrillage(int racineK, bool verbose){            //j'ai fait comme si les coordonnees
    if (verbose){                                                           //des sommets etaient dans le plan (pour simplifier)
        cout<<"debut preprocess avec quadrillage"<<endl;                   //mais j'ai pas teste et je sais pas si
    }                                                                     //c'est une approximation raisonnable
    begin();
    K=racineK*racineK;
    int x_min=INT_MAX;
    int x_max=INT_MIN;
    int y_min=INT_MAX;
    int y_max=INT_MIN;
    pair <int, int> coords;
    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        coords = it1->second.get_coords();
        if (coords.first<x_min)
            x_min=coords.first;
        if (coords.second<y_min)
            y_min=coords.second;
        if (coords.first>x_max)
            x_max=coords.first;
        if (coords.second>y_max)
            y_max=coords.second;
    }

    int division_verticale = (y_max-y_min)/racineK+1;
    int division_horizontale = (x_max-x_min)/racineK+1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        coords=it1->second.get_coords();
        affectationCells[it1->first] = racineK*((coords.first-x_min)/division_horizontale) + (coords.second-y_min)/division_verticale;
    }
    initialisationFlags(verbose);
    end();
    if (verbose){
        cout<<"fin preprocess avec quadrillage"<<endl;
        cout << "Duration : " << get_duration() << endl;
    }
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        if((getCell(it1->first))>=K)
            cerr <<"dans ArcFlags::preprocess_quadrillage, erreur d'affectation de sommet' : " << getCell(it1->first) << " > " << K << endl;
    }
}

void ArcFlags::preprocess_k_means(int k, bool verbose){
    //utiliser l'algo de k-means avec les distances geographiques. chercher dans la std ?
}

void ArcFlags::test_preprocess(){
    vector <long> compteurs (K, 0);
    map<long, Sommet>::iterator it1;
    bool b;           //true s'il existe dans delta+ des aretes avec flag k=1
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        long u = it1->first;
        for (int k=0;k<K;k++){
            vector<long>* deltaP;
            deltaP = ((*V)[u]).get_deltaP();
            bool b = false;         //true s'il existe dans delta+(u) des aretes avec flag k=1
            for(int j = 0; j < deltaP->size() && !b; j++){
                Arc* a = &((*A)[(*deltaP)[j]]);
                if (Flags[pair<long, int> (a->get_id(), k)])
                    b=true;
            }
            if (!b)
                compteurs[k]+=1;
            if (!b && getCell(u)==k)
                cerr<<"Il y a un sommet du cell "<<k<<" qui n'est relie a aucune arete menant a ce cell"<<endl;
        }
    }
    for (int k=0; k<K; k++){
        if (compteurs[k]>0){
            cerr<<"Il y a "<<compteurs[k]<<" sommets n'etant relies a aucune arete menant vers le cell "<<k<<endl;
        }
    }


}

void ArcFlags::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, map<long, long>& dist, long t, long s, bool reverse){
    long u = F.top().second;
    F.pop();
    vector<long>* deltaP;
    if(reverse)
        deltaP = ((*V)[u]).get_deltaM();
    else
        deltaP = ((*V)[u]).get_deltaP();

    for(int k = 0; k < deltaP->size(); k++){
        Arc* a = &((*A)[(*deltaP)[k]]);
        pair<long, int> z (a->get_id(), current_cell);
        if (Flags[z] == 0)           //on ignore les arcs qui ne sont pas sur un plus court chemin menant a un sommet du current cell
            continue;
        if(!reverse){
            long v = a->get_v();
            if(dist[v] > dist[u] + a->get_poids()){
                dist[v] = dist[u] + a->get_poids();
                F.push(pp(dist[v], v));   //potentiel = 0
            }
        }else{
            long v = a->get_u();
            if(dist[v] > dist[u] + a->get_poids()){
                dist[v] = dist[u] + a->get_poids();
                F.push(pp(dist[v], v));
            }
        }
    }
}

void ArcFlags::montrer_repartition(){
    vector<vector<long> > Cells(K);
    map<long, Sommet>::iterator it1;
    vector<int> compteurs(K, 0);
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        compteurs[getCell(it1->first)]+=1;
    }
    for (int k=0;k<K;k++){
        cout<<"Le cell "<<k<<" contient "<<compteurs[k]<<" sommets"<<endl;
    }
}

long ArcFlags::requete(long s, long t, bool verbose){
    begin();
    current_cell=affectationCells[t];                       //on le definit une fois pour toutes
    priority_queue<pp, vector<pp>, priorite> F;

    init_distanceForward(LONG_MAX);
    distanceForward[s] = 0;
    F.push(pp(0, s));
    bool stop = false;
    while(!F.empty() && !stop){
        if(F.top().second == t)
            stop = true;
        add_visite();
        depileEmpile(F, distanceForward, t);
    }
    if (F.empty()){
        cerr<<"F vide apres "<<get_visites()<<" visites"<<endl;
    }
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;

    return distanceForward[t];
}

long ArcFlags::requete_bi(long s, long t, bool verbose){
    begin();
    current_cell=affectationCells[t];
    priority_queue<pp, vector<pp>, priorite> Forward;
    priority_queue<pp, vector<pp>, priorite> Backward;
    init_distanceForward(LONG_MAX/3);
    init_distanceBackward(LONG_MAX/3);

    distanceForward[s] = 0;
    distanceBackward[t] = 0;

    Forward.push(pp(0, s));
    Backward.push(pp(0, t));
    long mu = LONG_MAX;
    while(!Forward.empty() && !Backward.empty()){

        long u = Forward.top().second;
        long v = Backward.top().second;
        if(distanceForward[u] + distanceBackward[v] > mu){
            end();
            if(verbose)
                cout << "Duration : " << get_duration() << endl;
            return mu;
        }
        add_visite();
        depileEmpile(Forward, distanceForward, t);
        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun = u;
        }
        add_visite();
        depileEmpile(Backward, distanceBackward, s, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
            point_commun = v;
        }
    }

    if(Forward.empty()){
        end();
        if(verbose){
            cout << "Forward empty " << endl;
            cout << "Duration : " << get_duration() << endl;

        }
        return distanceForward[t];
    }
    else{
        end();
        if(verbose){
            cout << "Backward empty " << endl;
            cout << "Duration : " << get_duration() << endl;
        }
        return distanceBackward[s];
   }
}
