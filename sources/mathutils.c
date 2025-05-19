#include <stdio.h>
#include <math.h>

#include "mathutils.h"

double moyenne(double* liste, int nb_elem)
{
    double somme = 0.;
    for(int i=0 ; i<nb_elem ; i++)
    {
        somme += liste[i];
    }

    return somme / nb_elem;
}

double ecart_type(double* liste, int nb_elem)
{
    double somme_carres = 0.0;
    double moy = moyenne(liste,nb_elem);
    for (int i=0 ; i<nb_elem ; i++) 
    {
        double diff = liste[i] - moy;
        somme_carres += diff * diff;
    }
    return sqrt(somme_carres / nb_elem);
}

void rgb_vers_lab(double r, double g, double b, double* L, double* a, double* b_out) {
    // Normalisation des couleurs
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;

    if (r > 0.04045) {
        r = pow((r + 0.055) / 1.055, 2.4);
    } else {
        r = r / 12.92;
    }

    if (g > 0.04045) {
        g = pow((g + 0.055) / 1.055, 2.4);
    } else {
        g = g / 12.92;
    }

    if (b > 0.04045) {
        b = pow((b + 0.055) / 1.055, 2.4);
    } else {
        b = b / 12.92;
    }

    // Conversion en CIEXYZ
    double X = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
    double Y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
    double Z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;

    // Normalisation CIEXYZ
    X /= 95.047;
    Y /= 100.000;
    Z /= 108.883;

    // Conversion en LAB
    if (X > 0.008856) {
        X = pow(X, 1.0 / 3.0);
    } else {
        X = 7.787 * X + 16.0 / 116.0;
    }

    if (Y > 0.008856) {
        Y = pow(Y, 1.0 / 3.0);
    } else {
        Y = 7.787 * Y + 16.0 / 116.0;
    }

    if (Z > 0.008856) {
        Z = pow(Z, 1.0 / 3.0);
    } else {
        Z = 7.787 * Z + 16.0 / 116.0;
    }

    *L = (116.0 * Y) - 16.0;
    *a = 500.0 * (X - Y);
    *b_out = 200.0 * (Y - Z);
}

double calcul_poids(double L1, double a1, double b1, double L2, double a2, double b2) {
    
    double distance_couleur = sqrt(pow(L1 - L2, 2) + pow(a1 - a2, 2) + pow(b1 - b2, 2));

    return distance_couleur;
}
