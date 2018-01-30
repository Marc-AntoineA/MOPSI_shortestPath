#include "ArcFlags.h"
#include <fstream>
#include <climits>
#include "fonctionsDiverses.h"
#include <pthread.h>
#include <cstdlib>
#include <algorithm>


void ArcFlags::initFlagsFalse(){
    //on initialise tous les flags a false
    vector<bool> temp(get_A()->size(), false);
    Flags = vector<vector<bool> > (K, temp);
}

void ArcFlags::empileInitFlags(priority_queue<triplet, vector<triplet>, priorite2> &F, vector<long> &dist, int u){
    int* deltaM; long dp;
    deltaM = ((*V)[u]).get_deltaM();
    int size = ((*V)[u]).get_nM();
    for(int j = 0; j < size; j++){
        Arc* a = &((*A)[deltaM[j]]);
        int v = a->get_u();
        dp = dist[u] + a->get_poids();
        if(dist[v] > dp){
            dist[v] = dp;
            F.push(triplet(pp(dist[v],v),deltaM[j]));
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
    for(int v=1;v<V->size();v++){
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
    for(int v=1;v<V->size();v++){
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
    vector<vector<int> > Cells(K);
    for(int v=1;v<V->size();v++){
        Cells[getCell(v)].push_back(v);
    }
    if (verbose) cout<<"assembling cells done"<<endl;
    Flags=vector<vector<bool> >(K);
    for (int k=0; k<K;k++){
        Flags[k]=vector<bool>(A->size());
        for(int a=0;a<A->size();a++){
            Flags[k][a]=false;
        }
    }
    frontieres = vector<vector<int> >(K);
    int* deltaM;
    int size;
    bool b1;
    for(int k=0; k<K;k++){
        for(int i=0; i < Cells[k].size(); i++){
            deltaM = ((*V)[Cells[k][i]]).get_deltaM();
            size = ((*V)[Cells[k][i]]).get_nM();
            b1=false;
            for(int j=0; j < size; j++){
                Arc* a = &((*A)[deltaM[j]]);
                Flags[k][deltaM[j]]=true;      //Ici on initialise les "k-flags" des aretes les plus proches
                int v = a->get_u();
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
             //map<int, int> visite;
    vector<bool> visite = vector<bool>(V->size());
    priority_queue<triplet, vector<triplet>, priorite2> F;
    int u;
    //on ne travaille pas avec distanceBackward car les threads se marcheraient dessus
    vector<long> customDistanceBackward = vector<long>(V->size()+1);
    int nbSommetsFrontieresTotal=0;
    for (int k=k1; k<k2;k++){
        nbSommetsFrontieresTotal+=frontieres[k].size();
    }

    int nbSommetsFrontieresTraites=0;
    for (int k=k1;k<k2;k++){
        int avancement=1;
        for (int i=0; i<frontieres[k].size();i++){
            for(int v=1;v<V->size();v++){
                visite[v] = false;
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
                visite[u]=true;
                Flags[k][F.top().second]=true;
                F.pop();
                empileInitFlags(F, customDistanceBackward, u);
            }

            int compteur = 0;
            for(int v=1; v<V->size();v++){
                if(!visite[v] && v != frontieres[k][i])
                    compteur+=1;
            }
            if (compteur >0)
                cerr <<"Dans ArcFlags::InitialisationFlags, " << compteur <<" sommets n'ont pas ete visites pour un point frontiere du cell " << k << endl;
            if (verbose && i%10==0) cout<<"avancement du thread : "<<100*float(avancement+nbSommetsFrontieresTraites )/nbSommetsFrontieresTotal<<"%. Cell "<< k <<endl;
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
    string root = "/home/dekeyser/Documents/MOPSI/MOPSI_shortestPath/Code/Instance/";
    string instance = "GrandsLacs/";
    my_data = (struct thread_data *) threadarg;
    for (int i=0;i<my_data->K;i++){
        if (!my_data->doneCells[i]){
            my_data->doneCells[i]=true;
            my_data->AF->initialisationFlags(i, i+1, true);
            my_data->AF->sauvegarde(root + instance + "outputAFkdTree1.pp", instance, true);
        }
    }
    cout<<"AF : destroying frame"<<endl;
    pthread_exit(NULL);
}

//partie commune aux deux preprocess
void ArcFlags::preprocess_base(bool verbose){
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
        td[i].verbose=verbose;
        td[i].AF=this;
        pthread_create(&threads[i], NULL, preprocessWithThreading, (void *)&td[i]);
    }
    for(int i = 0; i < threadsNb; i++ )
        pthread_join(threads[i], NULL);
    delete [] done;
    for(int v=1;v<V->size();v++){
        if((getCell(v))>=K)
            cerr <<"dans ArcFlags::preprocess_base, erreur d'affectation de sommet' : " << getCell(v) << " > " << K << endl;
    }
}

void ArcFlags::preprocess_quadrillage(int racineK, bool verbose){            //j'ai fait comme si les coordonnees
    if (verbose){                                                           //des sommets etaient dans le plan (pour simplifier)
        cout<<"debut preprocess avec quadrillage"<<endl;                   //mais j'ai pas teste et je sais pas si
    }                                                                     //c'est une approximation raisonnable
    begin();
    initCellsQuadrillage(racineK, verbose);
    preprocess_base(verbose);
    end();
    if (verbose){
        cout<<"fin preprocess avec quadrillage"<<endl;
        cout << "Duration : " << get_duration() << endl;
    }
}

void ArcFlags::init_cells_kd_tree(int k, bool verbose){
    if (verbose) cout<<"creating regions"<<endl;
    K=k;
    vector<Node> listRegions;
    Node all;
    int dir = 0;
    for (int i=1; i<get_V()->size(); i++){
        all.add(&((*get_V())[i]));
    }
    listRegions.push_back(all);

    int p=0;
    int v=0;

    while (listRegions.size() < k){
        vector<Node> newListRegions;
        for (int i=0; i<listRegions.size();i++){
            Node myNode = listRegions[i];
            if (listRegions.size()+i >= k){
                newListRegions.push_back(myNode);
                continue;
            }
            Node newNode1, newNode2;
            if (dir){
                //cout<<"sort /x"<<endl;
                std::sort(myNode.getEls()->begin(), myNode.getEls()->end(), compareX);
                //cout<<"done sort /x"<<endl;
            }
            else{
                //cout<<"sort /y"<<endl;
                sort(myNode.getEls()->begin(), myNode.getEls()->end(), compareY);
                //cout<<"done sort /y"<<endl;
            }
            for (int j=0; j<myNode.getEls()->size()/2; j++){
                newNode1.add((*myNode.getEls())[j]);
            }
            newListRegions.push_back(newNode1);
            for (int j=myNode.getEls()->size()/2; j<myNode.getEls()->size(); j++){
                newNode2.add((*myNode.getEls())[j]);
            }
            newListRegions.push_back(newNode2);
        }
        dir = 1 - dir;
        listRegions=newListRegions;
    }
    //maintenant qu'on a cree les kd_trees, on peut assigner correctement
    //les sommets aux regions
    p = 0;
    v = 0;
    for (int i=0; i<listRegions.size();i++){
        for (int j=0; j<listRegions[i].getEls()->size(); j++){
            affectationCells[(*listRegions[i].getEls())[j]->get_id()]=p;
            v++;
        }
        p++;
    }
    if (p!=k) cerr<<"ArcFlags::init_cells_kd_trees : nombre final de regions errone : "<<p<<" "<<k<<endl;
    if (v+1!=get_V()->size()) cerr<<"ArcFlags::preprocess_kd_trees : nombre final de sommets errone : "<<v+1<<" "<<get_V()->size()<<endl;
    if (verbose) cout<<"done creating regions"<<endl;
}

void ArcFlags::preprocess_kd_tree(int k, bool verbose){            //j'ai fait comme si les coordonnees
    if (verbose){                                                           //des sommets etaient dans le plan (pour simplifier)
        cout<<"debut preprocess avec kd_tree"<<endl;                   //mais j'ai pas teste et je sais pas si
    }                                                                     //c'est une approximation raisonnable
    begin();
    init_cells_kd_tree(k, verbose);
    preprocess_base(verbose);
    end();
    if (verbose){
        cout<<"fin preprocess avec kd_tree  "<<endl;
        cout << "Duration : " << get_duration() << endl;
    }
}

void ArcFlags::depileEmpile(priority_queue<pp, vector<pp>, priorite>& F, vector<long> &dist, int t, int s, bool reverse){
    int u = F.top().second; long dp;
    F.pop();
    int* deltaP;
    int size;
    if(reverse){
        deltaP = ((*V)[u]).get_deltaM();
        size = ((*V)[u]).get_nM();
    }
    else{
        deltaP = ((*V)[u]).get_deltaP();
        size = ((*V)[u]).get_nP();
    }
    int v;
    Arc* a;
    for(int k = 0; k < size; k++){
        a = &((*A)[deltaP[k]]);
        if (!reverse && !Flags[target_cell][deltaP[k]])           //on ignore les arcs qui ne sont pas sur un plus court chemin menant a un sommet du cell de t
            continue;
        if (reverse && !Flags[start_cell][G->getReverseArc(deltaP[k])])
            continue;
        if(!reverse){
            v = a->get_v();
        }else{
            v = a->get_u();
        }
        dp = dist[u] + a->get_poids();
        if(dist[v] > dp){
            dist[v] = dp;
            F.push(pp(dist[v], v));

        }
    }
}

void ArcFlags::montrer_repartition(){
    initFrontieres();
    vector<int> compteurs(K, 0);
    for(int v=1;v<V->size();v++){
        compteurs[getCell(v)]+=1;
    }
    for (int k=0;k<K;k++){
        cout<<"Le cell "<<k<<" contient "<<compteurs[k]<<" sommets, dont "<<frontieres[k].size()<<" sommets frontaliers."<<endl;
    }
}

long ArcFlags::requete(int s, int t, bool verbose){
    begin();
    target_cell=affectationCells[t];
    start_cell=affectationCells[s];
    priority_queue<pp, vector<pp>, priorite> F;

    init_distanceForward();
    distanceForward[s] = 0;
    F.push(pp(0, s));
    bool stop = false;
    while(!F.empty() && !stop){
        if(F.top().second == t)
            stop = true;
        add_visite();
        depileEmpile(F, distanceForward);
    }
    if (F.empty()){
        cerr<<"AF : F vide apres "<<get_visites()<<" visites"<<endl;
    }
    end();
    if(verbose)
        cout << "AF : Duration : " << get_duration() << endl;

    return distanceForward[t];
}

void ArcFlags::BD_finish(priority_queue<pp, vector<pp>, priorite> Forward, priority_queue<pp, vector<pp>, priorite> Backward, long& mu){
    int u, v;
    long dp;
    int *deltaP, *deltaM;
    int size;
    Arc* a;
    while(!Forward.empty()){
        u = Forward.top().second;
        Forward.pop();
        deltaP = ((*V)[u]).get_deltaP();
        size = ((*V)[u]).get_nP();
        for(int k = 0; k < size; k++){
            if (!Flags[target_cell][deltaP[k]])
                continue;
            a = &((*A)[deltaP[k]]);
            dp = distanceBackward[a->get_v()]+distanceForward[u]+a->get_poids();
            if (dp < mu){
                mu = dp;
                point_commun = u;
                distanceBackward[u] = distanceBackward[a->get_v()]+a->get_poids();
            }
        }
    }
    while(!Backward.empty()){
        v = Backward.top().second;
        Backward.pop();
        deltaM = ((*V)[v]).get_deltaM();
        size = ((*V)[v]).get_nM();
        for(int k = 0; k < size; k++){
            if (!Flags[start_cell][G->getReverseArc(deltaP[k])])
                continue;
            a = &((*A)[deltaM[k]]);
            dp = distanceForward[a->get_u()]+distanceBackward[v]+a->get_poids();
            if (dp < mu){
                mu = dp;
                point_commun = v;
                distanceForward[u] = distanceForward[a->get_u()]+a->get_poids();
            }
        }
    }
}

long ArcFlags::requete_bi(int s, int t, bool verbose){
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

        int u = Forward.top().second;
        int v = Backward.top().second;
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
        cout << "AFBD : Duration : " << get_duration() << endl;
    return mu;
}

void ArcFlags::preprocess(string nomInput, bool verbose){
    if(verbose) cout << "AF : Chargement en cours..." << endl;
    begin();
    ifstream input(nomInput.c_str());
    if(input.is_open()){
        string ligne;
        int id; int f; int cellNb;

        size_t k; size_t k2;
        while(getline(input, ligne)){
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
            else if (ligne.size()> 0 && string(ligne, 0, 1) == "a"){
                k = 2;
                k2 = ligne.find(' ', k);

                from_string(string(ligne, k, k2 - k), id);
                k = k2 + 1;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), cellNb);
                affectationCells[id] = cellNb;
            }
            else if (ligne.size()> 0 && string(ligne, 0, 1) == "k"){
                k = 2;
                k2 = ligne.find(' ', k);
                from_string(string(ligne, k, k2 - k), K);
                initFlagsFalse();
            }
        }
    }else{
        cerr << "AF::preprocess : " << nomInput
             << "n'a pas pu etre ouvert. Le chargement a echoue" << endl;
    }
    end();
    if(verbose) cout << "\t\t AF : Chargement termine en "<<get_duration()<<"s" << endl;
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

        for(int v=1;v<V->size();v++){
            output << "a " << v << " " << affectationCells[v]<< endl;
        }

        for(int a=0;a<A->size();a++){
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


