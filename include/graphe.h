#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdio.h>
#include <stdlib.h>
#include "kruskal.h"
#include "mathutils.h"


/**
 * @struct Sommet
 * Sommet d'un graphe
 */
typedef struct Sommet 
{
    int numero; /**< Etiquette du sommet */
    double poids; /**< Poids d'une arête où ce sommet est la destination (arêtes définies via une liste d'adjacence) */
} Sommet;

/**
 * @struct Graphe
 * Structure d'un graphe
 */
typedef struct Graphe
{
    int ordre; /**< Nombre de sommets du graphe */
    int nb_aretes; /**< Nombre d'arêtes du graphe */
    int* nb_voisins; /**< Nombre de voisins d'un graphe (nb_voisins[i] est le nombre de voisins du sommet i) */
    int* capacite_voisins; /**< Nombre de voisins maximal d'un sommet (capacite_voisins[i] est le nombre de voisins maximal du sommet i) */
    double** rgb; /**< Liste de 3-uplets contenant le code RGB de chaque sommet (rgb[i][0], rgb[i][1], rgb[i][2] sont respectivement le code RGB rouge, vert, bleu du sommet i) */
    Sommet** adj; /**< Liste d'adjacence du graphe (adj[i] contient une liste de tous les sommets adjacents au sommet i) */
    int width; /**< Largeur de l'image */
    int height; /**< Hauteur de l'image */
} Graphe;

/**
 * @brief Vérifie si l'arête (s1,s2) existe dans le graphe g
 * @param g Graphe
 * @param s1 Etiquette du premier sommet
 * @param s2 Etiquette du second sommet
 * @returns 1 si l'arête existe, 0 sinon
 */
int  graphe_existe_arete(Graphe* g, int s1, int s2);

/**
 * @brief Crée l'arête (s1,s2) dans le graphe g
 * @param g Graphe
 * @param s1 Etiquette du premier sommet
 * @param s2 Etiquette du second sommet
 * @param poids Poids de l'arête créée 
 */
void graphe_ajouter_arete(Graphe* g, int s1, int s2, double poids);

/**
 * @brief Supprime l'arête (s1,s2) dans le graphe g
 * @param g Graphe
 * @param s1 Etiquette du premier sommet
 * @param s2 Etiquette du second sommet
 */
void graphe_supprimer_arete(Graphe* g, int s1, int s2);

/**
 * @brief Affiche en console différentes caractéristiques du graphe
 * @param g Graphe
 * @param composantes Liste de listes d'étiquettes de sommets (Liste contenant les composantes connexes du graphe)
 * @param tailles Tailles des composantes connexes du graphe
 * Affiche en console des propriétés du graphe g : son ordre, le nombre d'arêtes, sa liste d'adjacence, ses composantes connexes, s'il possède un cycle 
 */
void graphe_afficher(Graphe* g, int** composantes, int* tailles);

/**
 * @brief Modifie le code RGB du sommet s
 * @param g Graphe
 * @param s Etiquette du sommet
 * @param rouge Nouveau code RGB rouge
 * @param vert Nouevau code RGB vert
 * @param bleu Nouevau code RGB bleu
 */
void graphe_modifier_rgb(Graphe* g, int s, double rouge, double vert, double bleu);

/**
 * @brief Crée un graphe de dimension hauteur x largeur
 * @param hauteur Hauteur de l'image
 * @param largeur Largeur de l'image
 * @return Un graphe de dimension hauteur x largeur
 * Créer un graphe rectangulaire de dimension hauteur x largeur
 * Le graphe est défini par 4-connexité
 * Les arêtes du graphe sont toutes de poids 1
 * Initialise les codes RGB des sommets à (0,0,0)
 */
Graphe* graphe_initialiser(int hauteur,int largeur);

/**
 * @brief Libère la mémoire allouée au graphe g
 * @param g Graphe
 */
void graphe_free(Graphe* g);

/**
 * @brief Modifie le poids de l'arête (s1,s2)
 * @param g Graphe
 * @param s1 Etiquette du premier sommet
 * @param s2 Etiquette du second sommet
 * @param poids Nouveau poids de l'arête
 */
void graphe_modifier_poids(Graphe* g, int s1, int s2, double poids);

/**
 * @brief Effectue un parcours en profondeur du graphe g
 * @param g Graphe
 * @param s Sommet initial du parcours en profondeur
 * @param visite Liste contenant si un sommet a été visité lors du parcours en profondeur
 * @param composantesConnexes Liste des composantes connexes
 * @param taillesComposantes Tailles des composantes connexes
 * Effectue un parcours en profondeur du graphe g à partir du sommet s 
 * La variable visite doit être initalisée
 * composantesConnexes et taillesComposantes ne doivent pas être obligatoirement être initialisées (mettre NULL si c'est le cas)
 */
void graphe_parcours_profondeur(Graphe* g, int s, int* visite, int* composantesConnexes, int* taillesComposantes);

/**
 * @brief Renvoie le nombre de composantes connexes du graphe g
 * @param g Graphe
 */
int graphe_nb_composantes_connexes(Graphe *g);

/**
 * @brief Ajoute à composantes et tailles les composantes connexes du graphe g et leurs tailles
 * @param g Graphe
 * @param composantes Composantes connexes du graphe
 * @param tailles Tailles des composantes connexes
 */
void graphe_composantes_connexes(Graphe *g, int** composantes, int* tailles);

/**
 * @brief Libère la mémoire allouée aux composantes connexes
 * @param g Graphe
 * @param composantesConnexes Composantes connexes du graphe
 * @param taillesComposantes Tailles des composantes connexes
 */
void graphe_composantes_connexes_free(Graphe* g, int** composantesConnexes, int* taillesComposantes);

/**
 * @brief Supprime toutes les arêtes de g ayant un poids supérieur ou égal au seuil
 * @param g Graphe
 * @param seuil Seuil de poids
 */
void graphe_seuillage(Graphe* g, double seuil);

/**
 * @brief Modifie le code RGB de tous les sommets d'une composante connexe
 * @param g Graphe
 * @param composanteConnexe Composante connexe
 * @param tailleComposante Taille de la composante connexe
 * @param rouge Code RGB rouge
 * @param vert Code RGB vert
 * @param bleu Code RGB bleu
 */
void graphe_colorier_composante_connexe(Graphe* g, int* composanteConnexe, int tailleComposante, double rouge, double vert, double bleu);

/**
 * @brief Permet d'obtenir le code RGB d'un sommet et de le stocker dans rouge, vert, bleu
 * @param g Graphe
 * @param sommet Etiquette du sommet
 * @param[out] rouge Code RGB rouge
 * @param[out] vert Code RGB vert
 * @param[out] bleu Code RGB bleu
 */
void graphe_obtenir_rgb(Graphe* g, int sommet, double* rouge, double* vert, double* bleu);

/**
 * @brief Compare deux doubles
 * @param a 1er double
 * @param b 2nd double
 * @returns 1 si a > b, -1 si a < b, 0 si a = b
 */
int comparer_double(const void* a, const void* b);

/**
 * @brief Créer un seuil de poids pour les arêtes, sélectionne un poids tel que 20% des arêtes du graphe aient un poids supérieur au seuil
 * @param g Graphe
 * @return Créer un seuil de poids pour les arêtes
 */
double creer_seuil(Graphe* g);

/**
 * @brief Affiche des statistiques sur le graphe
 * @param g Graphe
 */
void graphe_afficher_statistiques(Graphe* g);

/**
 * @brief Identifie les composantes connexes du graphe, stocke le nombre de composantes connexes
 * @param g Graphe
 * @param composantesConnexes Liste des composantes connexes
 * @param taillesComposantes Tailles des composantes connexes
 * @return Nombre de composantes connexes du graphe
 */
int graphe_composantes_connexes_union_find(Graphe* g, int** composantesConnexes, int* taillesComposantes);

/**
 * @brief Affiche les composantes connexes
 * @param composantes Liste des composantes connexes
 * @param tailles Tailles des composantes connexes
 * @param nb_composantes Nombre de composantes
 */
void graphe_afficher_composantes(int** composantes, int* tailles, int nb_composantes);

#endif // GRAPHE_H
