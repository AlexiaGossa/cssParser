#include "string.h"

eSTRING & eSTRING::substring      ( size_t indexStart )
{
    eSTRING * strNew = new eSTRING(*this);
    strNew->iTempAlloc = 1;

    strNew->iCharactersWordSize = this->iCharactersWordSize;
    strNew->iCharLocaleEncoding = this->iCharLocaleEncoding;

    if ((int)indexStart<this->iCharactersCount)
    {
        strNew->Buffer_Replace (
            ((uint8_t *)this->pCharactersBuffer) + ((int)indexStart * this->iCharactersWordSize),
            this->iCharactersCount - (int)indexStart );
    }

    return * strNew;
}

eSTRING & eSTRING::substring      ( size_t indexStart, size_t indexEnd )
{
    eSTRING * strNew = new eSTRING(*this);
    int iIndexStart;
    int iIndexEnd;
    int iLength;

    strNew->iTempAlloc = 1;

    iIndexStart = (int)indexStart;
    iIndexEnd   = (int)indexEnd;

    if (iIndexStart>iIndexEnd)
    {
        iIndexStart = (int)indexEnd;
        iIndexEnd   = (int)indexStart;
    }

    if (iIndexStart>this->iCharactersCount)
        iIndexStart = this->iCharactersCount;

    if (iIndexEnd>this->iCharactersCount)
        iIndexEnd = this->iCharactersCount;

    iLength = iIndexEnd - iIndexStart + 1;

    if ( (iLength>0) && (iIndexStart<this->iCharactersCount) )
    {
        strNew->Buffer_Replace (
            ((uint8_t *)this->pCharactersBuffer) + (iIndexStart * this->iCharactersWordSize),
            iLength );
    }

    return * strNew;
}


eSTRING & eSTRING::substr      ( size_t indexStart )
{
    return this->substring ( indexStart );
}


eSTRING & eSTRING::substr      ( size_t indexStart, size_t length )
{
    return this->substring ( indexStart, indexStart + length );
}


eSTRING & eSTRING::charAt         ( size_t indexAt )
{
    return this->substring ( indexAt, indexAt );
}

eSTRING & eSTRING::trim(void)
{
    int         iIndex;
    int         iIndexFirst;
    int         iIndexLast;
    uint32_t    uiChar;
    eSTRING *   strNew;

    iIndexFirst = -1;
    iIndexLast  = -1;

    for (iIndex = 0;iIndex < this->iCharactersCount;iIndex++)
    {
        uiChar = this->charGet(iIndex);
        if (uiChar>32)
        {
            if (iIndexFirst==-1)
                iIndexFirst = iIndex;

            iIndexLast = iIndex;
        }
    }


    if (iIndexFirst==-1)
    {
        strNew = new eSTRING(*this);
        strNew->iTempAlloc = 1;

        strNew->iCharactersCount = 0;
        strNew->length = 0;
        return * strNew;
    }
    else
    {
        return this->substring ( iIndexFirst, iIndexLast );
    }
}

eSTRING & eSTRING::trimStart(void)
{
    int iIndex;
    uint32_t uiChar;

    for (iIndex = 0;iIndex < this->iCharactersCount;iIndex++)
    {
        uiChar = this->charGet(iIndex);

        //Searching for the first occurence of normal character
        if (uiChar>32)
            return this->substr ( iIndex );
    }

    this->iCharactersCount = 0;
    this->length = 0;
    return *this;
}
eSTRING & eSTRING::trimLeft(void)
{
    return this->trimStart();

}

eSTRING & eSTRING::trimEnd(void)
{
    int iIndex, iLastIndex;
    uint32_t uiChar;
    
    iLastIndex = -1;
    for (iIndex = 0;iIndex < this->iCharactersCount;iIndex++)
    {
        uiChar = this->charGet(iIndex);

        //Searching for the latest occurence of normal character
        if (uiChar>32) iLastIndex = iIndex;
    }

    if (iLastIndex==-1)
    {
        this->iCharactersCount = 0;
        this->length = 0;
        return *this;
    }
    else
    {
        return this->substring ( 0, iLastIndex );
    }
}

eSTRING & eSTRING::trimRight(void)
{
    return this->trimEnd();
}
