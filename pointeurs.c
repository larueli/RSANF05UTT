#include "pointeurs.h"

/** \file pointeurs.c
 *
 * \brief Contient des fonctions permettant de traiter des pointeurs.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/** \fn void empiler(void *pointeur, pileDePointeurs *pile)
 *
 * \brief Empile l'adresse indiqu�e dans la pile.
 * \param[in] pointeur L'adresse � ajouter.
 * \param[in,out] pile La pile de pointeurs sur laquelle on ajoute le pointeur.
 * \deprecated Inutilis� en pratique, car long � mettre en oeuvre sur un code d�j� existant.
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
 * \brief D�pile toute la pile en lib�rant les pointeurs � chaque fois.
 * \param[in,out] pile La pile de pointeurs que l'on d�truit.
 * \note Sont d�truits les pointeurs et les �l�ments de la pile eux-m�mes.
 * \deprecated Inutilis� en pratique, car long � mettre en oeuvre sur un code d�j� existant.
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
 * \brief Lib�re le tableau d'un GRDNB et le met � NULL, pour �viter tout souci.
 * \param[in,out] a L'adresse du GRDNB dont on veut d�truire le tableau.
 * \deprecated Inutilis� actuellement, car ce garde-fou n'est plus n�cessaire.
 */
void liberer(GRDNB *a)
{
    free(a->tableau);
    a->tableau = NULL;
}

/** \fn void verifErreurAlloc(void *test)
 * \brief Quitte imm�diatement le programme si une erreur d'allocation dynamique survient.
 * \param[in] test Un pointeur non typ�.
 * \note Cette fonction doit �tre appell�e apr�s chaque allocation ou r�allocation dynamique.
 *
 * Si le pointeur est NULL, la fonction affiche un message � l'utilisateur et fait quitter le programme.
 */

void verifErreurAlloc(void *test)
{
    if(test != NULL)
        return;
    printf("\nERREUR FATALE lors de l'allocation de memoire.\nLe programme va fermer, appuyez sur une touche puis entree...");
    getchar();
    exit(EXIT_FAILURE);
}
