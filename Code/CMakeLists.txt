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
                            ALT.h ALT.cpp
                            ArcFlags.h ArcFlags.cpp)
#https://stackoverflow.com/questions/1620918/cmake-and-libpthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ProjetMOPSI Threads::Threads)
