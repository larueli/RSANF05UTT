#ifndef MENU_H
#define MENU_H

#include "chaines.h"
#include "rsamaths.h"
#include "grdnb.h"

/** \file menu.h
 *
 * \brief Contient les prototypes des fonctions permettant de générer et afficher les menus.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

void menuprincipal();
void menugeneration();
void menuchiffrement();
char *listeClefs(char *nomduDossier);
int fichierClef(GRDNB *a, GRDNB *b, int mode, char *fichier);
void menudechiffrement();

#endif
