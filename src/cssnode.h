#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdint.h>

#include "str.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4028)
#pragma warning(disable : 4996)
#endif

#ifndef zM_CSSNODE_HEADER
#define zM_CSSNODE_HEADER


class cssNODE {
    protected:
    private:

        cssNODE *   parentNode;
        cssNODE **  childNode;
        uint32_t    iChildNodeCount;
        int         iLevel;



    public:
        str strText;

        cssNODE()
        {
            this->parentNode      = NULL;
            this->childNode       = NULL;
            this->iChildNodeCount = 0;
            this->iLevel          = 0;
            strNew ( &(this->strText) );
        }


        cssNODE ( cssNODE * parentNode )
        {
            this->parentNode      = parentNode;
            this->childNode       = NULL;
            this->iChildNodeCount = 0;
            this->iLevel          = 0;
            strNew ( &(this->strText) );
        }


        ~cssNODE()
        {
            int iIndex;
            for (iIndex=0;iIndex<(int)(this->iChildNodeCount);iIndex++)
            {
                delete this->childNode[iIndex];
            }
            free ( this->childNode );

            this->childNode = NULL;
            this->iChildNodeCount = 0;
            strDelete ( &(this->strText) ) ;
        }


        void printTree ( )
        {
            char szTmp[256];
            int iIndex;

            memset ( szTmp, '.', 256 );
            //this->str.exportBytes ( szTmp+this->iLevel, 256, STRING_ENCODING_ISO8859_15, true );
            strExport ( &(this->strText), szTmp+this->iLevel, 256, true );

            //printf ( "> %s(%d)\n", szTmp, this->str.length );
            printf ( "> %s\n", szTmp );

            for (iIndex=0;iIndex<(int)(this->iChildNodeCount);iIndex++)
            {
                this->childNode[iIndex]->printTree ( );
            }

        
        }


        //Add a children
        cssNODE * addChild ( )
        {
            iChildNodeCount += 1;

            childNode = (cssNODE **)realloc ( childNode, sizeof(cssNODE *) * iChildNodeCount );
            childNode[iChildNodeCount-1] = new cssNODE ( this );

            childNode[iChildNodeCount-1]->iLevel = this->iLevel + 1;

            return childNode[iChildNodeCount-1];
        }


        //Get the parent
        cssNODE * getParent ( )
        {
            return parentNode;
        }


        int getChildCount ( )
        {
            return this->iChildNodeCount;
        }

        //Get the selected child
        cssNODE * getChildIndex ( uint32_t iIndex )
        {
            return this->childNode[iIndex];
        }


        //Copy the node and recursive children nodes
        cssNODE * copy ( cssNODE * parentNode )
        {
            int iIndex;
            cssNODE * currentNode = new cssNODE ( );
            currentNode->iChildNodeCount    = this->iChildNodeCount;
            currentNode->childNode          = NULL;
            currentNode->parentNode         = parentNode;
            currentNode->iLevel             = this->iLevel;
            strCopy ( &(currentNode->strText), &(this->strText) );

            if (currentNode->iChildNodeCount)
            {
                currentNode->childNode          = (cssNODE **)malloc ( sizeof(cssNODE *) * currentNode->iChildNodeCount );

                for (iIndex=0;iIndex<(int)(this->iChildNodeCount);iIndex++)
                {
                    currentNode->childNode[iIndex] = this->childNode[iIndex]->copy ( currentNode );
                }
            }

            return currentNode;
        }


        cssNODE * copy ( )
        {
            return this->copy ( this );
        }


        void cleanup ( )
        {
            int iIndex;
            int iCount;

            for (iIndex=0;iIndex<(int)(this->iChildNodeCount);iIndex++)
            {
                //Does child have null string and no sub-child
                if ( (strLength(&(this->childNode[iIndex]->strText))==0) &&
                     (this->childNode[iIndex]->iChildNodeCount==0) )
                {
                    delete this->childNode[iIndex];

                    iCount = this->iChildNodeCount - iIndex - 1;

                    if (iCount)
                        memmove (
                            this->childNode+iIndex,
                            this->childNode+iIndex+1,
                            iCount * sizeof(cssNODE *) );
                    
                    this->iChildNodeCount -= 1;
                    iIndex-=1;
                }
                else if (this->childNode[iIndex]->iChildNodeCount)
                    this->childNode[iIndex]->cleanup();
            }
        }
};



#endif