#include "domcontext.h"




    zRETURN domContextInit ( domCONTEXT * pContext )
    {
        memset ( pContext, 0, sizeof(domCONTEXT) );
        cssParseInit ( &(pContext->cssObject) );

        return 0;
    }

    zRETURN domContextFree ( domCONTEXT * pContext )
    {
        cssParseFree ( &(pContext->cssObject) );
        htmlDocumentFree ( &(pContext->htmlDocument) );

        memset ( pContext, 0, sizeof(domCONTEXT) );

        return 0;    
    }

    zRETURN domContextLoad ( domCONTEXT * pContext, char * pszHtmlFileName )
    {
        htmlELEMENT *       pElement;
        str                 strFileContent;
        int *               piHandleArray;
        int                 iHandleCount;
        int                 iIndex;
        char *              strzLinkHRef;
        str                 strContent;
        

        strNewFileGetContents ( &strFileContent, pszHtmlFileName );
        htmlParseStr ( &strFileContent, &(pContext->htmlDocument) );
        strDelete ( &strFileContent );

        /*
        Now try to load all css files
        */

        piHandleArray = htmlDocumentGetElementsByTagName ( &(pContext->htmlDocument), "link", &iHandleCount );       //Return all elements by tagname
        for (iIndex=0;iIndex<iHandleCount;iIndex++)
        {
            strzLinkHRef = htmlElementGetAttributeStrz ( 
                &(pContext->htmlDocument), 
                piHandleArray[iIndex],
                "href" );

            if (strzLinkHRef)
            {
                cssParseFile ( strzLinkHRef, &(pContext->cssObject) );
                strzFree ( strzLinkHRef );
            }
        }
        free ( piHandleArray );

        /*
        Process all inline style
        */
        strNew ( &strContent );
        for (iIndex=0;iIndex<pContext->htmlDocument.iElementCount;iIndex++)
        {
            pElement = pContext->htmlDocument.pElement + iIndex;
            if (pElement->pstrzStyle && strlen(pElement->pstrzStyle))
            {
                //printf ( "online style = %s\n", pElement->pstrzStyle );
                strCopy ( &strContent, pElement->pstrzStyle );

                cssParseElementInline (
                    pElement->iHandle,
                    &strContent,
                    &(pContext->cssObject) );

                /*
                cssParseElementContent (
                        &strElement,
                        &strContent,
                        pcssObject );
                */
            }
        }
        strDelete ( &strContent );
        

        return 0;    
    }