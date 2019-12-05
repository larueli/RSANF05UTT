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
 * \brief Objet contenant une liste chainée de pointeurs sur des entiers.
 *
 * \deprecated Inutilisé en pratique, car long à mettre en oeuvre sur un code déjà existant.
 */
typedef struct chaineDePointeurs chaineDePointeurs;
struct chaineDePointeurs
{
    int *pointeur; /**< Le contenu de l'élément de la liste, un pointeur sur un entier */
    chaineDePointeurs *suivant; /**< L'adresse du prochain élément. */
};

/**
 *
 * \struct pileDePointeurs
 * \brief Objet contenant l'adresse du premier élément d'une liste chainée de pointeurs sur des entiers.
 *
 * \deprecated Inutilisé en pratique, car long à mettre en oeuvre sur un code déjà existant.
 */
typedef struct
{
    chaineDePointeurs *premier; /**< Adresse du premier élément de la pile. */
} pileDePointeurs;

void empiler(void *pointeur, pileDePointeurs *pile);
void depiler(pileDePointeurs *pile);
void liberer(GRDNB *a);
void verifErreurAlloc(void *test);

#endif // POINTEURS_H_INCLUDED
