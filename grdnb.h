#ifndef GRDNB_H
#define GRDNB_H

/** \file grdnb.h
 *
 * \brief Contient les prototypes des fonctions et structures permettant de générer et travailler avec des grands nombres.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/**
 *
 * \struct GRDNB
 * \brief Objet représentant un grand nombre, sa taille et son signe.
 *
 */
typedef struct
{
    int *tableau; /**< Un pointeur sur un tableau d'entier, qui contiendra les valeurs de chaque chiffre du nombre */
    int indicemax; /**< La taille du tableau. ATTENTION : tableau[indicemax] n'est pas accessible ! */
    int signe; /**< Le signe de cet entier est le signe du nombre. */
} GRDNB;

GRDNB creerGRDNB(int taille);
GRDNB diveuclidedeux(GRDNB a, GRDNB b, GRDNB *quotient);
GRDNB str2grdnb(char *chaine);
int divisible(GRDNB e,GRDNB phi_n,int base);
int egal(GRDNB a, GRDNB b);
void affectation(GRDNB *a, GRDNB b);
int tailleBase2(GRDNB a);
int tailleIdentique(GRDNB a,GRDNB b, GRDNB *acopie, GRDNB *bcopie);
GRDNB somme(GRDNB a, GRDNB b, int base);
GRDNB expomodrap(GRDNB message, GRDNB e, GRDNB n, int base);
GRDNB sous(GRDNB a, GRDNB b, int base);
GRDNB soustraction(GRDNB a, GRDNB b, int base);
void copie(GRDNB a, GRDNB *acopie);
GRDNB b2Tob10(GRDNB a);
int sontpremiers(GRDNB a, GRDNB b, int base);
GRDNB addition(GRDNB a, GRDNB b, int base);
void supprimezero(GRDNB *a);
int vraishiftdroite(GRDNB *a);
GRDNB mulunique(int a, int b, int base);
char *GRDNBtoSTR(GRDNB a);
GRDNB b10Tob2(GRDNB a, int taille);
GRDNB euclideetendu(GRDNB dividende,GRDNB diviseur, int base, GRDNB *u, GRDNB *v);
GRDNB mulnaive(GRDNB a, GRDNB b,int base);
GRDNB div2(GRDNB a);
int isMulDeux(GRDNB a);
GRDNB IntToGRDNB(int a, int base);
GRDNB diveuclide(GRDNB a, GRDNB b, int base, GRDNB *quotient);
int vraishiftgauche(GRDNB *a);
GRDNB sousnombre(GRDNB a, int debut, int fin);
int ismax(GRDNB a, GRDNB b);
GRDNB PuissanceRapide(GRDNB a, GRDNB puissance, int base);
void afficher(char *chaine, GRDNB a);
int shiftdroite(GRDNB *a);
int shiftgauche(GRDNB *a);
void echanger(int *tableau, int i, int j);
GRDNB un();
int isBase2(GRDNB a);
//int DEBUG = 1;
GRDNB modulo(GRDNB a, GRDNB b, int base);
#endif
