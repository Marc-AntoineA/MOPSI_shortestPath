#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Chemin{
    vector<long> listeId;  //liste des indices des sommets menant de t a s (c'est a l'envers)
public:
    void push_back(long id);
    string chaine() const;

};
ostream& operator<<(ostream&str,const Chemin &C);
