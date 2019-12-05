#include <stdio.h>
#include <stdlib.h>
#include "grdnb.h"
#include "menu.h"
#include <time.h>
#include <string.h>
#include "pointeurs.h"

/** \mainpage
 * Ce programme a été conçu dans le cadre de l'UE NF05 à l'UTT en A2018.
 *
 * Il a pour but de faire le chiffrement RSA sur des fichiers, après avoir généré et stocké les clefs dans des fichiers.
 *
 * Cette documentation Doxygen a pour but de faire le mode d'emploi des fonctions et non du programme en lui-même.\n
 * Vous trouverez dans cette documentation tous les détails concernant le fonctionnement des algorithmes.\n
 * Le mode d'emploi du programme est présent dans le rapport.
 *
 * Il y a plusieurs modules :
 * - grdnb.c pour l'implémentation des grands nombres et des fonctions mathématiques de base
 * - chaines.c pour le traitement des chaines et des saisies
 * - menu.c pour l'affichage de l'interface
 * - pointeurs.c pour le traitement des pointeurs
 * - rsamaths.c pour toutes les fonctions mathémtiques relatives au chiffrement
 *
 * Si vous avez des questions, contactez les auteurs :
 * - Adriana MASCIULLI (adriana.masciulli@utt.fr)
 * - Ivann LARUELLE (ivann.laruelle@utt.fr)
 */


int main()
{
    srand(time(NULL));
    system("mkdir public_keys");
    system("mkdir private_keys");
    menuprincipal();
    return 0;
}


