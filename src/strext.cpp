//<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

/*
########################################################

This file is a part of InstallDesktop project and/or
Evika project.

InstallDesktop is source closed and cover by a NDA.
It's totaly forbidden to publish any information
from the source close or re-use them.

By accessing this file (read, modify or write), you
accept the non disclosure agreement without any
restriction.

########################################################

InstallDesktop
Designed, Written and Directed by Alexia V. Gossa

Copyright (c) 2018 Editions JOCATOP

Editions JOCATOP
950 route de Réalpanier
84310 MORIERES

########################################################
*/


#ifdef _WIN32
#pragma warning(disable : 4996)
#include <conio.h>
#include <direct.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strext.h"

static int lStrInit = 0;
static char cListStrUCase[256];


char * strxucase(char * psz);


void _strxInit(void)
{
    int i;
    char szTmp[1024];

    if (lStrInit) return;
    lStrInit = 1;

    for (i = 0; i<256; i++)
        szTmp[i] = (char)i;

    szTmp[0] = 1;
    szTmp[256] = 0;

    strxucase(szTmp);

    szTmp[0] = 0;
    memcpy(cListStrUCase, szTmp, 256);
}

/***
*char *stristr(string1, string2) - search for string2 in string1
*
*Purpose:
*       finds the first occurrence of string2 in string1
*
*Entry:
*       char *string1 - string to search in
*       char *string2 - string to search for
*
*Exit:
*       returns a pointer to the first occurrence of string2 in
*       string1, or NULL if string2 does not occur in string1
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

char * strxistr(const char * str1, const char * str2)
{
    char *cp = (char *)str1;
    char *s1, *s2;

    if (!*str2)
        return((char *)str1);

    //Initialisation des chaines
    if (!lStrInit) _strxInit();

    while (*cp)
    {
        s1 = cp;
        s2 = (char *)str2;

        while (*s1 && *s2 && !(cListStrUCase[*s1] - cListStrUCase[*s2]))
            s1++, s2++;

        if (!*s2)
            return(cp);

        cp++;
    }

    return(NULL);
}


/***
*char *strichr(string, c) - search a string for a character
*
*Purpose:
*       Searches a string for a given character, which may be the
*       null character '\0'.
*
*Entry:
*       char *string - string to search in
*       char c - character to search for
*
*Exit:
*       returns pointer to the first occurence of c in string
*       returns NULL if c does not occur in string
*
*Exceptions:
*
*******************************************************************************/

char * strxichr(const char * string, int ch)
{

    //Initialisation des chaines
    if (!lStrInit) _strxInit();

    ch &= 0xFF;

    while (*string && cListStrUCase[*string] != cListStrUCase[ch])
        string++;

    if (cListStrUCase[*string] == cListStrUCase[ch])
        return((char *)string);

    return(NULL);
}



void strxcutonlyascii(char * psz)
{
    int i;
    for (i = 0; ((unsigned)psz[i] >= 32) || (psz[i] == 9); i++);
    psz[i] = 0;
}

void strxcutonlylistchar(char * psz, char * pszListChar)
{
    int i;
    for (i = 0; psz[i]; i++)
    {
        if (strchr(pszListChar, psz[i]) == 0)
        {
            psz[i] = 0;
            return;
        }
    }
}

void strxsupprstr(char * pszIO, char * pszPattern)
{
    char * psz;

    do {
        psz = strstr(pszIO, pszPattern);
        if (psz) memcpy(psz, psz + strlen(pszPattern), (strlen(psz) + 1) - (strlen(pszPattern) + 1) + 1);
    } while (psz);
}

void strxsupprchar(char * pszIO, char cChar)
{
    char szTmp[2];
    szTmp[0] = cChar;
    szTmp[1] = 0;
    strxsupprstr(pszIO, szTmp);
}

void strxsupprlistchar(char * pszIO, char * pszListChar)
{
    char szTmp[2];
    int i;
    szTmp[1] = 0;
    for (i = 0; pszListChar[i]; i++)
    {
        szTmp[0] = pszListChar[i];
        strxsupprstr(pszIO, szTmp);
    }
}

void strxkeeplistchar(char * pszIO, char * pszListChar)
{
    char szTmp[256];
    int i;

    //Génération de la liste des caractères à supprimer
    for (i = 32; i<255; i++) szTmp[i - 32] = (char)i;
    szTmp[i - 32] = 0;

    //Suppression des caractères que l'on veut conserver
    strxsupprlistchar(szTmp, pszListChar);

    //Suppression des caractères
    strxsupprlistchar(pszIO, szTmp);
}

#ifdef __APPLE__
    //Sous MacOS, le D rayé, le y accent grave et le Y accent grave n'existe pas MacRoman
    char szAvecAccent[] =

        //    Çüéâ äàåç
        "\x82\x9f\x8E\x89\x8a\x88\x8C\x8D"

        //    êëèï îìÄÅ
        "\x90\x91\x8F\x95\x94\x93\x80\x81"

        //    ÉæÆô öòûù
        "\x83\xBE\xAE\x99\x9A\x98\x9E\x9D"

        //    ÿÖÜá íóúñ
        "\xD8\x85\x86\x87\x92\x97\x9C\x96"

        //    ÑÌÁÂ ÀãÃD
        "\x84\xED\xE7\xE5\xCB\x8B\xCC\x44"

        //    ÊËÈi ÍÎÏÌ
        "\xE6\xE8\xE9\x69\xEA\xEB\xEC\xED"

        //    ÓÔÒõ ÕÚÛÙ
        "\xEE\xEF\xF1\x9B\xCD\xF2\xF3\xF4";


#else
    //Accent ISO-8859-1 ( Western ISO Latin 1 )
    char szAvecAccent[] =
        "\xc7\xfc\xe9\xe2\xe4\xe0\xe5\xe7"
        "\xea\xeb\xe8\xef\xee\xec\xc4\xc5"
        "\xc9\xe6\xc6\xf4\xf6\xf2\xfb\xf9"
        "\xff\xd6\xdc\xe1\xed\xf3\xfa\xf1"
        "\xd1\xcc\xc1\xc2\xc0\xe3\xc3\xd0"
        "\xca\xcb\xc8i\xcd\xce\xcf\xcc\xd3"
        "\xd4\xd2\xf5\xd5\xda\xdb\xd9\xfd\xdd";
#endif
char szSansAccent[] = "CUEAAAACEEEIIIAAEAAOOOUUYOUAIOUNNIAAAAADEEEIIIIIOOOOOUUUYY";
char szSameCase[]   = "CueaaaaceeeiiiAAEaAooouuyOUaiounNIAAAaADEEEiIIIIOOOoOUUUyY";

char * strxucase(char * psz)
{
    int i, j;
    char c, *p;
    for (i = 0; psz[i] != 0; i++)
    {
        c = (unsigned char)psz[i];

        //Convertion : a...z -> A...Z
        if ((c >= 97) && (c <= 122)) c -= 32;

        //Convertion des accents
        p = strchr(szAvecAccent, c);
        if (p != NULL)
        {
            j = (int)(p - szAvecAccent);
            c = (unsigned char)szSansAccent[j];
        }
        psz[i] = (char)c;
    }
    return psz;
}

char * strxnoaccent(char * psz)
{
    int i, j;
    char c, *p;

    for (i = 0; psz[i] != 0; i++)
    {
        c = (unsigned char)psz[i];

        //Convertion des accents
        p = strchr(szAvecAccent, c);
        if (p != NULL)
        {
            j = (int)(p - szAvecAccent);
            c = (unsigned char)szSameCase[j];
        }
        psz[i] = (char)c;
    }
    return psz;
}



char szListGoToUpperCase[256];
bool bInit = 0;

void strxucasefast(char * psz)
{
    int i;

    if (!bInit)
    {
        bInit = 1;
        for (i = 0; i<256; i++)
            szListGoToUpperCase[i] = (char)i;

        szListGoToUpperCase[0] = 1;
        szListGoToUpperCase[255] = 0;

        strxucase(szListGoToUpperCase);

        szListGoToUpperCase[0] = 0;
        szListGoToUpperCase[255] = (char)0xFF;
    }

    if (psz)
    {
        for (i = 0; psz[i]; i++)
        {
            psz[i] = szListGoToUpperCase[(unsigned char)psz[i]];
        }
    }
}

char * strxlcase(char * psz)
{
    int i;
    char c;

    strxucasefast(psz);

    if (psz)
    {
        for (i = 0; psz[i]; i++)
        {
            c = (unsigned char)psz[i];
            if ((c >= 'A') && (c <= 'Z')) c += 32;
            psz[i] = c;
        }
    }

    return psz;
}

int strxucasecmp(const char * pszSource, const char * pszTarget)
{
    int ret;
    unsigned char bSource, bTarget;

    //Init
    strxucasefast(NULL);
    ret = 0;

    do {
        bSource = (unsigned char)szListGoToUpperCase[(unsigned char)*pszSource];
        bTarget = (unsigned char)szListGoToUpperCase[(unsigned char)*pszTarget];
        pszSource++;
        pszTarget++;
    } while (!(ret = bSource - bTarget) && bTarget && bSource);

    if (ret<0)      ret = -1;
    else if (ret>0) ret = 1;

    return ret;
}

int strxnucasecmp(const char * pszSource, const char * pszTarget, int lCharCountMax)
{
    int ret, lCount;
    unsigned char bSource, bTarget;

    //Init
    strxucasefast(NULL);
    ret = 0;
    lCount = lCharCountMax;

    do {
        bSource = (unsigned char)szListGoToUpperCase[(unsigned char)*pszSource];
        bTarget = (unsigned char)szListGoToUpperCase[(unsigned char)*pszTarget];
        pszSource++;
        pszTarget++;
    } while (--lCount && !(ret = bSource - bTarget) && bTarget && bSource);

    if (ret<0)      ret = -1;
    else if (ret>0) ret = 1;

    return ret;
}

void strxbmemcpy(char * pszTarget, char * pszSource, int lSize)
{
    if (!lSize) return;
    pszTarget += lSize;
    pszSource += lSize;
    do {
        lSize--;
        pszTarget--;
        pszSource--;
        *pszTarget = *pszSource;

    } while (lSize);

}

void strxreplace(char * pszIO, char * pszPattern, char * pszReplacement)
{
    char * psz;
    int lPatternLength;
    int lReplacementLength;

    lPatternLength = (int)strlen(pszPattern);

    if (pszReplacement == NULL) lReplacementLength = 0;
    else                        lReplacementLength = (int)strlen(pszReplacement);
    psz = pszIO;
    do {
        psz = strstr(psz, pszPattern);
        if (psz)
        {
            //Déplacement de la chaine !
#ifdef _WIN32
            if (lReplacementLength>lPatternLength)
                strxbmemcpy(psz + lReplacementLength, psz + lPatternLength, (int)strlen(psz + lPatternLength) + 1);
            else
                memcpy(psz + lReplacementLength, psz + lPatternLength, (int)strlen(psz + lPatternLength) + 1);
#else
            memcpy(psz + lReplacementLength, psz + lPatternLength, strlen(psz + lPatternLength) + 1);
#endif
            if (pszReplacement) memcpy(psz, pszReplacement, lReplacementLength);
            psz += lReplacementLength;
        }
    } while (psz);
}

void strxireplace(char * pszIO, char * pszPattern, char * pszReplacement)
{
    char * psz;
    int lPatternLength;
    int lReplacementLength;

    lPatternLength = (int)strlen(pszPattern);
    if (pszReplacement == NULL) lReplacementLength = 0;
    else                      lReplacementLength = (int)strlen(pszReplacement);
    psz = pszIO;
    do {
        psz = strxistr(psz, pszPattern);
        if (psz)
        {
            //Déplacement de la chaine !
#ifdef _WIN32
            if (lReplacementLength>lPatternLength)
                strxbmemcpy(psz + lReplacementLength, psz + lPatternLength, (int)strlen(psz + lPatternLength) + 1);
            else
                memcpy(psz + lReplacementLength, psz + lPatternLength, (int)strlen(psz + lPatternLength) + 1);
#else
            memcpy(psz + lReplacementLength, psz + lPatternLength, strlen(psz + lPatternLength) + 1);
#endif
            if (pszReplacement) memcpy(psz, pszReplacement, lReplacementLength);
            psz += lReplacementLength;
        }
    } while (psz);
}

void strxcreplace(char * psz, char cOriginal, char cReplace)
{
    int i, n;
    n = (int)strlen(psz);
    for (i = 0; i<n; i++)
    {
        if (psz[i] == cOriginal) psz[i] = cReplace;
    }
}

void strxcremove(char * psz, char cRemove)
{
    char sTemp[1024];
    int i, j, n;

    n = (int)strlen(psz);
        
    for (i = 0, j = 0; i<n; i++)
    {
        if (psz[i] != cRemove)
        {
            psz[j] = psz[i];
            j++;
        }
    }
    psz[j] = 0;

}

void strxsupprallright(char * psz, char cChar)
{
    int i, n;
    n = (int)strlen(psz);

    if (!n) return;

    for (i = n - 1; i >= 0; i++)
    {
        if (psz[i] == cChar) psz[i] = 0;
        else               return;
    }
}

char * strxsupprallleft(char * psz, char cChar)
{
    int i, n;
    n = (int)strlen(psz);

    if (!n) return psz;

    for (i = 0; psz[i]; i++)
    {
        if (psz[i] != cChar) return &(psz[i]);
    }
    return &(psz[n]);
}

char * strxsupprspaces(char * psz)
{
    int i, n;
    bool bSpaces;
    char * pszOut;
    n = (int)strlen(psz);

    if (!n) return psz;

    bSpaces = false;
    for (i = 0, pszOut = psz; psz[i]; i++)
    {
        if (psz[i] != 32)
        {
            bSpaces = false;
            *pszOut = psz[i];
            pszOut++;
        }
        else
        {
            if (!bSpaces)
            {
                *pszOut = psz[i];
                pszOut++;
                bSpaces = true;
            }
        }
    }
    *pszOut = 0;
    pszOut++;

    return psz;
}

char * strxgetfileextension(char * pszSource)
{
    int i;

    for (i = (int)strlen(pszSource) - 1; i >= 0; i--)
    {
        if (pszSource[i] == '.')
        {
            return pszSource + i;
        }
    }

    return NULL;
}

char * strxgetfilenameandextension(char * pszSource)
{
    int i;

    for (i = (int)strlen(pszSource) - 1; i >= 0; i--)
    {
        if ((pszSource[i] == '\\') || (pszSource[i] == '/'))
        {
            return pszSource + i + 1;
        }
    }

    return pszSource;
}


void * strxmemmem(void * pBuffer, void * pPattern, int lBufferLength, int lPatternLength)
{
    char * pSource;
    char * pFinder;
    int lFoundLength;

    if ((!pPattern) ||
        (!pBuffer) ||
        (!lBufferLength) ||
        (lBufferLength<lPatternLength)) return NULL;

    pSource = (char *)pBuffer;
    pFinder = (char *)pPattern;
    lFoundLength = 0;

    lBufferLength -= (lPatternLength - 1);

    do {

        if (*pSource == *pFinder)
        {
            if (!memcmp(pSource, pFinder, lPatternLength))
                return pSource;
        }

        lBufferLength--;
        pSource++;
    } while (lBufferLength);

    return NULL;
}

void strx2wc(wchar_t * pwcTarget, char * pszSource)
{
    int i;
    if ((!pwcTarget) || (!pszSource)) return;

    for (i = 0; pszSource[i]; i++)
    {
        pwcTarget[i] = (wchar_t)pszSource[i];
    }
    pwcTarget[i] = 0;
}


void strxcatchr(char * psz, char ch)
{
    int l;

    l = (int)strlen(psz);
    psz[l] = ch;
    psz[l + 1] = 0;
}

int strxisnumber(char * psz)
{
    char *cp = (char *)psz;
    do {
        if (((*cp) != '-') && (((*cp)<48) || ((*cp)>57)))
            return 0;
        cp++;
    } while (*cp);

    return 1;
}

int strxishexa(char * psz)
{
    char *cp = (char *)psz;
    do {
        if (    ( ((*cp)<48) || ((*cp)>57) ) &&
                ( ((*cp)<'a') || ((*cp)>'f') ) &&
                ( ((*cp)<'A') || ((*cp)>'F') ) )
            return 0;
        cp++;
    } while (*cp);

    return 1;
}



void strxbin2hexa ( unsigned char * pbBin, int iBinLen, char * pszHexa )
{
    char szTmp[16];
    int i;
    pszHexa[0] = 0;
    
    for (i=0;i<iBinLen;i++)
    {
        sprintf ( szTmp, "%02x", (unsigned char)(pbBin[i]) );
        strcat ( pszHexa, szTmp );
    }
}


/*
===================================================================

Is this character is alpha ?

===================================================================
*/
int strxisalpha(int iChar)
{
    unsigned int c;

    _strxInit();

    c = (unsigned int)cListStrUCase[((unsigned)iChar) & 0xFF];

    return ((c >= 'A') && (c <= 'Z')) ? (1) : (0);
}


int strxicmp(char * pszA, char * pszB)
{
    return (int)strxucasecmp(pszA, pszB);
}



/*
==========


*/

int strxccnt ( char * psz, char cChar )
{
    int i, n;
    int iCount = 0;
    n = (int)strlen(psz);
    for (i = 0; i<n; i++)
    {
        if (psz[i]==cChar) iCount++;
    }
    return iCount;
}


/*
===================================================================

auto path completition for Windows and Linux
if char 92 or 47 is founded...

if cDefaultSeparator is -1, the select 92 or 47 via _WIN32 define

===================================================================
*/
void strxautopathcompletition(char * pszPath, char cDefaultSeparator)
{
    char cPathChar;
    char *psz92, *psz47, *psz;
    int lLen;

    if ((cDefaultSeparator == 0) || (cDefaultSeparator == 127))
        cPathChar = 92; //The default path char separator is for Windows
    else if (cDefaultSeparator == -1)
#ifdef _WIN32    
        cPathChar = 92;
#else        
        cPathChar = 47;
#endif
    else
        cPathChar = cDefaultSeparator;

    psz92 = strrchr(pszPath, 92); // searching for '\\'
    psz47 = strrchr(pszPath, 47); // searching for '/'
    psz = strrchr(pszPath, cPathChar);

    //don't add anything if the separator isn't found
    if ((!psz47) && (!psz92) && (!psz))
        return;

    //Learn for Linux separator char
    if ((psz47) && (!psz92))
        cPathChar = 47;
    else if ((psz47) && (psz92) && (psz47>psz92))
        cPathChar = 47;

    lLen = (int)strlen(pszPath);
    if (lLen)
    {
        if (pszPath[lLen - 1] != cPathChar)
        {
            pszPath[lLen] = cPathChar;
            pszPath[lLen + 1] = 0;
        }
    }
}


int strxnlen(char *text, int maxlen)
{
    char * last = (char *)memchr(text, '\0', maxlen);
    return (int)( (last) ? ((size_t)(last - text)) : (maxlen));
}


char * strxrchr(char * psz, char ch)
{
    int n;
    if (!psz) return NULL;
    n = (int)strlen(psz) - 1;
    psz += n;

    do {
        if (*psz == ch) return psz;
        psz--;
        n--;
    } while (n);
    return NULL;
}
/*
##################################################################
C'est comme "explode" en PHP, mais un peu plus complet

Si le (mode&255) vaut :
0 - Séparateur espace
1 - Séparateur espace avec délimiteur de sous-chaine \"
2 - Séparateur espace avec délimiteur de sous-chaine \'

Si (mode>>8)&255 est différent de zéro, il est utilisé comme séparateur

##################################################################
*/
char ** strxexplode(char * pszString, unsigned int ulMode, int * plStringCount)
{
    char *              pszSource;
    char                cSeparator;
    char                cEscape;
    char                cExcludeEnclose;
    int                 lSeparatorCount;
    int                 lEscapeState;
    int                 lExcludeEncloseState;
    int                 lCharacterCount;
    int                 lTotalCharacterCount;
    int                 lTotalArraySize;
    unsigned char *     pbOutput;
    unsigned char *     pbPointer;
    char *              pszOutput;
    int                 lOffsetOutputArrayString;
    int                 lProcessingStep;

    //On va compter les itérations
    pszSource = pszString;

    switch (ulMode & 255)
    {
        case 1:
            cSeparator = ' ';
            cEscape = 0; //'\\';
            cExcludeEnclose = '\"';
            break;

        case 2:
            cSeparator = ' ';
            cEscape = 0; //'\\';
            cExcludeEnclose = '\'';
            break;

        default:
            cSeparator = ' ';
            cEscape = 0;
            cExcludeEnclose = 0;
            break;
    }

    //Le séparateur utilisateur
    if ((ulMode >> 8) & 255)
        cSeparator = (char)((ulMode >> 8) & 255);

    lProcessingStep = 1;


    pszOutput = (char *)pszSource;
    pbPointer = (unsigned char *)pszSource;
    pbOutput    = (unsigned char *)pszSource;
    do {

        lSeparatorCount = 0;
        lEscapeState = 0;
        lExcludeEncloseState = 0;
        lCharacterCount = 0;
        lTotalCharacterCount = 0;


        //Traitement
        for (pszSource = pszString; *pszSource; pszSource++)
        {

            if ((!lEscapeState) || (!cEscape)) //Test s'il y a un échappement en cours ou si l'échappement n'est pas géré
            {

                if (*pszSource == cEscape)    //Détection d'un échappement ?
                {
                    //On active l'échappement
                    lEscapeState = 1;
                    lCharacterCount += 1;
                }
                else if (*pszSource == cExcludeEnclose) //Détection d'un caractère d'enclosure ?
                {
                    lExcludeEncloseState = 1 - lExcludeEncloseState;
                    lCharacterCount += 1;
                }
                else if ((*pszSource == cSeparator) && (lExcludeEncloseState == 0)) //S'il s'agit d'un séparateur et que nous ne somme pas dans l'enclosure
                {
                    //C'est un séparateur effectif
                    lSeparatorCount += 1;
                    lTotalCharacterCount += lCharacterCount + 1;

                    if (lProcessingStep == 2)
                    {
                        //On stocke la chaine
                        memcpy(pszOutput, pszSource - lCharacterCount, lCharacterCount);
                        pszOutput[lCharacterCount] = 0;

                        //On stocke le pointeur de la chaine
                        memcpy(pbPointer, &pszOutput, sizeof(char *));

                        //On se prépare pour la suite
                        pszOutput += lCharacterCount + 1;
                        pbPointer += sizeof(char *);
                    }

                    lCharacterCount = 0;
                }
                else
                {
                    lCharacterCount += 1;
                }
            }
            else if (lEscapeState)
            {
                lEscapeState = 0;
                lCharacterCount += 1;
            }
        }
        if (lCharacterCount)
        {
            //C'est un séparateur effectif
            lSeparatorCount += 1;
            lTotalCharacterCount += lCharacterCount + 1;

            if (lProcessingStep == 2)
            {
                //On stocke la chaine
                memcpy(pszOutput, pszSource - lCharacterCount, lCharacterCount);
                pszOutput[lCharacterCount] = 0;

                //On stocke le pointeur de la chaine
                memcpy(pbPointer, &pszOutput, sizeof(char *));

                //On se prépare pour la suite
                pszOutput += lCharacterCount + 1;
                pbPointer += sizeof(char *);
            }

        }

        if (lProcessingStep == 1)
        {
            //L'allocation mémoire se base sur la formule suivante :
            lTotalArraySize = ((lSeparatorCount + 1) * sizeof(char *)) + lTotalCharacterCount;
#ifdef _mDES
            void * mDES_MemoryAlloc(unsigned long);
            pbOutput = (unsigned char *)mDES_MemoryAlloc(lTotalArraySize);
#else
            pbOutput = (unsigned char *)malloc(lTotalArraySize);
#endif
            lOffsetOutputArrayString = (lSeparatorCount + 1) * sizeof(char *);
            pszOutput = (char *)(pbOutput + lOffsetOutputArrayString);
            pbPointer = pbOutput;
        }

        lProcessingStep += 1;

    } while (lProcessingStep<3);

    //On ajoute le pointeur final à null
    memset(pbPointer, 0, sizeof(char *));

    if (plStringCount)
        *plStringCount = lSeparatorCount;

    return (char **)pbOutput;
}



char * strxpathclean ( char * pszPath, bool bEnableSpecialChars )
{
    char *  pszIn;
    char *  pszOut;
    bool    bFolderSeparator;
#ifdef _WIN32    
    char    cPathChar = 92;
#else        
    char    cPathChar = 47;
#endif

    if (!pszPath) return NULL;
    //if (strlen(pszPath)) return pszPath;

    bFolderSeparator = false;
    pszIn  = pszPath;
    pszOut = pszPath;
    do {
        if ( (*pszIn=='\\') || (*pszIn=='/') )
        {
            if (bFolderSeparator==false)
            {
                *pszOut = cPathChar;
                pszOut++;
            }
            bFolderSeparator = true;
        }
        else if (bEnableSpecialChars==false)
        {
            bFolderSeparator = false;

            if ( (*pszIn>=  1) && (*pszIn<= 31) )
            {
                //Caractères totalement invalides, on n'emet rien en sortie !
            }
            else 
            if ( ( (*pszIn>= 33) && (*pszIn<= 44) )     ||
                 ( (*pszIn>= 59) && (*pszIn<= 64) )     ||
                   (*pszIn== 91)                        ||
                 ( (*pszIn>= 93) && (*pszIn<= 96) )     ||
                 ( ((unsigned char)(*pszIn)>=123) && ((unsigned char)(*pszIn)<=255) ) )
            {
                *pszOut = 95;  
                pszOut++;
            }
            else
            {
                *pszOut = *pszIn;
                pszOut++;
            }
        }
        else
        {
            bFolderSeparator = false;

            *pszOut = *pszIn;
            pszOut++;
        }
        pszIn++;
    } while (*pszIn);

    //On s'assure qu'il y a bien un zéro à la fin
    *pszOut = 0;

    return pszPath;
}

static char strtrim_sDefaultChar[4] = " ";
char * strxltrim ( char * pszIO, char * pszMask )
{
    char *  pszIn;
    int     iCount;
    char *  pszTrimList;

    if (!pszIO) return NULL;
    iCount = (int)strlen(pszIO);
    if (!iCount) return pszIO;

    pszTrimList = (pszMask)?(pszMask):(strtrim_sDefaultChar);
    pszIn       = pszIO;
    do {
        if (strchr(pszTrimList,*pszIn)==NULL)
        {
            memmove ( pszIO, pszIn, iCount+1 );
            return pszIO;
        }

        pszIn++;
        iCount--;            
    } while (iCount);

    strcpy ( pszIO, "" );
    return pszIO;
}

char * strxrtrim ( char * pszIO, char * pszMask )
{
    char *  pszIn;
    int     iCount;
    char *  pszTrimList;

    if (!pszIO) return NULL;
    iCount = (int)strlen(pszIO);
    if (!iCount) return pszIO;

    pszTrimList = (pszMask)?(pszMask):(strtrim_sDefaultChar);
    pszIn       = pszIO + iCount - 1;
    do {
        if (strchr(pszTrimList,*pszIn)==NULL)
        {
            *(pszIn+1) = 0;
            return pszIO;
        }

        pszIn--;
        iCount--;            
    } while (iCount);

    return pszIO;
}

/*
 *
 *
 *      UTF8 converter
 *
 *
 */

long lGlobalCodePage = CODEPAGE_UTF8;

void _UTF8_encode ( unsigned int ulValue, unsigned char * pbOutputData, int * plOutputLen );
void _UTF8_decode ( unsigned char * pbInputData, int * plInputLen, unsigned int *pulValue );
int strxConvertFromUTF8 ( char * pszInputUTF8, char * pszOutputCodePage, int lCodePage );
char strxConvertToCodePage ( unsigned int ulValue, int lCodePage );
unsigned int strxConvertFromCodePage ( char cValue, int lCodePage );

#define UTF8_CODEPOINT_1BYTE 0x00
#define UTF8_CODEPOINT_2BYTE 0xC0
#define UTF8_CODEPOINT_3BYTE 0xE0
#define UTF8_CODEPOINT_4BYTE 0xF0
#define UTF8_CODEPOINT_NBYTE 0x80

//According to some code page like Windows 1252
#define UNUSED 0xFFFFFFFF


void _UTF8_encode ( unsigned int ulValue, unsigned char * pbOutputData, int * plOutputLen )
{
    ulValue &= 0x7FFFFFFF;
    
    if (ulValue<128)
    {
        pbOutputData[0] = ulValue | UTF8_CODEPOINT_1BYTE;
        *plOutputLen = 1;
    }
    else if (ulValue<2048)
    {
        pbOutputData[1] =  (ulValue     &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[0] =  (ulValue>> 6)       | UTF8_CODEPOINT_2BYTE;
        *plOutputLen = 2;
    }
    else if (ulValue<65536)
    {
        pbOutputData[2] = ( ulValue     &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[1] = ((ulValue>> 6)&0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[0] =  (ulValue>>12)       | UTF8_CODEPOINT_3BYTE;
        *plOutputLen = 3;
    }
    else if (ulValue<2097152)
    {    
        pbOutputData[3] = ( ulValue     &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[2] = ( ulValue>> 6 &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[1] = ((ulValue>>12)&0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[0] =  (ulValue>>16)       | UTF8_CODEPOINT_4BYTE;
        *plOutputLen = 4;
    }
    else *plOutputLen = 0; //Invalid UTF8 code
}

void _UTF8_decode ( unsigned char * pbInputData, int * plInputLen, unsigned int *pulValue )
{

    if (!(*pbInputData&0x80)) // 1 byte
    {
        *pulValue = pbInputData[0];
        *plInputLen = 1;
    }
    else if ((*pbInputData&0xE0)==UTF8_CODEPOINT_2BYTE) // 2 byte
    {
        *pulValue = (pbInputData[1]&0x3F) | ((pbInputData[0]&0x1F)<<6);
        *plInputLen = 2;
    }
    else if ((*pbInputData&0xF0)==UTF8_CODEPOINT_3BYTE) // 3 byte
    {
        *pulValue = (pbInputData[2]&0x3F) | ((pbInputData[1]&0x3F)<<6) | ((pbInputData[0]&0x0F)<<12);
        *plInputLen = 3;
    }
    else if ((*pbInputData&0xF8)==UTF8_CODEPOINT_4BYTE) // 4 byte
    {
        *pulValue = (pbInputData[3]&0x3F) | ((pbInputData[2]&0x3F)<<6) | ((pbInputData[1]&0x3F)<<12) | ((pbInputData[0]&0x07)<<18);
        *plInputLen = 4;
    }
    else //Invalid UTF8 CODEPOINT
    {
        *pulValue = 0;
        *plInputLen = -1;
    }
}


//Diacritical marks (non-spacing) : 0x0300 - 0x036F (0xcc80 - 0xcdaf)
typedef struct {
    char cPreviousCharacter;
    unsigned int iDiacriticalCode;
    unsigned int iLatinCode;
} TDiacriticalItem;

TDiacriticalItem DiacriticalItem[] = {
    { 'e', 0x0300, 0x00e8 }, //e grave
    { 'e', 0x0301, 0x00e9 }, //e acute
    { 'e', 0x0302, 0x00ea }, //e circumflex
    { 'e', 0x0308, 0x00eb }, //e diaeresis

    { 'i', 0x0300, 0x00ec }, //i grave
    { 'i', 0x0301, 0x00ed }, //i acute
    { 'i', 0x0302, 0x00ee }, //i circumflex
    { 'i', 0x0308, 0x00ef }, //i diaeresis
    
    { 'n', 0x0303, 0x00f1 }, //n tilde
    
    { 'o', 0x0300, 0x00f2 }, //o grave
    { 'o', 0x0301, 0x00f3 }, //o acute
    { 'o', 0x0302, 0x00f4 }, //o circumflex
    { 'o', 0x0303, 0x00f5 }, //o tilde
    { 'o', 0x0308, 0x00f6 }, //o diaeresis
    
    { 'u', 0x0300, 0x00f9 }, //u grave
    { 'u', 0x0301, 0x00fa }, //u acute
    { 'u', 0x0302, 0x00fb }, //u circumflex
    { 'u', 0x0308, 0x00fc }, //u diaeresis
    
    { 0, 0, 0 },
};

/*
 * We need to convert complex UTF8 character
 */


int strxConvertFromUTF8 ( char * pszInputUTF8, char * pszOutputCodePage, int lCodePage )
{
    int lIndex, lCount, lSizeUTF8;
    int lIndexOutput;
    unsigned int ulValue, ulLastValue;
    unsigned int ulDiacriticalMarkValue;
    int iDIndex;
    
    
    if ( (!pszInputUTF8) || (!pszOutputCodePage) )
        return -1;
        
    if (lCodePage==CODEPAGE_UTF8)
    {
        strcpy ( pszOutputCodePage, pszInputUTF8 );
        return 0;
    }
    
    lCount          = (int)strlen ( pszInputUTF8 );
    lIndexOutput    = 0;
    for (lIndex=0;lIndex<lCount;lIndex+=lSizeUTF8)
    {
        _UTF8_decode ( (unsigned char *)pszInputUTF8+lIndex, &lSizeUTF8, &ulValue );
        
        if (lSizeUTF8!=-1)
        {
            if ((ulValue>=0x0300)&&(ulValue<=0x036F))
            {
                //Diacritical mark detected !
                ulDiacriticalMarkValue = ulValue;
                for (iDIndex=0;DiacriticalItem[iDIndex].cPreviousCharacter!=0;iDIndex++)
                {
                    if ( (DiacriticalItem[iDIndex].cPreviousCharacter == ulLastValue) &&
                         (DiacriticalItem[iDIndex].iDiacriticalCode == ulValue) )
                    {
                        
                        pszOutputCodePage[lIndexOutput-1] = strxConvertToCodePage ( DiacriticalItem[iDIndex].iLatinCode, lCodePage );
                        ulLastValue = 0;
                        break;
                    }
                    
                }
                
            }
            else
            {
                //Transcoding
                pszOutputCodePage[lIndexOutput] = strxConvertToCodePage ( ulValue, lCodePage );
                lIndexOutput++;
                ulDiacriticalMarkValue = 0;
                ulLastValue = ulValue;
            }
        }
        else lSizeUTF8 = 1;
    }
    pszOutputCodePage[lIndexOutput] = 0;

    return 0;
}

int strxConvertToUTF8 ( char * pszInputCodePage, char * pszOutputUTF8, int lCodePage )
{
    int lIndex, lCount, lSizeUTF8;
    int lIndexOutput;
    unsigned int ulValue;
    
    
    if ( (!pszInputCodePage) || (!pszOutputUTF8) )
        return -1;

    if (lCodePage==CODEPAGE_UTF8)
    {
        strcpy ( pszOutputUTF8, pszInputCodePage );
        return 0;
    }

    lCount          = (int)strlen ( pszInputCodePage );
    lIndexOutput    = 0;
    for (lIndex=0;lIndex<lCount;lIndex++)
    {
        //Transcoding char to UNICODE
        ulValue = strxConvertFromCodePage ( pszInputCodePage[lIndex], lCodePage );
        
        if (ulValue!=UNUSED)
        {        
            //Convert UNICODE to UTF8
            _UTF8_encode ( ulValue, (unsigned char *)pszOutputUTF8+lIndexOutput, &lSizeUTF8 );
            
            //Data have been added
            lIndexOutput += lSizeUTF8;
        }
    }
    pszOutputUTF8[lIndexOutput] = 0;

    return 0;
}


unsigned int CodePageHigh_MacRoman_ListUnicode[128] = {
    0x00C4, 0x00C5, 0x00C7, 0x00C9, 0x00D1, 0x00D6, 0x00DC, 0x00E1,
    0x00E0, 0x00E2, 0x00E4, 0x00E3, 0x00E5, 0x00E7, 0x00E9, 0x00E8,
    0x00EA, 0x00EB, 0x00ED, 0x00EC, 0x00EE, 0x00EF, 0x00F1, 0x00F3,
    0x00F2, 0x00F4, 0x00F6, 0x00F5, 0x00FA, 0x00F9, 0x00FB, 0x00FC,
    0x2020, 0x00B0, 0x00A2, 0x00A3, 0x00A7, 0x2022, 0x00B6, 0x00DF,
    0x00AE, 0x00A9, 0x2122, 0x00B4, 0x00A8, 0x2260, 0x00C6, 0x00D8,
    0x221E, 0x00B1, 0x2264, 0x2265, 0x00A5, 0x00B5, 0x2202, 0x2211,
    0x220F, 0x03C0, 0x222B, 0x00AA, 0x00BA, 0x03A9, 0x00E6, 0x00F8,
    0x00BF, 0x00A1, 0x00AC, 0x221A, 0x0192, 0x2248, 0x2206, 0x00AB,
    0x00BB, 0x2026, 0x00A0, 0x00C0, 0x00C3, 0x00D5, 0x0152, 0x0153,
    0x2013, 0x2014, 0x201C, 0x201D, 0x2018, 0x2019, 0x00F7, 0x25CA,
    0x00FF, 0x0178, 0x2044, 0x20AC, 0x2039, 0x203A, 0xFB01, 0xFB02,
    0x2021, 0x00B7, 0x201A, 0x201E, 0x2030, 0x00C2, 0x00CA, 0x00C1,
    0x00CB, 0x00C8, 0x00CD, 0x00CE, 0x00CF, 0x00CC, 0x00D3, 0x00D4,
    0xF8FF, 0x00D2, 0x00DA, 0x00DB, 0x00D9, 0x0131, 0x02C6, 0x02DC,
    0x00AF, 0x02D8, 0x02D9, 0x02DA, 0x00B8, 0x02DD, 0x02DB, 0x02C7 };


unsigned int CodePageHigh_Windows1250_ListUnicode[128] = {
    0x20AC, UNUSED, 0x201A, UNUSED, 0x201E, 0x2026, 0x2020, 0x2021,
    UNUSED, 0x2030, 0x0160, 0x2039, 0x015A, 0x0164, 0x017D, 0x0179,
    UNUSED, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    UNUSED, 0x2122, 0x0161, 0x203A, 0x015B, 0x0165, 0x017E, 0x017A,
    0x00A0, 0x02C7, 0x02D8, 0x0141, 0x00A4, 0x0104, 0x00A6, 0x00A7,
    0x00A8, 0x00A9, 0x015E, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x017B,
    0x00B0, 0x00B1, 0x02DB, 0x0142, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
    0x00B8, 0x0105, 0x015F, 0x00BB, 0x013D, 0x02DD, 0x013E, 0x017C,
    0x0154, 0x00C1, 0x00C2, 0x0102, 0x00C4, 0x0139, 0x0106, 0x00C7,
    0x010C, 0x00C9, 0x0118, 0x00CB, 0x011A, 0x00CD, 0x00CE, 0x010E,
    0x0110, 0x0143, 0x0147, 0x00D3, 0x00D4, 0x0150, 0x00D6, 0x00D7,
    0x0158, 0x016E, 0x00DA, 0x0170, 0x00DC, 0x00DD, 0x0162, 0x00DF,
    0x0155, 0x00E1, 0x00E2, 0x0103, 0x00E4, 0x013A, 0x0107, 0x00E7,
    0x010D, 0x00E9, 0x0119, 0x00EB, 0x011B, 0x00ED, 0x00EE, 0x010F,
    0x0111, 0x0144, 0x0148, 0x00F3, 0x00F4, 0x0151, 0x00F6, 0x00F7,
    0x0159, 0x016F, 0x00FA, 0x0171, 0x00FC, 0x00FD, 0x0163, 0x02D9 };

unsigned int CodePageHigh_Windows1252_ListUnicode[128] = {
    0x20AC, UNUSED, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
    0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, UNUSED, 0x017D, UNUSED,
    UNUSED, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, UNUSED, 0x017E, 0x0178,
    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
    0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
    0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7,
    0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
    0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
    0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
    0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF };

unsigned int CodePageHigh_IBM437_ListUnicode[128] = {
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
    0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
    0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
    0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
    0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
    0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
    0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
    0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0 };

unsigned int CodePageHigh_IBM850_ListUnicode[128] = {
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
    0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
    0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
    0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0,
    0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3,
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
    0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE,
    0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
    0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE,
    0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
    0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8,
    0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0 };

unsigned int CodePageHigh_IBM852_ListUnicode[128] = {
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x016F, 0x0107, 0x00E7,
    0x0142, 0x00EB, 0x0150, 0x0151, 0x00EE, 0x0179, 0x00C4, 0x0106,
    0x00C9, 0x0139, 0x013A, 0x00F4, 0x00F6, 0x013D, 0x013E, 0x015A,
    0x015B, 0x00D6, 0x00DC, 0x0164, 0x0165, 0x0141, 0x00D7, 0x010D,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x0104, 0x0105, 0x017D, 0x017E,
    0x0118, 0x0119, 0x00AC, 0x017A, 0x010C, 0x015F, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x011A,
    0x015E, 0x2563, 0x2551, 0x2557, 0x255D, 0x017B, 0x017C, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x0102, 0x0103,
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
    0x0111, 0x0110, 0x010E, 0x00CB, 0x010F, 0x0147, 0x00CD, 0x00CE,
    0x011B, 0x2518, 0x250C, 0x2588, 0x2584, 0x0162, 0x016E, 0x2580,
    0x00D3, 0x00DF, 0x00D4, 0x0143, 0x0144, 0x0148, 0x0160, 0x0161,
    0x0154, 0x00DA, 0x0155, 0x0170, 0x00FD, 0x00DD, 0x0163, 0x00B4,
    0x00AD, 0x02DD, 0x02DB, 0x02C7, 0x02D8, 0x00A7, 0x00F7, 0x00B8,
    0x00B0, 0x00A8, 0x02D9, 0x0171, 0x0158, 0x0159, 0x25A0, 0x00A0 };

unsigned int CodePageHigh_ISO8859_1_ListUnicode[128] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
    0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
    0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
    0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
    0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7,
    0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
    0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
    0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
    0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF };

unsigned int CodePageHigh_ISO8859_2_ListUnicode[128] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
    0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
    0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x0104, 0x02D8, 0x0141, 0x00A4, 0x013D, 0x015A, 0x00A7,
    0x00A8, 0x0160, 0x015E, 0x0164, 0x0179, 0x00AD, 0x017D, 0x017B,
    0x00B0, 0x0105, 0x02DB, 0x0142, 0x00B4, 0x013E, 0x015B, 0x02C7,
    0x00B8, 0x0161, 0x015F, 0x0165, 0x017A, 0x02DD, 0x017E, 0x017C,
    0x0154, 0x00C1, 0x00C2, 0x0102, 0x00C4, 0x0139, 0x0106, 0x00C7,
    0x010C, 0x00C9, 0x0118, 0x00CB, 0x011A, 0x00CD, 0x00CE, 0x010E,
    0x0110, 0x0143, 0x0147, 0x00D3, 0x00D4, 0x0150, 0x00D6, 0x00D7,
    0x0158, 0x016E, 0x00DA, 0x0170, 0x00DC, 0x00DD, 0x0162, 0x00DF,
    0x0155, 0x00E1, 0x00E2, 0x0103, 0x00E4, 0x013A, 0x0107, 0x00E7,
    0x010D, 0x00E9, 0x0119, 0x00EB, 0x011B, 0x00ED, 0x00EE, 0x010F,
    0x0111, 0x0144, 0x0148, 0x00F3, 0x00F4, 0x0151, 0x00F6, 0x00F7,
    0x0159, 0x016F, 0x00FA, 0x0171, 0x00FC, 0x00FD, 0x0163, 0x02D9 };

unsigned int CodePageHigh_ISO8859_3_ListUnicode[128] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
    0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
    0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x0126, 0x02D8, 0x00A3, 0x00A4, UNUSED, 0x0124, 0x00A7,
    0x00A8, 0x0130, 0x015E, 0x011E, 0x0134, 0x00AD, UNUSED, 0x017B,
    0x00B0, 0x0127, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x0125, 0x00B7,
    0x00B8, 0x0131, 0x015F, 0x011F, 0x0135, 0x00BD, UNUSED, 0x017C,
    0x00C0, 0x00C1, 0x00C2, UNUSED, 0x00C4, 0x010A, 0x0108, 0x00C7,
    0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    UNUSED, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x0120, 0x00D6, 0x00D7,
    0x011C, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x016C, 0x015C, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, UNUSED, 0x00E4, 0x010B, 0x0109, 0x00E7,
    0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    UNUSED, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x0121, 0x00F6, 0x00F7,
    0x011D, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x016D, 0x015D, 0x02D9 };

unsigned int CodePageHigh_ISO8859_4_ListUnicode[128] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
    0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
    0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x0104, 0x0138, 0x0156, 0x00A4, 0x0128, 0x013B, 0x00A7,
    0x00A8, 0x0160, 0x0112, 0x0122, 0x0166, 0x00AD, 0x017D, 0x00AF,
    0x00B0, 0x0105, 0x02DB, 0x0157, 0x00B4, 0x0129, 0x013C, 0x02C7,
    0x00B8, 0x0161, 0x0113, 0x0123, 0x0167, 0x014A, 0x017E, 0x014B,
    0x0100, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x012E,
    0x010C, 0x00C9, 0x0118, 0x00CB, 0x0116, 0x00CD, 0x00CE, 0x012A,
    0x0110, 0x0145, 0x014C, 0x0136, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
    0x00D8, 0x0172, 0x00DA, 0x00DB, 0x00DC, 0x0168, 0x016A, 0x00DF,
    0x0101, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x012F,
    0x010D, 0x00E9, 0x0119, 0x00EB, 0x0117, 0x00ED, 0x00EE, 0x012B,
    0x0111, 0x0146, 0x014D, 0x0137, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
    0x00F8, 0x0173, 0x00FA, 0x00FB, 0x00FC, 0x0169, 0x016B, 0x02D9 };

unsigned int CodePageHigh_ISO8859_15_ListUnicode[128] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,
    0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
    0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x20AC, 0x00A5, 0x0160, 0x00A7,
    0x0161, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x017D, 0x00B5, 0x00B6, 0x00B7,
    0x017E, 0x00B9, 0x00BA, 0x00BB, 0x0152, 0x0153, 0x0178, 0x00BF,
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7,
    0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
    0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
    0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7,
    0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF };




char strxConvertToCodePage ( unsigned int ulValue, int lCodePage )
{
    unsigned int * pulCodePage;
    long i;
    
    switch (lCodePage)
    {
        case CODEPAGE_WINDOWS1250:
            pulCodePage = CodePageHigh_Windows1250_ListUnicode;
            break;
        
        default:    
        case CODEPAGE_WINDOWS1252:
            pulCodePage = CodePageHigh_Windows1252_ListUnicode;
            break;
        
        case CODEPAGE_IBM437:
            pulCodePage = CodePageHigh_IBM437_ListUnicode;
            break;
            
        case CODEPAGE_IBM850:
            pulCodePage = CodePageHigh_IBM850_ListUnicode;
            break;
            
        case CODEPAGE_IBM852:
            pulCodePage = CodePageHigh_IBM852_ListUnicode;
            break;
            
        case CODEPAGE_ISO8859_1:
            pulCodePage = CodePageHigh_ISO8859_1_ListUnicode;
            break;
        
        case CODEPAGE_ISO8859_2:
            pulCodePage = CodePageHigh_ISO8859_2_ListUnicode;
            break;

        case CODEPAGE_ISO8859_3:
            pulCodePage = CodePageHigh_ISO8859_3_ListUnicode;
            break;
        
        case CODEPAGE_ISO8859_4:
            pulCodePage = CodePageHigh_ISO8859_4_ListUnicode;
            break;
        
        case CODEPAGE_ISO8859_15:
            pulCodePage = CodePageHigh_ISO8859_15_ListUnicode;
            break;
            
        case CODEPAGE_MACROMAN:
            pulCodePage = CodePageHigh_MacRoman_ListUnicode;
            break;
    }
    
    if (ulValue<128) return ulValue;
    
    for (i=0;i<128;i++)
    {
        if (pulCodePage[i]==ulValue) return (char)(i+128);
    }
    
    return 32; //SPACE
}

unsigned int strxConvertFromCodePage ( char cValue, int lCodePage )
{
    unsigned int * pulCodePage;
    
    switch (lCodePage)
    {
        case CODEPAGE_WINDOWS1250:
            pulCodePage = CodePageHigh_Windows1250_ListUnicode;
            break;
        
        default:    
        case CODEPAGE_WINDOWS1252:
            pulCodePage = CodePageHigh_Windows1252_ListUnicode;
            break;
        
        case CODEPAGE_IBM437:
            pulCodePage = CodePageHigh_IBM437_ListUnicode;
            break;
            
        case CODEPAGE_IBM850:
            pulCodePage = CodePageHigh_IBM850_ListUnicode;
            break;
            
        case CODEPAGE_IBM852:
            pulCodePage = CodePageHigh_IBM852_ListUnicode;
            break;
            
        case CODEPAGE_ISO8859_1:
            pulCodePage = CodePageHigh_ISO8859_1_ListUnicode;
            break;
        
        case CODEPAGE_ISO8859_2:
            pulCodePage = CodePageHigh_ISO8859_2_ListUnicode;
            break;

        case CODEPAGE_ISO8859_3:
            pulCodePage = CodePageHigh_ISO8859_3_ListUnicode;
            break;
        
        case CODEPAGE_ISO8859_4:
            pulCodePage = CodePageHigh_ISO8859_4_ListUnicode;
            break;
        
        case CODEPAGE_ISO8859_15:
            pulCodePage = CodePageHigh_ISO8859_15_ListUnicode;
            break;
        
        case CODEPAGE_MACROMAN:
            pulCodePage = CodePageHigh_MacRoman_ListUnicode;
            break;

    }
    
    if ((unsigned char)cValue<128) return (unsigned char)cValue;
    else return pulCodePage[((unsigned char)cValue)-128];
}