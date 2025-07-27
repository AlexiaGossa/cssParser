#include "cssparse.h"

zRETURN cssParseElementInline (
    int iHandleElement,
    str * pstrContent,
    cssOBJECT *     pcssObject )
{
    cssNODE *       baseNode;
    cssNODE *       newNode;

    baseNode = cssParseContentStyle ( pstrContent );

    //newNode = baseNode->copy();
    //strCopy ( &(newNode->strText), &strTemp );


    //Allocate a new selector
    cssSELECTOR * pSelector;
    pcssObject->pSelectorArray = (cssSELECTOR *)realloc ( 
        pcssObject->pSelectorArray,
        sizeof(cssSELECTOR) * (pcssObject->iSelectorCount+1) );
    memset (
        pcssObject->pSelectorArray + pcssObject->iSelectorCount,
        0,
        sizeof(cssSELECTOR) );

    pSelector = pcssObject->pSelectorArray + pcssObject->iSelectorCount;
    pcssObject->iSelectorCount += 1;

    //The new selector name
    pSelector->iElementHandle = iHandleElement;

    //cssParseSelector ( pSelector, newNode );
    cssParseSelector ( pSelector, baseNode );

    //delete newNode;
    delete baseNode;
    return 0;
}

zRETURN cssParseElementContent (
    str *           pstrElement,
    str *           pstrContent,
    cssOBJECT *     pcssObject )
{
    int             iIndexPrev;
    int             iIndexNext;
    char            szTmp[64];
    cssNODE *       baseNode;
    str             strTemp;

    /*
        Style separators :
            ;   next style
            :   style description
            ,   next description (or next parameter in bracket)
            ()  parameters
            '"  String
    */

    //Extract content into a node
    /*
    baseNode = cssParseContentStyle ( 
        strContentStyleText,
        pcssDebug );
    */
    baseNode = cssParseContentStyle ( 
        pstrContent );

    /*
        Get all elements !
    */

    //strElementText.print();

    //static int iCount=0;
    //iCount++;

    strNew ( &strTemp );

    iIndexPrev = 0;
    do {
        
        iIndexNext = strIndexOf ( pstrElement, ",", iIndexPrev );

        if (iIndexNext!=-1)
        {
            strSubstring ( &strTemp, pstrElement, iIndexPrev, iIndexNext-1 );
            iIndexPrev = iIndexNext + 1;
        }
        else
        {
            strSubstring ( &strTemp, pstrElement, iIndexPrev );
        }

        //Clean
        strTrim ( &strTemp );

        //Element name
        //strTemp.exportBytes ( szTmp, 64, STRING_ENCODING_ISO8859_15, true );


        if (strLength(&strTemp))
        {

            cssNODE * newNode;



            //baseNode->printTree();

            newNode = baseNode->copy();
            strCopy ( &(newNode->strText), &strTemp );

            



            //newNode->printTree();

            /*
            
            Convert Node into a list
            
            */

            //Allocate a new selector
            cssSELECTOR * pSelector;
            pcssObject->pSelectorArray = (cssSELECTOR *)realloc ( 
                pcssObject->pSelectorArray,
                sizeof(cssSELECTOR) * (pcssObject->iSelectorCount+1) );
            memset (
                pcssObject->pSelectorArray + pcssObject->iSelectorCount,
                0,
                sizeof(cssSELECTOR) );

            pSelector = pcssObject->pSelectorArray + pcssObject->iSelectorCount;
            pcssObject->iSelectorCount += 1;

            //The new selector name
            strExport ( &strTemp, pSelector->sz, 64, true );
            //strcpy ( pSelector->sz, szTmp );

            cssParseSelector ( pSelector, newNode );



            delete newNode;

        }


        strTruncate ( &strTemp, 0 );

        //printf ( "Element : %s\n", szTmp );

    } while (iIndexNext!=-1);

    strDelete ( &strTemp );
    delete baseNode;

    return 0;
}
