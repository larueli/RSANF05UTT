#include "pointeurs.h"

/** \file pointeurs.c
 *
 * \brief Contient des fonctions permettant de traiter des pointeurs.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/** \fn void empiler(void *pointeur, pileDePointeurs *pile)
 *
 * \brief Empile l'adresse indiquée dans la pile.
 * \param[in] pointeur L'adresse à ajouter.
 * \param[in,out] pile La pile de pointeurs sur laquelle on ajoute le pointeur.
 * \deprecated Inutilisé en pratique, car long à mettre en oeuvre sur un code déjà existant.
 */

void empiler(void *pointeur, pileDePointeurs *pile)
{
    chaineDePointeurs *nouveau = malloc(sizeof(*nouveau));
    verifErreurAlloc(nouveau);
    nouveau->pointeur = pointeur;
    nouveau->suivant = pile->premier;
    pile->premier = nouveau;
}

/** \fn void depiler(pileDePointeurs *pile)
 *
 * \brief Dépile toute la pile en libérant les pointeurs à chaque fois.
 * \param[in,out] pile La pile de pointeurs que l'on détruit.
 * \note Sont détruits les pointeurs et les éléments de la pile eux-mêmes.
 * \deprecated Inutilisé en pratique, car long à mettre en oeuvre sur un code déjà existant.
 */
void depiler(pileDePointeurs *pile)
{
    chaineDePointeurs *prec;
    while(pile->premier != NULL)
    {
        prec = pile->premier;
        free(pile->premier->pointeur);
        pile->premier->pointeur = NULL;
        pile->premier = pile->premier->suivant;
        free(prec);
    }
}

/** \fn void liberer(GRDNB *a)
 *
 * \brief Libére le tableau d'un GRDNB et le met à NULL, pour éviter tout souci.
 * \param[in,out] a L'adresse du GRDNB dont on veut détruire le tableau.
 * \deprecated Inutilisé actuellement, car ce garde-fou n'est plus nécessaire.
 */
void liberer(GRDNB *a)
{
    free(a->tableau);
    a->tableau = NULL;
}

/** \fn void verifErreurAlloc(void *test)
 * \brief Quitte immédiatement le programme si une erreur d'allocation dynamique survient.
 * \param[in] test Un pointeur non typé.
 * \note Cette fonction doit être appellée après chaque allocation ou réallocation dynamique.
 *
 * Si le pointeur est NULL, la fonction affiche un message à l'utilisateur et fait quitter le programme.
 */

void verifErreurAlloc(void *test)
{
    if(test != NULL)
        return;
    printf("\nERREUR FATALE lors de l'allocation de memoire.\nLe programme va fermer, appuyez sur une touche puis entree...");
    getchar();
    exit(EXIT_FAILURE);
}
