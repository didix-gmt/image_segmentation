#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "stb_image.h"
#include "stb_image_write.h"
#include "kruskal.h"
#include "graphe.h"
#include "image.h"

int main(int argc, char* argv[]) {
    char* image_path = NULL;
    char* output_path = NULL;
    int limite = -1;
    int afficher_stats = 0;
    int afficher_chemins = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            image_path = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            limite = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0) {
            afficher_stats = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            afficher_chemins = 1;
        } else {
            fprintf(stderr, "Usage: %s -i CHEMIN -t FICHIER [-l LIMITE] [-s]\n", argv[0]);
            return EXIT_FAILURE;
        }

    }

    if (!image_path || !output_path) {
        fprintf(stderr, "Usage: %s -i CHEMIN -t FICHIER [-l LIMITE] [-s] [-f]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Conversion de l'image en graphe...\n");
    Graphe* g = image_to_graphe(image_path);
    if (!g) {
        fprintf(stderr, "Erreur : Impossible de convertir l'image en graphe\n");
        return EXIT_FAILURE;
    }

    printf("Graphe converti : ordre = %d, nb_aretes = %d\n", g->ordre, g->nb_aretes);

    if (limite > 0 && (g->width > limite || g->height > limite)) {
        fprintf(stderr, "Erreur : Dimensions de l'image supérieures à la limite\n");
        graphe_free(g);
        return EXIT_FAILURE;
    }

    printf("Application de l'algorithme de Kruskal...\n");
    Graphe* arbre_couvrant = kruskal(g);
    if (!arbre_couvrant) {
        fprintf(stderr, "Erreur : Impossible de générer l'arbre couvrant\n");
        graphe_free(g);
        return EXIT_FAILURE;
    }

    printf("Arbre couvrant généré : ordre = %d, nb_aretes = %d\n", arbre_couvrant->ordre, arbre_couvrant->nb_aretes);

    printf("Application du seuillage...\n");    
    double seuil = creer_seuil(arbre_couvrant);
    printf("Seuil calculé : %f\n", seuil);
    graphe_seuillage(arbre_couvrant, seuil);

    int** composantes = (int**) malloc(arbre_couvrant->ordre * sizeof(int*));
    int* tailles = (int*) malloc(arbre_couvrant->ordre * sizeof(int));
    if (!composantes || !tailles) {
        fprintf(stderr, "Erreur : Allocation de mémoire échouée\n");
        graphe_free(g);
        graphe_free(arbre_couvrant);
        return EXIT_FAILURE;
    }
    printf("Détection des composantes connexes...\n");
    int nb_composantes = graphe_composantes_connexes_union_find(arbre_couvrant, composantes, tailles);

    printf("Coloration des composantes connexes...\n");
    for (int i = 0; i < nb_composantes; i++) {
        if (tailles[i] > 70) {
            int rouge = (31 * i) % 256;
            int vert = (47 * i) % 256;
            int bleu = (59 * i) % 256;
            graphe_colorier_composante_connexe(arbre_couvrant, composantes[i], tailles[i], rouge, vert, bleu);
        }
        else {
            graphe_colorier_composante_connexe(arbre_couvrant, composantes[i], tailles[i], 1, 1, 1);
        }
    }

    printf("Sauvegarde de l'image segmentée...\n");
    graphe_to_image(arbre_couvrant, output_path, g->width, g->height);
    printf("Image segmentée sauvegardée.\n");

    printf("Segmentation terminée avec succès.\n");

    if (afficher_stats) {
        graphe_afficher_statistiques(arbre_couvrant);
    }

    if (afficher_chemins) {
        printf("Affichage des chemins segmentants du graphe...\n");
        graphe_afficher_composantes(composantes,tailles,nb_composantes);
    }

    graphe_composantes_connexes_free(arbre_couvrant,composantes,tailles);
    graphe_free(g);
    graphe_free(arbre_couvrant);

    return EXIT_SUCCESS;
}