#include "cssparse.h"


zRETURN cssParseTreePrintf ( 
    cssOBJECT *     pcssObject )
{
    int iIndex, iCount;
    int iPropertyIndex;
    int iPropertyCount;
    cssPROPERTY * pProperty;

    iCount = pcssObject->iSelectorCount;
    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        printf ( "Selector : %s\n", pcssObject->pSelectorArray[iIndex].sz );

        iPropertyCount = pcssObject->pSelectorArray[iIndex].iPropertyCount;
        for (iPropertyIndex=0;iPropertyIndex<iPropertyCount;iPropertyIndex++)
        {
            pProperty = pcssObject->pSelectorArray[iIndex].pPropertyArray + iPropertyIndex;

            printf ( "  Property : %s\n", 
                cssParseSelectorFromToken ( pProperty->iPropertyType ) );

            if (pProperty->value.bFloat)
                printf ( "    fValue = %f\n", pProperty->value.fValue );
            else if (pProperty->value.bInteger)
                printf ( "    iValue = %d\n", pProperty->value.iValue );
            else if (pProperty->value.bString)
                printf ( "    szValue = %s\n", pProperty->value.pszValue );

            printf ( "      > Type : %s\n", cssParseSelectorFromToken ( pProperty->value.iValueType ) );
        }
    }

    return 0;
}