#include "Sommets.h"
#include <cmath>


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
double to_grad(int alpha){
    return(alpha/10^6)*M_PI/180;
}

const int R = 636.6000; // rayon de la terre
long Sommet::distance(Sommet &v){
    pair<int, int> vCoords = v.get_coords();
    return 2*R*asin(sqrt(sin((to_grad(coords.second)-to_grad(vCoords.second))/2)
                         *sin((to_grad(coords.second)-to_grad(vCoords.second))/2)
                           + cos(to_grad(coords.second))*cos(to_grad(vCoords.second))
                           * sin((to_grad(coords.first) - to_grad(vCoords.first))/2)
                           * sin((to_grad(coords.first) - to_grad(vCoords.first))/2)));

}

ostream& operator<<(ostream& str, const Sommet& s){
    str << s.get_id() << " \t (" << s.get_coords().first << ", " << s.get_coords().second << ")";
}
