#include "cssparser.h"

#define strcharIndex(psz,len,idx) ((((idx)>=0)&&((idx)<(len)))?((psz)[idx]):(0))
/*
Parser CSS
Fast CSS parser uses byte analysis (faster than UTF-32)
*/
zRETURN cssParse (
    char *          pszText,
    int             iTextLength,
    cssOBJECT *     pcssObject,
    cssDEBUG *      pcssDebug )
{
    int         iIndex, iCount;
    int         iCurrentMode;
    int         iLineCount;
    int         iColumnCount;

    //eSTRING strTextElement(strText);
    //eSTRING strTextContent(strText);

    int         TextElementIndex_iStart;
    int         TextElementIndex_iStop;

    int         TextContentIndex_iStart;
    int         TextContentIndex_iStop;

    int         iTextElementBegin;
    int         iTextElementEnd;

    int         iTextContentStyleBegin;
    int         iTextContentStyleEnd;
    

    iCurrentMode            = cssMODE_BASE;
    iTextElementBegin       = -1;
    iTextContentStyleBegin  = -1;
    
    iCount                  = iTextLength;

    int         iStateElement           = 1;
    int         iStateContent           = 0;
    bool        bConcatElement          = false;
    bool        bConcatContent          = false;
    int         iStateComment           = 0;

    int         iStateElementLine       = -1;
    int         iStateContentLine       = -1;

    iLineCount                          = 1;
    iColumnCount                        = 1;

    pcssObject->iSelectorCount          = 0;
    pcssObject->pSelectorArray          = NULL;

    TextElementIndex_iStart             = -1;
    TextElementIndex_iStop              = -1;

    TextContentIndex_iStart             = -1;
    TextContentIndex_iStop              = -1;

    pcssDebug->iLineCount = 0;
    pcssDebug->piLineOffset = null;




    for (
        iIndex=0;
        iIndex<iCount;
        iIndex++)
    {
        bConcatElement = false;
        bConcatContent = false;

        switch (strcharIndex(pszText,iTextLength,iIndex))
        {
            //Des caractères inutiles
            case 9:
                break;

            case 13:    //CR
                if (strcharIndex(pszText,iTextLength,iIndex+1) != 10)
                {
                    iLineCount      += 1;
                    iColumnCount    = 1;

                    //Stocke l'offset de la ligne
                    pcssDebug->iLineCount += 1;
                    pcssDebug->piLineOffset = (int *)realloc ( pcssDebug->piLineOffset, sizeof(int) * pcssDebug->iLineCount );
                    pcssDebug->piLineOffset[pcssDebug->iLineCount-1] = iIndex;
                }
                break;

            case 10:    //LF
                iLineCount      += 1;
                iColumnCount    = 1;

                //Stocke l'offset de la ligne
                pcssDebug->iLineCount += 1;
                pcssDebug->piLineOffset = (int *)realloc ( pcssDebug->piLineOffset, sizeof(int) * pcssDebug->iLineCount );
                pcssDebug->piLineOffset[pcssDebug->iLineCount-1] = iIndex;
                break;

            case 32:
                if ((iStateElement)&&(strcharIndex(pszText,iTextLength,iIndex-1)!=32))
                    bConcatElement = true;

                if ((iStateContent)&&(strcharIndex(pszText,iTextLength,iIndex-1)!=32))
                    bConcatContent = true;

                break;

            default:
                if (iStateElement==1)
                    bConcatElement = true;

                if (iStateContent==1)
                    bConcatContent = true;
                break;

            //Comment protection
            case '/':
                if (strcharIndex(pszText,iTextLength,iIndex+1)=='*')
                    iStateComment = 1;
                else if (strcharIndex(pszText,iTextLength,iIndex-1)=='*')
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
                    pcssDebug->iCurrentLineElement = iStateElementLine;
                    pcssDebug->iCurrentLineContent = iStateContentLine;

                    //On parse !
                    /*
                    cssParseElementStyle (
                        strTextElement,     //Element text
                        strTextContent,     //Element style text
                        pcssObject,         //Objects data
                        pcssDebug );
                    */
                    cssParseElementStyle (
                        pszText, iTextLength,
                        TextElementIndex_iStart,        //Element text start
                        TextElementIndex_iStop,         //Element text stop
                        TextContentIndex_iStart,        //Element style text start
                        TextContentIndex_iStop,         //Element style text stop
                        pcssObject,         //Objects data
                        pcssDebug );

                    //strTextElement = "";
                    //strTextContent = "";
                    TextElementIndex_iStart             = -1;
                    TextElementIndex_iStop              = -1;
                    TextContentIndex_iStart             = -1;
                    TextContentIndex_iStop              = -1;


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

                //strTextElement.charConcat ( strText.charGet(iIndex) );
                if (TextElementIndex_iStart==-1) TextElementIndex_iStart = iIndex;
                TextElementIndex_iStop = iIndex;
                
            }

            if (bConcatContent)
            {
                if (iStateContentLine!=0) iStateContentLine = iLineCount;

                //strTextContent.charConcat ( strText.charGet(iIndex) );
                if (TextContentIndex_iStart==-1) TextContentIndex_iStart = iIndex;
                TextContentIndex_iStop = iIndex;
            }

        }

    }

    return 0;
}
