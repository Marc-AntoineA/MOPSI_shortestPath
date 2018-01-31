#include "Sommets.h"
#include <cmath>


Sommet::Sommet(){
    deltaM = new int[1];
    deltaP = new int[1];
    nM=0;
    nP=0;
    //id=0;
    x=0;
    y=0;
}

Sommet::Sommet(int ID){
    //id = ID;
    deltaM = new int[1];
    deltaP = new int[1];
    nM=0;
    nP=0;
}

//Sommet::Sommet(int ID, pair<int, int> COORDS){
//    id = ID;
//    coords = COORDS;
//    deltaM = new int[1];
//    deltaP = new int[1];
//}

Sommet::Sommet(const Sommet &S){
    nP = S.nP;
    nM = S.nM;
//    id = S.id;
    x = S.x;
    y = S.y;
    deltaP = new int (S.nP);
    for (int i=0; i<S.nP; i++)
        deltaP[i] = S.deltaP[i];
    deltaM = new int (S.nM);
    for (int i=0; i<S.nM; i++)
        deltaM[i] = S.deltaM[i];
}

Sommet::~Sommet(){
    delete[] deltaP;
    delete[] deltaM;
}

void Sommet::add_arcP(const int &a){
    if (nP < 1){
        deltaP[nP] = a;
    }
    else{
        int *old = deltaP;
        deltaP = new int [nP+1];
        for (int i=0; i<nP; i++){
            deltaP[i] = old[i];
        }
        deltaP[nP] = a;
        delete [] old;
    }
    nP++;
}

void Sommet::add_arcM(const int &a){
    if (nM < 1){
        deltaM[nM] = a;
    }
    else{
        int *old = deltaM;
        deltaM = new int [nM+1];
        for (int i=0; i<nM; i++){
            deltaM[i] = old[i];
        }
        deltaM[nM] = a;
        delete [] old;
    }
    nM++;
}
double to_grad(int alpha){
    return (alpha*M_PI/180)/(pow(10, 6));
}


const double R = 6378000; // rayon de la terre
long Sommet::distance(Sommet &v){
    if (id==v.id) return 0;
    pair<int, int> vCoords = v.get_coords();// intitude / latitude
    return .99*R*(M_PI/2 - asin(sin(to_grad(y))*sin(to_grad(vCoords.second))
                               + cos(to_grad(x) - to_grad(vCoords.first))
                               * cos(to_grad(y))* cos(to_grad(vCoords.second))));

}

//ostream& operator<<(ostream& str, const Sommet& s){
//    str << s.get_id() << " \t (" << s.get_coords().first << ", " << s.get_coords().second << ")";
//}

bool compareX(Sommet *s1, Sommet *s2){
    return s1->get_coords().first < s2->get_coords().first;
}

bool compareY(Sommet *s1, Sommet *s2){
    return s1->get_coords().second < s2->get_coords().second;
}
