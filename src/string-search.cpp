#include "string.h"


int stringMemmemBYTE ( void * pBuffer, void * pPattern, int lBufferLength, int lPatternLength, bool bEnableFirst )
{
    uint8_t * pSource;
    uint8_t * pFinder;
    int lFoundLength;
    int iFoundIndex;
    int iIndex;

    if ((!pPattern) ||
        (!pBuffer) ||
        (!lBufferLength) ||
        (lBufferLength<lPatternLength)) return -1;

    pSource = (uint8_t *)pBuffer;
    pFinder = (uint8_t *)pPattern;
    lFoundLength    = 0;
    iFoundIndex     = -1;
    iIndex          = 0;

    lBufferLength -= (lPatternLength - 1);

    do {

        if (*pSource == *pFinder)
        {
            if (!memcmp(pSource, pFinder, lPatternLength))
            {
                iFoundIndex = iIndex;            

                if (bEnableFirst)
                    return iIndex; //pSource;
            }
        }

        lBufferLength--;
        pSource++;
        iIndex++;
    } while (lBufferLength);

    return iFoundIndex;
}

int stringMemmemDWORD ( void * pBuffer, void * pPattern, int lBufferLength, int lPatternLength, bool bEnableFirst )
{
    uint32_t * pSource;
    uint32_t * pFinder;
    int lFoundLength;
    int iFoundIndex;
    int iIndex;

    if ((!pPattern) ||
        (!pBuffer) ||
        (!lBufferLength) ||
        (lBufferLength<lPatternLength)) return -1;

    pSource = (uint32_t *)pBuffer;
    pFinder = (uint32_t *)pPattern;
    lFoundLength    = 0;
    iFoundIndex     = -1;
    iIndex          = 0;

    lBufferLength -= (lPatternLength - 1);

    do {

        if (*pSource == *pFinder)
        {
            if (!memcmp(pSource, pFinder, lPatternLength<<2))
            {
                iFoundIndex = iIndex;            

                if (bEnableFirst)
                    return iIndex; //pSource;
            }
        }

        lBufferLength--;
        pSource++;
        iIndex++;
    } while (lBufferLength);

    return iFoundIndex;
}


size_t   eSTRING::_indexOfParams ( eSTRING & strValue, size_t indexStart, bool bEnableFirst )
{
    int iOffset;
    int iLength;
    int iReturn;


    iOffset = (int)indexStart;
    iLength = this->iCharactersCount - (int)indexStart;

    if ( (this->iCharactersWordSize==1) && (strValue.iCharactersWordSize==1) )
    {
        iReturn = stringMemmemBYTE (
            ((uint8_t *)this->pCharactersBuffer) + iOffset,
            strValue.pCharactersBuffer,
            iLength,
            strValue.iCharactersCount,
            bEnableFirst );
        if (iReturn!=-1) iReturn += iOffset;
        return iReturn;
    }

    if ( (this->iCharactersWordSize==4) && (strValue.iCharactersWordSize==4) )
    {
        iReturn = stringMemmemDWORD (
            ((uint32_t *)this->pCharactersBuffer) + iOffset,
            strValue.pCharactersBuffer,
            iLength,
            strValue.iCharactersCount,
            bEnableFirst );
        if (iReturn!=-1) iReturn += iOffset;
        return iReturn;
    }

    return -1;
}

size_t   eSTRING::_indexOfParams ( char * pszData, size_t indexStart, bool bEnableFirst )
{
    int iOffset;
    int iLength;
    int iReturn;

    iOffset = (int)indexStart;
    iLength = this->iCharactersCount - (int)indexStart;

    if (this->iCharactersWordSize==1)
    {
        iReturn = stringMemmemBYTE (
            ((uint8_t *)this->pCharactersBuffer) + iOffset,
            pszData,
            iLength,
            (int)strlen(pszData),
            bEnableFirst );
        if (iReturn!=-1) iReturn += iOffset;
        return iReturn;
    }
    else
    {
        size_t indexReturn;
        eSTRING * strNew = new eSTRING(*this);
        strNew->_strcpy ( pszData, this->iCharLocaleEncoding );
        indexReturn = this->_indexOfParams( *strNew, indexStart, bEnableFirst);
        delete strNew;
        return indexReturn;
    }
}

/*

    indexOf and indexLastOf with PSZ

*/
size_t   eSTRING::indexOf ( char * pszData, size_t indexStart )
{
    return this->_indexOfParams ( pszData, indexStart, true );
}

size_t   eSTRING::indexOf ( char * pszData)
{
    return this->_indexOfParams ( pszData, 0, true );
}

size_t   eSTRING::lastIndexOf ( char * pszData)
{
    return this->_indexOfParams ( pszData, 0, false );
}

size_t   eSTRING::lastIndexOf(char * pszData, size_t indexStart)
{
    return this->_indexOfParams(pszData, indexStart, false);
}


/*

indexOf and indexLastOf with STRING

*/
size_t   eSTRING::indexOf(eSTRING & strValue, size_t indexStart)
{
    return this->_indexOfParams(strValue, indexStart, true);
}

size_t   eSTRING::indexOf(eSTRING & strValue)
{
    return this->_indexOfParams(strValue, 0, true);
}

size_t   eSTRING::lastIndexOf(eSTRING & strValue)
{
    return this->_indexOfParams(strValue, 0, false);
}

size_t   eSTRING::lastIndexOf(eSTRING & strValue, size_t indexStart)
{
    return this->_indexOfParams(strValue, indexStart, false);
}




