cmake_minimum_required(VERSION 2.6)

project(ProjetMOPSI)
add_executable(ProjetMOPSI main.cpp
                            Sommets.h Sommets.cpp
                            Arcs.h  Arcs.cpp
                            Graphe.h Graphe.cpp
                            Algorithme.h Algorithme.cpp
                            fonctionsDiverses.h fonctionsDiverses.cpp
                            Dijkstra.h Dijkstra.cpp
                            Chemin.h Chemin.cpp
                            Tests.h Tests.cpp
                            AStar.h AStar.cpp
                            ALT.h ALT.cpp)

