#include <stdio.h>
#include <stdlib.h>
#include "pointeurs.h"
#include "menu.h"
#ifndef WIN32
#include <sys/types.h>
#endif
#include <dirent.h>
#include <string.h>

/** \file menu.c
 *
 * \brief Contient des fonctions et permettant de générer et afficher les menus.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

 /** \fn void menuprincipal()
  *
  * \brief Affiche le menu principal dans la console.
  */

void menuprincipal()
{
    int choix;
    do
    {
        printf("-------------------------------------------------\n");
        printf("----------------- Logiciel RSA ------------------\n");
        printf("-------------------------------------------------\n");
        printf(" Concepteurs : Ivann LARUELLE / Adriana MASCIULLI\n");
        printf(" Dans le cadre d'un cours sur le C (NF05) a l'UTT\n");
        printf("---------- https://github.com/larueli/ ----------\n");
        printf("\n");
        printf("Menu principal\n\n");
        printf("\t\t1. Generation d'une paire de clef\n");
        printf("\t\t2. Envoi d'un message\n");
        printf("\t\t3. Reception d'un message\n");
        printf("\n");
        printf("\t\t0. Quitter\n\n");
        choix = saisieInt("\tVotre choix : ");
        switch(choix)
        {
        case 1:
            menugeneration();
            break;
        case 2:
            menuchiffrement();
            break;
        case 3:
            menudechiffrement();
            break;
        }
    }while(choix!=0);
}

 /** \fn void menugeneration()
  *
  * \brief Affiche le menu de génération de clef dans la console, et enregistre les clefs dans leurs dossiers repectifs.
  * \warning La fonction srand() doit avoir été appellée auparavant.
  * \note Les clefs seront écrites sous la forme "n exposant" sur la même ligne d'un fichier en base 2.
  *
  * Les clefs seront enregistrées dans les dossiers public_keys et private_keys du répertoire d'exécution.
  */
void menugeneration()
{
    RSA newClef;
    printf("\n-----------------------\nGeneration de la clef\n\n");
    printf("\t\t1. Generation automatique\n");
    printf("\t\t2. Generation manuelle\n");
    printf("\n\t\t0. Revenir au menu\n\n");
    int choix = saisieInt("\tVotre choix : ");
    switch(choix)
    {
    case 1:
        printf("\n\nRecherche de p...");
        newClef.p = rabinmillerdeux();
        printf("\n\nRecherche de q...");
        newClef.q = rabinmillerdeux();
        printf("\n");
        break;
    case 2:
        printf("\n");
        int base;
        do
        {
            base = saisieInt("Dans quelle base allez-vous saisir p et q ? (10 ou 2) : ");
        }while(base != 10 && base != 2);
        newClef.p = saisieGRDNB("Veuillez saisir p premier : ");
        newClef.q = saisieGRDNB("Veuillez saisir q premier : ");
        if(base == 10)
        {
            affectation(&(newClef.p),b10Tob2(newClef.p,0));
            affectation(&(newClef.q),b10Tob2(newClef.q,0));
        }
        break;
    default:
        printf("\n");
        menuprincipal();
        break;
    }
    newClef.n = mulnaive(newClef.p,newClef.q,2);
    printf("\nLongueur de la clef : %d bits\n\n",(newClef.n).indicemax);
    newClef.phi = mulnaive(sous(newClef.p,un(),2),sous(newClef.q,un(),2),2);
    newClef.e = calcul_e(b2Tob10(newClef.phi),&(newClef.d));
    char *fichier = (char*)malloc(19*sizeof(char));
    verifErreurAlloc(fichier);
    strcpy(fichier,"private_keys\\");
    printf("Vos clefs seront enregistrees sous la forme nom.key dans des dossiers :\n\n\tpublic_keys\\ pour la clef publique\n\tprivate_keys\\ pour la clef privee\n\nVeuillez rentrer le nom de la clef privee : ");
    char *fichierclef = saisieBrute();
    fichier = realloc(fichier,strlen(fichierclef)+19);
    verifErreurAlloc(fichier);
    strcat(fichier,fichierclef);
    strcat(fichier,".key");
    fichierClef(&(newClef.n),&(newClef.d),1,fichier);
    printf("Veuillez rentrer le nom de la clef publique : ");
    free(fichier);
    fichier = (char*)malloc(19*sizeof(char));
    verifErreurAlloc(fichier);
    strcpy(fichier,"public_keys\\");
    free(fichierclef);
    fichierclef = saisieBrute();
    fichier = realloc(fichier,strlen(fichierclef)+19);
    verifErreurAlloc(fichier);
    strcat(fichier,fichierclef);
    strcat(fichier,".key");
    fichierClef(&(newClef.n),&(newClef.e),1,fichier);
    free(fichier);
}


/** \fn int fichierClef(GRDNB *a, GRDNB *b, int mode, char *fichier)
 *
 * \brief Ecrit/Lit sur une ligne deux grands nombres séparés par un espace.
 * \param[in,out] a Pointeur sur un GRDNB
 * \param[in,out] b Pointeur sur un GRDNB
 * \param[in] mode Si mode = 1, écriture de a et b.\n Si mode = 0, lecture dans a et b.
 * \param[in] fichier Chemin d'accès relatif ou absolu du fichier accessible en lecture ou écriture le cas échéant.
 * \warning En lecture, les clefs doivent être écrites dans le fichier en base 2.
 * \return 0 si l'opération s'est bien déroulée\n 1 sinon.
 *
 * En mode écriture, on convertit les GRDNB en chaine et on écrit sur une seule ligne les chaines séparées par un espace.
 *
 * En mode lecture, on essaye de récupérer les deux chaines, et on les convertit en GRDNB si tout se passe bien.
 */

int fichierClef(GRDNB *a, GRDNB *b, int mode, char *fichier)
{
    if(fichier  == NULL)
        return 1;
    if(mode)
    {
        FILE *fich = fopen(fichier,"w");
        if(fich!=NULL)
        {
            fputs(GRDNBtoSTR(*a),fich);
            fputc(' ',fich);
            fputs(GRDNBtoSTR(*b),fich);
            fclose(fich);
            return 0;
        }
    }
    else
    {
        FILE *fich = fopen(fichier,"r");
        if(fich!=NULL)
        {
            int retour = 0,test;
            fseek(fich,0,SEEK_END);
            int taille = ftell(fich);
            fseek(fich,0,SEEK_SET);
            char *achar = (char*)malloc(taille*sizeof(char));
            verifErreurAlloc(achar);
            char *bchar = (char*)malloc(taille*sizeof(char));
            verifErreurAlloc(bchar);
            test = fscanf(fich,"%s %s",achar,bchar);
            if(test != 2)
            {
                printf("\nErreur de lecture, verifiez le format de vos clefs.\n");
                retour = 1;
            }
            else
            {
                *a = str2grdnb(achar);
                *b = str2grdnb(bchar);
                if(a->signe == -1 || b->signe == -1 || !isBase2(*a) || !isBase2(*b) || a->indicemax < 9 || b->indicemax == 0)
                {
                    printf("n n'est pas bien ecrit : %d\nL'exposant n'est pas bien ecrit : %d\nn n'est pas en base 2 : %d\nL'exposant n'est pas en base 2 : %d\nn a moins de 8 bits %d\nL'exposant est nul : %d\n",a->signe == -1, b->signe == -1, !isBase2(*a), !isBase2(*b), a->indicemax < 9, b->indicemax == 0);
                    printf("\nErreur de lecture, verifiez le format de vos clefs.\n");
                    retour = 1;
                }
            }
            free(achar);
            free(bchar);
            fclose(fich);
            return retour;
        }
        else
        {
            printf("\nErreur lors de l'acces au fichier\n");
        }
    }
    return 1;
}

/** \fn char *listeClefs(char *nomduDossier)
 * \brief Affiche un menu ergonomique pour sélectionner une clef dans un répertoire donné.
 * \param[in] Chaine de caractère contenant le chemin d'accès relatif ou absolu du répertoire, accessible en lecture
 * \return La chaine de caractère comprenant le chemin d'accès relatif ou absolu à la clef selectionnée par l'utilisateur, ou NULL s'il n'y pas de clef.
 *
 * \warning Les clefs doivent avoir l'extension .key pour être affichées dans le menu.
 *
 * On ouvre le dossier, on ajoute le nom des fichiers finissant par .key dans un tableau, puis l'utilisateur choisit la clef voulue.
 *
 */

char *listeClefs(char *nomduDossier)
{
    struct dirent *contenu;
    int nbfichiers = 0;
    DIR *rep;
    rep = opendir(nomduDossier);
    char **listedefichiers = calloc(1,sizeof(char*));
    verifErreurAlloc(listedefichiers);
    while ((contenu = readdir(rep)))
    {
        if(strlen(contenu->d_name) > 4 && !strcmp((contenu->d_name)+strlen(contenu->d_name)-4,".key"))
            {
                nbfichiers++;
                listedefichiers = (char**)realloc(listedefichiers,nbfichiers*sizeof(char*));
                verifErreurAlloc(listedefichiers);
                listedefichiers[nbfichiers-1] = (char*)malloc((strlen(contenu->d_name) + strlen(nomduDossier) + 3)*sizeof(char));
                verifErreurAlloc(listedefichiers[nbfichiers-1]);
                strcpy(listedefichiers[nbfichiers-1],nomduDossier);
                strcat(listedefichiers[nbfichiers-1],"/");
                strcat(listedefichiers[nbfichiers-1],contenu->d_name);
            }
    }
    closedir(rep);
    int i = 0;
    int choixcorrect = 0, choix;
    if(nbfichiers > 0)
        printf("\n\nVeuillez selectionner une clef :\n\n");
    else
    {
        printf("\n\n! Vous n'avez pas de clef, generez-en ou importez-en !\n");
        return NULL;
    }
    while(!choixcorrect)
    {
        for(i=1;i<nbfichiers+1;i++)
        {
            printf("\t\t%d. %s\n",i,listedefichiers[i-1]);
        }
        printf("\n");
        choix = saisieInt("\tVotre choix : ");
        choixcorrect = choix >= 1 && choix <= nbfichiers;
    }
    return listedefichiers[choix-1];
}

/** \fn void menuchiffrement()
 * \brief Permet de chiffrer, signer ou de signer puis chiffrer un document.
 *
 * \warning Dans le cadre de la signature couplée au chiffrement, le fichier "tmp.tmp" du répertoire d'exécution sera créé ou écrasé puis effacé.
 *
 * Affiche le menu de chiffrement et lance les fonctions correspondantes.
 */

void menuchiffrement()
{
    printf("\n-----------------------\nChiffrement\n\n");
    printf("\t\t1. Chiffrement\n");
    printf("\t\t2. Signature\n");
    printf("\t\t3. Signature et chiffrement\n");
    printf("\n\t\t0. Revenir au menu\n\n");
    int choix = saisieInt("\tVotre choix : ");
    if(choix != 1 && choix != 2 && choix !=3)
        menuprincipal();
    printf("\nVeuillez saisir le nom du fichier a chiffrer : ");
    char *fichier = saisieBrute();
    printf("Veuillez saisir le nom du fichier chiffre : ");
    char *fichierdest = saisieBrute();
    RSA clefprivee,clefpublique;
    switch(choix)
    {
    case 1:
        printf("\nVous allez chiffrer ce message avec la clef publique du destinataire, importee au prealable dans le dossier public_keys\\\n");
        if(!fichierClef(&(clefpublique.n),&(clefpublique.e),0,listeClefs("public_keys")))
            chiffrer(fichier,fichierdest,clefpublique.n,clefpublique.e);
        break;
    case 2:
        printf("\nVous allez chiffrer ce message avec votre clef, importee au prealable dans le dossier private_keys\\ ou generee via le logiciel\n");
        if(!fichierClef(&(clefprivee.n),&(clefprivee.d),0,listeClefs("private_keys")))
            chiffrer(fichier,fichierdest,clefprivee.n,clefprivee.d);
        break;
    case 3:
        printf("\nVous allez chiffrer ce message avec votre clef privee, puis avec la clef publique du destinataire. \nVotre clef privee doit etre generee avec le logiciel ou importee dans le dossier private_keys\\\nLa clef publique du destinataire doit etre importee au prealable dans le dossier public_keys\\\n");
        if(!(fichierClef(&(clefprivee.n),&(clefprivee.d),0,listeClefs("private_keys")) || fichierClef(&(clefpublique.n),&(clefpublique.e),0,listeClefs("public_keys"))));
        {
            chiffrer(fichier,"tmp.tmp",clefprivee.n,clefprivee.d);
            printf("\nSecond passage");
            chiffrer("tmp.tmp",fichierdest,clefpublique.n,clefpublique.e);
            remove("tmp.tmp");
        }
        break;
    }
    free(fichier);
    free(fichierdest);
}

/** \fn void menudechiffrement()
 * \brief Permet de dechiffrer, vérifier la signature ou de dechiffrer puis verifier la signature d'un document.
 *
 * \warning Dans le cadre du double déchiffrement, le fichier "tmp.tmp" du répertoire d'exécution sera créé ou écrasé puis effacé.
 *
 * Affiche le menu de déchiffrement et lance les fonctions correspondantes.
 */
void menudechiffrement()
{
    printf("\n-----------------------\nDechiffrement\n\n");
    printf("\t\t1. Dechiffrement\n");
    printf("\t\t2. Verification de signature\n");
    printf("\t\t3. Dechiffrement puis verification de signature\n");
    printf("\n\t\t0. Revenir au menu\n\n");
    int choix = saisieInt("\tVotre choix : ");
    if(choix != 1 && choix != 2 && choix != 3)
        menuprincipal();
    printf("Veuillez saisir le nom du fichier a dechiffrer : ");
    char *fichier = saisieBrute();
    printf("Veuillez saisir le nom du fichier dechiffre : ");
    char *fichierdest = saisieBrute();
    RSA clefprivee,clefpublique;
    switch(choix)
    {
    case 1:
        printf("\nVous allez dechiffrer ce message avec votre clef, importee au prealable dans le dossier private_keys\\ ou generee via le logiciel\n");
        if(!fichierClef(&(clefprivee.n),&(clefprivee.d),0,listeClefs("private_keys")))
            dechiffrer(fichier,fichierdest,clefprivee.n,clefprivee.d);
        break;
    case 2:
        printf("\nVous allez dechiffrer ce message avec la clef publique de l'emetteur, importee au prealable dans le dossier public_keys\\\n");
        if(!fichierClef(&(clefpublique.n),&(clefpublique.e),0,listeClefs("public_keys")))
            dechiffrer(fichier,fichierdest,clefpublique.n,clefpublique.e);
        break;
    case 3:
        printf("\nVous allez dechiffrer ce message avec votre clef privee, puis avec la clef publique de l'emetteur.\nVotre clef privee doit etre generee avec le logiciel ou importee dans le dossier private_keys\\\nLa clef publique de l'emetteur doit etre importee au prealable dans le dossier public_keys\\\n");
        if(!(fichierClef(&(clefprivee.n),&(clefprivee.d),0,listeClefs("private_keys")) || fichierClef(&(clefpublique.n),&(clefpublique.e),0,listeClefs("public_keys"))));
        {
            dechiffrer(fichier,"tmp.tmp",clefprivee.n,clefprivee.d);
            printf("\nSecond passage");
            dechiffrer("tmp.tmp",fichierdest,clefpublique.n,clefpublique.e);
            remove("tmp.tmp");
        }
        break;
    }
    free(fichier);
    free(fichierdest);
}
