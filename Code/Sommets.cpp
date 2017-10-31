#include "Sommets.h"


Sommet::Sommet(){
    deltaM = vector<long>();
    deltaP = vector<long>();
}

Sommet::Sommet(long ID){
    id = ID;
    deltaM = vector<long>();
    deltaP = vector<long>();
}

Sommet::Sommet(long ID, pair<int, int> COORDS){
    id = ID;
    coords = COORDS;
    deltaM = vector<long>();
    deltaP = vector<long>();
}


void Sommet::add_arcP(Arc a){
    deltaP.push_back(a.get_id());
}

void Sommet::add_arcM(Arc a){
    deltaM.push_back(a.get_id());
}


ostream& operator<<(ostream& str, const Sommet& s){
    str << s.get_id() << " \t (" << s.get_coords().first << ", " << s.get_coords().second << ")";
}
