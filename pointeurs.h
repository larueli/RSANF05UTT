#ifndef POINTEURS_H_INCLUDED
#define POINTEURS_H_INCLUDED

#include <stdio.h>
#include "grdnb.h"
#include <stdlib.h>

/** \file pointeurs.h
 *
 * \brief Contient les prototypes des fonctions permettant de traiter des pointeurs.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/**
 *
 * \struct chaineDePointeurs
 * \brief Objet contenant une liste chain�e de pointeurs sur des entiers.
 *
 * \deprecated Inutilis� en pratique, car long � mettre en oeuvre sur un code d�j� existant.
 */
typedef struct chaineDePointeurs chaineDePointeurs;
struct chaineDePointeurs
{
    int *pointeur; /**< Le contenu de l'�l�ment de la liste, un pointeur sur un entier */
    chaineDePointeurs *suivant; /**< L'adresse du prochain �l�ment. */
};

/**
 *
 * \struct pileDePointeurs
 * \brief Objet contenant l'adresse du premier �l�ment d'une liste chain�e de pointeurs sur des entiers.
 *
 * \deprecated Inutilis� en pratique, car long � mettre en oeuvre sur un code d�j� existant.
 */
typedef struct
{
    chaineDePointeurs *premier; /**< Adresse du premier �l�ment de la pile. */
} pileDePointeurs;

void empiler(void *pointeur, pileDePointeurs *pile);
void depiler(pileDePointeurs *pile);
void liberer(GRDNB *a);
void verifErreurAlloc(void *test);

#endif // POINTEURS_H_INCLUDED
