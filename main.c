#include <stdio.h>
#include <stdlib.h>
#include "grdnb.h"
#include "menu.h"
#include <time.h>
#include <string.h>
#include "pointeurs.h"

/** \mainpage
 * Ce programme a �t� con�u dans le cadre de l'UE NF05 � l'UTT en A2018.
 *
 * Il a pour but de faire le chiffrement RSA sur des fichiers, apr�s avoir g�n�r� et stock� les clefs dans des fichiers.
 *
 * Cette documentation Doxygen a pour but de faire le mode d'emploi des fonctions et non du programme en lui-m�me.\n
 * Vous trouverez dans cette documentation tous les d�tails concernant le fonctionnement des algorithmes.\n
 * Le mode d'emploi du programme est pr�sent dans le rapport.
 *
 * Il y a plusieurs modules :
 * - grdnb.c pour l'impl�mentation des grands nombres et des fonctions math�matiques de base
 * - chaines.c pour le traitement des chaines et des saisies
 * - menu.c pour l'affichage de l'interface
 * - pointeurs.c pour le traitement des pointeurs
 * - rsamaths.c pour toutes les fonctions math�mtiques relatives au chiffrement
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


