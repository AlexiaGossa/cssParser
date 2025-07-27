#include "htmlnode.h"
#include "str.h"
#include "strz.h"

void            internal_htmlDocumentElementFree ( htmlELEMENT * pElement );
htmlELEMENT *   internal_htmlElementGet ( htmlDOCUMENT * pDocument, int iElementHandle );
void            htmlDocumentShowDebugElement ( htmlDOCUMENT * pDocument, int iCurrentElementHandle, int iCurrentElementLevel );


void        htmlDocumentInit ( htmlDOCUMENT * pDocument )
{
    memset ( pDocument, 0, sizeof(htmlDOCUMENT) );

    pDocument->iElementCount = 1;
    pDocument->pElement = (htmlELEMENT *)malloc ( sizeof(htmlELEMENT) );
    memset ( pDocument->pElement, 0, sizeof(htmlELEMENT) );

    pDocument->pElement->iHandle            = 0;
    pDocument->pElement->iHandleParent      = -1;
    pDocument->pElement->pstrzTagName       = strzAlloc ( "root" );

    pDocument->iCurrentHandleIndex = 1;
}


void        htmlDocumentFree ( htmlDOCUMENT * pDocument )
{
    int iIndex;
    htmlELEMENT * pElement;

    pElement = pDocument->pElement;
    for (iIndex=0;iIndex<pDocument->iElementCount;iIndex++,pElement++)
    {
        internal_htmlDocumentElementFree ( pElement );
    }

    free ( pDocument->pElement );
    memset ( pDocument, 0, sizeof(htmlDOCUMENT) );
}


void htmlDocumentShowDebug ( htmlDOCUMENT * pDocument )
{
    int             iElementRootHandle;

    iElementRootHandle = htmlDocumentGetRoot ( pDocument );
    htmlDocumentShowDebugElement ( pDocument, iElementRootHandle, 1 );
}


void htmlDocumentShowDebugElement ( htmlDOCUMENT * pDocument, int iCurrentElementHandle, int iCurrentElementLevel )
{
    int             iCount;
    int             iIndex;
    htmlELEMENT *   pElement;
    char            szLevel[256];
    char *          pszText;
    int             iTextLength;

    if ((pElement=internal_htmlElementGet(pDocument,iCurrentElementHandle))==NULL) return;

    //display tab space
    memset ( szLevel, ' ', iCurrentElementLevel*4 );
    szLevel[iCurrentElementLevel*4] = 0;


    //Show tag name
    printf ( "%s%s\n", szLevel, pElement->pstrzTagName );

    //Show id
    if (pElement->pstrzId) printf ( "%s\\unique id=[%s]\n", szLevel, pElement->pstrzId );

    //Show class
    iCount = pElement->iClassCount;
    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        printf ( "%s\\class=[%s]\n", szLevel, pElement->pstrzClass[iIndex] );
    }

    //Show attributes
    iCount = pElement->iAttributeCount;
    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        printf ( "%s\\%s=[%s]\n", szLevel, pElement->pAttributeArray[iIndex].pstrzAttributeName, pElement->pAttributeArray[iIndex].pstrzAttributeValue );
    }

    //Show content
    if (iTextLength=strLength(&(pElement->strContent)))
    {
        pszText = (char *)malloc ( iTextLength + 1 );
        strExport (
            &(pElement->strContent),
            pszText,
            iTextLength+1,
            true );
        printf ( "%s\\content=[%s]\n", szLevel, pszText );
        free ( pszText );
    }
    

    //Show all child elements
    iCount = pElement->iChildCount;
    if (iCount) printf ( "\n" );
    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        htmlDocumentShowDebugElement ( 
            pDocument,
            pElement->pChildArray[iIndex],
            iCurrentElementLevel+1 );
    }
}



void        internal_htmlDocumentElementFree ( htmlELEMENT * pElement )
{
    int iIndexClass;
    int iIndexAttribute;

    //Clean Tag name and Id
    strzFree ( pElement->pstrzTagName );
    strzFree ( pElement->pstrzId );
    strzFree ( pElement->pstrzStyle );

    //Clean classes
    for (iIndexClass=0;iIndexClass<pElement->iClassCount;iIndexClass++)
    {
        strzFree ( pElement->pstrzClass[iIndexClass] );
    }
    free ( pElement->pstrzClass );

    //Clean attribute
    for (iIndexAttribute=0;iIndexAttribute<pElement->iAttributeCount;iIndexAttribute++)
    {
        strzFree ( pElement->pAttributeArray[iIndexAttribute].pstrzAttributeName );
        strzFree ( pElement->pAttributeArray[iIndexAttribute].pstrzAttributeValue );
    }
    free ( pElement->pAttributeArray );

    //Clean children
    free ( pElement->pChildArray );
    strDelete ( &(pElement->strContent) );
}

int         htmlDocumentGetElementById ( htmlDOCUMENT * pDocument, char * pszId )
{
    int iIndex;
    htmlELEMENT * pElement;

    pElement = pDocument->pElement;
    for (iIndex=0;iIndex<pDocument->iElementCount;iIndex++,pElement++)
    {
        if (strcmp(pszId,pElement->pstrzId)==0)
            return pElement->iHandle;
    }

    return -1;
}


int *       htmlDocumentGetElementsByClassName ( htmlDOCUMENT * pDocument, char * pszClassName, int * piElementsFoundCount )   //Return all elements by classname
{
    int             iIndex;
    int             iIndexClass;
    htmlELEMENT *   pElement;
    int *           piHandleFoundArray;
    int             iHandleFoundCount;
    int             iHandleFoundCountAlloc;

    iHandleFoundCountAlloc = 16;
    piHandleFoundArray = (int *)malloc ( sizeof(int) * iHandleFoundCountAlloc );
    iHandleFoundCount = 0;

    pElement = pDocument->pElement;
    for (iIndex=0;iIndex<pDocument->iElementCount;iIndex++,pElement++)
    {
        for (iIndexClass=0;iIndexClass<pElement->iClassCount;iIndexClass++)
        {
            if (strcmp(pszClassName,pElement->pstrzClass[iIndexClass])==0)
            {
                iHandleFoundCount += 1;
                if (iHandleFoundCount>iHandleFoundCountAlloc)
                {
                    iHandleFoundCountAlloc += 16;
                    piHandleFoundArray = (int *)realloc ( piHandleFoundArray, sizeof(int) * iHandleFoundCountAlloc );
                }

                piHandleFoundArray[iHandleFoundCount-1] = pElement->iHandle;
                break;
            }
        }
    }

    *piElementsFoundCount = iHandleFoundCount;

    if (iHandleFoundCount)
    {
        return piHandleFoundArray;
    }
    else
    {
        free ( piHandleFoundArray );
        return NULL;
    }
}

int *       htmlDocumentGetElementsByTagName ( htmlDOCUMENT * pDocument, char * pszTagName, int * piElementsFoundCount )
{
    int             iIndex;
    htmlELEMENT *   pElement;
    int *           piHandleFoundArray;
    int             iHandleFoundCount;
    int             iHandleFoundCountAlloc;

    iHandleFoundCountAlloc = 16;
    piHandleFoundArray = (int *)malloc ( sizeof(int) * iHandleFoundCountAlloc );
    iHandleFoundCount = 0;

    pElement = pDocument->pElement;
    for (iIndex=0;iIndex<pDocument->iElementCount;iIndex++,pElement++)
    {
        if (strcmp(pszTagName,pElement->pstrzTagName)==0)
        {
            iHandleFoundCount += 1;
            if (iHandleFoundCount>iHandleFoundCountAlloc)
            {
                iHandleFoundCountAlloc += 16;
                piHandleFoundArray = (int *)realloc ( piHandleFoundArray, sizeof(int) * iHandleFoundCountAlloc );
            }

            piHandleFoundArray[iHandleFoundCount-1] = pElement->iHandle;
        }
    }

    *piElementsFoundCount = iHandleFoundCount;

    if (iHandleFoundCount)
    {
        return piHandleFoundArray;
    }
    else
    {
        free ( piHandleFoundArray );
        return NULL;
    }
}

int         htmlDocumentGetFirstElementByTagName ( htmlDOCUMENT * pDocument, char * pszTagName )
{
    int             iIndex;
    htmlELEMENT *   pElement;

    pElement = pDocument->pElement;
    for (iIndex=0;iIndex<pDocument->iElementCount;iIndex++,pElement++)
    {
        if (strcmp(pszTagName,pElement->pstrzTagName)==0)
        {
            return pElement->iHandle;
        }
    }
    return -1;
}


int         htmlDocumentGetRoot ( htmlDOCUMENT * pDocument )
{
    return htmlDocumentGetFirstElementByTagName ( pDocument, "root" );
}

int         htmlDocumentGetBody ( htmlDOCUMENT * pDocument )
{
    return htmlDocumentGetFirstElementByTagName ( pDocument, "body" );
}


int         htmlElementCreate ( htmlDOCUMENT * pDocument )
{
    htmlELEMENT *   pElement;

    pDocument->iElementCount += 1;
    pDocument->pElement = (htmlELEMENT *)realloc ( pDocument->pElement, sizeof(htmlELEMENT) * (pDocument->iElementCount) );
    
    pElement = pDocument->pElement + (pDocument->iElementCount - 1);
    memset ( pElement, 0, sizeof(htmlELEMENT) );

    pElement->iHandle               = pDocument->iCurrentHandleIndex;
    pElement->iHandleParent         = -1;
    pDocument->iCurrentHandleIndex  += 1;

    return pElement->iHandle;
}

htmlELEMENT * internal_htmlElementGet ( htmlDOCUMENT * pDocument, int iElementHandle )
{
    int             iIndex;
    htmlELEMENT *   pElement;

    pElement = pDocument->pElement;
    for (iIndex=0;iIndex<pDocument->iElementCount;iIndex++,pElement++)
    {
        if (pElement->iHandle == iElementHandle)
        {
            return pElement;
        }
    }
    return NULL;   
}



/*
 *  Add an element to a parent list (to the end of the list)
 */
int         htmlElementAppendChild ( htmlDOCUMENT * pDocument, int iElementHandleParent, int iElementHandleChild )
{
    htmlELEMENT *   pElementParent;
    htmlELEMENT *   pElementChild;

    if ((pElementParent=internal_htmlElementGet ( pDocument, iElementHandleParent ))==NULL) return -1;
    if ((pElementChild=internal_htmlElementGet ( pDocument, iElementHandleChild ))==NULL) return -1;

    pElementParent->iChildCount += 1;
    pElementParent->pChildArray = (int *)realloc ( pElementParent->pChildArray, sizeof(int) * (pElementParent->iChildCount) );
    pElementParent->pChildArray[pElementParent->iChildCount-1] = iElementHandleChild;

    pElementChild->iHandleParent = iElementHandleParent;

    return 0;
}

/*
 *  Get the current element child count relative to the current element
 */
int         htmlElementGetChildCount ( htmlDOCUMENT * pDocument, int iElementHandleParent )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandleParent ))==NULL) return -1;

    return pElement->iChildCount;
}

/*
 *  Get the handle element child relative to the current element
 */
int         htmlElementGetChild ( htmlDOCUMENT * pDocument, int iElementHandleParent, int iChildIndex )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandleParent ))==NULL) return -1;

    if ( (iChildIndex>=0) && (iChildIndex<pElement->iChildCount) )
        return pElement->pChildArray[iChildIndex];

    return -1;
}

/*
 *  Get the parent handle element
 */
int         htmlElementGetParent ( htmlDOCUMENT * pDocument, int iElementHandleChild )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandleChild ))==NULL) return -1;

    return pElement->iHandleParent;
}
/*
 *  Remove an element
 */ 


int         internal_htmlElementRemoveRecursive ( htmlDOCUMENT * pDocument, int iElementHandle )
{
    htmlELEMENT *   pElement;
    int             iIndexChild;
    int             iElementHandleParent;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    //Recurse the children
    for (iIndexChild=0;iIndexChild<pElement->iChildCount;iIndexChild++)
    {
        //Remove the children
        internal_htmlElementRemoveRecursive ( pDocument, pElement->pChildArray[iIndexChild] );
    }

    //Clean the element
    internal_htmlDocumentElementFree ( pElement );

    //The parent
    iElementHandleParent = pElement->iHandleParent;

    //Remove element but no realloc
    memcpy ( pElement, pDocument->pElement + (pDocument->iElementCount-1), sizeof(htmlELEMENT) );
    pDocument->iElementCount -= 1;

    return iElementHandleParent;
}


int         htmlElementRemove ( htmlDOCUMENT * pDocument, int iElementHandle )
{
    htmlELEMENT *   pElementParent;
    int             iIndexChild;
    int             iElementHandleParent;
    int             iMoveChildCount;

    iElementHandleParent = internal_htmlElementRemoveRecursive ( pDocument, iElementHandle );

    //Element realloc
    pDocument->pElement = (htmlELEMENT *)realloc ( pDocument->pElement, sizeof(htmlELEMENT) * pDocument->iElementCount );

    //Get the parent
    if ((pElementParent=internal_htmlElementGet ( pDocument, iElementHandleParent ))==NULL) return -1;

    //Try to find the child...
    for (iIndexChild=0;iIndexChild<pElementParent->iChildCount;iIndexChild++)
    {
        if (pElementParent->pChildArray[iIndexChild] == iElementHandleParent)
        {
            iMoveChildCount = pElementParent->iChildCount - (iIndexChild+1);

            if (iMoveChildCount)
                memmove ( pElementParent->pChildArray + iIndexChild, pElementParent->pChildArray + (iIndexChild+1), sizeof(htmlELEMENT) * iMoveChildCount );

            pElementParent->iChildCount -= 1;
            pElementParent->pChildArray = (int *)realloc ( pElementParent->pChildArray, sizeof(int) * pElementParent->iChildCount );

            break;
        }
    }

    return 0;
}


int         htmlElementSetTagName ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszTagName )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    strzCopy ( &(pElement->pstrzTagName), pszTagName );
    return 0;
}

int         htmlElementSetTagName ( htmlDOCUMENT * pDocument, int iElementHandle, str * pstrTagName )
{
    htmlELEMENT *   pElement;
    char            szTagName[256];

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    strExport ( pstrTagName, szTagName, 256, true );
    strzCopy ( &(pElement->pstrzTagName), szTagName );
    return 0;
}

int         htmlElementSetIdName ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszIdName )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    strzCopy ( &(pElement->pstrzId), pszIdName );
    return 0;
}

int         htmlElementSetStyle ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszStyle )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    strzCopy ( &(pElement->pstrzStyle), pszStyle );
    return 0;
}




char *      htmlElementGetTagNameStrz ( htmlDOCUMENT * pDocument, int iElementHandle )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return NULL;

    return strzAlloc ( pElement->pstrzTagName );
}

/*
 *  Attribute management
 */

int         internal_htmlElementGetAttributeIndex ( htmlELEMENT * pElement, char * pszAttributeName )
{
    int iIndexAttribute;

    //1. Try to find an actual attribute name
    for (iIndexAttribute=0;iIndexAttribute<pElement->iAttributeCount;iIndexAttribute++)
    {
        if (strcmp(pszAttributeName,pElement->pAttributeArray[iIndexAttribute].pstrzAttributeName)==0)
        {
            return iIndexAttribute;
        }
    }

    return -1;
}

int         htmlElementSetAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName, char * pszAttributeValue )
{
    htmlELEMENT *   pElement;
    int             iIndexAttribute;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    //1. Try to find an actual attribute name
    iIndexAttribute = internal_htmlElementGetAttributeIndex ( pElement, pszAttributeName );
    if (iIndexAttribute>=0)
    {
        //It exists ! Replace the current value...
        strzCopy ( &(pElement->pAttributeArray[iIndexAttribute].pstrzAttributeValue), pszAttributeValue );
        return 0;
    }

    //Add a new name and a new value
    pElement->iAttributeCount+=1;
    pElement->pAttributeArray = (htmlELEMENTATTRIBUTE *)realloc ( pElement->pAttributeArray, sizeof(htmlELEMENTATTRIBUTE) * pElement->iAttributeCount );
    pElement->pAttributeArray[pElement->iAttributeCount-1].pstrzAttributeName = strzAlloc ( pszAttributeName );
    pElement->pAttributeArray[pElement->iAttributeCount-1].pstrzAttributeValue = strzAlloc ( pszAttributeValue );

    return 0;
}

                                                      
int         htmlElementRemoveAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName )
{
    htmlELEMENT *   pElement;
    int             iIndexAttribute;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    //1. Try to find the attribute name
    iIndexAttribute = internal_htmlElementGetAttributeIndex ( pElement, pszAttributeName );
    if (iIndexAttribute>=0)
    {
        //It exists !

        //Free memory
        strzFree ( pElement->pAttributeArray[iIndexAttribute].pstrzAttributeName );
        strzFree ( pElement->pAttributeArray[iIndexAttribute].pstrzAttributeValue );

        //Switching
        memcpy (
            pElement->pAttributeArray + iIndexAttribute,
            pElement->pAttributeArray + (pElement->iAttributeCount - 1),
            sizeof(htmlELEMENTATTRIBUTE) );

        pElement->iAttributeCount -= 1;
        pElement->pAttributeArray = (htmlELEMENTATTRIBUTE *)realloc ( pElement->pAttributeArray, sizeof(htmlELEMENTATTRIBUTE) * pElement->iAttributeCount );
        return 0;
    }

    return -1;
}


char *      htmlElementGetAttributeStrz ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName )
{
    htmlELEMENT *   pElement;
    int             iIndexAttribute;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return NULL;

    //1. Try to find the attribute name
    iIndexAttribute = internal_htmlElementGetAttributeIndex ( pElement, pszAttributeName );
    if (iIndexAttribute>=0)
    {
        //It exists ! Return the value
        return strzAlloc ( pElement->pAttributeArray[iIndexAttribute].pstrzAttributeValue );
    }
    return NULL;
}

int         htmlElementHasAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszAttributeName )
{
    htmlELEMENT *   pElement;
    int             iIndexAttribute;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    //1. Try to find the attribute name
    iIndexAttribute = internal_htmlElementGetAttributeIndex ( pElement, pszAttributeName );
    if (iIndexAttribute>=0)
    {
        //It exists ! Return no error
        return 0;
    }
    return -1;
}

/*
 *  Class management
 */

int         internal_htmlElementGetClassIndex ( htmlELEMENT * pElement, char * pszClassName )
{
    int             iIndexClass;

    for (iIndexClass=0;iIndexClass<pElement->iClassCount;iIndexClass++)
    {
        if (strcmp(pElement->pstrzClass[iIndexClass],pszClassName)==0)
            return iIndexClass;
    }
    return -1;
}

int         htmlElementAddClass ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszClassName )
{
    htmlELEMENT *   pElement;
    int             iIndexClass;
    char            szClassName[256];
    char *          pszBegin;
    char *          pszDetect;
    bool            bAddClass;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    pszBegin = pszClassName;
    do {
        pszDetect = strchr ( pszBegin, ' ' );

        bAddClass = false;

        if (pszDetect==NULL)
        {
            if (strlen(pszBegin))
            {
                strcpy ( szClassName, pszBegin );
                bAddClass = true;
            }
        }
        else
        {
            if ( ((int)(pszDetect-pszBegin)) > 1 )
            {
                memcpy ( 
                    szClassName,
                    pszBegin,
                    pszDetect-pszBegin );
                szClassName[pszDetect-pszBegin] = 0;
                bAddClass = true;
            }

            pszBegin = pszDetect + 1;
        }

        if (bAddClass)
        {
            //1. Try to find the class name
            iIndexClass = internal_htmlElementGetClassIndex ( pElement, szClassName );
            if (iIndexClass==-1)
            {
                pElement->iClassCount += 1;
                pElement->pstrzClass = (char **)realloc ( pElement->pstrzClass, sizeof(char *) * pElement->iClassCount );
                pElement->pstrzClass[pElement->iClassCount-1] = strzAlloc ( szClassName );
            }
        }
    } while (pszDetect);

    
    return 0;
}

int         htmlElementHasClass ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszClassName )
{
    htmlELEMENT *   pElement;
    int             iIndexClass;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    //1. Try to find the class name
    iIndexClass = internal_htmlElementGetClassIndex ( pElement, pszClassName );
    if (iIndexClass>=0)
        return 0;
    
    return -1;
}

int         htmlElementRemoveClass ( htmlDOCUMENT * pDocument, int iElementHandle, char * pszClassName )
{
    htmlELEMENT *   pElement;
    int             iIndexClass;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    //1. Try to find the class name
    iIndexClass = internal_htmlElementGetClassIndex ( pElement, pszClassName );
    if (iIndexClass>=0)
    {
        strzFree ( pElement->pstrzClass[iIndexClass] );
        pElement->pstrzClass[iIndexClass] = pElement->pstrzClass[pElement->iClassCount-1];
        pElement->iClassCount -= 1;
        pElement->pstrzClass = (char **)realloc ( pElement->pstrzClass, sizeof(char *) * pElement->iClassCount );
        return 0;
    }
    
    return -1;
}

char *      htmlElementGetAllClassStrz ( htmlDOCUMENT * pDocument, int iElementHandle )
{
    int             iIndexClass;
    char *          pstrz;
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return NULL;

    if (pElement->iClassCount)
    {
        pstrz = NULL;
        for (iIndexClass=0;iIndexClass<pElement->iClassCount;iIndexClass++)
        {     
            if (iIndexClass) strzConcat ( &pstrz, " " );
            strzConcat ( &pstrz, pElement->pstrzClass[iIndexClass] );
        }
        return pstrz;
    }

    return NULL;
}

/*
 *  Content management
 */

int         htmlElementSetContent ( htmlDOCUMENT * pDocument, int iElementHandle, str * pstrContent )
{
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return -1;

    strDelete ( &(pElement->strContent) );
    strNew ( &(pElement->strContent), pstrContent );
    strCopy ( &(pElement->strContent), pstrContent );

    return 0;
}

str *       htmlElementGetContentStr ( htmlDOCUMENT * pDocument, int iElementHandle )
{
    str *           pstr;
    htmlELEMENT *   pElement;

    if ((pElement=internal_htmlElementGet ( pDocument, iElementHandle ))==NULL) return NULL;

    pstr = (str *)malloc ( sizeof(str) );
    strNew ( pstr, &(pElement->strContent) );
    strCopy ( pstr, &(pElement->strContent) );
    return pstr;
}

/*
 *  Attribute name & value parsing
 */
int         htmlElementSetAttribute ( htmlDOCUMENT * pDocument, int iElementHandle, str * pstrAttributeNameAndValue )
{
    int         iCount;
    int         iIndex;
    uint32_t    uiCurrentChar;
    bool        bDetectedSingleQuote;
    bool        bDetectedDoubleQuote;
    uint32_t    uiDetectedQuote;
    bool        bDetectedValue;
    bool        bDetectedQuote;
    bool        bEnableOutput;

    str         strName;
    str         strValue;

    char        szAttributeName[256];
    char        szAttributeValue[2048];


    strNew ( &strName, pstrAttributeNameAndValue );
    strNew ( &strValue, pstrAttributeNameAndValue );
    bDetectedSingleQuote    = false;
    bDetectedDoubleQuote    = false;
    bDetectedValue          = false;
    bDetectedQuote          = false;
    uiDetectedQuote         = 0;

    iCount                  = strLength ( pstrAttributeNameAndValue );
    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        uiCurrentChar = strCharGet(pstrAttributeNameAndValue,iIndex);
        bEnableOutput = false;

        switch (uiCurrentChar)
        {
            case 39: //single quote
            case 34: //double quote
                if (bDetectedValue)
                {
                    if (!uiDetectedQuote)
                    {
                        //Open a new single or double quote
                        uiDetectedQuote = uiCurrentChar;
                    }
                    else if (uiDetectedQuote==uiCurrentChar)
                    {
                        //Detect a closing single or double quote
                        //...
                    }
                    else
                    {
                        bEnableOutput = true;
                    }
                }
                break;

            case '=':
                if (!uiDetectedQuote)
                {
                    bDetectedValue = true;
                }
                else
                {
                    bEnableOutput = true;
                }
                break;

            default:
                bEnableOutput = true;
                break;
        }


        if (bEnableOutput)
        {
            if (!uiDetectedQuote)
                strCharConcat ( &strName, uiCurrentChar );
            else
                strCharConcat ( &strValue, uiCurrentChar );
        }
    }

    strExport ( &strName, szAttributeName, 256, true );
    strExport ( &strValue, szAttributeValue, 2048, true );

    if (stricmp(szAttributeName,"class")==0)
        htmlElementAddClass ( 
            pDocument, 
            iElementHandle, 
            szAttributeValue );
    else if (stricmp(szAttributeName,"id")==0)
        htmlElementSetIdName (
            pDocument, 
            iElementHandle, 
            szAttributeValue );
    else if (stricmp(szAttributeName,"style")==0)
        htmlElementSetStyle (
            pDocument, 
            iElementHandle, 
            szAttributeValue );
    else
        htmlElementSetAttribute ( 
            pDocument, 
            iElementHandle, 
            szAttributeName, 
            szAttributeValue );

    strDelete ( &strName );
    strDelete ( &strValue );

    return 0;
}

