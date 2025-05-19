#include <stdio.h>

#include "stb_image.h"
#include "stb_image_write.h"

#include "kruskal.h"
#include "graphe.h"

int main() {

    Graphe* g = graphe_initialiser(4,4);
    int nb_composantes_g = graphe_nb_composantes_connexes(g);
    int** composantes_g = (int**) malloc(nb_composantes_g * sizeof(int*));
    int* tailles_g = (int*) malloc(nb_composantes_g * sizeof(int));
    graphe_composantes_connexes(g, composantes_g, tailles_g);
    graphe_modifier_rgb(g, 5, 0.5, 0.5, 0);
    double rouge;
    double vert;
    double bleu;
    graphe_obtenir_rgb(g, 5, &rouge, &vert, &bleu);
    printf("%f ", rouge);
    printf("%f ", vert);
    printf("%f\n", bleu);
    graphe_afficher(g, composantes_g, tailles_g);

    Graphe* k = kruskal(g);

    int** composantes_k = (int**) malloc(k->ordre * sizeof(int*));
    int* tailles_k = (int*) malloc(k->ordre * sizeof(int));
    graphe_modifier_rgb(k, 5, 0.5, 0.5, 0);
    graphe_supprimer_arete(k, 4, 8);
    graphe_modifier_poids(k, 8, 12, 3.);
    graphe_seuillage(k, 2.);
    graphe_modifier_poids(k, 1, 2, 3.);
    int nb_composantes = graphe_composantes_connexes_union_find(k, composantes_k, tailles_k);
    nb_composantes = nb_composantes;
    graphe_colorier_composante_connexe(k, composantes_k[0], tailles_k[0], 1., 0., 0.);
    graphe_afficher(k, composantes_k, tailles_k);

    double seuil = creer_seuil(k);
    printf("Seuil : %f\n", seuil);

    graphe_composantes_connexes_free(k, composantes_k, tailles_k);
    graphe_free(k);
    graphe_composantes_connexes_free(g, composantes_g, tailles_g);
    graphe_free(g);

    return 0;
}