#include "Arcs.h"

Arc::Arc(){
}

Arc::Arc(long identifiant, long src, long dest, double poids){
    id = identifiant;
    u = src;
    v =  dest;
    w = poids;
}



ostream& operator<<(ostream& str, const Arc& a){
    str << a.get_id() << " : \t" << a.get_u() << " ---> " << a.get_v() << "(" << a.get_poids() << ")";
}
