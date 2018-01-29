#include "Arcs.h"

Arc::Arc(){
}

Arc::Arc(int identifiant, int src, int dest, int poids){
    //id = identifiant;
    u = src;
    v =  dest;
    w = poids;
}



//ostream& operator<<(ostream& str, const Arc& a){
//    str << a.get_id() << " : \t" << a.get_u() << " ---> " << a.get_v() << "(" << a.get_poids() << ")";
//}
