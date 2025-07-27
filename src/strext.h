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


#ifndef _strext_h_
#define _strext_h_

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4028)
#endif






//#ifndef __cplusplus
//extern "C" {
//#endif



//Coupure dés le premier caractère non ascii : 0...31
void strxcutonlyascii(char * psz);

//Coupure dés le premier caractère non présent dans la liste
void strxcutonlylistchar(char * psz, char * pszListChar);

//Suppression d'une chaine dans une autre
void strxsupprstr(char * pszIO, char * pszPattern);

//Suppression d'un caractère dans une chaine
void strxsupprchar(char * pszIO, char cChar);

//Suppression d'une liste de caractères d'une chaine
void strxsupprlistchar(char * pszIO, char * pszListChar);

//Conservation d'une liste de caractères d'une chaine
void strxkeeplistchar(char * pszIO, char * pszListChar);

//Passage en majuscule en détruisant les accents
char * strxucase(char * psz);
void strxucasefast(char * psz);   //Plus rapide (mais utilise le cache)

char * strxnoaccent(char * psz);

//Passage en minuscule
char * strxlcase(char * psz);

//Remplacement d'une chaîne par une autre
void strxreplace(char * pszIO, char * pszPattern, char * pszReplacement);

void strxireplace(char * pszIO, char * pszPattern, char * pszReplacement);

void strxcreplace(char * psz, char cOriginal, char cReplace);

void strxcremove(char * psz, char cRemove);

int strxucasecmp(const char * pszSource, const char * pszTarget);
int strnxucasecmp(const char * pszSource, const char * pszTarget, int lCharCountMax);

#ifndef _WIN32
#define stricmp         strxucasecmp
#define strnicmp        strxnucasecmp
#define _strlwr         strxlcase
#define _strupr         strxucase
#endif



//Supprime tous les caractères à droite identiques
void strxsupprallright(char * psz, char cChar);

//Renvoie la chaîne en supprimant tous les caractères à gauche
char * strxsupprallleft(char * psz, char cChar);

//Récupération de l'extension d'un fichier d'une chaine (avec le point. Ex : "MonFichier.txt" => ".txt")
char * strxgetfileextension(char * pszSource);

//Récupération du nom et de l'extension d'un fichier d'une chaine (avec le point. Ex : "C:\Mod\Ess\MonFichier.txt" => "MonFichier.txt")
char * strxgetfilenameandextension(char * pszSource);

//Conversion ASCII => WideChar
void strx2wc(wchar_t * pwcTarget, char * pszSource);

//idem a strstr mais sans le respect des accents
char * strxistr(const char * str1, const char * str2);

//idem a strchr mais sans le respect des accents
char * strxichr(const char * string, int ch);

//Idem à strcat mais pour des char
void strxcatchr(char * psz, char ch);

//comme strchr, mais commence par la fin
char * strxrchr(char * psz, char ch);


int strxisalpha(int iChar);

int strxisnumber(char * psz);
int strxishexa(char * psz);

void strxbin2hexa ( unsigned char * pbBin, int iBinLen, char * pszHexa );

//The name is... use -1 or NULL for the second parameter
void strxautopathcompletition(char * pszPath, char cDefaultSeparator);

//Comme "explode" en PHP, retourne un tableau de chaines devant être libérée
char ** strxexplode(char * pszString, unsigned int ulMode, int * plStringCount);

int strxccnt ( char * psz, char cChar );

//Remplacement de tous les caractères pouvant poser des soucis par '_' ainsi que les séparateurs
char * strxpathclean ( char * pszPath, bool bEnableSpecialChars );



//Suppression en début ou fin de chaine d'espace (pszMask == NULL) ou d'autres caractères
char * strxltrim ( char * pszIO, char * pszMask );
char * strxrtrim ( char * pszIO, char * pszMask );



//#ifndef __cplusplus
//}
//#endif


//Recherche de données...
void * strxmemmem(void * pBuffer, void * pPattern, int lBufferLength, int lPatternLength);



#define CODEPAGE_MACROMAN       0x30000001
#define CODEPAGE_WINDOWS1250    0x20001250
#define CODEPAGE_WINDOWS1252    0x20001252
#define CODEPAGE_IBM437         0x10000437
#define CODEPAGE_IBM850         0x10000850
#define CODEPAGE_IBM852         0x10000852
#define CODEPAGE_ISO8859_1      0x00885901
#define CODEPAGE_ISO8859_2      0x00885902
#define CODEPAGE_ISO8859_3      0x00885903
#define CODEPAGE_ISO8859_4      0x00885904
#define CODEPAGE_ISO8859_15     0x00885915
#define CODEPAGE_UTF8           0x00000000

int strxConvertToUTF8 ( char * pszInputCodePage, char * pszOutputUTF8, int lCodePage );
int strxConvertFromUTF8 ( char * pszInputUTF8, char * pszOutputCodePage, int lCodePage );
//extern int lGlobalCodePage;





//size_t
int strxnlen(char *text, int maxlen);



#endif
