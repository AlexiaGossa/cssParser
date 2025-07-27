#include "strz.h"

/*
 *
 *  Trim processing
 *
 */
void strzTrimProcess ( strz * pstrzTarget, strz * pstrzSource, bool bLinkOnly )
{
    int         iIndex;
    int         iIndexFirst;
    int         iIndexLast;
    uint32_t    uiChar;

    iIndexFirst = -1;
    iIndexLast  = -1;

    for (iIndex = 0;iIndex < pstrzSource->iUsedLength;iIndex++)
    {
        uiChar = pstrzSource->pszSource[pstrzSource->iUsedOffset + iIndex];
        if (uiChar>32)
        {
            if (iIndexFirst==-1)
                iIndexFirst = iIndex;

            iIndexLast = iIndex;
        }
    }

    if (iIndexFirst==-1)
    {
        if (pstrzTarget==pstrzSource)
        {
            pstrzTarget->iUsedLength = 0;
        }
        else
        {        
            if (bLinkOnly)
            {
                strzLink ( pstrzTarget, pstrzSource );
                pstrzTarget->iUsedLength = 0;
            }
            else
                strzClear ( pstrzTarget );
        }
        
    }
    else
    {
        if (pstrzTarget==pstrzSource)
        {
            strz strzTemp;
            strzTemp = strzSubstringLink ( pstrzSource, iIndexFirst, iIndexLast );
            pstrzTarget->iUsedOffset = strzTemp.iUsedOffset;
            pstrzTarget->iUsedLength = strzTemp.iUsedLength;
        }
        else
        {
            if (bLinkOnly)
                *pstrzTarget = strzSubstringLink ( pstrzSource, iIndexFirst, iIndexLast );
            else
                *pstrzTarget = strzSubstringCopy ( pstrzSource, iIndexFirst, iIndexLast );
        }
    }
}

strz strzTrimCopy ( strz * pstrz )
{
    strz strzTarget;
    strzTrimProcess ( &strzTarget, pstrz, false );
    return strzTarget;
}

strz strzTrimLink ( strz * pstrz )
{
    strz strzTarget;
    strzTrimProcess ( &strzTarget, pstrz, true );
    return strzTarget;
}

void strzTrim ( strz * pstrz )
{
    strzTrimProcess ( pstrz, pstrz, true );
}


/*
 *
 *  Left/Start trim processing
 *
 */
void strzTrimStartProcess ( strz * pstrzTarget, strz * pstrzSource, bool bLinkOnly )
{
    int iIndex;
    uint32_t uiChar;

    for (iIndex = 0;iIndex < pstrzSource->iUsedLength;iIndex++)
    {
        uiChar = pstrzSource->pszSource[pstrzSource->iUsedOffset + iIndex];

        //Searching for the first occurence of normal character
        if (uiChar>32)
        {
            if (bLinkOnly)
                *pstrzTarget = strzSubstrLink ( pstrzSource, iIndex );
            else
                *pstrzTarget = strzSubstrCopy ( pstrzSource, iIndex );

            return;
        }
    }

    if (bLinkOnly)
    {
        strzLink ( pstrzTarget, pstrzSource );
        pstrzTarget->iUsedLength = 0;
    }
    else
        strzClear ( pstrzTarget );
}

strz strzTrimStartCopy ( strz * pstrz )
{
    strz strzTarget;
    strzTrimStartProcess ( &strzTarget, pstrz, false );
    return strzTarget;
}

strz strzTrimStartLink ( strz * pstrz )
{
    strz strzTarget;
    strzTrimStartProcess ( &strzTarget, pstrz, true );
    return strzTarget;
}

void strzTrimStart ( strz * pstrz )
{
    strzTrimStartProcess ( pstrz, pstrz, true );
}




/*
 *
 *  Right/End trim processing
 *
 */
void strzTrimEndProcess ( strz * pstrzTarget, strz * pstrzSource, bool bLinkOnly )
{
    int iIndex, iLastIndex;
    uint32_t uiChar;

    iLastIndex = -1;
    for (iIndex = 0;iIndex < pstrzSource->iUsedLength;iIndex++)
    {
        uiChar = pstrzSource->pszSource[pstrzSource->iUsedOffset + iIndex];

        //Searching for the latest occurence of normal character
        if (uiChar>32) iLastIndex = iIndex;
    }

    if (iLastIndex==-1)
    {
        if (bLinkOnly)
        {
            strzLink ( pstrzTarget, pstrzSource );
            pstrzTarget->iUsedLength = 0;
        }
        else
            strzClear ( pstrzTarget );
    }
    else
    {

        if (bLinkOnly)
            *pstrzTarget = strzSubstringLink ( pstrzSource, 0, iLastIndex );
        else
            *pstrzTarget = strzSubstringCopy ( pstrzSource, 0, iLastIndex );
    }
}

strz strzTrimEndCopy ( strz * pstrz )
{
    strz strzTarget;
    strzTrimEndProcess ( &strzTarget, pstrz, false );
    return strzTarget;
}

strz strzTrimEndLink ( strz * pstrz )
{
    strz strzTarget;
    strzTrimEndProcess ( &strzTarget, pstrz, true );
    return strzTarget;
}

void strzTrimEnd ( strz * pstrz )
{
    strzTrimEndProcess ( pstrz, pstrz, true );
}
