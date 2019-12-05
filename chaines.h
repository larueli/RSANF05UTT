#ifndef CHAINES_H
#define CHAINES_H

#include "grdnb.h"

/** \file chaines.h
 *
 * \brief Contient les prototypes des fonctions permettant de manipuler les chaines de caract�res et les saisies s�curis�es.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

char* saisieBrute();
void clearBuffer();
int saisieInt(const char chaine[]);
GRDNB saisieGRDNB(const char chaine[]);
#endif // CHAINES_H
