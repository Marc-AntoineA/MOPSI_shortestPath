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
    vector<long>::const_iterator it = listeId.end();
    for(; it-- != listeId.begin();){
        str+=to_string(*it);
        str+=" ";
    }
    return str;
}

ostream& operator<<(ostream &str,const Chemin &C){
    cout << C.chaine() << endl;
    return str;
}
