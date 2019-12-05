#include <stdio.h>
#include <stdlib.h>
#include "grdnb.h"
#include "chaines.h"
#include <string.h>
#include "pointeurs.h"
#include <math.h>

/** \file grdnb.c
 *
 * \brief Contient des fonctions permettant de g�n�rer et travailler avec des grands nombres.
 * \author I. Laruelle / A. Masciulli - UTT NF05 A18
 *
 */

/** \fn GRDNB creerGRDNB(int taille)
 * \brief Cr�e un grand nombre
 * \param[in] taille La taille du tableau du GRDNB � cr�er.
 * \return Un GRDNB dont chaque case du tableau allou� dynamiquement vaut 0.\n Le signe vaut 1.\n Indicemax vaut taille.
 *
 */

GRDNB creerGRDNB(int taille)
{
    GRDNB retour;
    if(taille == 0)
        taille++;
    retour.tableau = (int*)calloc(taille,sizeof(int));
    verifErreurAlloc(retour.tableau);
    retour.indicemax = taille;
    retour.signe=1;
    return retour;
}

/** \fn GRDNB un()
 * \brief Cr�e un grand nombre valant 1.
 * \return Un GRDNB de taille 1, de signe 1 et tableau[0] = 1.
 *
 */
GRDNB un()
{
    GRDNB test = creerGRDNB(1);
    test.tableau[0] = 1;
    return test;
}

/** \fn GRDNB diveuclide(GRDNB a, GRDNB b, int base, GRDNB *quotient)
 * \brief Permet d'exprimer a sous la forme <em>a = b * quotient + retour, avec retour < b</em>
 *
 * \param[in] a Un GRDNB positif
 * \param[in] b Un GRDNB positif diff�rent de z�ro.
 * \param[in] base La base dans laquelle a,b, le quotient et le reste sont exprim�s.
 * \param[in] quotient L'adresse pointant sur le GRDNB du quotient.
 * \return Le reste sous forme de GRDNB
 * \note Si seul le reste vous int�resse, allez sur GRDNB modulo(GRDNB a, GRDNB b, int base)
 *
 * On regarde si des cas simples se pr�sentent, en effet :
 * - Si l'on veut a/1, on a <em>a = 1*a + 0</em>. (quotient = a, reste = 0)
 * - Si l'on veut a/a, on a <em>a = a*1 + 0</em>. (quotient = 1, reste = 0)
 * - Si l'on veut a/b et b > a, on a <em>a = b*0 + a</em>. (quotient = 0, reste = a).
 *
 * Sinon on determine la taille maximale du quotient (taille de a - taille de b + 1).
 *
 * On regarde ensuite combien de fois a peut contenir la puissance maximale du quotient dans la base correspondante.\n
 * Tant que <em>a > b * puissancemax</em>, on diminue a : <em>a = a - b * puissancemax</em> et on incr�mente autant de fois la case du quotient correspondante.\n
 * On diminue la valeur de a autant que possible.\n
 * On d�cr�mente la taille pour avoir la puissance inf�rieure et on recommence tant que a >= b.\n
 * Il est possible que la premi�re case du quotient soit nulle, on la supprime si c'est le cas.\n
 * Le reste, c'est le a restant quand on a <em>a < b</em>.
 *
 * Exemple : 520 / 3\n
 * Le quotient sera au maximum 999 (donc de taille 3). En effet, on peut avoir (au pire), avec des nombres de cette taille, 999 / 1 (le quotient sera 999 donc bien de taille 3).\n
 * - On calcule 10^2 (la puissance de la case 0 du quotient) : 100. On multiplie par b donc on a 3*100 = 300.\n
 * On ne peut soustraire qu'une seule fois 300 dans 520, donc a <em>a = 520 - 1 * 300 = 220</em>. On a donc 1 fois quotient.tableau[0]++ donc la case 0 du quotient vaut donc 1.\n
 * On a toujours a >= b, on continue.
 * - On calcule 10^1 = 10. On multiplie par b : 3*10 = 30.\n
 * On peut peut soustraire 7 fois 30 dans 220, donc on a <em>a = 220 - 7*30 = 10</em>. On a donc 7 fois quotient.tableau[1]++ donc la case 1 du quotient vaut donc 7.\n
 * On a toujours a >= b, on continue.
 * - On calcule 10^0 = 1. On multiplie par b : 3 * 1 = 3.\n
 * On peut peut soustraire 3 fois 3 dans 10, donc on a <em>a = 10 - 3*3 = 1</em>. On a donc 3 fois quotient.tableau[2]++ donc la case 2 du quotient vaut donc 3.\n
 * On n'a plus a >= b, on s'arr�te.\n
 * Le reste vaut la valeur de a, c'est-�-dire 1.
 *
 * On a bien <em>520 = 3 * 173 + 1</em>
 */

GRDNB diveuclide(GRDNB a, GRDNB b, int base, GRDNB *quotient)
{
    if(b.indicemax==1 && b.tableau[0]==1)
    {
        copie(a,quotient);
        return creerGRDNB(1);
    }
    else if(egal(a,b))
    {
        *quotient = creerGRDNB(1);
        quotient->tableau[0]=1;
        return creerGRDNB(1);
    }
    else if(ismax(b,a))
    {
        *quotient = creerGRDNB(1);
        GRDNB retour;
        copie(a,&retour);
        return retour;
    }
    else
    {
        GRDNB mul,res;
        GRDNB acopie;
        copie(a,&acopie);
        int taille = a.indicemax - b .indicemax + 1;
        *quotient = creerGRDNB(taille);
        while(ismax(acopie,b))
        {
            mul = creerGRDNB(taille);
            mul.tableau[0] = 1;
            res = mulnaive(b,mul,base);
            while(ismax(acopie,res))
            {
                affectation(&acopie,soustraction(acopie,res,base));
                quotient->tableau[quotient->indicemax - taille]++;
            }
            free(mul.tableau);
            free(res.tableau);
            taille--;
        }
        supprimezero(quotient);
        return acopie;
    }
}

/** \fn GRDNB modulo(GRDNB a, GRDNB b, int base)
 * \brief Permet d'exprimer a sous la forme <em>a = b * quotient + retour, avec retour < b</em>
 *
 * \param[in] a Un GRDNB positif
 * \param[in] b Un GRDNB positif diff�rent de z�ro.
 * \param[in] base La base dans laquelle a, b et le reste sont exprim�s.
 * \return Le reste sous forme de GRDNB
 * \note Si avoir le quotient et le reste vous int�resse, allez sur GRDNB diveuclide(GRDNB a, GRDNB b, int base, GRDNB *quotient)
 *
 * On regarde si des cas simples se pr�sentent, en effet :
 * - Si l'on veut a/1, on a <em>a = 1*a + 0</em>. (reste = 0)
 * - Si l'on veut a/a, on a <em>a = a*1 + 0</em>. (reste = 0)
 * - Si l'on veut a/b et b > a, on a <em>a = b*0 + a</em>. (reste = a).
 *
 * Sinon on determine la taille maximale du quotient (taille de a - taille de b + 1).
 *
 * On regarde ensuite combien de fois a peut contenir la puissance maximale du quotient dans la base correspondante.\n
 * Tant que <em>a > b * puissancemax</em>, on diminue a : <em>a = a - b * puissancemax</em>\n
 * On diminue la valeur de a autant que possible.\n
 * On d�cr�mente la taille pour avoir la puissance inf�rieure et on recommence tant que a >= b.\n
 * Le reste, c'est le a restant quand on a <em>a < b</em>.
 *
 * Exemple : 520 / 3\n
 * Le quotient sera au maximum 999 (donc de taille 3). En effet, on peut avoir (au pire), avec des nombres de cette taille, 999 / 1 (le quotient sera 999 donc bien de taille 3).\n
 * - On calcule 10^2 (la puissance de la case 0 du quotient) : 100. On multiplie par b donc on a 3*100 = 300.\n
 * On ne peut soustraire qu'une seule fois 300 dans 520, donc a <em>a = 520 - 1 * 300 = 220</em>.\n
 * On a toujours a >= b, on continue.
 * - On calcule 10^1 = 10. On multiplie par b : 3*10 = 30.\n
 * On peut peut soustraire 7 fois 30 dans 220, donc on a <em>a = 220 - 7*30 = 10</em>.\n
 * On a toujours a >= b, on continue.
 * - On calcule 10^0 = 1. On multiplie par b : 3 * 1 = 3.\n
 * On peut peut soustraire 3 fois 3 dans 10, donc on a <em>a = 10 - 3*3 = 1</em>.\n
 * On n'a plus a >= b, on s'arr�te.\n
 * Le reste vaut la valeur de a, c'est-�-dire 1.
 *
 * On a bien <em>520 = 3 * X + 1</em>
 */
GRDNB modulo(GRDNB a, GRDNB b, int base)
{
    if(b.indicemax==1 && b.tableau[0]==1)
    {
        return creerGRDNB(1);
    }
    else if(egal(a,b))
    {
        return creerGRDNB(1);
    }
    else if(ismax(b,a))
    {
        GRDNB copiee;
        copie(a,&copiee);
        return copiee;
    }
    else
    {
        GRDNB mul,res;
        GRDNB acopie;
        copie(a,&acopie);
        int taille = a.indicemax - b .indicemax + 1;
        while(ismax(acopie,b))
        {
            mul = creerGRDNB(taille);
            mul.tableau[0] = 1;
            res = mulnaive(b,mul,base);
            while(ismax(acopie,res))
            {
                affectation(&acopie,soustraction(acopie,res,base));
            }
            free(mul.tableau);
            free(res.tableau);
            taille--;
        }
        return acopie;
    }
}

/** \fn int tailleIdentique(GRDNB a,GRDNB b, GRDNB *acopie, GRDNB *bcopie)
 * \brief Permet de mettre deux grands nombres � la m�me taille sans les modifier.
 *
 * \param[in] a Un GRDNB.
 * \param[in] b Un GRDNB.
 * \param[out] acopie Un pointeur sur le GRDNB qui contiendra la copie de a de m�me taille que bcopie.
 * \param[out] bcopie Un pointeur sur le GRDNB qui contiendra la copie de b de m�me taille que acopie.
 * \return Un entier, la taille de acopie et bcopie, equivalente � max(a.indicemax,b.indicemax).
 *
 * L'agorithme prend le plus grand des deux, et le copie directement case par case dans le pointeur sur le GRDNB correspondant.\n
 * Pour le plus petit, il va cr�er un GRDNB dans le pointeur correspondant avec un tableau de taille du plus grand.\n
 * Pour les cases de 0 � |a.indicemax - b.indicemax| exclu, on ne met que des z�ros, puis ensuite on copie case par case du petit nombre vers ce tableau.\n
 * Si les deux nombres sont de taille identique, il copie (case par case) les deux nombres dans les pointeurs correspondants.
 *
 * Initialement pr�vue pour impl�menter l'algorithme de Karatsuba pour la multiplication, elle est utilis�e dans de nombreuses autres fonctions pour faciliter leur impl�mentation.
 */

int tailleIdentique(GRDNB a,GRDNB b, GRDNB *acopie, GRDNB *bcopie)
{
    int difference, i,max=0;
    GRDNB completer;
    difference = abs(a.indicemax - b.indicemax);
    if(a.indicemax > b.indicemax)
    {
        max = a.indicemax;
        completer = creerGRDNB(a.indicemax);
        for(i=0;i<difference;i++)
            completer.tableau[i]=0;
        for(i=difference;i<completer.indicemax;i++)
            completer.tableau[i]=b.tableau[i-difference];
        *bcopie=completer;
        *acopie = creerGRDNB(a.indicemax);
        for(i=0;i<a.indicemax;i++)
            acopie->tableau[i] = a.tableau[i];
    }
    else if(a.indicemax < b.indicemax)
    {
        max = b.indicemax;
        completer = creerGRDNB(b.indicemax);
        for(i=0;i<difference;i++)
            completer.tableau[i]=0;
        for(i=difference;i<completer.indicemax;i++)
            completer.tableau[i]=a.tableau[i-difference];
        *acopie=completer;
        *bcopie = creerGRDNB(b.indicemax);
        for(i=0;i<b.indicemax;i++)
            bcopie->tableau[i] = b.tableau[i];
    }
    else
    {
        max = a.indicemax;
        *acopie = creerGRDNB(a.indicemax);
        for(i=0;i<a.indicemax;i++)
            acopie->tableau[i] = a.tableau[i];
        *bcopie = creerGRDNB(b.indicemax);
        for(i=0;i<b.indicemax;i++)
            bcopie->tableau[i] = b.tableau[i];
    }
    acopie->signe = a.signe;
    bcopie->signe = b.signe;
    return max;
}

/** \fn void copie(GRDNB a, GRDNB *acopie)
 *
 * \param[in] a Le GRDNB � copier.
 * \param[out] acopie Le pointeur sur le GRDNB qui va contenir la copie.
 *
 * \note Pourquoi ne pas faire directement acopie = a ?\n Si on fait free(acopie.tableau), on lib�rera le tableau de a aussi. Cette fonction permet de modifier acopie sans modifier a.
 *
 * On cr�e un grand nombre ayant une taille identique � a.\n
 * On copie case par case de a vers acopie, ainsi que le signe.
 */

void copie(GRDNB a, GRDNB *acopie)
{
    *acopie = creerGRDNB(a.indicemax);
    acopie->signe = a.signe;
    int i;
    for(i=0;i<a.indicemax;i++)
        acopie->tableau[i] = a.tableau[i];
}

/** \fn void affectation(GRDNB *a, GRDNB b)
 *
 * \brief Permet de faire a = b sans perte de m�moire
 * \param[in,out] a Un pointeur sur le GRDNB qui va recevoir l'affectation.
 * \param[in] b Le GRDNB qui va �tre affect�.
 *
 * - Si on a une boucle et que l'on a l'instruction\n    <em>a = soustraction(a,b,base);</em>\n
 * A chaque tour de boucle, la valeur de l'adresse du tableau de a va changer, mais les pr�c�dents tableaux ne seront pas lib�r�s.\n
 * Pour �viter cela, on lib�re le tableau de a avant le calcul.\n
 * - Probl�me : si on fait :\n<em>
 * free(a.tableau);\n
 * a = soustraction(a,b,base);</em>\n
 * Un incident se produira, car on a besoin du tableau de a pour faire la soustraction !
 *
 * - Il faut donc coder une fonction affectation qui s'appellera comme suit :\n    <em>affectation(&a,soustraction(a,b,base));</em>\n
 * Le code s'ex�cute de droite � gauche.\n
 * En arrivant dans la fonction, la soustraction est d�j� effectu�e, on peut donc lib�rer le tableau de a.
 *
 * - Une autre question s'est pos�e :\n
 * L'instruction *a=b peut laisser perplexe, car ce "b" est le b interne � la fonction, et effac� � la fin de celle-ci.\n
 * Or, quand on fait *a = b, C copie le contenu de b dans a (et une vraie copie). La valeur du tableau de b sera celle de a, ainsi que pour le signe...\n
 * Donc m�me si b est effac�, les valeurs seront sauvegard�es dans a. De plus, le tableau de b �tant allou� dynamiquement, il est conserv� sur la m�me adresse tant qu'il n'est pas lib�r� manuellement.
 */

void affectation(GRDNB *a, GRDNB b)
{
    free(a->tableau);
    *a=b;
}


/** \fn GRDNB mulnaive(GRDNB a, GRDNB b,int base)
 * \brief Permet de faire la multiplication entre deux GRDNB dans une base donn�e.
 * \param[in] a Un GRDNB
 * \param[in] b Un GRDNB
 * \return Un GRDNB : a * b
 *
 * Algorithme de multiplication naive.
 *
 * - Pour la base 2:\n
 *  111\n
 *  * 10\n
 *  = 0 * 111 + (1 * 111 << 1).\n
 *  Ce qui est pratique en base 2, c'est que l'on multiplie soit par z�ro, soit par un. On ne fait donc pas de multiplication, mais juste des additions.\n
 *  Pour un indice donn� de b, si cela vaut 1, on additionne a au r�sultat d�cal� par rapport � la position de b.
 *
 * - Pour une autre base :\n
 *  On met les nombres � la m�me taille.\n
 *  - On fait varier l'indice de a et on fixe l'indice de b
 *  - Pour chaque indice de a on forme la multiplication unique de la case de a par celle de b.\n
 *  - On d�cale ce r�sultat en fonction de la position de l'indice de a et on l'ajoute � la ligne interm�diaire.
 *  On d�cale ligne en fonction de l'indice de b en cours la ligne interm�diaire.
 *  On ajoute cette ligne au r�sultat et on recommence jusqu'� avoir parcouru tout le tableau de b.
 *
 */

GRDNB mulnaive(GRDNB a, GRDNB b,int base)
{
    int nbmax,ii,i,j;
    if(base == 2)
    {
        GRDNB acopie = creerGRDNB(1), retour = creerGRDNB(1);
        for(i=b.indicemax-1;i>=0;i--)
        {
            free(acopie.tableau);
            copie(a,&acopie);
            if(b.tableau[i])
            {
                for(j=0;j<b.indicemax-1-i;j++)
                    vraishiftgauche(&acopie);
                affectation(&retour,somme(retour,acopie,2));
            }
        }
        free(acopie.tableau);
        return retour;
    }
    GRDNB acopie, bcopie,tmp = creerGRDNB(1), *tmp2, sommefinale = creerGRDNB(1);
    nbmax = tailleIdentique(a,b,&acopie,&bcopie);
    tmp2 = (GRDNB*)calloc(nbmax,sizeof(GRDNB));
    verifErreurAlloc(tmp2);
    for(i=0;i<nbmax;i++)
        tmp2[i]=creerGRDNB(1);
    for(ii=nbmax-1;ii>=0;ii--)
    {
        for(i=nbmax-1;i>=0;i--)
        {
            tmp = mulunique(acopie.tableau[i],bcopie.tableau[ii],base);
            for(j=nbmax-1;j>i;j--)
                vraishiftgauche(&tmp);
            affectation(tmp2+ii,somme(tmp2[ii],tmp,base));
            free(tmp.tableau);
        }
        for(j=0;j<-ii+nbmax-1;j++)
            vraishiftgauche(tmp2+ii);
    }
    for(i=0;i<nbmax;i++)
    {
        affectation(&sommefinale,somme(sommefinale,tmp2[i],base));
    }
    supprimezero(&sommefinale);
    sommefinale.signe=a.signe*b.signe;
    for(i=0;i<nbmax;i++)
        free(tmp2[i].tableau);
    free(tmp2);
    free(acopie.tableau);
    free(bcopie.tableau);
    return sommefinale;
}

/**\fn GRDNB euclideetendu (GRDNB a,GRDNB b, int base, GRDNB *u, GRDNB *v)
 * \brief Trouve le pgcd de deux entiers a et b et le couple(u,v) tel que au + bv = PGCD(a,b)
 * \param[in] a Un GRDNB
 * \param[in] b Un GRDNB tel que b < a
 * \param[in] base Un entier pr�cisant la base de a, de b, de u, de v ainsi que du PGCD retourn�.
 * \param[out] u Un pointeur sur le GRDNB recevant l'un des deux coefficients de B�zout.
 * \param[out] v Un pointeur sur le GRDNB recevant l'un des deux coefficients de B�zout. Ce coefficient sera positif.
 * \return Un GRDNB r le pgcd de a et b, un entier
 * Pour a>b, on commence par faire la division euclidienne de a par b, on obtient un quotient q et un reste r.\n
 * On obtient <em>1 * a - q * b = r (1)</em>\n
 * u prend la valeur 1 et v la valeur -q.\n
 *
 * - On fait ensuite la division euclidienne de b par r, c'est-�-dire que le diviseur devient le dividende et le reste devient le diviseur.
 * - On trouve un nouveau reste et un nouveau quotient tels que <em>1 * b � q�r = r�</em> et d�apr�s (1), on a <em>1 * b � q�(a - qb) = r�</em>.
 * - On a des nouvelles valeurs pour u = -q�et v = 1+qq�. On r�p�te ce sch�ma jusqu�� obtenir un reste de 0.
 * Le PGCD est le dernier reste diff�rent de 0 obtenu.
 * u et v sont somm�es � chaque tour de boucle suivant les quotients et restes obtenus par division euclidienne.
 *
 * On adapte la valeur de v pour que v soit positif en faisant (k = 1):
 * - <em> v = v + k*a</em>
 * - <em> u = u - k*b</em>
 * On incr�mente k tant que v est n�gatif.
 */
GRDNB euclideetendu(GRDNB a,GRDNB b, int base, GRDNB *u, GRDNB *v)
{
    GRDNB q, us,rs, vs, r=a, u2,v2, r2,k=un();
    *u=creerGRDNB(1);
    *v=creerGRDNB(1);
    u->tableau[0]=1;
    u2 = creerGRDNB(1);
    v2 = creerGRDNB(1);
    v2.tableau[0] = 1;
    r2=b;
    while(!(r2.indicemax==1 && r2.tableau[0]==0))
    {
        diveuclide(r,r2,base,&q);
        copie(r,&rs);
        us=*u;
        vs=*v;
        copie(r2,&r);
        *u=u2;
        *v=v2;
        r2 = soustraction(rs, mulnaive(q,r2,base),base);
        u2 = soustraction(us, mulnaive(q,u2,base),base);
        v2 = soustraction(vs, mulnaive(q,v2,base),base);
        free(q.tableau);
        free(rs.tableau);
        free(us.tableau);
        free(vs.tableau);
    }
    while(v->signe < 0)
    {
       *v=addition(*v,mulnaive(k,a,base),base);
       *u=soustraction(*u,mulnaive(k,b,base),base);
       k=somme(k,un(),base);
    }
    return r;
}

/**
 * \fn int egal(GRDNB a, GRDNB b)
 * \brief Permet de savoir si deux GRDNB sont �gaux.
 * \warning Le bit de poids fort de a et de b doit toujours �tre diff�rent de z�ro. Ex : "124" != "00124", car taille 3 et taille 5.\n Les deux nombres doivent �tre dans la m�me base.
 * \param[in] a Un GRDNB � comparer avec b.
 * \param[in] b Un GRDNB � comparer avec a.
 * \return Un entier : 1 si a et b sont �gaux\n 0 sinon.
 *
 * Le programme regarde d�j� s'il y a une diff�rence de taille entre a et b, si oui les nombres ne peuvent pas �tre �gaux.\n
 * Ensuite, on lit case par case. Si pour un m�me indice, la case de a est diff�rente de celle de b, alors les nombres ne sont pas �gaux.\n
 * Si on a parcouru toutes les cases sans trouver de diff�rences, alors les nombres a et b sont �gaux.\n
 *
 * Nous aurions pu traiter le cas "124" == "00124", en supprimant les z�ros devant � chaque fois, mais cela aurait n�cessit� de faire une copie, et aurait allong� le temps d'ex�cution.
 *
 */

int egal(GRDNB a, GRDNB b)
{
    if(a.indicemax != b.indicemax || a.signe * b.signe < 0)
        return 0;
    else
    {
        int i;
        for (i=0; i<a.indicemax ; i++)
        {
            if (a.tableau[i] != b.tableau[i])
                return 0;
        }
        return 1;
    }
}

/** \fn GRDNB mulunique(int a, int b, int base)
 *
 * \brief Permet de multiplier deux entiers et de renvoyer le r�sultat sous forme de GRDNB
 * \param[in] a Un entier a < base.
 * \param[in] b Un entier b < base.
 * \param[in] base base > 0. La base dans laquelle sont �crits a et b et sera �crit le r�sultat
 * \return Un GRDNB de taille au plus 2 comprenant le r�sultat de a*b dans la base sp�cifi�e.
 *
 * On fait a*b, si le r�sultat est sup�rieur � la base, alors on a besoin de deux cases.\n
 * La case 0 comprendra (a*b)/base et la case 1 (a*b)%base.\n
 * En effet : (a*b) = (a*b)/ base * base^1 + (a*b)%base * base^0.
 *
 */

GRDNB mulunique(int a, int b, int base)
{
        int test = a*b;
        GRDNB retour;
        if(test<base)
        {
            retour = creerGRDNB(1);
            retour.tableau[0]=test;
        }
        else
        {
            retour = creerGRDNB(2);
            retour.tableau[1]=test%base;
            retour.tableau[0]=test/base;
        }
        return retour;
}

/** \fn GRDNB sousnombre(GRDNB a, int debut, int fin)
 * \brief Recr�e un autre nombre � partir d'un autre nombre et d'une position de d�but (inclue) et de fin (exclue)
 *
 * \param[in] a Le GRDNB dont on va extraire le sous-nombre
 * \param[in] debut L'indice de d�but tel que debut < a.indicemax - 2.
 * \param[in] fin L'indice de fin tel que debut < fin < a.indicemax
 * \return Un GRDNB positif qui sera le nombre extrait de a de d�but � fin exclu.
 * \deprecated Anciennement utilis�e pour impl�menter l'algorithme de Karatsuba pour une multiplication rapide.
 *
 * Exemple : sousnombre("12345",1,3) = "23".
 *
 */

GRDNB sousnombre(GRDNB a, int debut, int fin)
{
    GRDNB retour = creerGRDNB(fin-debut);
    int i;
    for(i=debut;i<fin;i++)
        retour.tableau[i-debut]=a.tableau[i];
    return retour;
}

/** \fn GRDNB PuissanceRapide(GRDNB a, GRDNB puissance, int base)
 * \brief Calcule a^puissance dans la base sp�cifi�e.
 *
 * \param[in] a Le GRDNB positif que l'on veut multiplier.
 * \param[in] puissance L'exposant positif en base 10 ou 2 sous forme de GRDNB.
 * \param[in] base La base de a et du r�sultat (10 ou 2)
 * \return Un GRDNB valant a^puissance dans la base pr�cis�e
 *
 * a^n = a * a * a * ... * a (n fois) : n multiplications
 *
 * Le principe :\n
 * - a^(2k)   = a^k * a^k = (a*a)^k. On a juste � calculer une seule fois a^k\n
 * - a^(2k+1) = a * a^(2k).
 *
 * Exemple : a^5 = a * (a^4) = a * (a^2)^2 = a * (a*a)^2 = 3 multiplications au lieu de 5.
 *
 * Le programme regarde si la puissance est �gale � 1\n
 * - Si oui, il renvoie a.
 * - Sinon il regarde si la puissance est paire, si oui il renvoie (a*a)^(puissance/2)
 * - Si la puissance est impaire, il renvoie a * (a*a)^(puissance/2)
 *
 * Bien que cette fonction soit r�cursive, une attention particuli�re a �t� apport�e � la gestion de la m�moire.
 */
GRDNB PuissanceRapide(GRDNB a, GRDNB puissance, int base)
{
    GRDNB retour = creerGRDNB(1);
    GRDNB tmp2 = creerGRDNB(1);
    if(base == 2)
    {
        free(tmp2.tableau);
        copie(puissance,&tmp2);
    }
    if(puissance.indicemax == 1 && puissance.tableau[0] == 0)
    {
        retour = creerGRDNB(1);
        retour.tableau[0]=1;
    }
    else
    {
        GRDNB tmp = mulnaive(a,a,base);
        if(base == 2)
        {
            vraishiftdroite(&tmp2);
        }
        else
        {
            tmp2 = div2(puissance);
        }
        if(isMulDeux(puissance))
        {
            retour = PuissanceRapide(tmp,tmp2,base);
        }
        else
        {
            GRDNB tmp3 = PuissanceRapide(tmp,tmp2,base);
            retour = mulnaive(a,tmp3,base);
            free(tmp3.tableau);
        }
        free(tmp.tableau);
        free(tmp2.tableau);
    }
    return retour;
}

/** \fn char *GRDNBtoSTR(GRDNB a)
 * \brief Transforme un grand nombre en chaine de caract�res.
 *
 * \param[in] a Le GRDNB a convertir, base <= 10
 * \return Le GRDNB sous forme de chaine de caract�res.
 *
 * On sait que '0' s'�crit 48 en ASCII, '1' s'�crit 49...\n
 * Il suffit de prendre chaque chiffre et lui ajouter 48 pour obtenir son code ASCII.\n
 * On ajoute '\0' � la fin de la chaine nouvellement cr��e.
 */

char *GRDNBtoSTR(GRDNB a)
{
    char *retour = (char*)malloc((a.indicemax+1)*sizeof(char));
    verifErreurAlloc(retour);
    int i;
    for(i=0;i<a.indicemax;i++)
    {
        retour[i]=a.tableau[i]+48;
    }
    retour[i] = '\0';
    return retour;
}

/** \fn GRDNB IntToGRDNB(int a, int base)
 * \brief Transforme un int en GRDNB dans une base donn�e.
 *
 * \param[in] a Le int � convertir.
 * \param[in] base Doit �tre > 0.
 * \return Le GRDNB comprenant a dans la base donn�e.
 *
 * Pour savoir quelle est la taille du nombre dans une base donn�e, il suffit de compter le nombre de divisions enti�res successives possibles.
 *
 * On remplit le tableau du nouveau GRDNB � l'envers (des unit�s vers les bits de poids fort)\n
 * Pour r�cup�rer le caract�re des unit�s, il faut r�cup�rer le reste de la division de a par 10.\n
 * On divise a par 10 pour faire passer le chiffre des dizaines au chiffre des unit�s.\n
 * On r�p�te �a pour remplir le tableau de GRDNB.
 */

GRDNB IntToGRDNB(int a, int base)
{
    int b = a, compteur;
    if(a==0)
        return creerGRDNB(1);
    for(compteur = 0; b != 0 ; compteur++)
        b/=base;
    GRDNB retour = creerGRDNB(compteur);
    if(a < 0)
        retour.signe = -1;
    int i=0;
    for(i=0;i<retour.indicemax;i++)
    {
        retour.tableau[retour.indicemax-i-1] = a%base;
        a/=base;
    }
    return retour;
}

/** \fn GRDNB expomodrap(GRDNB message, GRDNB e, GRDNB n, int base)
 * \brief Permet de calculer message^e = resultat mod[n] dans une base donn�e.
 *
 * \param[in] message Un GRDNB positif qui sera �lev� � la puissance e
 * \param[in] e Un GRDNB positif qui sera l'exposant de message.
 * \param[in] n Le GRDNB positif qui modulera le r�sultat de message^e
 * \param[in] base L'entier correspondant la base de message, e, n et du r�sultat renvoy�.
 * \return Un GRDNB resultat tel que message^e = resultat mod[n]
 *
 * L'exponentiation modulaire rapide repose sur le principe suivant :\n
 * <em>a*b mod n = (a mod n) * (b mod n)</em>
 *
 * Si on �crit l'exposant en binaire, on a <em>message ^ e = produit(k=0, k < nbbits(exposant), (message^2k) ^ exposant.tableau[k])</em>
 * On a donc <em>resultat = produit(k=0, k < nbbits(exposant), (message^2k) ^ exposant.tableau[k]) mod n</em>
 * On fait donc, pour chaque bit de e, <em>resultat =  (resultat * message^((1 + le bit de e en cours)%2)) % n</em> puis <em>message = message * (message)</em>
 *
 * En pratique :\n
 * On pose resultat = 1.
 * Tant que l'exposant n'est pas nul :
 * - Si l'exposant n'est pas un multiple de 2 (c'est-�-dire son �criture binaire comporte 1 pour les unit�s), on a <em>resultat = (resultat * message) % n</em>
 * - Sinon on ne touche pas au r�sultat, car son �criture binaire comporte un 0 pour les unit�s.
 * - Dans tous les cas, on fait <em>message = (message * message) % n</em> et <em>exposant = exposant / 2 (division enti�re) pour passer au bit suivant de l'exposant</em>
 */

GRDNB expomodrap(GRDNB message, GRDNB e, GRDNB n, int base)
{
    GRDNB c = un();
    GRDNB quotient;
    GRDNB deux = str2grdnb("2");
    if(base == 2)
        deux = str2grdnb("10");
    GRDNB zero = creerGRDNB(1);
    GRDNB ecopie,messagecopie;
    copie(e,&ecopie);
    copie(message,&messagecopie);
    GRDNB tmp = creerGRDNB(1);
    while(ismax(ecopie,zero) && !egal(ecopie,zero))
    {
        if(!isMulDeux(ecopie))
        {
            affectation(&tmp,mulnaive(c,messagecopie,base));
            affectation(&c,modulo(tmp,n,base));
        }
        affectation(&tmp,mulnaive(messagecopie,messagecopie,base));
        affectation(&messagecopie,modulo(tmp,n,base));
        if(base == 10)
        {
            affectation(&ecopie,div2(ecopie));
        }
        else if(base == 2)
        {
            vraishiftdroite(&ecopie);
        }
        else
        {
            diveuclide(ecopie,deux,base,&quotient);
            free(ecopie.tableau);
            copie(quotient,&ecopie);
        }
        free(quotient.tableau);
    }
    free(zero.tableau);
    free(deux.tableau);
    free(ecopie.tableau);
    free(quotient.tableau);
    free(tmp.tableau);
    free(messagecopie.tableau);
    return c;
}

/** \fn GRDNB div2(GRDNB a)
 * \brief Permet de diviser (enti�rement) rapidement un nombre en base 10 par 2.
 *
 * \param[in] a Le GRDNB a diviser
 * \return a/2 sous la forme d'un GRDNB
 *
 * On divise le chiffre des unit�s par 2.\n
 * Ensuite pour chaque case en partant de la case � gauche de celle des unit�s, il faut regarder si le chiffre est pair.\n
 * S'il est pair, on divise par 2 directement.\n
 * Sinon, on divise par 2 et on ajoute 5 � la case de droite.
 */

GRDNB div2(GRDNB a)
{
    GRDNB acopie;
    copie(a,&acopie);
    int i;
    acopie.tableau[acopie.indicemax - 1] /= 2;
    for(i=acopie.indicemax-2;i>=0;i--)
    {
        if(acopie.tableau[i]%2==0)
            acopie.tableau[i] /= 2;
        else
        {
            acopie.tableau[i] /=2;
            acopie.tableau[i+1] +=5;
        }
    }
    supprimezero(&acopie);
    return acopie;
}

/** \fn int isMulDeux(GRDNB a)
 * \brief Permet de savoir si un nombre est divisible par 2
 *
 * \param[in] a Le GRDNB � tester.
 * \return 1 si le nombre est divisible par 2, 0 sinon.
 *
 * Il suffit de regarder le chiffre des unit�s. Si celui-ci est pair (son reste dans la division par 2 est 0), le nombre est divisible par deux, peu importe la base.
 *
 */

int isMulDeux(GRDNB a)
{
    return !(a.tableau[a.indicemax-1]%2);
}

/** \fn int ismax(GRDNB a, GRDNB b)
 * \brief Permet de savoir si a >= b.
 * \warning Le bit de poids fort de a et de b doit toujours �tre diff�rent de z�ro. Ex : ismax("124","0025") = 0, car taille 3 et taille 4.\n Les deux nombres doivent �tre dans la m�me base.
 *
 * \param[in] a Un GRDNB positif � tester.
 * \param[in] b Un GRDNB positif � tester.
 * \return Un int:\n1 si a >= b\n0 sinon.
 *
 * - Si la taille de a est plus grande que celle de b, on sait tout de suite que a >= b.
 * - Sinon on regarde pour chaque indice.
 *  - Si, pour un indice donn�, une case de b est strictement plus grande que celle de a, alors on retourne 0.
 *  - Si, pour un indice donn�, une case de a est strictement plus grande que celle de b, alors on retourne 1.
 * - Si on a parcouru tout le tableau sans apercevoir de diff�rence, alors a = b donc a >= b donc on renvoie 1.
 */

int ismax(GRDNB a, GRDNB b)
{
    int test = a.indicemax > b.indicemax;
    if(test)
        return test;
    test = b.indicemax > a.indicemax;
    if(test)
        return !test;
    int i;
    for (i=0; i<a.indicemax ; i++)
    {
        if (a.tableau[i] < b.tableau[i])
            return 0;
        if (a.tableau[i] > b.tableau[i])
            return 1;
    }
    return 1;
}

/** \fn GRDNB somme(GRDNB a, GRDNB b, int base)
 * \brief Permet d'additionner deux GRDNB positifs.
 *
 * \param[in] a Un GRDNB
 * \param[in] b Un GRDNB
 * \return Un GRDNB valant |a| + |b|
 *
 * On met les nombres � la m�me taille, et la retenue � 0.
 *
 * - Pour la base deux, on utilise un additionneur n bits :\n
 *  La retenue vaut ((acopie.tableau[i] XOR bcopie.tableau[i]) AND retenue) OR (acopie.tableau[i] AND bcopie.tableau[i])\n
 *  Le retour.tableau[i] vaut retenue XOR (acopie.tableau[i] XOR bcopie.tableau[i])\n
 *
 * - Pour une autre base, on additionne case par case avec la retenue.\n
 * Si le r�sultat de cette addition (case a + case b + retenue) est sup�rieur � la base,
 *  - la retenue vaudra le r�sultat / base
 *  - le retour.tableau[i] vaudra r�sultat%base (chiffre des unit�s)
 *
 * S'il reste une retenue � la fin du calcul, on ajoute une case au d�but du retour et met la retenue dedans.
 */

GRDNB somme(GRDNB a, GRDNB b, int base)
{
    int nbmax, test, retenue = 0, i;
    GRDNB acopie, bcopie;
    nbmax = tailleIdentique(a,b,&acopie,&bcopie);
    GRDNB retour = creerGRDNB(nbmax);
    if(base == 2)
    {
        int sprime;
        for(i=nbmax-1;i>=0;i--)
        {
            sprime = acopie.tableau[i]^bcopie.tableau[i];
            retour.tableau[i] = retenue ^ sprime;
            retenue = (sprime & retenue) | (acopie.tableau[i]&bcopie.tableau[i]);
        }
        if(retenue)
        {
            shiftdroite(&retour);
            retour.tableau[0]=1;
        }
        free(acopie.tableau);
        free(bcopie.tableau);
        return retour;
    }
    for(i=nbmax-1;i>=0;i--)
    {
        test = acopie.tableau[i]+bcopie.tableau[i]+retenue;
        if(test >= base)
        {
            retour.tableau[i] = test%base;
            retenue = test/base;
        }
        else
        {
            retour.tableau[i] = test;
            retenue = 0;
        }
    }
    if(retenue!=0)
    {
        shiftdroite(&retour);
        retour.tableau[0]=retenue;
    }
    free(acopie.tableau);
    free(bcopie.tableau);
    return retour;
}

/** \fn GRDNB soustraction(GRDNB a, GRDNB b, int base)
 * \brief Permet de faire a - b, peu importe leur signe et on peut avoir b > a.
 *
 * \param[in] a Un GRDNB
 * \param[in] b Un GRDNB
 * \param[in] base La base de a, b et du GRDNB renvoy�.
 * \return Un GRDNB : a - b.
 *
 * La fonction "sous(GRDNB a, GRDNB b, int base)" ne peut traiter que deux nombres positifs, avec a > b.\n Il faut donc faire une fonction qui va adapter les appels selon tous les cas possibles.
 *
 * - Si a et b positifs,
 *  - Si a > b, on renvoie directement |a| - |b|.
 *  - Si b > a, on renvoie -1 * (|b| - |a|).
 * - Si a positif, b n�gatif, on renvoie |a| + |b|
 * - Si a n�gatif, b positif, on renvoie -1 * (|a| + |b|)
 * - Si a et b n�gatifs, on recommence en les mettant positifs et on multiplie le r�sultat par -1.
 *
 */

GRDNB soustraction(GRDNB a, GRDNB b, int base)
{
    if(egal(a,b))
        return creerGRDNB(1);
    if(a.signe > 0 && b.signe > 0)
    {
        if(ismax(b,a))
        {
            GRDNB retour = sous(b,a,base);
            retour.signe=-1;
            return retour;
        }
        return sous(a,b,base);
    }
    if(a.signe > 0 && b.signe < 0)
        return somme(a,b,base);
    if(a.signe < 0 && b.signe > 0)
    {
        GRDNB retour = somme(a,b,base);
        retour.signe = -1;
        return retour;
    }
    GRDNB acopie = a, bcopie = b;
    acopie.signe = 1;
    bcopie.signe = 1;
    GRDNB retour = soustraction(acopie,bcopie,base);
    retour.signe *= -1;
    return retour;
}

/** \fn GRDNB addition(GRDNB a, GRDNB b, int base)
 * \brief Permet de faire a - b, peu importe leur signe et on peut avoir b > a.
 *
 * \param[in] a Un GRDNB
 * \param[in] b Un GRDNB
 * \param[in] base La base de a, b et du GRDNB renvoy�.
 * \return Un GRDNB : a + b.
 *
 * La fonction somme(GRDNB a, GRDNB b, int base) ne peut traiter que deux nombres positifs. Il faut donc faire une fonction qui va adapter les appels selon tous les cas possibles.
 *
 * - Si a positif, b n�gatif, on renvoie |a| - |b|
 * - Si b positif, a n�gatif, on renvoie |b| - |a|
 * - Si a et b n�gatifs, on renvoie -1 * (|a| + |b|)
 * - Sinon a et b sont positifs et on renvoie a + b
 *
 */

GRDNB addition(GRDNB a, GRDNB b, int base)
{
    if(a.signe > 0 && b.signe < 0)
    {
        GRDNB bcopie = b;
        bcopie.signe = 1;
        GRDNB retour = soustraction(a,bcopie,base);
        return retour;
    }
    if(a.signe < 0 && b.signe > 0)
    {
        GRDNB acopie = a;
        acopie.signe = 1;
        GRDNB retour = soustraction(b,acopie,base);
        return retour;
    }
    if(a.signe < 0 && b.signe < 0)
    {
        GRDNB acopie = a, bcopie = b;
        acopie.signe = 1;
        bcopie.signe = 1;
        GRDNB retour = somme(acopie,bcopie,base);
        retour.signe *= -1;
        return retour;
    }
    return somme(a,b,base);
}

/** \fn GRDNB sous(GRDNB a, GRDNB b, int base)
 * \brief Permet d'additionner deux GRDNB positifs avec a > b.
 *
 * \param[in] a Un GRDNB
 * \param[in] b Un GRDNB tel que b < a
 * \return Un GRDNB valant |a| - |b|
 *
 * On met les nombres � la m�me taille, et la retenue � 0.
 *
 * - Pour la base deux, on on additionne a et le compl�ment � 2 de b auquel on ajoute 1 puis on additionne (voir somme(GRDNB a, GRDNB b, int base)):\n
 *  La retenue vaut ((acopie.tableau[i] XOR bcopie.tableau[i]) AND retenue) OR (acopie.tableau[i] AND bcopie.tableau[i])\n
 *  Le retour.tableau[i] vaut retenue XOR (acopie.tableau[i] XOR bcopie.tableau[i])\n
 *  Cependant, s'il y a une retenue restante lors de l'addition, on l'oublie.
 *
 * - Pour une autre base, on fait la soustraction naive, c'est-�-dire que l'on soustrait case par case avec la retenue.\n
 * Si le r�sultat de cette soustraction (case a - (case b + retenue)) est n�gatif,
 *  - la retenue vaudra 1.
 *  - le retour.tableau[i] vaudra base + test. C'est-�-dire que l'on ajoute base et que l'on enl�ve base � la soustraction d'apr�s en mettant retenue � 1.\n
 *
 */
GRDNB sous(GRDNB a, GRDNB b, int base)
{
    int nbmax, test, retenue = 0, i;
    if(base == 2)
    {
        GRDNB acopie = creerGRDNB(1), bcopie = creerGRDNB(1);
        nbmax = tailleIdentique(a,b,&acopie,&bcopie);
        GRDNB compa2 = creerGRDNB(nbmax);
        for(i=0;i<nbmax;i++)
            compa2.tableau[i] = !bcopie.tableau[i];
        affectation(&compa2,somme(compa2,un(),2));
        GRDNB retour = creerGRDNB(nbmax);
        int sprime;
        for(i=nbmax-1;i>=0;i--)
        {
            sprime = acopie.tableau[i]^compa2.tableau[i];
            retour.tableau[i] = retenue ^ sprime;
            retenue = (sprime & retenue) | (acopie.tableau[i]&compa2.tableau[i]);
        }
        free(acopie.tableau);
        free(bcopie.tableau);
        free(compa2.tableau);
        supprimezero(&retour);
        return retour;
    }
    GRDNB acopie, bcopie;
    nbmax = tailleIdentique(a,b,&acopie,&bcopie);
    GRDNB retour = creerGRDNB(nbmax);
    for(i=nbmax-1;i>=0;i--)
    {
        test = acopie.tableau[i]-(bcopie.tableau[i]+retenue);
        if(test < 0)
        {
            retour.tableau[i] = base+test;
            retenue = 1;
        }
        else
        {
            retour.tableau[i] = test;
            retenue = 0;
        }
    }
    supprimezero(&retour);
    free(acopie.tableau);
    free(bcopie.tableau);
    return retour;
}

/** \fn void supprimezero(GRDNB *a)
 * \brief Permet de supprimer les z�ros superflus au d�but d'un GRDNB.
 * \param[in,out] a L'adresse du GRDNB dont on va supprimer les z�ros.
 *
 */

void supprimezero(GRDNB *a)
{
    while(a->tableau[0] == 0 && a->indicemax!=1)
        shiftgauche(a);
}

/** \fn int tailleBase2(GRDNB a)
 * \brief Permet de connaitre la taille en base 2 d'un GRDNB �crit en base 10.
 *
 * \param[in] a Le GRDNB en base 10 dont on veut avoir la taille en base 2.
 * \return Un int : la taille en base 2 de a.
 *
 * On compte le nombre de divisions enti�res par 2 que l'on peut faire � ce nombre.
 */

int tailleBase2(GRDNB a)
{
    GRDNB acopie;
    copie(a,&acopie);
    int compteur = 0;
    while(!(acopie.indicemax == 1 && acopie.tableau[0] == 0))
    {
        affectation(&acopie,div2(acopie));
        compteur++;
    }
    free(acopie.tableau);
    return compteur;
}

/** \fn GRDNB b10Tob2(GRDNB a, int taille)
 * \brief Permet de convertir un GRDNB de la base 10 � la base 2.
 *
 * \param[in] a Le GRDNB � convertir.
 * \param[in] taille La taille du nombre en base 2.\n Si taille = 0, on calcule la taille dans la fonction.
 * \return Un GRDNB : a en base 2.
 *
 */

GRDNB b10Tob2(GRDNB a, int taille)
{
    if (taille == 0)
        taille = tailleBase2(a);
    GRDNB retour = creerGRDNB(taille);
    GRDNB acopie;
    copie(a,&acopie);
    int compteur = 1;
    while(!(acopie.indicemax == 1 && acopie.tableau[0] == 0))
    {
        if(isMulDeux(acopie))
        {
            retour.tableau[retour.indicemax - compteur] = 0;

        }
        else
        {
            retour.tableau[retour.indicemax - compteur] = 1;
        }
        affectation(&acopie,div2(acopie));
        compteur++;
    }
    supprimezero(&retour);
    free(acopie.tableau);
    return retour;
}

/** \fn GRDNB b2Tob10(GRDNB a)
 * \brief Permet de convertir un GRDNB de la base 2 � la base 10.
 *
 * \param[in] a Le GRDNB � convertir.
 * \return Un GRDNB : a en base 10.
 *
 * On parcourt le tableau en base 2, et on ajoute au nouveau nombre � chaque 1 la puissance de deux correspondante.\n
 * Ex : "1011" = 1 * 2^0 + 1 * 2^1 + 1 * 2^3
 *
 */
GRDNB b2Tob10(GRDNB a)
{
    GRDNB retour = creerGRDNB(1);
    GRDNB deux = creerGRDNB(1);
    deux.tableau[0]=2;
    int i;
    for(i=a.indicemax-1;i>=0;i--)
    {
        if(a.tableau[i] != 0)
            retour = somme(retour,PuissanceRapide(deux,IntToGRDNB(a.indicemax-i-1,10),10),10);
    }
    supprimezero(&retour);
    return retour;
}

/** \fn int shiftdroite(GRDNB *a)
 * \brief Permet d'ajouter une case valant 0 � gauche d'un GRDNB.
 * \param[in,out] a Un pointeur sur le GRDNB � modifier
 * \warning Cette fonction N'EST PAS �quivalente � a >> 1, voir int vraishiftdroite(GRDNB *a).
 * \return Le nouvel indicemax.
 *
 * On ajoute une case � la fin du tableau, puis on fait descendre la derni�re case � l'indice 0 par �changes successifs.
 */

int shiftdroite(GRDNB *a)
{
        int k;
        (a->indicemax)++;
        a->tableau=(int*)realloc(a->tableau,a->indicemax*sizeof(int));
        verifErreurAlloc(a->tableau);
        for(k=a->indicemax-1;k>=1;k--)
            echanger(a->tableau,k,k-1);
        a->tableau[0]=0;
        return a->indicemax;
}

/** \fn int vraishiftdroite(GRDNB *a)
 * \brief Permet de supprimer la derni�re case du tableau de a.
 * \param[in,out] a Un pointeur sur le GRDNB � modifier, celui doit avoir une taille de 1 minimum
 * \return Le nouvel indicemax.
 *
 * \note Si a est en base b, faire vraishiftdroite(&a) <=> a = a/b.
 */
int vraishiftdroite(GRDNB *a)
{
        (a->indicemax)--;
        if(a->indicemax == 0)
        {
            a->tableau[0] = 0;
            a->indicemax++;
        }
        else
        {
            a->tableau=(int*)realloc(a->tableau,a->indicemax*sizeof(int));
        }
        verifErreurAlloc(a->tableau);
        return a->indicemax;
}

/** \fn int vraishiftgauche(GRDNB *a)
 * \brief Permet d'ajouter une case � 0 � la fin du tableau de a.
 * \param[in,out] a Un pointeur sur le GRDNB � modifier.
 * \return Le nouvel indicemax.
 *
 * \note Si a est en base b, faire vraishiftgauche(&a) <=> a = a*b.
 */
int vraishiftgauche(GRDNB *a)
{
        (a->indicemax)++;
        a->tableau=(int*)realloc(a->tableau,a->indicemax*sizeof(int));
        verifErreurAlloc(a->tableau);
        a->tableau[a->indicemax-1] = 0;
        return a->indicemax;
}

/** \fn int shiftgauche(GRDNB *a)
 * \brief Permet de supprimer la case la plus � gauche du tableau d'un GRDNB.
 * \param[in,out] a Un pointeur sur le GRDNB � modifier
 * \warning Cette fonction N'EST PAS �quivalente � a << 1, voir int vraishiftgauche(GRDNB *a).
 * \return Le nouvel indicemax.
 *
 * On fait monter la premi�re case tout � la fin du tableau par �changes successifs.\n
 * On r�alloue le tableau pour supprimer la derni�re case.
 */
int shiftgauche(GRDNB *a)
{
        if(a->indicemax == 1)
        {
            a->tableau[0] = 0;
            return 1;
        }
        int k;
        for(k=0;k<(a->indicemax-1);k++)
            echanger(a->tableau,k,k+1);
        (a->indicemax)--;
        a->tableau=(int*)realloc(a->tableau,a->indicemax*sizeof(int));
        verifErreurAlloc(a->tableau);
        return a->indicemax;
}

/** \fn void echanger(int *tableau, int i, int j)
 * \brief Echange deux cases dans un tableau d'entiers.
 * \param[in,out] Un tableau d'entiers qui verra deux de ses cases permut�es
 * \param[in] L'indice i < taille du tableau
 * \param[in] l'indice j < taille du tableau
 *
 */

void echanger(int *tableau, int i, int j)
{
    int temp = tableau[i];
    tableau[i]=tableau[j];
    tableau[j]=temp;
}

/** \fn void afficher(char *chaine, GRDNB a)
 * \brief Affiche dans la console un GRDNB pr�c�d� de son signe et d'une chaine de caract�re, et suivi d'un retour � la ligne.
 *
 * \param[in] chaine Une chaine de carct�re qui sera affich�e
 * \param[in] a Le GRDNB � afficher.
 *
 */

void afficher(char *chaine, GRDNB a)
{
    int i=0;
    printf("%s",chaine);
    if(a.signe>=0)
        printf("+");
    else
        printf("-");
    for(i=0; i<a.indicemax; i++)
        printf("%d",a.tableau[i]);
    printf("\n");
}

/** \fn GRDNB str2grdnb(char *chaine)
 * \brief Convertit un nombre �crit dans une chaine de caract�re en GRDNB.
 * \param[in] chaine Une chaine de caract�re ne comprenant �ventuellement au d�but le signe puis que des chiffres. La base doit �tre inf�rieure ou �gale � 10.
 * \return Un GRDNB correspondant � la chaine et la base pass�es en param�tre\nS'il y a une erreur, renvoie un GRDNB correspondant � -0.
 *
 * On lit le premier caract�re pour regarder le signe.\n
 * Si le signe n'est pas pr�cis�, le nombre est positif.\n
 * Dans la table ASCII, '0' s'�crit 48, '1' s'�crit 49 ...\n
 * Il suffit de faire carac - 48 pour r�cup�rer la valeur �crite dans le caract�re.
 */

GRDNB str2grdnb(char *chaine)
{
    int taille = strlen(chaine);
    GRDNB retour;
    if(chaine[0]=='-' || chaine[0]=='+')
    {
        taille--;
        retour = creerGRDNB(taille);
        if(chaine[0]=='-')
            retour.signe=-1;
    }
    else
        retour = creerGRDNB(taille);
    int i;
    for(i=strlen(chaine) - taille;i<retour.indicemax;i++)
    {
        if(chaine[i] < '0' || chaine[i] > '9')
        {
            free(retour.tableau);
            retour = creerGRDNB(1);
            retour.signe = -1;
            return retour;
        }
        retour.tableau[i]=chaine[i] - 48;
    }
    return retour;
}

/** \fn int isBase2(GRDNB a)
 * \brief Permet de savoir si le tableau d'un GRDNB ne comprend que des 0 ou des 1
 *
 * \param[in] a Le GRDNB � tester.
 * \return Un entier valant\n 1 si le nombre ne s'�crit qu'avec des un et des z�ros\n 0 sinon
 *
 * On parcourt case par case, et d�s que l'on trouve un chiffre diff�rent de 0 ou 1, on renvoie 0.\n
 * Si on a atteint la fin du nombre, on renvoie 1.
 */

int isBase2(GRDNB a)
{
    int i;
    for(i=0;i<a.indicemax;i++)
    {
        if(a.tableau[i] != 1 && a.tableau[i] != 0)
            return 0;
    }
    return 1;
}
