#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "stb_image.h"
#include "stb_image_write.h"
#include "kruskal.h"
#include "graphe.h"
#include "image.h"
#include "mathutils.h"

Graphe* image_to_graphe(const char* image_path) {
    int largeur, hauteur, canaux;
    uint8_t* data = stbi_load(image_path, &largeur, &hauteur, &canaux, 3);
    if (!data) {
        fprintf(stderr, "Erreur : Impossible de charger l'image %s\n", image_path);
        return NULL;
    }

    LabColor* lab_colors = precalculate_lab_colors(data, largeur, hauteur);
    if (!lab_colors) {
        stbi_image_free(data);
        return NULL;
    }

    // Initialiser le graphe
    Graphe* g = graphe_initialiser(hauteur, largeur);
    if (!g) {
        free(lab_colors);
        stbi_image_free(data);
        return NULL;
    }

    // Stocker largeur, hauteur
    g->width = largeur;
    g->height = hauteur;

    // Remplir le graphe
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {

            int sommet = i * largeur + j;
            int idx = (i * largeur + j) * 3;
            double r = data[idx]   / 255.0;
            double v = data[idx+1] / 255.0;
            double b = data[idx+2] / 255.0;
            graphe_modifier_rgb(g, sommet, r, v, b);

            LabColor courant = lab_colors[sommet];

            // Calcul des poids pour les voisins 4-connectés

            if (j + 1 < largeur) { // Voisin de droite
                int voisin = sommet + 1;
                LabColor voisin_color = lab_colors[voisin];
                double poids = calcul_poids(courant.L, courant.a, courant.b, voisin_color.L, voisin_color.a, voisin_color.b);
                graphe_modifier_poids(g, sommet, voisin, poids);
            }
            if (i + 1 < hauteur) { // Voisin du bas
                int voisin = sommet + largeur;
                LabColor voisin_color = lab_colors[voisin];
                double poids = calcul_poids(courant.L, courant.a, courant.b, voisin_color.L, voisin_color.a, voisin_color.b);
                graphe_modifier_poids(g, sommet, voisin, poids);
            }

        }
    }

    free(lab_colors);
    stbi_image_free(data);
    return g;
}

void graphe_to_image(Graphe* g, const char* output_path, int largeur, int hauteur) {
    uint8_t* data = (uint8_t*) malloc(largeur * hauteur * 3);
    if (!data) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour l'image\n");
        return;
    }

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int sommet = i * largeur + j;
            double r, v, b;
            graphe_obtenir_rgb(g, sommet, &r, &v, &b);
            int idx = (i * largeur + j) * 3;
            data[idx]   = (uint8_t)(r * 255.0);
            data[idx+1] = (uint8_t)(v * 255.0);
            data[idx+2] = (uint8_t)(b * 255.0);
        }
    }

    if (!stbi_write_png(output_path, largeur, hauteur, 3, data, largeur * 3)) {
        fprintf(stderr, "Erreur : Impossible d'écrire l'image %s\n", output_path);
    }
    free(data);
}

LabColor* precalculate_lab_colors(uint8_t* data, int largeur, int hauteur) {
    LabColor* lab_colors = (LabColor*) malloc(largeur * hauteur * sizeof(LabColor));
    if (!lab_colors) {
        fprintf(stderr, "Erreur : allocation mémoire pour LabColors\n");
        return NULL;
    }

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int idx = (i * largeur + j) * 3;
            double r = data[idx] / 255.0;
            double g = data[idx + 1] / 255.0;
            double b = data[idx + 2] / 255.0;
            rgb_vers_lab(r, g, b, &lab_colors[i * largeur + j].L, &lab_colors[i * largeur + j].a, &lab_colors[i * largeur + j].b);
        }
    }

    return lab_colors;
}