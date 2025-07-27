#include "str.h"

int         strNew                          ( str * pstr )
{
    return strNew ( pstr, STRING_ENCODING_SIMPLEBYTE );
}

int         strNew                          ( str * pstr, int iEncoding )
{
    //Clean
    memset ( pstr, 0, sizeof(str) );

    //Encoding
    pstr->iEncoding = iEncoding;

    //security
    strSecurityInit ( pstr );

    return 0;
}

int         strNew                          ( str * pstr, str * pstrBasedFrom )
{
    return strNew ( pstr, pstrBasedFrom->iEncoding );
}

int         strDelete                       ( str * pstr )
{
    strSecurityWhitness ( 
        pstr, 
        pstr->p,
        pstr->iCharacterCount * strEncodingGetCharSize ( pstr->iEncoding ) );

    free ( pstr->p );
    
    memset ( pstr, 0, sizeof(str) );

    return 0;
}


int strLength ( str * pstr )
{
    return pstr->iCharacterCount;
}

int strByteCount ( str * pstr )
{
    return pstr->iCharacterCount * strEncodingGetCharSize ( pstr->iEncoding );
}

int         strZero ( str * pstr )
{
    pstr->iCharacterCount = 0;
    
    return 0;    
}

int         strTruncate ( str * pstr, size_t length )
{
    if (length<pstr->iCharacterCount)
        pstr->iCharacterCount = length;

    return pstr->iCharacterCount;
}

int         strEncodingGetCharSize          ( int iEncoding )
{
    if (iEncoding==STRING_ENCODING_UTF32)
        return 4;
    else
        return 1;
}

