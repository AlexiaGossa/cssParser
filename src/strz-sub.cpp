#include "strz.h"



strz strzSubstringCopy ( strz * pstrz, size_t indexStart )
{
    strz strzNew;
    strzClear ( &strzNew );
    strzNew.bAllocated = true;

    if ( (int)indexStart < pstrz->iUsedLength )
    {
        strzCopy ( 
            &strzNew,
            pstrz->pszSource + pstrz->iUsedOffset + ((int)indexStart),
            pstrz->iUsedLength - ((int)indexStart) );
    }

    return strzNew;
}

strz strzSubstringLink ( strz * pstrz, size_t indexStart )
{
    strz strzNew;
    strzClear ( &strzNew );
    strzNew.bAllocated = false;

    if ( (int)indexStart < pstrz->iUsedLength )
    {
        strzNew.pszSource       = pstrz->pszSource;
        strzNew.iSourceLength   = pstrz->iSourceLength;

        strzNew.iUsedOffset     = pstrz->iUsedOffset + ((int)indexStart);
        strzNew.iUsedLength     = pstrz->iUsedLength - ((int)indexStart);
    }

    return strzNew;
}

strz strzSubstringCopy ( strz * pstrz, size_t indexStart, size_t indexEnd )
{
    strz strzNew;
    int iIndexStart;
    int iIndexEnd;
    int iLength;

    strzClear ( &strzNew );
    strzNew.bAllocated = true;

    iIndexStart = (int)indexStart;
    iIndexEnd   = (int)indexEnd;

    if (iIndexStart>iIndexEnd)
    {
        iIndexStart = (int)indexEnd;
        iIndexEnd   = (int)indexStart;
    }

    if (iIndexStart>pstrz->iUsedLength)
        iIndexStart = pstrz->iUsedLength;

    if (iIndexEnd>pstrz->iUsedLength)
        iIndexEnd = pstrz->iUsedLength;

    iLength = iIndexEnd - iIndexStart + 1;

    if ( iLength>0 && iIndexStart<pstrz->iUsedLength )
    {
        strzCopy (
            &strzNew,
            pstrz->pszSource + pstrz->iUsedOffset + iIndexStart,
            iLength );
    }

    return strzNew;
}

strz strzSubstringLink ( strz * pstrz, size_t indexStart, size_t indexEnd )
{
    strz strzNew;
    int iIndexStart;
    int iIndexEnd;
    int iLength;

    strzClear ( &strzNew );
    strzNew.bAllocated = false;

    iIndexStart = (int)indexStart;
    iIndexEnd   = (int)indexEnd;

    if (iIndexStart>iIndexEnd)
    {
        iIndexStart = (int)indexEnd;
        iIndexEnd   = (int)indexStart;
    }

    if (iIndexStart>pstrz->iUsedLength)
        iIndexStart = pstrz->iUsedLength;

    if (iIndexEnd>pstrz->iUsedLength)
        iIndexEnd = pstrz->iUsedLength;

    iLength = iIndexEnd - iIndexStart + 1;

    if ( iLength>0 && iIndexStart<pstrz->iUsedLength )
    {
        strzNew.pszSource       = pstrz->pszSource;
        strzNew.iSourceLength   = pstrz->iSourceLength;

        strzNew.iUsedOffset     = pstrz->iUsedOffset + iIndexStart;
        strzNew.iUsedLength     = iLength;
    }

    return strzNew;
}


strz strzSubstrCopy ( strz * pstrz, size_t indexStart )
{
    return strzSubstringCopy ( pstrz, indexStart );
}

strz strzSubstrCopy ( strz * pstrz, size_t indexStart, size_t length )
{
    return strzSubstringCopy ( pstrz, indexStart, indexStart + length );
}

strz strzSubstrLink ( strz * pstrz, size_t indexStart )
{
    return strzSubstringCopy ( pstrz, indexStart );
}

strz strzSubstrLink ( strz * pstrz, size_t indexStart, size_t length )
{
    return strzSubstringCopy ( pstrz, indexStart, indexStart + length );
}


strz strzCharAtCopy ( strz * pstrz, size_t indexAt )
{
    return strzSubstringCopy ( pstrz, indexAt, indexAt );
}

strz strzCharAtLink ( strz * pstrz, size_t indexAt )
{
    return strzSubstringLink ( pstrz, indexAt, indexAt );
}

uint32_t    strzCharGet ( strz * pstrz, int iIndex )
{
    if ( (iIndex>=0) && (iIndex<pstrz->iUsedLength) )
        return (uint32_t)pstrz->pszSource[pstrz->iUsedOffset + iIndex];
    else
        return 0;
}

void        strzCharConcatCopy          ( strz * pstrz, uint32_t uiChar )
{
    int iLength;

    //L'allocation n'est pas toujours nécessaire
    if ((pstrz->iUsedOffset + pstrz->iUsedLength) < pstrz->iSourceLength)
        pstrz->iUsedLength += 1;
    else
    {
        pstrz->iSourceLength = (pstrz->iSourceLength&0xFFFFF80) + 256;
        pstrz->pszSource = (char *)realloc ( pstrz->pszSource, pstrz->iSourceLength );
        pstrz->iUsedLength += 1;
    }

    pstrz->pszSource[pstrz->iUsedOffset + pstrz->iUsedLength - 1] = uiChar;
}



/*
        void        charSet                 ( int iIndex, uint32_t cValue );
        uint32_t    charGet                 ( int iIndex );
        void        charConcat              ( uint32_t cValue );
        void        charStrcpy              ( uint32_t cValue );
*/





