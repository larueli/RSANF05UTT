#include "rsamaths.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pointeurs.h"
#include <time.h>

/** \file rsamaths.c
 *
 * \brief Contient des fonctions permettant de g�n�rer les clefs RSA et de les utiliser pour chiffrer et d�chiffrer.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/**
 * \fn GRDNB calcul_e(GRDNB phi_n, GRDNB *d)
 * \brief Permet de calculer e et d � partir de phi(n) tel que e * d = 1 mod phi(n)
 * \param[in] phi_n Un grand nombre phi_n
 * \param[out] d Addresse du GRDNB de la clef priv�e
 * \return Un GRDNB qui contient la clef publique
 * \warning La fonction srand() doit avoir �t� appell�e auparavant.
 *
 * Cette fonction g�n�re al�atoirement e impair et non divisible par 5, et recherche <em>e * d + phi_n * k = pgcd(e,phi_n)</em> via euclide �tendu.
 *
 * Tant que pgcd(e,phi_n) diff�rent de 1, on r�g�n�re e et on recommence.
 */

GRDNB calcul_e(GRDNB phi_n, GRDNB *d)
{
    GRDNB e,verif,k;
    int i,premier;
    do
    {
        free(e.tableau);
        e = creerGRDNB(phi_n.indicemax-1);
        premier = aleatoire(1, 4);
        switch(premier)
        {
            case 1:
                e.tableau[e.indicemax-1]=1;
                break;
            case 2:
                e.tableau[e.indicemax-1]=3;
                break;
            case 3:
                e.tableau[e.indicemax-1]=7;
                break;
            case 4:
                e.tableau[e.indicemax-1]=9;
                break;
        }
        for(i=0;i<e.indicemax-1;i++)
            e.tableau[i]=aleatoire(0,9);
        if(e.tableau[0]==0)
            (e.tableau[0])++;
    verif = euclideetendu(phi_n,e,10,&k,d);
    }while(!(verif.indicemax == 1 && verif.tableau[0] == 1));
    affectation(d,b10Tob2(*d,0));
    return b10Tob2(e,0);
}

/**
 * \fn int aleatoire(int a, int b)
 * \brief G�n�re al�toirement un entier entre a et b, les deux inclus, avec a <= b
 * \param[in] a est un entier
 * \param[in] b est un entier tel que b >= a
 * \return un entier al�atoire compris entre a et b les deux inclus.
 *
 * \warning La fonction srand() doit avoir �t� appell�e auparavant.
 */

int aleatoire(int a, int b)
{
    return rand()%(b-a+1) + a;
}

/**
 * \fn GRDNB rabinmillerdeux()
 * \brief G�n�re un entier ayant une tr�s forte chance d'�tre premier.
 * \return Un GRDNB positif en base 2 s'�crivant sous la forme <em>2^s * d + 1</em>, avec <em>70 <= s <= 125</em> et <em>2^9 <= d < 2^20</em> et ayant une probabilit� <em>1 - 1/4^5</em> d'�tre premier.
 *
 * \warning La fonction srand() doit avoir �t� appell�e auparavant.
 *
 * - On g�n�re <em>n = 2^s * d + 1</em>, avec <em>70 <= s (al�atoire) <= 125</em> et <em>2^9 <= d (al�atoire) < 2^21 avec d impair</em>, et on r�p�te la g�n�ration tant que n pas impair.
 *  - On g�n�re a al�atoirement entre 10 et n/100
 *   - Si <em>reste = a^d % n</em> est �gal � <em>1 ou n-1</em>, on dit que a passe le test.
 *   - Sinon, on fait s fois l'op�ration <em>reste = reste^2 % n</em>, en regardant � chaque fois si reste est �gal � <em>n-1</em>. Si on en trouve un seul, alors a passe le test.
 *   - Si on fait l'op�ration s fois sans trouver de reste �gal � n-1, a ne passe pas le test et n ne peut pas �tre premier. On g�n�re alors un nouveau n.
 *  - Si a passe le test, alors on reg�n�re un nouveau a et on refait les op�ration pr�c�dentes pour 4 a (5 a en tout). Si un seul d'entre eux ne passe pas le test, on reg�n�re n.
 * - Si on a 5 a qui passent le test de Rabin Miller, alors n a une probabilit� <em>1 - 1/4^5 = 0.9990234375</em> d'�tre premier.
 *
 */

GRDNB rabinmillerdeux()
{
    GRDNB d = creerGRDNB(1);
    GRDNB n = creerGRDNB(1);
    GRDNB a = creerGRDNB(1);
    GRDNB nmoinsun = creerGRDNB(1);
    GRDNB reste = creerGRDNB(1);
    GRDNB tmp =creerGRDNB(1);
    int i;
    int s;
    GRDNB deux = str2grdnb("10");
    GRDNB uno = un();
    int nbchiffrestestes = 0;
    GRDNB deuxpuissances =creerGRDNB(1);
    int pseudopremier;
    int nbtest = 5,compteur = 0;
    do
    {
        compteur = 0;
        {
            free(d.tableau);
            free(n.tableau);
            s = aleatoire(70,125);
            free(deuxpuissances.tableau);
            deuxpuissances = creerGRDNB(s);
            deuxpuissances.tableau[0] = 1;
            d = aleaGRDNB(aleatoire(9,20),2);
            d.tableau[d.indicemax - 1] = 1;
            affectation(&tmp,mulnaive(deuxpuissances,d,2));
            n = somme(tmp,uno,10);
            pseudopremier = !isMulDeux(n) ;
        }while(!pseudopremier);
        free(nmoinsun.tableau);
        nmoinsun = sous(n,uno,10);
        printf(".");
        nbchiffrestestes++;
        while(compteur<nbtest && pseudopremier)
        {
            free(a.tableau);
            free(reste.tableau);
            a = aleaGRDNB(aleatoire(2,n.indicemax-2),2);
            reste = expomodrap(a,d,n,2);
            pseudopremier = 0;
            if((reste.indicemax == 1 && reste.tableau[0]==1) || egal(reste,nmoinsun))
                pseudopremier = 1;
            else
            {
                i = 0;
                while(i<s && !pseudopremier)
                {
                    affectation(&reste,expomodrap(reste,deux,n,2));
                    if(egal(reste,nmoinsun))
                        pseudopremier = 1;
                    i++;
                }
            }
            compteur++;
        }
    }while(!pseudopremier);
    free(d.tableau);
    free(a.tableau);
    printf("\n%d nombres testes en tout",nbchiffrestestes);
    return n;
}

/** \fn GRDNB aleaGRDNB(int taille, int base)
 * \brief G�n�re un entier al�atoire d'une taille et base pr�cises.
 * \param[in] taille Un entier sp�cifiant la taille que le nombre g�n�r� doit respecter.
 * \param[in] base Un entier sp�cifiant la base.
 * \return Un GRDNB tel que nombre >= base^(taille-1).
 * \warning La fonction srand() doit avoir �t� appell�e auparavant.
 *
 * Exemple : aleaGRDNB(5,10) = 77760 (taille = 5, base 10).\n
 * Vous n'aurez jamais aleaGRDNB(5,10) = 1452 (car le r�sultat doit �tre de taille 5)
 *
 */

GRDNB aleaGRDNB(int taille, int base)
{
    GRDNB retour = creerGRDNB(taille);
    int i=0;
    retour.tableau[0] = aleatoire(1,base - 1);
    for(i=1;i<taille;i++)
        retour.tableau[i] = aleatoire(0,base - 1);
    return retour;
}


/**
 * \fn void chiffrer(char *fichier, char *fichdest, GRDNB n, GRDNB a)
 * \brief Chiffre un fichier selon la clef a (d ou e, tout d�pend le contexte de l'appel, signature ou chiffrement) et son produit pq = n et �crit le r�sultat dans fichdest.
 * \param[in] fichier Chaine de caract�res contenant le chemin relatif ou absolu du fichier � chiffrer, accessible en lecture.
 * \param[in] fichdest Chaine de caract�res contenant le chemin relatif ou absolu du fichier chiffr�, accessible en �criture, cr�� s'il n'existe pas et �cras� s'il existe.
 * \param[in] n C'est le produit p*q en base 2, qui va servir ici de modulo � toutes les donn�es chiffr�es. nb de bits(n) > 8 (taille d'un caract�re).
 * \param[in] a C'est l'exposant en base 2 de toutes les donn�es chiffr�es, d ou e selon que l'on signe avec sa clef priv�e ou que l'on chiffre avec la clef publique du destinataire.
 *
 * \warning "fichbin.txt", dans le r�pertoire d'�x�cution doit �tre accessible en �criture et sera cr�� ou sera �cras� puis effac�.
 *
 * - Le programme commence par lire le fichier source en convertissant un � un tous les caract�res en ASCII en base 2 sur 8 bits et en les �crivant dans un autre fichier (fichbin.txt) sur une seule ligne sous forme binaire (un caract�re = 8 * '0' et '1').
 * - On s'assure que la taille de ce nouveau fichier soit un multiple de ent(log2(n)) (�quivalent � (nb de bits de n en base 2) - 1), sinon on compl�te avec des '0' � la fin.
 * - On �crit au d�but du fichier de destination le nombre de '0' ajout�s.
 * - On lit le nouveau fichier par bloc de ent(log2(n)) caract�res (car 1 caract�re = '0' ou '1' dans ce nouveau fichier).
 *  - Pour chaque bloc, on r�cup�re bloc^a % n, c'est notre message chiffr�. On compl�te ce nouveau chiffre par des zeros pour qu'il ait une taille ent(log2(n)) + 1, la taille maximale que le reste peut avoir.
 *  - On �crit ce nombre dans le fichier de destination
 * - On r�p�te la lecture jusqu'� ce qu'il n'y ait plus de blocs � lire.
 */

void chiffrer(char *fichier, char *fichdest, GRDNB n, GRDNB a)
{
    FILE *fich = fopen(fichier,"r");
    FILE *fichbin = fopen("fichbin.txt","w");
    FILE *fichcrypte = fopen(fichdest,"w");
    if(!(fich == NULL || fichcrypte == NULL || fichbin == NULL))
    {
        int lu = fgetc(fich) % 256;
        GRDNB base2,tmp = creerGRDNB(1);
        int compteur = 0;
        while(lu != EOF)
        {
            compteur+=8;
            base2 = IntToGRDNB(lu,2);
            while(base2.indicemax < 8)
                shiftdroite(&base2);
            fputs(GRDNBtoSTR(base2),fichbin);
            lu = fgetc(fich) % 256;
        }
        fclose(fichbin);
        fichbin = fopen("fichbin.txt","r+");
        fseek(fichbin,0,SEEK_END);

        int tailleBloc = n.indicemax - 1,nbzeros=0;
        printf("\n\nNB : Lecture par blocs de %d bits\n",tailleBloc);
        while(compteur%tailleBloc != 0)
        {
            fputc('0',fichbin);
            compteur++;
            nbzeros++;
        }
        printf("\n\n");
        fclose(fichbin);
        fichbin = fopen("fichbin.txt","r+");
        int i=0;
        fprintf(fichcrypte,"%d",nbzeros);
        fputc(' ',fichcrypte);
        char *bloc;
        GRDNB temp = creerGRDNB(1);
        int nbBlock = compteur/tailleBloc;
        printf("Chiffrement en cours...");
        for(i=0; i<nbBlock; i++)
        {
            bloc = (char*)malloc((tailleBloc+1)*sizeof(char));
            fgets(bloc,tailleBloc+1,fichbin);
            affectation(&tmp,str2grdnb(bloc));
            supprimezero(&tmp);
            temp = expomodrap(tmp,a,n,2);
            while(temp.indicemax < tailleBloc + 1)
                shiftdroite(&temp);
            fputs(GRDNBtoSTR(temp),fichcrypte);
            printf("..");
            free(temp.tableau);
            free(bloc);
        }
        printf("\n\n");
        free(fichier);
        fclose(fich);
        fclose(fichbin);
        fclose(fichcrypte);
        remove("fichbin.txt");
    }
    else
    {
        printf("\nNous n'avons pas reussi a ouvrir les fichiers necessaires.\n");
    }
}

/**
 * \fn void dechiffrer(char *fichier, char *fichdest, GRDNB n, GRDNB a)
 * \brief Dechiffre un fichier selon la clef a (e ou d, tout d�pend le contexte de l'appel, v�rification de signature ou d�chiffrement) et son produit pq = n et �crit le r�sultat dans fichdest.
 * \param[in] fichier Chaine de caract�res contenant le chemin relatif ou absolu du fichier � d�chiffrer, accessible en lecture.
 * \param[in] fichdest Chaine de caract�res contenant le chemin relatif ou absolu du fichier d�chiffr�, accessible en �criture, cr�� s'il n'existe pas et �cras� s'il existe.
 * \param[in] n C'est le produit p*q en base 2, qui va servir ici de modulo � toutes les donn�es chiffr�es. nb de bits(n) > 8 (taille d'un caract�re).
 * \param[in] a C'est l'exposant en base 2 de toutes les donn�es chiffr�es, d ou e selon que l'on d�chiffre avec sa clef priv�e ou que l'on v�rifie une signature avec la clef publique de l'�metteur.
 *
 * \warning "dechifbin.txt", dans le r�pertoire d'�x�cution doit �tre accessible en �criture et sera cr�� ou sera �cras� puis effac�.
 *
 * - On lit dans le fichier chiffr� le nombre de zeros ajout�s.
 * - On lit le fichier chiffr� par bloc de ent(log2(n)) caract�res, donc nb de bits(n) - 1.
 *  - Pour chaque bloc, on enl�ve les z�ros devant, on r�cup�re bloc^a % n, c'est notre message d�chiffr�. On compl�te ce nouveau chiffre par des zeros pour qu'il ait une taille ent(log2(n)), la taille maximale que le reste peut avoir.
 *  - Si on est sur le dernier bloc, on enl�ve les zeros ajout�s � la fin s'il y en a.
 *  - On �crit ce nombre dans le fichier temporaire
 * - On r�p�te la lecture jusqu'� ce qu'il n'y ait plus de blocs � lire.
 * - Le programme recommence � lire le fichier temporaire en convertissant les '0' et les '1' par bloc de 8 en caract�res en ASCII et en �crivant le caract�re dans le fichier de destination.
 *
 */

void dechiffrer(char *fichier, char *fichdest, GRDNB n, GRDNB a)
{
    FILE *fich = fopen(fichier,"r");
    FILE *dechif = fopen(fichdest,"w");
    FILE *dechifbin=fopen("dechifbin.txt","w");
    if(!(fich == NULL || dechif ==NULL || dechifbin == NULL))
    {
        int tailleBloc = n.indicemax - 1;
        char *bloc;
        GRDNB chiffre = creerGRDNB(1);
        int compteur = 0;
        fseek(fich,0,SEEK_END);
        int taille = ftell(fich);
        fseek(fich,0,SEEK_SET);
        int nbzeros = 0;
        int test = fscanf(fich,"%d ",&nbzeros);
        if(test != 1)
        {
            printf("\nERREUR FATALE, votre fichier a dechiffrer n'est pas au format valide\n");
            fclose(fich);
            fclose(dechif);
            fclose(dechifbin);
            remove("dechifbin.txt");
            return;
        }
        taille -= tailleNombre(nbzeros,10) + 1;
        GRDNB tmp;
        printf("\n\nDechiffrement...");
        int i;
        while(compteur < taille)
        {

            bloc = (char*)malloc((tailleBloc+2)*sizeof(char));
            verifErreurAlloc(bloc);
            fgets(bloc,tailleBloc+2,fich);
            tmp = str2grdnb(bloc);
            if(tmp.signe < 0)
            {
                printf("\nERREUR FATALE, votre fichier a dechiffrer n'est pas au format valide\n");
                fclose(fich);
                fclose(dechif);
                fclose(dechifbin);
                remove("dechifbin.txt");
                return;
            }
            supprimezero(&tmp);
            chiffre = expomodrap(tmp,a,n,2);
            while(chiffre.indicemax<tailleBloc)
                shiftdroite(&chiffre);
            if(compteur == taille - tailleBloc - 1)
            {
                for(i=0;i<nbzeros;i++)
                {
                    (chiffre.indicemax)--;
                    chiffre.tableau=(int*)realloc(chiffre.tableau,chiffre.indicemax*sizeof(int));
                    verifErreurAlloc(chiffre.tableau);
                }
            }
            fputs(GRDNBtoSTR(chiffre),dechifbin);
            printf("..");
            compteur+=tailleBloc+1;
            free(bloc);
            free(tmp.tableau);
            free(chiffre.tableau);
        }
        fclose(dechifbin);
        printf("\n");
        dechifbin = fopen("dechifbin.txt","r");
        fseek(dechifbin,0,SEEK_END);
        int tailledefichbin = ftell(dechifbin);
        fseek(dechifbin,0,SEEK_SET);
        char carac[9],*ptr;
        int compteur2=0;
        while(compteur2<tailledefichbin)
        {
            fgets(carac,9,dechifbin);
            fputc(strtol(carac,&ptr,2),dechif);
            compteur2+=8;
        }
        fclose(fich);
        fclose(dechif);
        fclose(dechifbin);
        remove("dechifbin.txt");
    }
    else
    {
        printf("\nNous n'avons pas reussi a ouvrir les fichiers necessaires.\n");
    }
}

/** \fn int tailleNombre(int nombre, int base)
 * \brief Permet de savoir quelle taille le nombre prend � �crire dans une base donn�e.
 *
 * \param[in] nombre Un entier � tester.
 * \param[in] base La base dans laquelle on veut savoir la taille.
 * \return Un entier, le nombre de chiffres n�cessaires pour �cire nombre dans la base donn�e.
 *
 * Il suffit de compter le nombre de fois que le nombre est divisible enti�rement par la base donn�e.
 *
 */

int tailleNombre(int nombre, int base)
{
    int compteur;
    if(nombre==0)
        return 1;
    for(compteur = 0; nombre > 0; compteur++)
        nombre/=base;
    return compteur;
}

