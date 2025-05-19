#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <stdio.h>

/**
 * @brief Calcule la moyenne d'une liste
 * @param liste Liste d'éléments
 * @param nb_elem Nombre d'éléments de la liste
 * @return Moyenne de la liste
 */
double moyenne(double* liste, int nb_elem);

/**
 * @brief Calcule l'écart-type d'une liste
 * @param liste Liste d'éléments
 * @param nb_elem Nombre d'éléments de la liste
 * @return Ecart-type de la liste
 */
double ecart_type(double* liste, int nb_elem);

/**
 * @brief Convertit une couleur RGB en L*a*b*.
 * @param r Valeur rouge (0-255).
 * @param g Valeur verte (0-255).
 * @param b Valeur bleue (0-255).
 * @param[out] L Pointeur pour stocker la composante L* (luminosité).
 * @param[out] a Pointeur pour stocker la composante a* (teinte verte à magenta).
 * @param[out] b_out Pointeur pour stocker la composante b* (teinte bleue à jaune).
 *
 */
void rgb_vers_lab(double r, double g, double b, double* L, double* a, double* b_out);

/**
 * @brief Calcule le poids entre deux couleurs L*a*b*.
 * @param L1 Luminosité de la première couleur.
 * @param a1 Composante a* de la première couleur.
 * @param b1 Composante b* de la première couleur.
 * @param L2 Luminosité de la deuxième couleur.
 * @param a2 Composante a* de la deuxième couleur.
 * @param b2 Composante b* de la deuxième couleur.
 * @return Le poids calculé comme une la distance en L*a*b* des deux couleurs
 */
double calcul_poids(double L1, double a1, double b1, double L2, double a2, double b2);

#endif // MATHUTILS_H