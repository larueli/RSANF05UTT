#ifndef RSAMATH_H
#define RSAMATH_H
#include "pointeurs.h"
#include "chaines.h"
#include "grdnb.h"

/** \file rsamaths.h
 *
 * \brief Contient les prototypes des fonctions permettant de générer les clefs RSA et de les utiliser pour chiffrer et déchiffrer.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/**
 *
 * \struct RSA
 * \brief Objet contenant les éléments essentiels d'une clef RSA.
 *
 */

typedef struct
{
    GRDNB p; /**< GRDNB p premier */
    GRDNB q; /**< GRDNB q premier */
    GRDNB n; /**< GRDNB n = p*q */
    GRDNB phi; /**< GRDNB phi = (p-1)*(q-1) */
    GRDNB e; /**< GRDNB e, exposant public de la clef tel que ed = 1 mod phi */
    GRDNB d; /**< GRDNB d, exposant privé de la clef tel que ed = 1 mod phi */
} RSA;

GRDNB calcul_e(GRDNB phi_n, GRDNB *d);
int aleatoire(int a, int b);
GRDNB rabinmiller();
GRDNB aleaGRDNB(int taille, int base);
GRDNB rabinmillerdeux();
void dechiffrer(char *fichier, char *fichdest, GRDNB n, GRDNB a);
void chiffrer(char *fichier, char *fichdest, GRDNB n, GRDNB a);
int tailleNombre(int nombre, int base);
#endif
