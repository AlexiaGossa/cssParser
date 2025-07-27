#include "str.h"


int strSubstring ( str * pstr, str * pstrSource, size_t indexStart )
{
    if ( (int)indexStart < pstrSource->iCharacterCount )
    {
        if (strEncodingGetCharSize(pstrSource->iEncoding)==1)
            strCopy (
                pstr,
                pstrSource->psz + (int)indexStart,
                pstrSource->iCharacterCount - (int)indexStart );
        else
            strCopy (
                pstr,
                pstrSource->pdw + (int)indexStart,
                (pstrSource->iCharacterCount - (int)indexStart) * strEncodingGetCharSize(pstrSource->iEncoding) );
    }
    else
    {
        strZero ( pstr );
    }

    return 0;
}

int strSubstring ( str * pstr, str * pstrSource, size_t indexStart, size_t indexEnd )
{
    int iIndexStart;
    int iIndexEnd;
    int iLength;

    iIndexStart = (int)indexStart;
    iIndexEnd   = (int)indexEnd;

    if (iIndexStart>iIndexEnd)
    {
        iIndexStart = (int)indexEnd;
        iIndexEnd   = (int)indexStart;
    }

    if (iIndexStart>pstrSource->iCharacterCount)
        iIndexStart = pstrSource->iCharacterCount;

    if (iIndexEnd>pstrSource->iCharacterCount)
        iIndexEnd = pstrSource->iCharacterCount;

    iLength = iIndexEnd - iIndexStart + 1;

    if ( iLength>0 && iIndexStart<pstrSource->iCharacterCount )
    {
        if (strEncodingGetCharSize(pstrSource->iEncoding)==1)
            strCopy (
                pstr,
                pstrSource->psz + iIndexStart,
                iLength );
        else
            strCopy (
                pstr,
                pstrSource->pdw + iIndexStart,
                iLength * strEncodingGetCharSize(pstrSource->iEncoding) );
    }
    else
    {
        strZero ( pstr );
    }

    return iLength;
}



int strSubstr ( str * pstr, str * pstrSource, size_t indexStart )
{
    return strSubstring ( pstr, pstrSource, indexStart );
}

int strSubstr ( str * pstr, str * pstrSource, size_t indexStart, size_t length )
{
    return strSubstring ( pstr, pstrSource, indexStart, indexStart + length );
}



int strCharAt ( str * pstr, str * pstrSource, size_t indexAt )
{
    return strSubstring ( pstr, pstrSource, indexAt, indexAt );
}


uint32_t    strCharGet ( str * pstr, int iIndex )
{
    if ( (iIndex>=0) && (iIndex<pstr->iCharacterCount) )
    {
        if (strEncodingGetCharSize(pstr->iEncoding)==1)
            return (uint32_t)pstr->psz[iIndex];
        else
            return (uint32_t)pstr->pdw[iIndex];
    }
    else
        return 0;
}

void        strCharConcat          ( str * pstr, uint32_t uiChar )
{
    //L'allocation n'est pas toujours nécessaire
    pstr->iCharacterCount += 1;
    pstr->p = realloc ( pstr->p, pstr->iCharacterCount * strEncodingGetCharSize(pstr->iEncoding) );

    if (strEncodingGetCharSize(pstr->iEncoding)==1)
        pstr->psz[pstr->iCharacterCount - 1] = uiChar;
    else
        pstr->pdw[pstr->iCharacterCount - 1] = uiChar;
}


void strTrim ( str * pstr )
{
    strTrim ( pstr, pstr );
}

void strTrim ( str * pstr, str * pstrSource )
{
    int         iIndex;
    int         iIndexFirst;
    int         iIndexLast;
    uint32_t    uiChar;

    iIndexFirst = -1;
    iIndexLast  = -1;

    for (iIndex = 0;iIndex < pstrSource->iCharacterCount;iIndex++)
    {
        uiChar = strCharGet ( pstrSource, iIndex );
        if (uiChar>32)
        {
            if (iIndexFirst==-1)
                iIndexFirst = iIndex;

            iIndexLast = iIndex;
        }
    }


    if (iIndexFirst==-1)
    {
        strZero ( pstr );
    }
    else
    {
        strSubstring ( pstr, pstrSource, iIndexFirst, iIndexLast );
    }
}


void strTrimStart ( str * pstr, str * pstrSource )
{
    int iIndex;
    uint32_t uiChar;

    for (iIndex = 0;iIndex < pstrSource->iCharacterCount;iIndex++)
    {
        uiChar = strCharGet ( pstrSource, iIndex );

        //Searching for the first occurence of normal character
        if (uiChar>32)
        {
            strSubstr ( pstr, pstrSource, iIndex );
            return;
        }
    }
    strZero ( pstr );
}

void strTrimStart ( str * pstr )
{
    strTrimStart ( pstr, pstr );
}


void strTrimEnd ( str * pstr, str * pstrSource )
{
    int iIndex, iLastIndex;
    uint32_t uiChar;
    
    iLastIndex = -1;
    for (iIndex = 0;iIndex < pstrSource->iCharacterCount;iIndex++)
    {
        uiChar = strCharGet ( pstrSource, iIndex );

        //Searching for the latest occurence of normal character
        if (uiChar>32) iLastIndex = iIndex;
    }

    if (iLastIndex==-1)
    {
        strZero ( pstr );
    }
    else
    {
        strSubstring ( pstr, pstrSource, 0, iLastIndex );
    }
}

void strTrimEnd ( str * pstr )
{
    strTrimEnd ( pstr, pstr );
}