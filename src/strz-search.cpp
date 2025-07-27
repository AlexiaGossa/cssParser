#include "strz.h"


int strzMemmemBYTE ( void * pBuffer, void * pPattern, int lBufferLength, int lPatternLength, bool bEnableFirst )
{
    uint8_t * pSource;
    uint8_t * pFinder;
    int lFoundLength;
    int iFoundIndex;
    int iIndex;

    if ((!pPattern) ||
        (!pBuffer) ||
        (!lBufferLength) ||
        (lBufferLength<lPatternLength)) return -1;

    pSource = (uint8_t *)pBuffer;
    pFinder = (uint8_t *)pPattern;
    lFoundLength    = 0;
    iFoundIndex     = -1;
    iIndex          = 0;

    lBufferLength -= (lPatternLength - 1);

    do {

        if (*pSource == *pFinder)
        {
            if (!memcmp(pSource, pFinder, lPatternLength))
            {
                iFoundIndex = iIndex;            

                if (bEnableFirst)
                    return iIndex; //pSource;
            }
        }

        lBufferLength--;
        pSource++;
        iIndex++;
    } while (lBufferLength);

    return iFoundIndex;
}



size_t   strz_indexOfParams ( strz * pstrz, char * pszData, size_t indexStart, bool bEnableFirst )
{
    int     iOffset;
    int     iLength;
    int     iReturn;
    char *  pszSource;
    int     iSourceLength;

    pszSource       = pstrz->pszSource + pstrz->iUsedOffset;
    iSourceLength   = pstrz->iUsedLength;
    iOffset         = (int)indexStart;
    iLength         = iSourceLength - (int)indexStart;

    iReturn = strzMemmemBYTE (
        ((uint8_t *)pszSource) + iOffset,
        pszData,
        iLength,
        (int)strlen(pszData),
        bEnableFirst );
    if (iReturn!=-1) iReturn += iOffset;
    return iReturn;
}

/*

    indexOf and indexLastOf with PSZ

*/
size_t   strzIndexOf ( strz * pstrz, char * pszData, size_t indexStart )
{
    return strz_indexOfParams ( 
        pstrz, 
        pszData, 
        indexStart, 
        true );
}

size_t   strzIndexOf ( strz * pstrz, char * pszData )
{
    return strz_indexOfParams ( 
        pstrz, 
        pszData, 
        0,
        true );
}

size_t   strzLastIndexOf ( strz * pstrz, char * pszData)
{
    return strz_indexOfParams (
        pstrz, 
        pszData, 
        0, 
        false );
}

size_t   strzLastIndexOf ( strz * pstrz, char * pszData, size_t indexStart)
{
    return strz_indexOfParams (
        pstrz, 
        pszData, 
        indexStart, 
        false );
}





