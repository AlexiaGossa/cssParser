#include "cssparse.h"

cssNODE * cssParseContentStyle (
    str *           pstrContent )
{
    int         iIndex, iCount;
    bool        bEnableString;
    bool        bEnableDescription;
    cssNODE *   rootNode;
    cssNODE *   currentNode;
    str         strTank;
    uint32_t    uiCurrentchar;
    bool        bTankConcat;

    strNew ( &strTank );

    bEnableDescription  = false;
    bEnableString       = 0;
    bTankConcat         = false;
    iCount              = strLength(pstrContent);

    //create the root node
    currentNode         = new cssNODE ( );
    rootNode            = currentNode;
    currentNode         = rootNode->addChild ( );

    for (iIndex=0;iIndex<iCount;iIndex++)
    {
        bTankConcat     = false;
        uiCurrentchar   = strCharGet(pstrContent,iIndex);

        if (!bEnableString)
        {
            switch (uiCurrentchar)
            {
                //Description
                case 58: //':':
                    if (bEnableDescription==false)
                    {
                        bEnableDescription      = true;

                        //Transfert data and new allocation of strTank
                        currentNode->strText    = strTank;
                        strNew ( &strTank );

                        //Create a new node child
                        currentNode         = currentNode->addChild ( );
                    }
                    break;

                case 59: //';':
                //Next style
                    bEnableDescription = false;

                    if (strLength(&strTank))
                    {
                        //Transfert data and new allocation of strTank
                        currentNode->strText   = strTank;
                        strNew ( &strTank );
                    }

                    //Back to parent node
                    currentNode = rootNode->addChild ( );
                    
                    break;

                case 32: //' ':
                    if (!currentNode->getParent()->getChildCount())
                        break;

                case 44: //',':
                    if (strLength(&strTank))
                    {
                        //Transfert data and new allocation of strTank
                        currentNode->strText   = strTank;
                        strNew ( &strTank );
                    }

                    //Add a side child
                    currentNode = currentNode->getParent()->addChild ( );

                    break;                    
                    

                //New node
                case 40: //'(':
                    if (strLength(&strTank))
                    {
                        //Transfert data and new allocation of strTank
                        currentNode->strText   = strTank;
                        strNew ( &strTank );
                    }

                    //Add a new child node                    
                    currentNode = currentNode->addChild( );

                    break;

                case 41: //')':
                    if (strLength(&strTank))
                    {
                        //Transfert data and new allocation of strTank
                        currentNode->strText   = strTank;
                        strNew ( &strTank );
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
        if ( (uiCurrentchar == 34) || 
             (uiCurrentchar == 39) )
        {
            if (!bEnableString)
            {
                //strTemp = strTank;
                strZero ( &strTank );
            }
            else
            {
                //strText = strTank;
                //strTank = "";
                if (strLength(&strTank))
                {
                    //Transfert data and new allocation of strTank
                    currentNode->strText   = strTank;
                    strNew ( &strTank );
                }


            }
            bEnableString   = (bool)(~bEnableString);
            bTankConcat     = false;
        }

        /*
            The tank
        */
        if (bTankConcat)
            strCharConcat ( &strTank, strCharGet(pstrContent,iIndex) );
    }

    strDelete ( &strTank );

    //rootNode->printTree();
    rootNode->cleanup();
    return rootNode;
}