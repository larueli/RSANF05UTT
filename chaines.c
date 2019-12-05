#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pointeurs.h"
#include "chaines.h"

/** \file chaines.c
 *
 * \brief Contient des fonctions permettant de manipuler les chaines de caractères et les saisies sécurisées.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/** \fn char* saisieBrute()
 * \brief Permet de saisir une chaine de caractère sans dépassement de mémoire et sans savoir à l'avance la taille de la saisie.
 *
 * \return Une chaine de caractère qui contient la saisie de l'utilisateur.
 *
 * On ouvre un fichier temporaire avec <em>tmpfile()</em>\n
 * - Avec <em>getchar()</em>, on lit le caractère saisi.
 * - On le stocke dans le fichier temporaire.
 * - On incrémente notre compteur de caractère.
 * - On continue tant que le prochain caractère lu est différent du retour à la ligne.
 *
 * On crée la chaine dynamiquement avec la taille trouvée précédemment.\n
 * On lit toute la ligne du fichier temporaire dans la chaine créée.\n
 * On renvoie la chaine.
 */

char* saisieBrute()
{
    FILE *temp = tmpfile();
    if(temp != NULL)
    {
        int i;
        char saisieCaractere = getchar();
        for(i=1;saisieCaractere != '\n';i++)
        {
            fputc(saisieCaractere, temp);
            saisieCaractere = getchar();
        }
        char *chaine = (char*)malloc(sizeof(char)*i);
        verifErreurAlloc(chaine);
        if(chaine != NULL)
        {
            rewind(temp);
            fgets(chaine, i, temp);
            fclose(temp);
            return chaine;
        }
    }
    return NULL;
}

/** \fn void clearBuffer()
 * \brief Permet d'effacer le buffer de l'entrée standard.
 * \deprecated La fonction char* saisieBrute() nettoie toujours le buffer.
 * \warning Cette fonction doit être utilisée quand vous savez qu'il y a des caractères à nettoyer.\n Sinon elle va attendre que l'utilisateur saisisse quelque chose.
 *
 * On lit un caractère tant que que le caractère lu est différent du retour à la ligne ou de EOF.
 */
void clearBuffer()
{
    char c = getchar();
    while(c != '\n' && c != EOF)
        c = getchar();
}

/** \fn int saisieInt(const char chaine[])
 * \brief Permet d'afficher une chaine et récupérer la saisie de l'utilisateur sous la forme d'un entier
 * \param[in] chaine Chaine contenant la question posée à l'utilisateur.
 * \return Un entier saisi par l'utilisateur.
 *
 * On affiche la chaine, on récupére la saisie brute de l'utilisateur sous forme de chaine.\n
 * La fonction strtol nous permet de convertir la chaine en entier.\n
 * Cette fonction nous donne également le restant de la chaine si éventuellement des choses autre que des chiffres sont présentes.\n
 * Tant que la taille de cette chaine n'est pas nulle, on recommence la saisie.
 *
 * Quand on est sûr que l'utilisateur n'a saisi que des chiffres, on convertit et on renvoie le résultat.
 */

int saisieInt(const char chaine[])
{
    char *finPtr, *saisie = malloc(sizeof(char));
    verifErreurAlloc(saisie);
    int choix;
    do
    {
        free(saisie);
        printf("%s", chaine);
        saisie = saisieBrute();
        strtol(saisie, &finPtr, 10);
    }while(strlen(finPtr) != 0);
    choix = (int)strtol(saisie, &finPtr, 10);
    free(saisie);
    return choix;
}

/** \fn GRDNB saisieGRDNB(const char chaine[])
 * \brief Permet d'afficher une chaine et récupérer la saisie de l'utilisateur sous la forme d'un GRDNB.
 * \param[in] chaine Chaine contenant la question posée à l'utilisateur.
 * \return Un GRDNB saisi par l'utilisateur.
 *
 * On affiche la chaine, on récupére la saisie brute de l'utilisateur sous forme de chaine.\n
 * On regarde la première case pour voir éventuellement le signe du nombre.\n
 * On parcourt chaque case de la chaine pour s'assurer que ce ne sont que des chiffres.\n
 * Tant qu'il y a des caractères différents de chiffres, on recommence.
 *
 * Quand on est sûr que l'utilisateur n'a saisi que des chiffres, on convertit avec str2grdnb() et on renvoie le résultat.
 */
GRDNB saisieGRDNB(const char chaine[])
{
    char *saisie = malloc(sizeof(char));
    int debut = 0, i, probleme = 0;
    GRDNB choix;
    do
    {
        free(saisie);
        printf("%s", chaine);
        saisie = saisieBrute();
        if(saisie[debut] == '+' || saisie[debut] == '-')
        {
            debut++;
        }
        for(i=debut; i<strlen(saisie) && !probleme; i++)
            probleme = !(saisie[i] >= '0' && saisie[i] <= '9');
    }
    while(probleme);
    choix = str2grdnb(saisie);
    free(saisie);
    supprimezero(&choix);
    return choix;
}
