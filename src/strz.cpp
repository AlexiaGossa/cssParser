#include "strz.h"
#pragma warning(disable : 4996)

char * strzAlloc ( const char * pszText )
{
    char * psz;
    psz = (char *)malloc ( strlen(pszText) + 1 );
    strcpy ( psz, pszText );
    return psz;
}

void strzAlloc ( char ** pszIO, const char * pszText )
{
    *pszIO = (char *)malloc ( strlen(pszText) + 1 );
    strcpy ( *pszIO, pszText );
}

char * strzRealloc ( char * pszCurrent, const char * pszNewText )
{
    char * psz;
    psz = (char *)realloc ( pszCurrent, strlen(pszNewText) + 1 );
    strcpy ( psz, pszNewText );
    return psz;
}

void strzRealloc ( char ** pszIO, const char * pszNewText )
{
    *pszIO = (char *)realloc ( *pszIO, strlen(pszNewText) + 1 );
    strcpy ( *pszIO, pszNewText );
}

void strzFree ( char * psz )
{
    free ( psz );
}

void strzCopy ( char ** pszIO, const char * pszNewText )
{
    strzRealloc ( pszIO, pszNewText );
}

void strzConcat ( char ** pszIO, const char * pszConcatText )
{
    int iBaseLength;
    int iConcatLength;

    if (*pszIO)
        iBaseLength = strlen ( *pszIO );
    else
        iBaseLength = 0;

    iConcatLength = strlen( pszConcatText );

    *pszIO = (char *)realloc ( *pszIO, iBaseLength + iConcatLength + 1 );
    strcpy ( (*pszIO) + iBaseLength, pszConcatText );
}
