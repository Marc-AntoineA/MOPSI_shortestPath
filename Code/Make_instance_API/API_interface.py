# -*- coding: utf-8 -*-
"""
Created on Wed Nov 08 17:22:07 2017

@author: Marc-Antoine
"""
import json
import pandas as pd
import overpy
from math import pi, asin, sin, cos
#-------------------------------------------------------------
#0/ Parametres

#pour trouver la bounding_box : http://boundingbox.klokantech.com/
bounding_box = (45.481318, 5.83374,45.636127, 6.060333) # Chambéry
#inverser les coords comme pour Chy
#bounding_box = (-9.5,35.9,46.8,60.0) #Europe
#bounding_box = (-5.1,42.13,8.17,51.54) # France
#bounding_box = (4.4385,45.0347,6.1414,45.9664) # Rhone alpes
#bounding_box = (5.9559,47.5135,6.7017,47.8084) # test
file_description = "Chambery_small"
output_coords = "coordsGraphe.co"
output_distance = "distancesGraphe.gr"
roads = ["motorway", "trunk","primary", "secondary", "tertiary", "unclassified", "residential"]
level = 1 # on execute la requete sur "motorway|motorway_link|trunk|trunk_link"

def requete_road():
    str = "'a"
    for i in range(level):
        str += "|{}|{}_link".format(roads[i], roads[i])
    return str + "'"


#-------------------------------------------------------------
#1/ On télécharge les routes comprises dans notre bounding-box

# Si tu veux tester Guillaume
api = overpy.Overpass(url="http://overpass-api.de/api/interpreter")
#api = overpy.Overpass(url="http://127.0.0.1/api/interpreter")
query = "[out:json];way['highway'~{}]{};node(w);out;way['highway'~{}]{};out;".format(requete_road(), bounding_box, requete_road(), bounding_box)
print(query)

result = api.query(query)
print(len(result.ways))
# Forme du resultat
# * obtenir les routes result.ways (liste)
# * obtenir une route : result.ways[n]
# * obtenir le type de la route : result.ways[n].tags['highway'] (result.ways[n].tags est un dictonnaire)
# * obtenir l'identifiant : result.ways[n].id
# * obtenir les noeuds (il les télécharge comme un grand :p ) : result.ways[n].get_nodes(resolve_missing=True)
len(result.nodes)
len(result.ways)
#---------------------------------------------------------------
#2/ On repere tout les noeuds qui apparaissent plusieurs fois
counter = {}

for way in result.ways:
    way.get_nodes(resolve_missing=True) # pour télécharger les noeuds
    for node in way.nodes:
        if node.id in counter:
            counter[node.id] += 1
        else:
            counter[node.id] = 1

#--------------------------------------------------------------
#3/ On calcule la distance entre deux noeuds

def deg_to_rad(alpha): # deg to radian
    return alpha*pi/180

R = 6378000 # rayon de la Terre en m
"""
    Calcul de la distance entre deux points sur la Terre
"""
def distance(node_1, node_2):
    [lat1, lat2, lon1, lon2] = map(lambda x:deg_to_rad(float(x)), [node_1.lat, node_2.lat, node_1.lon, node_2.lon])
    return R*(pi/2 - asin(sin(lat1)*sin(lat2) + cos(lon2 - lon1)*cos(lat1)*cos(lat2)))



#---------------------------------------------------------------
#4/ On construit le nouveau chemin simplifié avec les bonnes distances

def simplify_way(w):
    W = [[w.nodes[0].id],[]] # [nodes, distances]
    dist = 0 # = distance courante entre deux intersections
    n = len(w.nodes)
    for k in range(1, n - 1):
        dist += distance(w.nodes[k -1], w.nodes[k])

        if counter[w.nodes[k].id] > 1: # i.e. le noeud est une intersection
            W[0].append(w.nodes[k].id)
            W[1].append(dist)
            dist = 0

    W[0].append(w.nodes[n - 1].id)
    W[1].append(dist)

    return W

#---------------------------------------------------------------
#5/ On calcule tout les nouveaux chemins

simplified_ways = []
for w in result.ways:
    simplified_ways.append(simplify_way(w))

#---------------------------------------------------------------
#6/ On ecrit le fichier de coords


current_id = 0
newsId = {}
for n in result.nodes:
    newsId[n.id] = current_id
    current_id += 1
newsId
for W in simplified_ways:
    for k in range(len(W[0])):
        W[0][k] = newsId[W[0][k]]
simplified_ways[0]
W=simplified_ways[0]
W[0][2] = newsId[W[0][2]]
W

"""
    Écriture au format 'v id long lat' où long et lat sont
    exprimes en entiers (= flottants à 8 décimales)
"""
def node_to_string(node):
    precision = 10**6
    lat = int(node.lat*precision)
    lon = int(node.lon*precision)
    return "v {} {} {}".format(newsId[node.id], lon, lat)



f = open(output_coords, 'w')

def write_intro(f):
    f.write("c Instance calculated with OSM data \n")
    f.write("c Algorithme made with Python's library 'OverPy'\n")
    f.write("c Instance description : {} \n".format(file_description))
    f.write("c Roads : {}".format(requete_road()))
    f.write("c Bounding box : {} \n".format(bounding_box))

write_intro(f)
for node in result.nodes:
    if counter[node.id] > 1:
        f.write("{} \n".format(node_to_string(node)))
f.close()

#---------------------------------------------------------------
#7/ On ecrit le fichier de graphe
def way_to_string(W):
    [nodes, dist] = W
    str = ""
    for k in range(len(nodes) - 1):
        str += "a {} {} {}\n".format(nodes[k], nodes[k + 1], int(dist[k]))
    return str

f = open(output_distance, 'w')
write_intro(f)
for W in simplified_ways:
    f.write(way_to_string(W))

f.close()
