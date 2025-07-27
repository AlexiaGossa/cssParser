#include "cssparser.h"


/*zRETURN cssParseElementStyle ( 
    eSTRING & strElementText,
    eSTRING & strContentStyleText,
    cssOBJECT * pcssObject,
    cssDEBUG * pcssDebug )*/

zRETURN cssParseElementStyle ( 
    char *          pszText,
    int             iTextLength,
    int             TextElementIndex_iStart,        //Element text start
    int             TextElementIndex_iStop,         //Element text stop
    int             TextContentIndex_iStart,        //Element style text start
    int             TextContentIndex_iStop,         //Element style text stop
    cssOBJECT *     pcssObject,         //Objects data
    cssDEBUG *      pcssDebug )
{
    int iIndexPrev;
    int iIndexNext;
    eSTRING strTemp ( strElementText );
    char szTmp[64];
    cssNODE * baseNode;

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
        pszText,
        iTextLength,
        TextContentIndex_iStart,        //Element style text start
        TextContentIndex_iStop,         //Element style text stop
        pcssDebug );


    /*
        Get all elements !
    */

    //strElementText.print();

    //static int iCount=0;
    //iCount++;

    iIndexPrev = 0;
    do {

        //iIndexNext = strElementText.indexOf ( ",", iIndexPrev );
        iIndexNext = szIndexOf ( 
            pszText + TextElementIndex_iStart,
            TextElementIndex_iStop - TextElementIndex_iStart + 1,
            ",", 
            iIndexPrev );

        if (iIndexNext!=-1)
        {
            strTemp = strElementText.substring ( iIndexPrev, iIndexNext-1 );
            iIndexPrev = iIndexNext + 1;
        }
        else
        {
            strTemp = strElementText.substring ( iIndexPrev );
        }

        //Clean
        strTemp = strTemp.trim();

        //Element name
        strTemp.exportBytes ( szTmp, 64, STRING_ENCODING_ISO8859_15, true );


        if (strTemp.length)
        {
            cssNODE * newNode;

            newNode = baseNode->copy();
            newNode->str = strTemp;

            newNode->printTree();

            /*
            
            Convert Node into a list
            
            */

            //Allocate a new selector
            cssSELECTOR * pSelector;
            pcssObject->pSelectorArray = (cssSELECTOR *)realloc ( 
                pcssObject->pSelectorArray,
                sizeof(cssSELECTOR) * (pcssObject->iSelectorCount+1) );

            pSelector = pcssObject->pSelectorArray + pcssObject->iSelectorCount;
            pcssObject->iSelectorCount += 1;

            //The new selector name
            strcpy ( pSelector->sz, szTmp );

            cssParseSelector ( pSelector, newNode );



            delete newNode;
        }

        //printf ( "Element : %s\n", szTmp );

    } while (iIndexNext!=-1);

    delete baseNode;

    return 0;
}





/*

selector
    property: 
        value;

selector
    property: 
        value0 value1 value2...;

selector
    property: 
        value
            data;

selector
    property: 
        value
            data0 data1 ...;


    iValueCount
    pValueArray


*/
/*
typedef struct {
    char *              psz;
    int                 iEntry;
    
} cssVALUE;

typedef struct {
    int                 iCodeName;

    cssVALUE *          pValueArray;
    int                 iValueCount;

} cssPROPERTY;

typedef struct {

    char *              pszName;

    cssPROPERTY *       pPropertyArray;
    int                 iPropertyCount;


} cssSELECTOR;
*/


cssNODE * cssParseContentStyle ( eSTRING & strContent, cssDEBUG * pcssDebug )
{
/*


*/
    int iIndex;
    bool bEnableString;
    bool bEnableDescription;
    cssNODE * rootNode;
    cssNODE * currentNode;
    eSTRING strTank ( strContent );

    bEnableDescription = false;
    bEnableString = 0;

    //create the root node
    currentNode = new cssNODE ( );
    rootNode = currentNode;

    currentNode = rootNode->addChild ( );
    

    bool bTankConcat;
    bTankConcat = false;

    for (iIndex=0;iIndex<strContent.length;iIndex++)
    {
        bTankConcat = false;

        if (!bEnableString)
        {
            switch (strContent.charGet(iIndex))
            {
                //Description
                case ':':
                    if (bEnableDescription==false)
                    {
                        bEnableDescription  = true;
                        currentNode->str    = strTank;
                        strTank             = "";

                        //Create a new node child
                        currentNode         = currentNode->addChild ( );
                    }
                    break;

                //Next style
                case ';':
                    
                    bEnableDescription = false;

                    if (strTank.length)
                    {
                        currentNode->str = strTank;
                        strTank = "";
                    }

                    //Back to parent node
                    currentNode = rootNode->addChild ( );
                    
                    break;

                case ' ':

                    if (!currentNode->getParent()->getChildCount())
                        break;

                case ',':
                

                    if (strTank.length)
                    {
                        currentNode->str = strTank;
                        strTank = "";
                    }

                    //Add a side child
                    currentNode = currentNode->getParent()->addChild ( );

                    break;                    
                    

                //New node
                case '(':

                    if (strTank.length)
                    {
                        currentNode->str = strTank;
                        strTank = "";
                    }

                    //Add a new child node                    
                    currentNode = currentNode->addChild( );

                    break;

                case ')':

                    if (strTank.length)
                    {
                        currentNode->str = strTank;
                        strTank = "";
                    }

                    //Back to parent node
                    currentNode = currentNode->getParent();

                    break;

                default:
                    bTankConcat = true;
                    break;
            }
        }
        else
        {
            bTankConcat = true;
        }

        /*
            String management
        */
        if ( (strContent.charGet(iIndex) == 34) || 
             (strContent.charGet(iIndex) == 39) )
        {
            if (!bEnableString)
            {
                //strTemp = strTank;
                strTank = "";
            }
            else
            {
                //strText = strTank;
                //strTank = "";
                if (strTank.length)
                {
                    currentNode->str = strTank;
                    strTank = "";
                }


            }
            bEnableString   = (bool)(~bEnableString);
            bTankConcat     = false;
        }

        /*
            The tank
        */
        if (bTankConcat)
            strTank.charConcat ( strContent.charGet(iIndex) );

    }

    //rootNode->printTree();
    rootNode->cleanup();
    return rootNode;
}

/*
List of css-property

List of css-values

    number+units
    !important
    auto
    #rgb        => rgb(0xRR,0xGG,0xBB)
    #RrGgBb     => rgb(0xRr,0xGg,0xBb)

    rgb
    rgba
    hsl
    hsla


    none
    inline
    inline-block
    block

    static
    absolute
    relative
    fixed

    inherit
    initial
    default

    linear-gradient

transform
    translate3d



        

List of css-units

    vmin
    vmax
    vw
    vh
    %
    px

List of css-numbers

    int32
    float32 (when dot is detected)


*/

