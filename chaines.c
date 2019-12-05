#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pointeurs.h"
#include "chaines.h"

/** \file chaines.c
 *
 * \brief Contient des fonctions permettant de manipuler les chaines de caract�res et les saisies s�curis�es.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/** \fn char* saisieBrute()
 * \brief Permet de saisir une chaine de caract�re sans d�passement de m�moire et sans savoir � l'avance la taille de la saisie.
 *
 * \return Une chaine de caract�re qui contient la saisie de l'utilisateur.
 *
 * On ouvre un fichier temporaire avec <em>tmpfile()</em>\n
 * - Avec <em>getchar()</em>, on lit le caract�re saisi.
 * - On le stocke dans le fichier temporaire.
 * - On incr�mente notre compteur de caract�re.
 * - On continue tant que le prochain caract�re lu est diff�rent du retour � la ligne.
 *
 * On cr�e la chaine dynamiquement avec la taille trouv�e pr�c�demment.\n
 * On lit toute la ligne du fichier temporaire dans la chaine cr��e.\n
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
 * \brief Permet d'effacer le buffer de l'entr�e standard.
 * \deprecated La fonction char* saisieBrute() nettoie toujours le buffer.
 * \warning Cette fonction doit �tre utilis�e quand vous savez qu'il y a des caract�res � nettoyer.\n Sinon elle va attendre que l'utilisateur saisisse quelque chose.
 *
 * On lit un caract�re tant que que le caract�re lu est diff�rent du retour � la ligne ou de EOF.
 */
void clearBuffer()
{
    char c = getchar();
    while(c != '\n' && c != EOF)
        c = getchar();
}

/** \fn int saisieInt(const char chaine[])
 * \brief Permet d'afficher une chaine et r�cup�rer la saisie de l'utilisateur sous la forme d'un entier
 * \param[in] chaine Chaine contenant la question pos�e � l'utilisateur.
 * \return Un entier saisi par l'utilisateur.
 *
 * On affiche la chaine, on r�cup�re la saisie brute de l'utilisateur sous forme de chaine.\n
 * La fonction strtol nous permet de convertir la chaine en entier.\n
 * Cette fonction nous donne �galement le restant de la chaine si �ventuellement des choses autre que des chiffres sont pr�sentes.\n
 * Tant que la taille de cette chaine n'est pas nulle, on recommence la saisie.
 *
 * Quand on est s�r que l'utilisateur n'a saisi que des chiffres, on convertit et on renvoie le r�sultat.
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
 * \brief Permet d'afficher une chaine et r�cup�rer la saisie de l'utilisateur sous la forme d'un GRDNB.
 * \param[in] chaine Chaine contenant la question pos�e � l'utilisateur.
 * \return Un GRDNB saisi par l'utilisateur.
 *
 * On affiche la chaine, on r�cup�re la saisie brute de l'utilisateur sous forme de chaine.\n
 * On regarde la premi�re case pour voir �ventuellement le signe du nombre.\n
 * On parcourt chaque case de la chaine pour s'assurer que ce ne sont que des chiffres.\n
 * Tant qu'il y a des caract�res diff�rents de chiffres, on recommence.
 *
 * Quand on est s�r que l'utilisateur n'a saisi que des chiffres, on convertit avec str2grdnb() et on renvoie le r�sultat.
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
