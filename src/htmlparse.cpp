#include "htmlparse.h"
#include "cssparse.h"

htmlDefTAG htmlDefTag[] = 
{
    { 
        "body",
        false,
        "display:       block;"
        "margin-left:   8px;"
        "margin-right:  8px;"
        "margin-top:    8px;"
        "margin-bottom: 8px;"
    },
    {
        "div",
        false,
        "display:       block;"
    },
    { 
        "span",
        false,
        "",
    },
    {
        "p",
        false,
        "display: block;"
        "margin-left: 0;"
        "margin-right: 0;"
        "margin-top: 12px;"
        "margin-bottom: 12px;"
    },
    {
        "input",
        true,
        "",
    },
    {
        "meta",
        true,
        "",
    },
    {
        "link",
        true,
        "",
    },
    {
        "a",
        false,
        "",
    },
    {
        "img",
        true,
        "",
    },
    {
        "label",
        false,
        "",
    },
    {
        "!doctype",
        true,
        ""
    },
    {
        ""
    }

};

/*
 *  Return length of the htmlDefTag structure as fast as possible
 */
int htmlDefTagLength ( void )
{
    int iIndex;
    static int iLength = -1;

    if (iLength!=-1)
        return iLength;

    iIndex = 0;
    do {
        if (htmlDefTag[iIndex].pszTagName[0]==0)
        {
            iLength = iIndex;
            return iLength;
        }
        iIndex++;
    } while (1);
}


zRETURN htmlGetTag ( str * pstrTag, htmlDefTAG ** p )
{
    int iIndex, iCount;
        
    iCount = htmlDefTagLength();
    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        if (strCompare(pstrTag,htmlDefTag[iIndex].pszTagName)==0)
        {
            *p = htmlDefTag+iIndex;
            return 0;
        }
    }

    return -1;
}


#define htmlTAGLEVEL_CONTENTCOUNT   (32)
typedef struct {
    str     strTagName;
} htmlTAGLEVEL;



void htmlTagLevelAlloc ( htmlTAGLEVEL ** pTagLevel, int iCurrentCount, int iNewCount )
{
    int iIndex;

    //Clear removed data
    if (iNewCount<iCurrentCount)
    {
        for (iIndex=iNewCount;iIndex<iCurrentCount;iIndex++)
        {
            strDelete ( &((*pTagLevel)[iIndex].strTagName) );
        }

    }

    if (iNewCount==0)
    {
        free ( *pTagLevel );
        *pTagLevel = NULL;
    }
    else
    {
        //New alloc
        *pTagLevel = (htmlTAGLEVEL *)realloc ( *pTagLevel, iNewCount * sizeof(htmlTAGLEVEL) );

        //Prepare the new data
        if (iNewCount>iCurrentCount)
        {
            for (iIndex=iCurrentCount;iIndex<iNewCount;iIndex++)
            {
                strNew ( &((*pTagLevel)[iIndex].strTagName) );
            }
        }
    }
}

void htmlTagLevelFree ( htmlTAGLEVEL ** pTagLevel, int iCount )
{
    int iIndex;
    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        strDelete ( &((*pTagLevel)[iIndex].strTagName) );
    }
    free ( *pTagLevel );
    *pTagLevel = NULL;
}




static char htmlParse_szErrorMessage[1024];
void htmlParseSetError ( char * pszErrorMessage )
{
    strcpy ( htmlParse_szErrorMessage, pszErrorMessage );
}


/*
 *  Parse html from a string
 *
 */
zRETURN htmlParseStr (
    str *           pstr,
    htmlDOCUMENT *  pDocument )
{
    int         iIndex, iCount;
    uint32_t    uiCurrentChar;
    uint32_t    uiPreviousChar;


    int         iTagParamIndex;

    
    bool        bEnableComment;


    str         strContent;
    str         strTagName;
    str         strTagContent[htmlTAGLEVEL_CONTENTCOUNT];

    int         iCurrentParentHandle;
    int         iCurrentHandle;
    int         iReturn;
    char        szTmp[1024], szTmp2[1024];


    htmlDocumentInit ( pDocument );
    iCurrentParentHandle = htmlDocumentGetRoot ( pDocument );


    strNew ( &strContent );
    strNew ( &strTagName );
    for (iIndex=0;iIndex<htmlTAGLEVEL_CONTENTCOUNT;iIndex++)
    {
        strNew ( &strTagContent[iIndex] );
    }

    

    htmlDefTAG * pDefTag;
    bool bDetectedBracket;
    bool bDetectedSingleQuote;
    bool bDetectedDoubleQuote;
    bool bEnableOutputTagChar;
    bool bDetectedSlashFirst;
    bool bDetectedSlashLast;

    htmlTAGLEVEL * pTagLevel;
    int iTagDepthLevel; 
    int iTagDepthAllocCount;


    int iDebugLineIndex;
    int iDebugColumnIndex;


    

    //The depth level...
    iTagDepthLevel      = 0;
    iTagDepthAllocCount = 32;
    pTagLevel           = NULL;
    htmlTagLevelAlloc ( &pTagLevel, 0, iTagDepthAllocCount );

    //Prepare to start
    bDetectedBracket    = false;
    bEnableComment      = false;
    iCount              = strLength ( pstr );
    iDebugLineIndex     = 1;
    iDebugColumnIndex   = 1;
    uiPreviousChar      = 0;
    uiCurrentChar       = 0;
    

    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        uiPreviousChar = uiCurrentChar;
        uiCurrentChar = strCharGet(pstr,iIndex);

        //Line counter
        if (uiCurrentChar==13)
        {
            iDebugLineIndex += 1;
            iDebugColumnIndex = 1;
        }
        else
        {
            iDebugColumnIndex += 1;
        }

        //Transform all invalid char into space
        switch (uiCurrentChar)
        {
            case 8:     //Backspace
            case 9:     //Tab
            case 10:    //LF
            case 13:    //CR
            case 27:    //Escape
                uiCurrentChar = 32;
        }

        //Skip all multiple spaces
        if ( (uiCurrentChar==32) && (uiPreviousChar==32) )
            continue;

        if (bEnableComment)
        {
            /*
             *  Comment mode, detect end of comment
             */
            if (uiCurrentChar=='>')
            {
                if ( (strCharGet(pstr,iIndex-1)=='-') && (strCharGet(pstr,iIndex-2)=='-') )
                    bEnableComment = false;
            }
        
        }
        else
        {
/*

Gestion des tag ouvrant et fermant :
- On détecte un bracket ouvant et fermant.
- Si on découvre un slash juste avant le bracket fermant, il n'y aura pas d'autre tag
- Si le tag name est listé comme "single-tag" ou "tag unique", il n'y aura pas de tag fermant
- Si on découvre un slash juste après le bracket ouvrant, c'est le tag fermant


*/


            switch (uiCurrentChar)
            {
                case 8:     //Backspace
                case 9:     //Tab
                case 10:    //LF
                case 13:    //CR
                case 27:    //Escape
                    break;

                case '<':   //Open bracket (could not been escaped)
                    bDetectedBracket        = true;
                    iTagParamIndex          = 0;
                    bDetectedSingleQuote    = false;
                    bDetectedDoubleQuote    = false;
                    bDetectedSlashFirst     = false;
                    bDetectedSlashLast      = false;
                    strZero ( &strTagName );

                    //Add the content into a "text" tag
                    if (strLength(&strContent))
                    {
                        strTrim ( &strContent );
                        if (strLength(&strContent))
                        {
                            iCurrentHandle = htmlElementCreate ( pDocument );
                            htmlElementSetTagName ( pDocument, iCurrentHandle, "text" );
                            htmlElementSetContent ( pDocument, iCurrentHandle, &strContent );
                            htmlElementAppendChild ( pDocument, iCurrentParentHandle, iCurrentHandle );
                        }
                    }
                    strZero ( &strContent );
                    break;

                case '>':   //Close bracket (could not been escaped)
                    bDetectedBracket        = false;

                    if (bDetectedSlashFirst)
                    {
                        /*  ############################################################################################
                         *
                         *  We're exiting an 'ending tag'
                         *
                         *  ############################################################################################
                         *
                         *
                         *  
                         */

                        //Go to surface...
                        iTagDepthLevel -= 1;

                        if (iTagDepthLevel>=0)
                        {
                            //Check the tag name...
                            if (strCompare(&strTagName,&(pTagLevel[iTagDepthLevel].strTagName))==0)
                            {
                                //Ok, same tag to close...
                                //...

                                //Store text content
                               //strCopy ( &(pTagLevel[iTagDepthLevel].strTextContent), &strContent );

                                //Clear text content
                                //strZero ( &strContent );

                                //Go the parent element
                                iCurrentParentHandle = htmlElementGetParent ( pDocument, iCurrentParentHandle );
                            }
                            else
                            {
                                //Error
                                strExport ( &strTagName, szTmp2, 256, true );
                                sprintf ( szTmp, "Could not find the similar ending tag [%s] at position %d,%d", szTmp2, iDebugLineIndex, iDebugColumnIndex );
                                htmlParseSetError ( szTmp );
                                goto error_return;
                            }
                        }
                        else
                        {
                            //Error
                            sprintf ( szTmp, "Could not go upper than root tag at position %d,%d", iDebugLineIndex, iDebugColumnIndex );
                            htmlParseSetError ( szTmp );
                            goto error_return;
                        }


                    }
                    else if ( (bDetectedSlashLast) || ((!htmlGetTag(&strTagName,&pDefTag))&&(pDefTag->bNoCloseTag)) )
                    {
                        /*  ############################################################################################
                         *
                         *  We're exiting a detected XHTML style 'start tag only' or a simple 'start tag only'
                         *
                         *  ############################################################################################
                         *
                         *  Create the new DOM element
                         *  
                         */
                        {
                            int iTagContentCount;
                            int iTagContentIndex;

                            iCurrentHandle = htmlElementCreate ( pDocument );
                            htmlElementSetTagName ( pDocument, iCurrentHandle, &strTagName );
                            htmlElementAppendChild ( pDocument, iCurrentParentHandle, iCurrentHandle );

                            iTagContentCount = iTagParamIndex;
                            for (iTagContentIndex=0;iTagContentIndex<iTagContentCount;iTagContentIndex++)
                            {
                                htmlElementSetAttribute (
                                    pDocument, 
                                    iCurrentHandle,
                                    strTagContent + iTagContentIndex );
                            }
                        }
                    }
                    else
                    {
                        // Add the text content
                        //...

                        /*  ############################################################################################
                         *
                         *  We're exiting a 'start tag'
                         *
                         *  ############################################################################################
                         *
                         *
                         *  
                         */
                        {
                            int iTagContentCount;
                            int iTagContentIndex;

                            iCurrentHandle = htmlElementCreate ( pDocument );
                            htmlElementSetTagName ( pDocument, iCurrentHandle, &strTagName );
                            htmlElementAppendChild ( pDocument, iCurrentParentHandle, iCurrentHandle );

                            iTagContentCount = iTagParamIndex;
                            for (iTagContentIndex=0;iTagContentIndex<iTagContentCount;iTagContentIndex++)
                            {
                                htmlElementSetAttribute (
                                    pDocument, 
                                    iCurrentHandle,
                                    strTagContent + iTagContentIndex );
                            }

                            iCurrentParentHandle = iCurrentHandle;
                        }


                        //Store tag name at current depth
                        strCopy ( &(pTagLevel[iTagDepthLevel].strTagName), &strTagName );

                        //Go deeper
                        iTagDepthLevel += 1;

                        //Alloc a deeper tag...
                        if (iTagDepthLevel>iTagDepthAllocCount)
                        {
                            htmlTagLevelAlloc ( &pTagLevel, iTagDepthAllocCount, iTagDepthAllocCount+4 );
                            iTagDepthAllocCount += 4;
                        }

                    }

                    /*
                    - Detect if the tag is stand-alone element or has an only start tag
                    - Detect if the tag has a slash first or a slash last

                    */
                    break;

                default:    //Send character to content string

                    if (!bDetectedBracket)
                    {
                        // Content storage
                        strCharConcat ( &strContent, uiCurrentChar );
                    }
                    else
                    {
                        bEnableOutputTagChar = true;

                        // Detect for quotes behavior...
                        switch (uiCurrentChar)
                        {
                            case 39: //single quote
                                bDetectedSingleQuote ^= true;
                                break;

                            case 34: //double quote
                                bDetectedDoubleQuote ^= true;
                                break;
                        }

                        //Only if quotes are not active
                        if ( (!bDetectedSingleQuote) && (!bDetectedDoubleQuote) )
                        {
                            switch (uiCurrentChar)
                            {
                                case 32:    //space (separator)
                                case 8:     //Backspace
                                case 9:     //Tab
                                case 10:    //LF
                                case 13:    //CR
                                case 27:    //Escape
                                    if ( (iTagParamIndex==0) || ((iTagParamIndex>0)&&(strLength(strTagContent+(iTagParamIndex-1)))) )
                                    {
                                        iTagParamIndex          += 1;
                                        if (iTagParamIndex>=htmlTAGLEVEL_CONTENTCOUNT)
                                        {
                                            //Error
                                            sprintf ( szTmp, "Could not add more than %d tag content at position %d,%d", htmlTAGLEVEL_CONTENTCOUNT, iDebugLineIndex, iDebugColumnIndex );
                                            htmlParseSetError ( szTmp );
                                            goto error_return;
                                        }
                                        strZero ( strTagContent+(iTagParamIndex-1) );
                                    }
                                    bEnableOutputTagChar    = false;
                                    break;

                                case '/': //slash

                                    //Detect '</'
                                    if (strCharGet(pstr,iIndex-1)=='<')
                                    {
                                        bDetectedSlashFirst     = true;
                                        bEnableOutputTagChar    = false;
                                    }

                                    //Detect '/>
                                    if (strCharGet(pstr,iIndex+1)=='>')
                                    {
                                        bDetectedSlashLast      = true;
                                        bEnableOutputTagChar    = false;
                                    }
                                    break;

                                case '!': //exclamation
                                    break;

                                case '-': //minus
                                    break;

                                default:
                                    break;
                            }
                        }

                        if (bEnableOutputTagChar)
                        {
                            if (!iTagParamIndex)
                            {
                                //The first tag
                                strCharConcat ( &strTagName, uiCurrentChar );

                                //Comment start detection !
                                if (strLength(&strTagName)==3)
                                {
                                    if (strIndexOf(&strTagName,"!--")==0)
                                    {
                                        //We have detected a comment start !
                                        bDetectedBracket = false;
                                        bEnableComment = true;
                                        continue;
                                    }
                                }
                            }
                            else
                            {
                                //Next tag
                                strCharConcat ( strTagContent+(iTagParamIndex-1), uiCurrentChar );
                            }
                        }

                    }
                    break;
            }
        }
    }

    iReturn = 0;
    goto return_function;

error_return:
    htmlDocumentFree ( pDocument );
    iReturn = -1;


return_function:

    htmlTagLevelFree ( &pTagLevel, iTagDepthAllocCount );
    
    strDelete ( &strContent );
    strDelete ( &strTagName );
    for (iIndex=0;iIndex<htmlTAGLEVEL_CONTENTCOUNT;iIndex++)
    {
        strDelete ( &strTagContent[iIndex] );
    }

    return iReturn;
}


/*

    union {
        int32_t     iValue;
        uint32_t    uiValue;
    };
    float       fValue;
    char *      pszValue;
    bool        bInteger;
    bool        bFloat;
    bool        bString;

    int         iValueType;

} cssVALUE;
/*
enum cssVALUE_TYPE {
    cssPX,
    cssPERCENT,
    cssVMIN,
    cssVMAX,
    cssVW,
    cssVH,
    cssRGB,
    cssRGBA,
    cssAUTO,

    cssINHERIT,
    cssINITIAL,
    cssDEFAULT,
    cssUNSET,

    cssDISPLAY_NONE,
    cssDISPLAY_INLINE,
    cssDISPLAY_BLOCK,

*/