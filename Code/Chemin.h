#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Chemin{
    vector<long> listeId;  //liste des indices des sommets formant le chemin
public:
    void push_back(long id);
    void inversion();   //inverser le sens du chemin
    Chemin join(Chemin C);
    string chaine() const;

};
ostream& operator<<(ostream&str,const Chemin &C);
