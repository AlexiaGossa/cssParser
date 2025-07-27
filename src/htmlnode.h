/*
 *  (c) 2019 - Alexia Vasiliky Gossa
 */

#ifndef zM_HTMLNODE_HEADER
#define zM_HTMLNODE_HEADER

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include "str.h"


typedef struct {
    char * pstrzAttributeName;
    char * pstrzAttributeValue;
} htmlELEMENTATTRIBUTE;

typedef struct {
    int                         iHandle;    //0 if it's html
    int                         iHandleParent; //-1 if there's no parent (just created element or body)

    char *                      pstrzTagName;
    char *                      pstrzId;
    char *                      pstrzStyle;

    char **                     pstrzClass;
    int                         iClassCount;

    htmlELEMENTATTRIBUTE *      pAttributeArray;
    int                         iAttributeCount;

    str                         strContent;


    int                         iChildCount;
    int *                       pChildArray;
} htmlELEMENT;

typedef struct {
    htmlELEMENT *   pElement;
    int             iElementCount;

    int             iCurrentHandleIndex;
        
} htmlDOCUMENT;


void        htmlDocumentInit ( htmlDOCUMENT * pDocument );
void        htmlDocumentFree ( htmlDOCUMENT * pDocument );

void        htmlDocumentShowDebug ( htmlDOCUMENT * pDocument );





/*
The first element handle, the body, is 0 (zero)
*/

/*
 *  Get the first element with the specified ID
 *  In case of error, return -1
 */
int         htmlDocumentGetElementById ( htmlDOCUMENT * pDocument, char * pszId );

/*
 *  Try to find all element with the specified classname.
 *  Return the elements array and elements count
 *  In case of error, return NULL
 */
int *       htmlDocumentGetElementsByClassName ( htmlDOCUMENT * pDocument, char * pszClassName, int * piElementsFoundCount );   //Return all elements by classname

/*
 *  Try to find all element with the specified tagname.
 *  Return the elements array and elements count
 *  In case of error, return NULL
 */
int *       htmlDocumentGetElementsByTagName ( htmlDOCUMENT * pDocument, char * pszTagName, int * piElementsFoundCount );       //Return all elements by tagname

/*
 *  Try to find first element with the specified tagname.
 *  Return the first element handle
 *  In case of error, return NULL
 */
int         htmlDocumentGetFirstElementByTagName ( htmlDOCUMENT * pDocument, char * pszTagName );

/*
 *  Get the root element (html tag)
 */
int         htmlDocumentGetRoot ( htmlDOCUMENT * pDocument );

/*
 *  Get the body element (body tag)
 */
int         htmlDocumentGetBody ( htmlDOCUMENT * pDocument );


/*
 *  Add an element to a parent list (to the end of the list)
 */
int         htmlElementAppendChild ( htmlDOCUMENT * pDocument, int iElementHandleParent, int iElementHandleChild );

/*
 *  Get the current element child count relative to the current element
 */
int         htmlElementGetChildCount ( htmlDOCUMENT * pDocument, int iElementHandleParent );

/*
 *  Get the handle element child relative to the current element
 */
int         htmlElementGetChild ( htmlDOCUMENT * pDocument, int iElementHandleParent, int iChildIndex );

/*
 *  Get the parent handle element
 */
int         htmlElementGetParent ( htmlDOCUMENT * pDocument, int iElementHandleChild );

/*
 *  Remove an element
 */ 
int         htmlElementRemove ( htmlDOCUMENT * pDocument, int iElementHandle );

/*
 *  Create a stand-alone new element relative to no one
 */
int         htmlElementCreate ( htmlDOCUMENT * pDocument );



/*
 *  Set the tag name
 */
int         htmlElementSetTagName ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszTagName );
int         htmlElementSetTagName ( htmlDOCUMENT * pDocument, int iElementHandle, str * pstrTagName );
char *      htmlElementGetTagNameStrz ( htmlDOCUMENT * pDocument, int iElementHandle );

int         htmlElementSetIdName ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszIdName );

int         htmlElementSetStyle ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszStyle );

int         htmlElementSetAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, str * pstrAttributeNameAndValue );
int         htmlElementSetAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName, char * pszAttributeValue );
int         htmlElementRemoveAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName );
char *      htmlElementGetAttributeStrz ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName );
int         htmlElementHasAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName );

int         htmlElementAddClass ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszClassName );
int         htmlElementAddMultipleClass ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszMultipleClassName );
int         htmlElementHasClass ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszClassName );
int         htmlElementRemoveClass ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszClassName );
char *      htmlElementGetAllClassStrz ( htmlDOCUMENT * pDocument, int iElementHandle );

int         htmlElementSetContent ( htmlDOCUMENT * pDocument, int iElementHandle, str * pstrContent );
str *       htmlElementGetContentStr ( htmlDOCUMENT * pDocument, int iElementHandle );

 

#endif