#if !defined(IMAGE_H)
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "graphe.h"
#include "kruskal.h"
#include "mathutils.h"

/**
 * @struct LabColor
 * Structure CIELAB d'un pixel
 */
typedef struct LabColor {
    double L; /**< Clarté du sommet */
    double a; /**< Axe des couleurs vert-rouge */
    double b; /**< Axe des couleurs bleu-jaune */
} LabColor;

/**
 * @brief Transforme une image en un graphe pondéré (les poids sont les différences de couleurs des deux sommets)
 * @param image_path Chemin de l'image à transformer en graphe
 * @return Graphe de l'image
 */
Graphe* image_to_graphe(const char* image_path);

/**
 * @brief Transforme un graphe en une image
 * @param g Graphe
 * @param output_path Chemin de l'image
 * @param largeur Largeur de l'image
 * @param hauteur Hauteur de l'image
 */
void graphe_to_image(Graphe* g, const char* output_path, int largeur, int hauteur);

/**
 * @brief Pré-calcul les couleurs des pixels en CIELAB pour une image donnée.
 * @param data Données de l'image source.
 * @param largeur Largeur de l'image en pixels.
 * @param hauteur Hauteur de l'image en pixels.
 * @return Tableau de sortie pour stocker les couleurs en CIELAB.
 */
LabColor* precalculate_lab_colors(uint8_t* data, int largeur, int hauteur);

#endif // IMAGE_H