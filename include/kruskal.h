#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <stdio.h>
#include <math.h>
#include "graphe.h"
#include "mathutils.h"

/**
 * @brief Trouve le parent d'un sommet
 * @param parent Tableau des parents représentant les ensembles, chaque index correspond à un sommet et sa valeur à son parent              
 * @param i Indice du sommet
 * @return Le parent de i
 */
int find(int parent[], int i);

/**
 * @brief Unir deux ensembles en fonction de leurs racines.
 * @param parent Tableau des parents représentant les ensembles.
 * @param rank Tableau représentant le rang de chaque sommet (approximativement la hauteur de l'arbre sous chaque sommet)
 * @param x Un sommet de parent
 * @param y Un sommet de rank
 * @returns 1 si les deux ensembles sont fusionnés, 0 sinon
 */
int unionSets(int parent[], int rank[], int x, int y);

/**
 * @brief Trie un tableau en utilisant l'algorithme de tri par base (Radix Sort)
 * @param aretes Liste d'arêtes de la forme (s1,s2,poids)
 * @param nbAretes Nombre d'arêtes
 * @param precision Précision du tri
 */
void radix_sort(double** aretes, int nbAretes, int precision);

/**
 * @brief Crée un arbre couvrant de poids minimal à partir du graphe g
 * @param g Graphe
 * @return Arbre couvrant de poids minimal à partir de g
 * Crée un arbre couvrant de poids minimal à partir du graphe g à l'aide de l'algorithme de Kruskal
 */
Graphe* kruskal(Graphe* g);

/**
 * @brief Vérifie si le graphe g possède un cycle
 * @param g Graphe
 * @returns 1 si g possède un cycle, 0 sinon
 */
int contientCycle(Graphe* g);

#endif // KRUSKAL_H