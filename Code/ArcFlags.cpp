#include "ArcFlags.h"
#include <climits>  //Pour LONG_MAX

void ArcFlags::initialisation_quadrillage(int racineK){
    K=racineK*racineK;
    int x_min=INT_MIN;
    int x_max=INT_MAX;
    int y_min=INT_MIN;
    int y_max=INT_MAX;
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
    int division_verticale = (y_max-y_min)/racineK;
    int division_horizontale = (x_max-x_min)/racineK;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        coords=it1->second.get_coords();
        affectationCells[it1->first] = racineK*coords.first/division_horizontale + coords.first/division_verticale;
    }
}

void ArcFlags::initialisation_k_means(int k){
    //utiliser l'algo de k-means avec les distances geographiques. chercher dans la std ?
}

void ArcFlags::initialisationFlags(){
    //initialiser Flags[a, C]=0 pour tout arc a, cell C
    //d'abord, on regroupe dans des vector les sommets de chaque cell
    vector<vector<long> > Cells(K);
    map<long, Sommet>::iterator it1;
    for(it1 = V->begin(); it1 != V->end(); ++it1){
        Cells[getCell(it1->first)].push_back(it1->first);
    }
    //puis on recherche les frontieres de chaque cell
    vector<vector<long> > frontieres(K);
    vector<long>* deltaM;
    bool stop;
    for(int k=0; k<K;k++){
        for(int i=0; i < Cells[k].size(); i++){
            deltaM = ((*V)[Cells[k][i]]).get_deltaM();
            stop=false;
            for(int j=0; !stop && (j < deltaM->size()); j++){
                Arc* a = &((*A)[(*deltaM)[j]]);
                long v = a->get_u();
                if (affectationCells[v]!=k){
                    frontieres[k].push_back(Cells[k][i]);
                    stop=true;
                }
            }
        }
    }
    //puis on fait une recherche backward de plus court chemin avec les sommets frontieres du cell C et on "flag" les arcs par lesquels on passe : Flags[a, C]=1
    priority_queue<pp, vector<pp>, priorite> F;
    for (int k=0; k<K;k++){
        init_distanceForward(LONG_MAX);
        for (int i=0; i<frontieres[k].size();i++){
            F.push(pp(0,frontieres[k][i]));
            distanceBackward[frontieres[k][i]] = 0;
        }
        while(!F.empty()){
            long u = F.top().second;
            F.pop();
            vector<long>* deltaM;
            for(int j = 0; j < deltaM->size(); j++){
                Arc* a = &((*A)[(*deltaM)[j]]);
                long v = a->get_u();
                if(distanceBackward[v] > distanceBackward[u] + a->get_poids()){
                    Flags[pair<long, int>(a->get_id(), k)]=1;                             //sauf qu'ici, le bug qui fait que des sommets sont depiles
                    distanceBackward[v] = distanceBackward[u] + a->get_poids();           //deux fois de F est vraiment problematique
                    F.push(pp(distanceBackward[v], v));
                }
            }
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

long ArcFlags::requete(long s, long t, bool verbose){
    current_cell=affectationCells[t];                       //on le definit une fois pour toutes
    begin();
    priority_queue<pp, vector<pp>, priorite> F;

    init_distanceForward(LONG_MAX);
    distanceForward[s] = 0;
    F.push(pp(0, s));
    bool stop = false;
    while(!F.empty() && !stop){
        if(F.top().second == t)
            stop = true;
        depileEmpile(F, distanceForward, t);
    }
    end();
    if(verbose)
        cout << "Duration : " << get_duration() << endl;

    return distanceForward[t];
}
