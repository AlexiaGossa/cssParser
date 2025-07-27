#include "str.h"


#define strmax(a,b) (((a) > (b)) ? (a) : (b))
#define strmin(a,b) (((a) < (b)) ? (a) : (b))



int strMemmemBYTE ( void * pBuffer, void * pPattern, int lBufferLength, int lPatternLength, bool bEnableFirst )
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

int strMemmemDWORD ( void * pBuffer, void * pPattern, int lBufferLength, int lPatternLength, bool bEnableFirst )
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


size_t   _strIndexOfParams ( str * pstr, str * pstrValue, size_t indexStart, bool bEnableFirst )
{
    int iOffset;
    int iLength;
    int iReturn;


    iOffset = (int)indexStart;
    iLength = pstr->iCharacterCount - (int)indexStart;

    if ( (strEncodingGetCharSize(pstr->iEncoding)==1) && (strEncodingGetCharSize(pstrValue->iEncoding)==1) )
    {
        iReturn = strMemmemBYTE (
            pstr->psz + iOffset,
            pstrValue->psz,
            iLength,
            pstrValue->iCharacterCount,
            bEnableFirst );
        if (iReturn!=-1) iReturn += iOffset;
        return iReturn;
    }

    if ( (strEncodingGetCharSize(pstr->iEncoding)==4) && (strEncodingGetCharSize(pstrValue->iEncoding)==4) )    
    {
        iReturn = strMemmemDWORD (
            pstr->pdw + iOffset,
            pstrValue->pdw,
            iLength,
            pstrValue->iCharacterCount,
            bEnableFirst );
        if (iReturn!=-1) iReturn += iOffset;
        return iReturn;
    }

    return -1;
}

size_t   _strIndexOfParams ( str * pstr, char * pszData, size_t indexStart, bool bEnableFirst )
{
    int iOffset;
    int iLength;
    int iReturn;

    iOffset = (int)indexStart;
    iLength = pstr->iCharacterCount - (int)indexStart;

    if (strEncodingGetCharSize(pstr->iEncoding)==1)
    {
        iReturn = strMemmemBYTE (
            pstr->psz + iOffset,
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
        str strTemp;

        strNew ( 
            &strTemp, 
            pstr );

        strCopy ( 
            &strTemp,
            pszData );

        indexReturn = _strIndexOfParams (
            pstr,
            &strTemp,
            indexStart, 
            bEnableFirst);
            
        strDelete ( &strTemp );

        return indexReturn;
    }
}


/*

    indexOf and indexLastOf with PSZ

*/
size_t   strIndexOf ( str * pstr, char * pszData, size_t indexStart )
{
    return _strIndexOfParams ( pstr, pszData, indexStart, true );
}

size_t   strIndexOf ( str * pstr, char * pszData)
{
    return _strIndexOfParams ( pstr, pszData, 0, true );
}

size_t   strLastIndexOf ( str * pstr, char * pszData)
{
    return _strIndexOfParams ( pstr, pszData, 0, false );
}

size_t   strLastIndexOf ( str * pstr, char * pszData, size_t indexStart)
{
    return _strIndexOfParams ( pstr, pszData, indexStart, false);
}


/*

indexOf and indexLastOf with STRING

*/
size_t   strIndexOf ( str * pstr, str * pstrValue, size_t indexStart)
{
    return _strIndexOfParams ( pstr, pstrValue, indexStart, true);
}

size_t   strIndexOf ( str * pstr, str * pstrValue)
{
    return _strIndexOfParams ( pstr, pstrValue, 0, true);
}

size_t   strLastIndexOf ( str * pstr, str * pstrValue)
{
    return _strIndexOfParams ( pstr, pstrValue, 0, false);
}

size_t   strLastIndexOf ( str * pstr, str * pstrValue, size_t indexStart)
{
    return _strIndexOfParams(pstr, pstrValue, indexStart, false);
}






int   _strCompareParams ( str * pstr, str * pstrValue, size_t indexStart )
{
    int iOffset;
    int iLength;
    int iReturn;
    int iMinimumLength;

    iReturn         = 0;
    iOffset         = (int)indexStart;
    iLength         = pstr->iCharacterCount - (int)indexStart;
    iMinimumLength  = strmin(iLength,pstrValue->iCharacterCount);

    if ( (strEncodingGetCharSize(pstr->iEncoding)==1) && (strEncodingGetCharSize(pstrValue->iEncoding)==1) )
    {
        iReturn = memcmp (
            pstr->psz + iOffset,
            pstrValue->psz,
            iMinimumLength );

        if (iReturn==0)
        {
            if (iLength<pstrValue->iCharacterCount)
                iReturn = -1;
            else if (iLength>pstrValue->iCharacterCount)
                iReturn = 1;
        }
    }

    if ( (strEncodingGetCharSize(pstr->iEncoding)==4) && (strEncodingGetCharSize(pstrValue->iEncoding)==4) )    
    {
        iReturn = memcmp (
            pstr->pdw + iOffset,
            pstrValue->pdw,
            iMinimumLength<<2 );

        if (iReturn==0)
        {
            if (iLength<pstrValue->iCharacterCount)
                iReturn = -1;
            else if (iLength>pstrValue->iCharacterCount)
                iReturn = 1;
        }
    }

    return iReturn;
}

size_t   _strCompareParams ( str * pstr, char * pszData, size_t indexStart )
{
    int iOffset;
    int iLength;
    int iReturn;
    int iMinimumLength;
    int iLengthStringZero;

    iReturn = 0;
    iOffset = (int)indexStart;
    iLength = pstr->iCharacterCount - (int)indexStart;
    iLengthStringZero = (int)strlen(pszData);
    iMinimumLength  = strmin(iLength,iLengthStringZero);

    if (strEncodingGetCharSize(pstr->iEncoding)==1)
    {
        iReturn = memcmp (
            pstr->psz + iOffset,
            pszData,
            iMinimumLength );

        if (iReturn==0)
        {
            if (iLength<iLengthStringZero)
                iReturn = -1;
            else if (iLength>iLengthStringZero)
                iReturn = 1;
        }
    }
    else
    {
        str strTemp;

        strNew ( 
            &strTemp, 
            pstr );

        strCopy ( 
            &strTemp,
            pszData );

        iReturn = _strCompareParams (
            pstr,
            &strTemp,
            indexStart );
            
        strDelete ( &strTemp );
    }

    return iReturn;
}


/*

compare with PSZ

*/
int     strCompare ( str * pstr, char * pszData )
{
    return _strCompareParams ( pstr, pszData, 0 );
}

int     strCompare ( str * pstr, char * pszData, size_t indexStart )
{
    return _strCompareParams ( pstr, pszData, indexStart );
}

/*

compare with STRING

*/
int     strCompare ( str * pstr, str * pstrValue )
{
    return _strCompareParams ( pstr, pstrValue, 0 );
}

int     strCompare ( str * pstr, str * pstrValue, size_t indexStart )
{
    return _strCompareParams ( pstr, pstrValue, indexStart );
}





