#include <stdio.h>
#include <stdlib.h>
#include "kruskal.h"
#include "graphe.h"
#include "mathutils.h"

int graphe_existe_arete(Graphe* g, int s1, int s2) {
    for (int i = 0; i < g->nb_voisins[s1]; i++) {
        if (g->adj[s1][i].numero == s2) {
            return 1;
        }
    }
    return 0;
}

void graphe_ajouter_arete(Graphe* g, int s1, int s2, double poids) {
    if (!graphe_existe_arete(g, s1, s2)) {
        
        g->adj[s1][g->nb_voisins[s1]].numero = s2;
        g->adj[s1][g->nb_voisins[s1]].poids = poids;
        g->nb_voisins[s1]++;

        g->adj[s2][g->nb_voisins[s2]].numero = s1;
        g->adj[s2][g->nb_voisins[s2]].poids = poids;
        g->nb_voisins[s2]++;

        g->nb_aretes++;
    }
}

void graphe_supprimer_arete(Graphe* g, int s1, int s2) {
    for (int i = 0; i < g->nb_voisins[s1]; i++) {
        if (g->adj[s1][i].numero == s2) {
            g->adj[s1][i] = g->adj[s1][g->nb_voisins[s1] - 1];
            g->nb_voisins[s1]--;
            break;
        }
    }
    for (int i = 0; i < g->nb_voisins[s2]; i++) {
        if (g->adj[s2][i].numero == s1) {
            g->adj[s2][i] = g->adj[s2][g->nb_voisins[s2] - 1];
            g->nb_voisins[s2]--;
            break;
        }
    }
    g->nb_aretes--;
}

void graphe_afficher(Graphe* g, int **composantes, int *tailles) {
    printf("Ordre : %d\n", g->ordre);
    printf("Nombre d'arêtes : %d\n", g->nb_aretes);
    if (composantes && tailles) {
        printf("Nombre de composantes connexes : %d\n", graphe_nb_composantes_connexes(g));
    }
    printf("Possède un cycle : %d\n", contientCycle(g));
    printf("\n");
    for (int i = 0; i < g->ordre; i++) {
        printf("%d : ", i);
        for (int j = 0; j < g->nb_voisins[i]; j++) {
            int voisin = g->adj[i][j].numero;
            printf("%d (%f,%f,%f) (Poids : %f) ", voisin, g->rgb[voisin][0], g->rgb[voisin][1], g->rgb[voisin][2], g->adj[i][j].poids);
        }
        printf("\n");
    }
    if (composantes && tailles) {
        printf("\n");
        for (int i = 0; i < graphe_nb_composantes_connexes(g); i++) {
            printf("Composante connexe %d (taille %d) : ", i + 1, tailles[i]);
            for (int j = 0; j < tailles[i]; j++) {
                printf("%d ", composantes[i][j]);
            }
            printf("\n");
        }
    }
}

void graphe_modifier_rgb(Graphe* g, int s, double rouge, double vert, double bleu) {
    g->rgb[s][0] = rouge;
    g->rgb[s][1] = vert;
    g->rgb[s][2] = bleu;
}

Graphe* graphe_initialiser(int hauteur, int largeur) {
    Graphe* g = (Graphe*) malloc(sizeof(Graphe));
    if (!g) return NULL;
    g->ordre = hauteur * largeur;
    g->nb_aretes = 0;
    g->nb_voisins = (int*) calloc(g->ordre, sizeof(int));
    g->capacite_voisins = (int*) malloc(g->ordre*sizeof(int));
    g->rgb = (double**) malloc(g->ordre*sizeof(double*));
    g->adj = (Sommet**) malloc(g->ordre*sizeof(Sommet*));
    if(!g->nb_voisins || !g->capacite_voisins || !g->rgb || !g->adj) {
        // Gérer l'erreur d'allocation
        free(g->nb_voisins); 
        free(g->capacite_voisins);
        free(g->rgb);
        free(g->adj);
        free(g); 
        return NULL;
    }

    for (int i = 0; i < g->ordre; i++) {
        g->nb_voisins[i] = 0;
        g->capacite_voisins[i] = 4; 
        g->rgb[i] = (double*) calloc(3, sizeof(double)); // Code RGB à (0,0,0)
        g->adj[i] = (Sommet*) malloc(g->capacite_voisins[i]*sizeof(Sommet));
        if(!g->rgb[i] || !g->adj[i]) {
            // Gérer l'erreur si besoin
        }
    }

    // Ajouter les arêtes par 4-connexité
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int courant = i * largeur + j;
            if (j + 1 < largeur) {
                int d = courant + 1; // Voisin de droite
                graphe_ajouter_arete(g, courant, d, 1.0);
            }
            if (i + 1 < hauteur) {
                int b = (i+1)*largeur + j; // Voisin du bas
                graphe_ajouter_arete(g, courant, b, 1.0);
            }
        }
    }
    return g;
}

void graphe_free(Graphe* g) {
    for (int i = 0; i < g->ordre; i++) {
        free(g->adj[i]);
        free(g->rgb[i]);
    }
    free(g->adj);
    free(g->nb_voisins);
    free(g->capacite_voisins);
    free(g->rgb);
    free(g);
}

void graphe_modifier_poids(Graphe* g, int s1, int s2, double poids) {
    if (graphe_existe_arete(g, s1, s2)) {
        for (int i = 0; i < g->nb_voisins[s1]; i++) {
            if (g->adj[s1][i].numero == s2) {
                g->adj[s1][i].poids = poids;
                break;
            }
        }
        for (int i = 0; i < g->nb_voisins[s2]; i++) {
            if (g->adj[s2][i].numero == s1) {
                g->adj[s2][i].poids = poids;
                break;
            }
        }
    }
}

// Ancien parcours en profondeur
// A éviter
void graphe_parcours_profondeur(Graphe* g, int s, int* visite, int* composantesConnexes, int* taillesComposantes) {
    int* pile = (int*) malloc(g->ordre * sizeof(int));
    int sommetCourant;
    int indexPile = 0; // Pointeur de la pile

    pile[indexPile++] = s; // Ajouter le sommet de départ à la pile
    visite[s] = 1; // Marquer le sommet comme visité

    while (indexPile > 0) {
        sommetCourant = pile[--indexPile]; // Extraire le sommet de la pile
        if (composantesConnexes && taillesComposantes) {
            composantesConnexes[(*taillesComposantes)++] = sommetCourant; // Ajouter le sommet à sa composante connexe
        }

        // Parcourir les voisins du sommet courant
        for (int i = 0; i < g->nb_voisins[sommetCourant]; i++) {
            int v = g->adj[sommetCourant][i].numero;
            if (!visite[v]) { // Si le sommet n'a pas encore été visité
                visite[v] = 1; // Marquer comme visité
                pile[indexPile++] = v; // Ajouter à la pile
            }
        }
    }

    free(pile);
}

// Utilise l'ancien parcours en profondeur
// A éviter
int graphe_nb_composantes_connexes(Graphe* g) {
    int* visite = (int*) calloc(g->ordre, sizeof(int));
    int nb_composantes = 0;
    for (int i = 0; i < g->ordre; i++) {
        if (!visite[i]) {
            nb_composantes++;
            graphe_parcours_profondeur(g, i, visite, NULL, NULL);
        }
    }
    free(visite);
    return nb_composantes;
}

// Utilise l'ancien parcours en profondeur
// A éviter
void graphe_composantes_connexes(Graphe* g, int** composantes, int* tailles) {
    if (composantes == NULL || tailles == NULL) {
        return;
    }
    int* visite = (int*) calloc(g->ordre, sizeof(int));
    int indice = 0;
    for (int i = 0; i < g->ordre; i++) {
        if (!visite[i]) {
            composantes[indice] = (int*) malloc(g->ordre * sizeof(int));
            int taille = 0;
            graphe_parcours_profondeur(g, i, visite, composantes[indice], &taille);
            tailles[indice] = taille;
            indice++;
        }
    }
    free(visite);
}

void graphe_composantes_connexes_free(Graphe* g, int** composantesConnexes, int* taillesComposantes) {
    int nbComposantes = graphe_nb_composantes_connexes(g);
    for (int i = 0; i < nbComposantes; i++) {
        if (composantesConnexes[i] != NULL) {
            free(composantesConnexes[i]);
        }
    }
    free(composantesConnexes);
    free(taillesComposantes);
}

void graphe_seuillage(Graphe* g, double seuil) {
    for (int i = 0; i < g->ordre; i++) {
        for (int j = 0; j < g->nb_voisins[i]; j++) {
            if (g->adj[i][j].poids >= seuil) {
                graphe_supprimer_arete(g, i, g->adj[i][j].numero);
                j--; // Ajuster l'index après avoir retiré une arête
            }
        }
    }
}

void graphe_colorier_composante_connexe(Graphe* g, int* composanteConnexe, int tailleComposante, double rouge, double vert, double bleu) {
    for (int i = 0; i < tailleComposante; i++) {
        graphe_modifier_rgb(g, composanteConnexe[i], rouge, vert, bleu);
    }
}

void graphe_obtenir_rgb(Graphe* g, int sommet, double* rouge, double* vert, double* bleu) {
    *rouge = g->rgb[sommet][0];
    *vert = g->rgb[sommet][1];
    *bleu = g->rgb[sommet][2];
}

int comparer_double(const void* a, const void* b) {
    double diff = (*(double*)a - *(double*)b);
    return (diff > 0) - (diff < 0); // Retourne 1, -1 ou 0
}

double creer_seuil(Graphe* g) {
    double* liste_poids = (double*) malloc(g->nb_aretes * sizeof(double));
    int indice = 0;
    for (int i = 0; i < g->ordre; i++) {
        for (int j = 0; j < g->nb_voisins[i]; j++) {
            int v = g->adj[i][j].numero;
            double poids = g->adj[i][j].poids;
            if (i < v) {
                liste_poids[indice++] = poids;
            }
        }
    }
    
    qsort(liste_poids, g->nb_aretes, sizeof(double), comparer_double);
    int quart_index = (int)(0.85f * g->nb_aretes);
    double seuil = liste_poids[quart_index];

    free(liste_poids);
    return seuil;
}

void graphe_afficher_statistiques(Graphe* g) {
    printf("\n== Statistiques du graphe ==\n");
    printf("Ordre : %d\n", g->ordre);
    printf("Nombre d'arêtes : %d\n", g->nb_aretes);
    printf("Nombre de composantes connexes : %d\n", graphe_nb_composantes_connexes(g));
    printf("Possède un cycle : %d\n \n", contientCycle(g));
}

int graphe_composantes_connexes_union_find(Graphe* g, int** composantesConnexes, int* taillesComposantes) {
    int* parent = (int*) malloc(g->ordre * sizeof(int));
    int* rank = (int*) calloc(g->ordre, sizeof(int));
    int* indicesComposantes = (int*) calloc(g->ordre, sizeof(int));
    int* mapComposantes = (int*) calloc(g->ordre, sizeof(int));

    if (!parent || !rank || !indicesComposantes || !mapComposantes) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }

    // Initialiser chaque sommet comme étant sa propre racine
    for (int i = 0; i < g->ordre; i++) {
        parent[i] = i;
    }

    // Appliquer l'union-find sur toutes les arêtes
    for (int i = 0; i < g->ordre; i++) {
        for (int j = 0; j < g->nb_voisins[i]; j++) {
            int voisin = g->adj[i][j].numero;
            unionSets(parent, rank, i, voisin);
        }
    }

    // Trouver les représentants pour chaque sommet
    for (int i = 0; i < g->ordre; i++) {
        parent[i] = find(parent, i);
    }

    // Initialiser les structures pour compter le nombre de composantes connexes
    int compteurComposantes = 0;
    for (int i = 0; i < g->ordre; i++) {
        int racine = parent[i];
        if (racine < 0 || racine >= g->ordre) {
            fprintf(stderr, "Indice de racine invalide : %d\n", racine);
            free(parent);
            free(rank);
            free(indicesComposantes);
            free(mapComposantes);
            return -1;
        }
        if (mapComposantes[racine] == 0) {
            mapComposantes[racine] = ++compteurComposantes;
        }
    }

    // Initialiser les tailles des composantes connexes à 0
    for (int i = 0; i < compteurComposantes; i++) {
        taillesComposantes[i] = 0;
    }

    // Associer les sommets à leurs composantes connexes et compter les tailles des composantes
    for (int i = 0; i < g->ordre; i++) {
        int composanteId = mapComposantes[parent[i]] - 1;
        taillesComposantes[composanteId]++;
    }

    // Allouer de la mémoire pour les composantes connexes
    for (int i = 0; i < compteurComposantes; i++) {
        composantesConnexes[i] = (int*) malloc(taillesComposantes[i] * sizeof(int));
        if (!composantesConnexes[i]) {
            fprintf(stderr, "Erreur d'allocation pour composante %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Réinitialiser les indices pour le remplissage des composantes
    for (int i = 0; i < compteurComposantes; i++) {
        indicesComposantes[i] = 0;
    }

    // Remplir les composantes avec les sommets
    for (int i = 0; i < g->ordre; i++) {
        int composanteId = mapComposantes[parent[i]] - 1;
        composantesConnexes[composanteId][indicesComposantes[composanteId]++] = i;
    }

    free(parent);
    free(rank);
    free(indicesComposantes);
    free(mapComposantes);

    return compteurComposantes;
}

void graphe_afficher_composantes(int** composantes, int* tailles, int nb_composantes)
{
    for (int i = 0; i < nb_composantes; i++) {
            printf("Composante connexe %d (taille %d) : ", i + 1, tailles[i]);
            for (int j = 0; j < tailles[i]; j++) {
                printf("%d ", composantes[i][j]);
            }
            printf("\n");
        }
}