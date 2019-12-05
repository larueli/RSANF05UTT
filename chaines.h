#ifndef CHAINES_H
#define CHAINES_H

#include "grdnb.h"

/** \file chaines.h
 *
 * \brief Contient les prototypes des fonctions permettant de manipuler les chaines de caractères et les saisies sécurisées.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

char* saisieBrute();
void clearBuffer();
int saisieInt(const char chaine[]);
GRDNB saisieGRDNB(const char chaine[]);
#endif // CHAINES_H
