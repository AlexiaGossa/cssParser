/*
 *  (c) 2019 - Alexia Vasiliky Gossa
 */

#ifndef zM_STRZ_HEADER
#define zM_STRZ_HEADER

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdint.h>

char * strzAlloc ( const char * pszText );
char * strzRealloc ( char * pszCurrent, const char * pszNewText );
void strzAlloc ( char ** pszIO, const char * pszText );
void strzRealloc ( char ** pszIO, const char * pszNewText );
void strzFree ( char * psz );

void strzCopy ( char ** pszIO, const char * pszNewText );
void strzConcat ( char ** pszIO, const char * pszConcatText );


#endif