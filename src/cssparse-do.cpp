#include "cssparse.h"

zRETURN cssParseInit ( cssOBJECT * pcssObject )
{
    memset ( pcssObject, 0, sizeof(cssOBJECT) );

    return 0;
}

zRETURN cssParseFree ( cssOBJECT * pcssObject )
{
    int             iIndexSelector;
    int             iIndexProperty;
    cssSELECTOR *   pSelectorArray;
    cssPROPERTY *   pPropertyArray;

    for (iIndexSelector=0;iIndexSelector<pcssObject->iSelectorCount;iIndexSelector++)
    {
        pSelectorArray = pcssObject->pSelectorArray + iIndexSelector;

        for (iIndexProperty=0;iIndexProperty<pSelectorArray->iPropertyCount;iIndexProperty++)
        {
            pPropertyArray = pSelectorArray->pPropertyArray + iIndexProperty;


            if (pPropertyArray->value.pszValue)
                free ( pPropertyArray->value.pszValue );
        }
        free ( pSelectorArray->pPropertyArray );
    }

    free ( pcssObject->pSelectorArray );

    return 0;
}


zRETURN cssParseFile (
    char *          pszFilename,
    cssOBJECT *     pcssObject )
{
    str     strTemp;
    zRETURN ret;

    if (strNewFileGetContents ( &strTemp, pszFilename ))
    {
        ret = cssParseStr (
            &strTemp,
            pcssObject );
                
        strDelete ( &strTemp );    
    }
    else ret = -1;

    return ret;
}



zRETURN cssParseText (
    char *          pszText,
    cssOBJECT *     pcssObject )
{
    str     strTemp;
    zRETURN ret;

    strNew ( &strTemp, STRING_ENCODING_SIMPLEBYTE );
    strCopy ( &strTemp, pszText );

    ret = cssParseStr (
        &strTemp,
        pcssObject );

    strDelete ( &strTemp );

    return ret;
}


zRETURN cssParseStr (
    str *           pstr,
    cssOBJECT *     pcssObject )
{
    int         iIndex, iCount;
    int         iCurrentMode;
    int         iLineCount;
    int         iColumnCount;
/*
    int         TextElementIndex_iStart;
    int         TextElementIndex_iStop;

    int         TextContentIndex_iStart;
    int         TextContentIndex_iStop;
*/
    int         iTextElementBegin;
    //int         iTextElementEnd;

    int         iTextContentStyleBegin;
//    int         iTextContentStyleEnd;

    int         iStateElement;
    int         iStateContent;
    bool        bConcatElement;
    bool        bConcatContent;
    int         iStateComment;

    int         iStateElementLine;
    int         iStateContentLine;

    str         strElement;
    str         strContent;
    uint32_t    uiCurrentChar;
    

    iCurrentMode                        = cssMODE_BASE;
    iTextElementBegin                   = -1;
    iTextContentStyleBegin              = -1;
    
    iCount                              = strLength(pstr);

    iStateElement                       = 1;
    iStateContent                       = 0;
    bConcatElement                      = false;
    bConcatContent                      = false;
    iStateComment                       = 0;

    iStateElementLine                   = -1;
    iStateContentLine                   = -1;

    iLineCount                          = 1;
    iColumnCount                        = 1;

    /*
    TextElementIndex_iStart             = -1;
    TextElementIndex_iStop              = -1;

    TextContentIndex_iStart             = -1;
    TextContentIndex_iStop              = -1;
    */

    strNew ( &strElement );
    strNew ( &strContent );



    for (
        iIndex=0;
        iIndex<iCount;
        iIndex++)
    {
        bConcatElement  = false;
        bConcatContent  = false;
        uiCurrentChar   = strCharGet(pstr,iIndex);

        switch (uiCurrentChar)
        {
            //Des caractères inutiles
            case 9:
                break;

            case 13:    //CR
                if (strCharGet(pstr,iIndex+1) != 10)
                {
                    iLineCount      += 1;
                    iColumnCount    = 1;

                    //Stocke l'offset de la ligne
                }
                break;

            case 10:    //LF
                iLineCount      += 1;
                iColumnCount    = 1;

                //Stocke l'offset de la ligne
                break;

            case 32:
                if ((iStateElement)&&(strCharGet(pstr,iIndex-1)!=32))
                    bConcatElement = true;

                if ((iStateContent)&&(strCharGet(pstr,iIndex-1)!=32))
                    bConcatContent = true;

                break;

            default:
                if (uiCurrentChar>32)
                {
                    if (iStateElement==1)
                        bConcatElement = true;

                    if (iStateContent==1)
                        bConcatContent = true;
                }
                break;

            //Comment protection
            case '/':
                if (strCharGet(pstr,iIndex+1)=='*')
                    iStateComment = 1;
                else if (strCharGet(pstr,iIndex-1)=='*')
                    iStateComment = 0;                               
                else
                {
                    if (iStateElement==1)
                        bConcatElement = true;

                    if (iStateContent==1)
                        bConcatContent = true;
                }
                break;


            case '{':
                if (iStateComment==0)
                {
                    //Disable Element concat
                    bConcatElement  = false;
                    iStateElement   = 0;

                    //Enable Content concat
                    iStateContent   = 1;
                }
                break;

            case '}':
                if (iStateComment==0)
                {

                    //On parse !
                    /*
                    cssParseElementStyle (
                        strTextElement,     //Element text
                        strTextContent,     //Element style text
                        pcssObject,         //Objects data
                        pcssDebug );
                    */
/*
                    strzLink ( &strzElement, pstr );
                    strzLink ( &strzContent, pstr );

                    strzElement.iUsedOffset +=  TextElementIndex_iStart;
                    strzElement.iUsedLength =   TextElementIndex_iStop - TextElementIndex_iStart + 1;

                    strzContent.iUsedOffset +=  TextContentIndex_iStart;
                    strzContent.iUsedLength =   TextContentIndex_iStop - TextContentIndex_iStart + 1;
*/
                    
                    
                    
                    
                    
                    cssParseElementContent (
                        &strElement,
                        &strContent,
                        pcssObject );




/*                                    
                    cssParseElementStyle (
                        pszText, iTextLength,
                        TextElementIndex_iStart,        //Element text start
                        TextElementIndex_iStop,         //Element text stop
                        TextContentIndex_iStart,        //Element style text start
                        TextContentIndex_iStop,         //Element style text stop
                        pcssObject,         //Objects data
                        pcssDebug );
*/

                    //strTextElement = "";
                    //strTextContent = "";

                    /*
                    TextElementIndex_iStart             = -1;
                    TextElementIndex_iStop              = -1;
                    TextContentIndex_iStart             = -1;
                    TextContentIndex_iStop              = -1;
                    */

                    //Nettoyage
                    strZero ( &strElement );
                    strZero ( &strContent );

                    //Enable Element concat
                    iStateElement   = 1;
                
                    //Enable Content concat
                    iStateContent   = 0;
                }
                break;
        }

        if (iStateComment == 0)
        {
            if (bConcatElement)
            {
                if (iStateElementLine!=0) iStateElementLine = iLineCount;
                strCharConcat ( &strElement, uiCurrentChar );

                //strTextElement.charConcat ( strText.charGet(iIndex) );
                //if (TextElementIndex_iStart==-1) TextElementIndex_iStart = iIndex;
                //TextElementIndex_iStop = iIndex;
                
            }

            if (bConcatContent)
            {
                if (iStateContentLine!=0) iStateContentLine = iLineCount;
                strCharConcat ( &strContent, uiCurrentChar );

                //strTextContent.charConcat ( strText.charGet(iIndex) );
                //if (TextContentIndex_iStart==-1) TextContentIndex_iStart = iIndex;
                //TextContentIndex_iStop = iIndex;
            }

        }

    }

    //Nettoyage !
    strDelete ( &strElement );
    strDelete ( &strContent );

    return 0;

}


