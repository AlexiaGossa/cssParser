#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include "cssparse.h"
#include "htmlnode.h"
#include "str.h"
#include "strz.h"

#ifndef __H_HTMLPARSE__
#define __H_HTMLPARSE__

typedef int zRETURN;


/*

Les balises HTML




body
    display: block;
    margin: 8px;

div
    display: block;

span
    none.

p
    display: block;
    margin-top: 1em;
    margin-bottom: 1em;
    margin-left: 0;
    margin-right: 0;

input
    text-area
    password


The tag element 'text' does not exists but, it created to embbed any text
The tag element 'root' contain the DOM file

*/

typedef struct {
    
    char *          szTagName;
    bool            bAllowSingle; //with no ending tag (like HR...)

    int             iPropertyCount;
    cssPROPERTY *   pPropertyItem;
    
} htmlTAG;


typedef struct {
    int             iProperty;
    int             iValue;
    int             iValueType;
} htmlDescTAG;


typedef struct {
    char *          pszTagName;
    bool            bNoCloseTag;
    char *          pszTagStyleDefinition;
    //htmlDescTAG     pDescTag[16];
} htmlDefTAG;









/*
document.createElement
childElementCount
document.getElementById
document.getElementsByClassName
document.getElementsByTagName

element.appendChild
element.getChild
element.getParent
element.removeChild
*/






zRETURN htmlParseStr (
    str *           pstr,
    htmlDOCUMENT *  pDocument );





#endif