#include "Chemin.h"
#include "fonctionsDiverses.h"
#include <vector>

void Chemin::push_back(long id){
    listeId.push_back(id);
}

string Chemin::chaine() const{
    if (listeId.empty()){
        return "Le chemin est vide";
    }
    string str="";
    vector<long>::const_iterator it = listeId.begin();
    for(; it!= listeId.end(); it++){
        str+=to_string(*it);
        str+=" ";
    }
    return str;
}

ostream& operator<<(ostream &str,const Chemin &C){
    cout << C.chaine() << endl;
    return str;
}

void Chemin::inversion(){
    vector<long> newListe;
    int n=listeId.size();
    for (int i=0;i<n;i++){
        newListe.push_back(listeId[n-1-i]);
    }
    listeId=newListe;
}

Chemin Chemin::join(Chemin C){
    if (listeId[listeId.size()-1]!=C.listeId[0])
        cerr << "jointure de deux chemins ne correspondant pas"<<endl;
    Chemin result;
    for (int i=0;i<C.listeId.size();i++){
        result.push_back(listeId[i]);
    }
    for (int i=1;i<C.listeId.size();i++){
        result.push_back(C.listeId[i]);
    }
    return result;
}

double Chemin::longueur(map<long, Arc> *A, map<long, Sommet>* V) const{
    long uCourant, vCourant;
    double result = 0;
    bool trouve;
    for (int i=0; i<listeId.size()-1;i++){
        uCourant = listeId[i];
        vCourant = listeId[i+1];
        vector<long> *deltaM = ((*V)[vCourant]).get_deltaM();
        trouve=false;
        for(int k = 0; (k < deltaM->size()) && !trouve; k++){
            Arc* a = &((*A)[(*deltaM)[k]]);
            if (uCourant==a->get_u()){
                result+=a->get_poids();
                trouve=true;
            }
        }
        if (!trouve){
            cerr<<"Chemin::longueur : ceci n'est pas un chemin"<<endl;
        }
    }
}
