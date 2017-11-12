#pragma once
#include <sstream>

template<typename T>
bool from_string( const std::string & Str, T & Dest )
{
    // créer un flux à partir de la chaîne donnée
    std::istringstream iss( Str );
    // tenter la conversion vers Dest
    return iss >> Dest != 0;
}

template<typename T>
const std::string to_string(T & Src){
std::stringstream ss;
ss<<Src;
return ss.str();
}


