#include "ArcFlags.h"
#include <fstream>
#include <climits>
#include "fonctionsDiverses.h"
#include <pthread.h>
#include <cstdlib>


void ArcFlags::initFlagsFalse(){
    //on initialise tous les flags a false
    for (int k=0; k<K;k++){
        vector<bool> temp;
        for(long a=0;a<get_A()->size();a++){
            temp.push_back(false);
        }
        Flags.push_back(temp);
    }
}

void ArcFlags::empileInitFlags(priority_queue<triplet, vector<triplet>, priorite2> &F, vector<long> &dist, long u){
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

//on initialise les cells pour quadrillage
void ArcFlags::initCellsQuadrillage(int racineK, bool verbose){
    if (verbose) cout<<"debut initCells"<<endl;
    K=racineK*racineK;
    int x_min=INT_MAX;
    int x_max=INT_MIN;
    int y_min=INT_MAX;
    int y_max=INT_MIN;
    pair <int, int> coords;
    for(long v=1;v<V->size();v++){
        coords = ((*V)[v]).get_coords();
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
    for(long v=1;v<V->size();v++){
        coords=((*V)[v]).get_coords();
        affectationCells[v] = racineK*((coords.first-x_min)/division_horizontale) + (coords.second-y_min)/division_verticale;
    }
    if (verbose) cout<<"fin initCells"<<endl;
}

//on identifie les frontieres
//les points isoles sont places sur la frontiere
void ArcFlags::initFrontieres(bool verbose){
    if (verbose) cout<<"starting initFrontieres"<<endl;
    if (verbose) cout<<"starting assembling cells"<<endl;
    vector<vector<long> > Cells(K);
    for(long v=1;v<V->size();v++){
        Cells[getCell(v)].push_back(v);
    }
    if (verbose) cout<<"assembling cells done"<<endl;
    Flags=vector<vector<bool> >(K);
    for (int k=0; k<K;k++){
        Flags[k]=vector<bool>(A->size());
        for(long a=0;a<A->size();a++){
            Flags[k][a]=false;
        }
    }
    cout<<"ok1"<<endl;
    frontieres = vector<vector<long> >(K);
    vector<long>* deltaM;
    bool b1;
    cout<<"ok2"<<endl;
    for(int k=0; k<K;k++){
        for(int i=0; i < Cells[k].size(); i++){
            deltaM = ((*V)[Cells[k][i]]).get_deltaM();
            b1=false;
            for(int j=0; j < deltaM->size(); j++){
                Arc* a = &((*A)[(*deltaM)[j]]);
                Flags[k][a->get_id()]=true;      //Ici on initialise les "k-flags" des aretes les plus proches
                long v = a->get_u();
                if (affectationCells[v]!=k){
                    b1=true;                          //il est sur la frontiere car relie a qq qui n'est pas ds le cell k
                }
                if (b1)
                    frontieres[k].push_back(Cells[k][i]);
            }
        }
    }
    if (verbose) cout<<"initFrontieres done"<<endl;
}

//initialiser Flags[a, C]=0 pour tout arc a, cell C
void ArcFlags::initialisationFlags(int k1, int k2, bool verbose){
    if (verbose){
        cout<<"initialising flags"<<endl;
    }

    //on fait une recherche backward de plus court chemin avec chaque sommet de la frontiere du cell C.
    //chaque fois qu'on depile un sommet de la file de priorite, l'arc via lequel on a ajoute le sommet
    //est sur un plus court chemin vers un sommet de C donc on le "flag" : Flags[a, C]=1
    //il est indispensable de retenir quels sommets ont deja ete visites pour ne pas les depiler plusieurs fois
             //map<long, int> visite;
    vector<bool> visite = vector<bool>(V->size());
    priority_queue<triplet, vector<triplet>, priorite2> F;
    long u;
    //on ne travaille pas avec distanceBackward car les threads se marcheraient dessus
    vector<long> customDistanceBackward = vector<long>(V->size());
    customDistanceBackward.push_back(0);
    int nbSommetsFrontieresTotal=0;
    for (int k=k1; k<k2;k++){
        nbSommetsFrontieresTotal+=frontieres[k].size();
    }
    int nbSommetsFrontieresTraites=0;
    for (int k=k1;k<k2;k++){
        int avancement=1;
        for (int i=0; i<frontieres[k].size();i++){
            for(long v=1;v<V->size();v++){
                visite[v] = 0;
                customDistanceBackward[v]=LONG_MAX;
            }
            u = frontieres[k][i];
            customDistanceBackward[u] = 0;
            empileInitFlags(F, customDistanceBackward, u);
            while(!F.empty()){
                u = F.top().first.second;
                if (visite[u]){
                    F.pop();
                    continue;
                }
                visite[u]=1;
                Flags[k][F.top().second]=true;
                F.pop();
                empileInitFlags(F, customDistanceBackward, u);
            }

            int compteur = 0;
            for(long v=1; v<V->size();v++){
                if(!visite[v] && v != frontieres[k][i])
                    compteur+=1;
            }
            if (compteur >0)
                cerr <<"Dans ArcFlags::InitialisationFlags, " << compteur <<" sommets n'ont pas ete visites pour un point frontiere du cell " << k << endl;
            if (verbose) cout<<"avancement du thread : "<<100*float(avancement+nbSommetsFrontieresTraites )/nbSommetsFrontieresTotal<<"%. Cell "<< k <<endl;
            avancement++;
        }
        nbSommetsFrontieresTraites+=avancement;
        if (verbose) cout<<"cell "<< k <<" fini"<<endl;
    }
    if (verbose){
        cout<<"flags initialised"<<endl;
    }
}

//https://www.tutorialspoint.com/cplusplus/cpp_multithreading.html
void* preprocessWithThreading(void *threadarg){
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    //my_data->AF->initialisationFlags(my_data->k1, my_data->k2, my_data->verbose);
    for (int i=0;i<my_data->K;i++){
        if (!my_data->doneCells[i]){
            my_data->doneCells[i]=true;
            my_data->AF->initialisationFlags(i, i+1, true);
        }
    }
    cout<<"AF : destroying frame"<<endl;
    pthread_exit(NULL);
}

void ArcFlags::preprocess_quadrillage(int racineK, bool verbose){            //j'ai fait comme si les coordonnees
    if (verbose){                                                           //des sommets etaient dans le plan (pour simplifier)
        cout<<"debut preprocess avec quadrillage"<<endl;                   //mais j'ai pas teste et je sais pas si
    }                                                                     //c'est une approximation raisonnable
    begin();
    initCellsQuadrillage(racineK, verbose);
    initFrontieres(verbose);
    int threadsNb=4;
    pthread_t threads[threadsNb];
    struct thread_data td[threadsNb];
    bool* done = new bool [K];
    for (int j=0;j<K;j++){
        done[j]=false;
    }
    for(int i = 0; i < threadsNb; i++ ){
        cout << "AF : creating thread, " << i << endl;
        td[i].K=K;
        td[i].doneCells=done;
        //td[i].k1 = i*K/threadsNb;
        //td[i].k2 = (i!=threadsNb) ? (i+1)*K/threadsNb : K;
        td[i].verbose=verbose;
        td[i].AF=this;
        pthread_create(&threads[i], NULL, preprocessWithThreading, (void *)&td[i]);
    }
    for(int i = 0; i < threadsNb; i++ )
        pthread_join(threads[i], NULL);
    delete [] done;
    end();
    if (verbose){
        cout<<"fin preprocess avec quadrillage"<<endl;
        cout << "Duration : " << get_duration() << endl;
    }
    for(long v=1;v<V->size();v++){
        if((getCell(v))>=K)
            cerr <<"dans ArcFlags::preprocess_quadrillage, erreur d'affectation de sommet' : " << getCell(v) << " > " << K << endl;
    }
    if (verbose) cout<<"fin preprocess avec quadrillage"<<endl;
}

void ArcFlags::preprocess_k_means(int k, bool verbose){
    //utiliser l'algo de k-means avec les distances geographiques. chercher dans la std ?
}

void ArcFlags::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, vector<long> &dist, long t, long s, bool reverse){
    long u = F.top().second;
    F.pop();
    vector<long>* deltaP;
    if(reverse)
        deltaP = ((*V)[u]).get_deltaM();
    else
        deltaP = ((*V)[u]).get_deltaP();

    for(int k = 0; k < deltaP->size(); k++){
        Arc* a = &((*A)[(*deltaP)[k]]);
        if (!reverse && !Flags[target_cell][a->get_id()])           //on ignore les arcs qui ne sont pas sur un plus court chemin menant a un sommet du cell de t
            continue;
        if (reverse && !Flags[start_cell][G->getReverseArc(a->get_id())])
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
    initFrontieres();
    vector<int> compteurs(K, 0);
    for(long v=1;v<V->size();v++){
        compteurs[getCell(v)]+=1;
    }
    for (int k=0;k<K;k++){
        cout<<"Le cell "<<k<<" contient "<<compteurs[k]<<" sommets, dont "<<frontieres[k].size()<<" sommets frontaliers."<<endl;
    }
}

long ArcFlags::requete(long s, long t, bool verbose){
    begin();
    target_cell=affectationCells[t];
    start_cell=affectationCells[s];
    priority_queue<pp, vector<pp>, priorite> F;

    init_distanceBackward();
    distanceBackward[s] = 0;
    F.push(pp(0, s));
    bool stop = false;
    while(!F.empty() && !stop){
        if(F.top().second == t)
            stop = true;
        add_visite();
        depileEmpile(F, distanceBackward);
    }
    if (F.empty()){
        cerr<<"F vide apres "<<get_visites()<<" visites"<<endl;
    }
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;

    return distanceBackward[t];
}

long ArcFlags::requete_bi(long s, long t, bool verbose){
    begin();
    target_cell=affectationCells[t];
    start_cell=affectationCells[s];
    priority_queue<pp, vector<pp>, priorite> Forward;
    priority_queue<pp, vector<pp>, priorite> Backward;
    init_distanceForward();
    init_distanceBackward();

    distanceForward[s] = 0;
    distanceBackward[t] = 0;

    Forward.push(pp(0, s));
    Backward.push(pp(0, t));
    long mu = LONG_MAX;
    bool stop=false;
    while(!Forward.empty() && !Backward.empty() && !stop){

        long u = Forward.top().second;
        long v = Backward.top().second;
        if(distanceForward[u] + distanceBackward[v] > mu){
             stop=true;
            //return mu;
        }
        add_visite();
        depileEmpile(Forward, distanceForward, t);

        if (distanceBackward[u] + distanceForward[u] < mu){
            mu = distanceBackward[u] + distanceForward[u];
            point_commun = u;
        }
        add_visite();
        depileEmpile(Backward, distanceBackward, s, t, true);
        if(distanceBackward[v] + distanceForward[v] < mu){
            mu = distanceBackward[v] + distanceForward[v];
            point_commun = v;
        }
    }
    BD_finish(Forward, Backward, mu);
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;
    return mu;
}

void ArcFlags::preprocess(string nomInput, bool verbose){
    if(verbose) cout << "Chargement en cours..." << endl;
    begin();
    ifstream input(nomInput.c_str());
    if(input.is_open()){
        string ligne;
        long id; int f; int cellNb;

        size_t k; size_t k2;
        while(getline(input, ligne)){
            if (ligne.size()> 0 && string(ligne, 0, 1) == "k"){
                k = 2;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), K);
                initFlagsFalse();
            }
            if (ligne.size()> 0 && string(ligne, 0, 1) == "a"){
                k = 2;
                k2 = ligne.find(' ', k);

                from_string(string(ligne, k, k2 - k), id);
                k = k2 + 1;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), cellNb);
                affectationCells[id] = cellNb;
            }
            if(ligne.size() > 0 && string(ligne, 0, 1) == "f"){
                k = 2;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), id);
                k = k2 + 1;
                for (int i=0; i<K;i++){
                    k2 = ligne.find(' ', k);
                    from_string(string(ligne, k, k2 - k), f);
                    k = k2 + 1;
                    if (f==0)
                        Flags[i][id] = false;
                    else
                        Flags[i][id] = true;
                }
            }
        }
    }else{
        cerr << "AF::preprocess : " << nomInput
             << "n'a pas pu etre ouvert. Le chargement a echoue" << endl;
    }
    end();
    if(verbose) cout << "\t\t Chargement terminé" << endl;
}

void ArcFlags::sauvegarde(string nomOutput, string instance, bool verbose){
    begin();
    if(verbose){
        cout << "Sauvegarde en cours..." << endl;
    }
    ofstream output(nomOutput.c_str());
    if (output.is_open()){
        output << "c Projet MOPSI - Algorithmes de plus courts chemins "<< endl;
        output << "c Donnees de preprocess pour l'algorithme AF " << endl;
        output << "c Instance de : " << instance << endl;
        output << "c Format pour le nombre de cells : k <K> "<< endl;
        output << "c Format pour les affectations de sommets : a <id> <cell nb>" << endl;
        output << "c Format pour les flags : f <id> <f1> <f2> ... <fK>" << endl;
        output << "k " << K <<endl;

        for(long v=1;v<V->size();v++){
            output << "a " << v << " " << affectationCells[v]<< endl;
        }

        for(long a=0;a<A->size();a++){
            output << "f " << a;
            for (int k=0; k<K;k++){
                if (Flags[k][a])
                    output << " " << 1;
                else
                    output << " " << 0;
            }
            output << endl;
        }
    }else{
        cerr << "AF::sauvegarde | Le fichier : "
             << nomOutput << " n'a pas pu être ouvert et la sauvegarde n'a pas ete effectuee" << endl;
    }
    if(verbose)
        cout << "\t\t Sauvegarde terminée" << endl;
    end();
}


