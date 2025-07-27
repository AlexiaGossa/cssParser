#include "cssparse.h"

void * cssParseMalloc ( size_t size )
{
    void * p;

    if (size)
    {
        p = malloc ( size );
        memset ( p, 0, size );
    }
    else
    {
        p = NULL;
    }

    return p;
}

cssPROPERTY * cssParseSelectorAlloc ( cssSELECTOR * pSelector, int iNewPropertyCount )
{
    cssPROPERTY * pPropertyArray;

    pSelector->pPropertyArray = (cssPROPERTY *)realloc (
        pSelector->pPropertyArray,
        sizeof(cssPROPERTY) * (pSelector->iPropertyCount + iNewPropertyCount) );

    pPropertyArray = pSelector->pPropertyArray + pSelector->iPropertyCount;
    memset ( pPropertyArray, 0, sizeof(cssPROPERTY) * iNewPropertyCount );

    pSelector->iPropertyCount += iNewPropertyCount;
    
    return pPropertyArray;
}

