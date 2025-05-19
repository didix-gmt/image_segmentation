#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kruskal.h"
#include "graphe.h"
#include "mathutils.h"

int find(int parent[], int i) {
    while (parent[i] != i) {
        parent[i] = parent[parent[i]]; // Compression de chemin
        i = parent[i];
    }
    return i;
}

int unionSets(int parent[], int rank[], int x, int y) {
    int rootX = find(parent, x);
    int rootY = find(parent, y);

    if (rootX == rootY) return 0; // Pas besoin de fusionner

    if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
    return 1; // Fusion effectuée
}

// Tri radix des arêtes basé sur leurs poids (0 <= poids <= 1)
void radix_sort(double** aretes, int nbAretes, int precision) {
    int facteur = (int)pow(10, precision); // Facteur pour convertir flottants en entiers
    int* compte = (int*) calloc(facteur + 1, sizeof(int));
    double** aretestemp = (double**) malloc(nbAretes * sizeof(double*));

    // Comptage des poids
    for (int i = 0; i < nbAretes; i++) {
        int poids = (int)(aretes[i][2] * facteur);
        compte[poids]++;
    }

    // Position cumulative
    for (int i = 1; i <= facteur; i++) {
        compte[i] += compte[i - 1];
    }

    // Placement des arêtes dans l'ordre
    for (int i = nbAretes - 1; i >= 0; i--) {
        int poids = (int)(aretes[i][2] * facteur);
        aretestemp[--compte[poids]] = aretes[i];
    }

    // Copier le résultat trié
    for (int i = 0; i < nbAretes; i++) {
        aretes[i] = aretestemp[i];
    }

    free(compte);
    free(aretestemp);
}

Graphe* kruskal(Graphe* g) {
    if (!g || g->nb_aretes <= 0) return NULL;

    int nbAretes = g->nb_aretes;

    // Récupérer la liste d'arêtes (s1, s2, poids)
    double* aretes_data = (double*) malloc(nbAretes * 3 * sizeof(double));
    double** aretes = (double**) malloc(nbAretes * sizeof(double*));
    for (int i = 0; i < nbAretes; i++) {
        aretes[i] = &aretes_data[i * 3];
    }

    int indice = 0;
    for (int i = 0; i < g->ordre; i++) {
        for (int j = 0; j < g->nb_voisins[i]; j++) {
            int v = g->adj[i][j].numero;
            double poids = g->adj[i][j].poids;
            if (i < v) { // Ajouter l'arête uniquement si i < v (éviter les doublons)
                aretes[indice][0] = i;
                aretes[indice][1] = v;
                aretes[indice][2] = poids;
                indice++;
            }
        }
    }

    // Tri des arêtes par radix sort
    radix_sort(aretes, nbAretes, 6); // 6 décimales de précision

    // Créer l'arbre couvrant de poids minimal
    Graphe* t = (Graphe*) malloc(sizeof(Graphe));
    if (!t) {
        free(aretes_data);
        free(aretes);
        return NULL;
    }
    t->ordre = g->ordre;
    t->nb_aretes = 0;
    t->nb_voisins = (int*) calloc(t->ordre, sizeof(int));
    t->capacite_voisins = (int*) malloc(t->ordre * sizeof(int));
    t->rgb = (double**) malloc(t->ordre * sizeof(double*));
    t->adj = (Sommet**) malloc(t->ordre * sizeof(Sommet*));
    if (!t->nb_voisins || !t->capacite_voisins || !t->rgb || !t->adj) {
        free(t->nb_voisins);
        free(t->capacite_voisins);
        free(t->rgb);
        free(t->adj);
        free(t);
        free(aretes_data);
        free(aretes);
        return NULL;
    }
    for (int i = 0; i < t->ordre; i++) {
        t->capacite_voisins[i] = 4; // Maximum pour un graphe 4-connecté
        t->adj[i] = (Sommet*) malloc(4 * sizeof(Sommet));
        t->nb_voisins[i] = 0;
        t->rgb[i] = (double*) malloc(3 * sizeof(double));
        if (!t->adj[i] || !t->rgb[i]) {
            for (int j = 0; j <= i; j++) {
                free(t->adj[j]);
                free(t->rgb[j]);
            }
            free(t->nb_voisins);
            free(t->capacite_voisins);
            free(t->rgb);
            free(t->adj);
            free(t);
            free(aretes_data);
            free(aretes);
            return NULL;
        }
        for (int c = 0; c < 3; c++) {
            t->rgb[i][c] = g->rgb[i][c];
        }
    }

    // Initialiser les structures pour l'union-find
    int* parent = (int*) malloc(t->ordre * sizeof(int));
    int* rank = (int*) calloc(t->ordre, sizeof(int));
    for (int i = 0; i < t->ordre; i++) parent[i] = i;

    // Construction de l'arbre couvrant de poids minimal
    for (int i = 0; i < nbAretes; i++) {
        int s1 = (int)aretes[i][0];
        int s2 = (int)aretes[i][1];
        double poids = aretes[i][2];

        if (unionSets(parent, rank, s1, s2)) {
            graphe_ajouter_arete(t, s1, s2, poids);
        }
    }

    free(parent);
    free(rank);
    free(aretes_data);
    free(aretes);

    return t;
}

int contientCycle(Graphe* g) {
    int* parent = (int*) malloc(g->ordre * sizeof(int));
    int* rank = (int*) calloc(g->ordre, sizeof(int));

    for (int i = 0; i < g->ordre; i++) {
        parent[i] = i;
    }

    for (int i = 0; i < g->ordre; i++) {
        for (int j = 0; j < g->nb_voisins[i]; j++) {
            int u = i;
            int v = g->adj[i][j].numero;

            int rootU = find(parent, u);
            int rootV = find(parent, v);

            if (rootU == rootV) {
                free(parent);
                free(rank);
                return 1; // Cycle détecté
            }

            unionSets(parent, rank, rootU, rootV);
        }
    }

    free(parent);
    free(rank);
    return 0; // Pas de cycle
}