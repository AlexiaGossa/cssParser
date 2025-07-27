/*
 *  (c) 2019 - Alexia Vasiliky Gossa
 */

#ifndef zM_DOMCONTEXT_HEADER
#define zM_DOMCONTEXT_HEADER

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include "str.h"
#include "strz.h"
#include "cssparse.h"
#include "htmlparse.h"

    typedef struct {
        htmlDOCUMENT    htmlDocument;
        cssOBJECT       cssObject;
    } domCONTEXT;

    zRETURN domContextInit ( domCONTEXT * pContext );
    zRETURN domContextFree ( domCONTEXT * pContext );
    zRETURN domContextLoad ( domCONTEXT * pContext, char * pszHtmlFileName );

#endif