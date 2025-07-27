
#include "string.h"

/*
 *
 *  '=', '+=', '+' operator for use of STRING
 *
 */
eSTRING & eSTRING::operator = ( eSTRING & strValue )
{
    this->_strcpy ( strValue );
    if (strValue.iTempAlloc==1) delete &strValue;
    return * this;
}

eSTRING & eSTRING::operator += ( eSTRING & strValue )
{
    this->_strcat ( strValue );
    if (strValue.iTempAlloc==1) delete &strValue;
    return * this;
}

eSTRING & eSTRING::operator + ( eSTRING & strValue )
{
    this->_strcat ( strValue );
    if (strValue.iTempAlloc==1) delete &strValue;
    return * this;
}

/*
 *
 *  '=', '+=', '+' operator for use of (char*)psz
 *
 */
eSTRING & eSTRING::operator = ( char * pszData )
{
    this->_strcpy ( pszData, this->iCharLocaleEncoding );
    return * this;
}

eSTRING & eSTRING::operator += ( char * pszData )
{
    this->_strcat ( pszData, this->iCharLocaleEncoding );
    return * this;
}

eSTRING & eSTRING::operator + ( char * pszData )
{
    this->_strcat ( pszData, this->iCharLocaleEncoding );
    return * this;
}


/*
 *
 *  '=', '+=', '+' operator for use of int32Value
 *
 */
eSTRING & eSTRING::operator =     ( int int32Value )
{
    sprintf ( this->szTmp, "%d", int32Value );
    this->_strcpy ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}
eSTRING & eSTRING::operator +=    ( int int32Value )
{
    sprintf ( this->szTmp, "%d", int32Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}

eSTRING & eSTRING::operator +     ( int int32Value )
{
    sprintf ( this->szTmp, "%d", int32Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}


/*
 *
 *  '=', '+=', '+' operator for use of uint32Value
 *
 */
eSTRING & eSTRING::operator =     ( unsigned int uint32Value )
{
    sprintf ( this->szTmp, "%u", uint32Value );
    this->_strcpy ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}
eSTRING & eSTRING::operator +=    ( unsigned int uint32Value )
{
    sprintf ( this->szTmp, "%u", uint32Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}

eSTRING & eSTRING::operator +     ( unsigned int uint32Value )
{
    sprintf ( this->szTmp, "%u", uint32Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}

/*
 *
 *  '=', '+=', '+' operator for use of CHAR32
 *
 */
/*
eSTRING & eSTRING::operator =     ( eCHAR cValue )
{
    eSTRING strTemp;
    uint32_t uiChar;
    uiChar = cValue;
    strTemp.Buffer_NewAlloc ( 1, STRING_MODE_UNICODE );
    strTemp.Buffer_Replace ( (uint8_t *)&uiChar, 1 );

    this->_strcpy ( strTemp );
    return * this;
}
eSTRING & eSTRING::operator +=    ( eCHAR cValue )
{
    eSTRING strTemp;
    uint32_t uiChar;
    uiChar = cValue;
    strTemp.Buffer_NewAlloc ( 1, STRING_MODE_UNICODE );
    strTemp.Buffer_Replace ( (uint8_t *)&uiChar, 1 );

    this->_strcat ( strTemp );
    return * this;
}

eSTRING & eSTRING::operator +     ( eCHAR cValue )
{
    eSTRING strTemp;
    uint32_t uiChar;
    uiChar = cValue;
    strTemp.Buffer_NewAlloc ( 1, STRING_MODE_UNICODE );
    strTemp.Buffer_Replace ( (uint8_t *)&uiChar, 1 );

    this->_strcat ( strTemp );
    return * this;
}
*/


/*
 *
 *  '=', '+=', '+' operator for use of int8Value
 *
 */
eSTRING & eSTRING::operator =     ( char int8Value )
{
    sprintf ( this->szTmp, "%d", int8Value );
    this->_strcpy ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}
eSTRING & eSTRING::operator +=    ( char int8Value )
{
    sprintf ( this->szTmp, "%d", int8Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}

eSTRING & eSTRING::operator +     ( char int8Value )
{
    sprintf ( this->szTmp, "%d", int8Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}


/*
 *
 *  '=', '+=', '+' operator for use of uint8Value
 *
 */
eSTRING & eSTRING::operator =     ( unsigned char uint8Value )
{
    sprintf ( this->szTmp, "%u", uint8Value );
    this->_strcpy ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}
eSTRING & eSTRING::operator +=    ( unsigned char uint8Value )
{
    sprintf ( this->szTmp, "%u", uint8Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}

eSTRING & eSTRING::operator +     ( unsigned char uint8Value )
{
    sprintf ( this->szTmp, "%u", uint8Value );
    this->_strcat ( this->szTmp, STRING_ENCODING_NONE );
    return * this;
}



/*
STRCHAR & STRING::operator [] ( int iIndex )
{
    //Operation A
    STRCHAR * ch;

    iIndex = 0;

    ch = new STRCHAR();

    if ( (iIndex>=this->iCharactersCount) ||
         (iIndex<0) ||
         (this->iCharactersCount==0) )
        return *ch;

    if (this->iCharactersWordSize == STRING_MODE_BYTE)
        ch->data = ((uint8_t *)this->pCharactersBuffer)[iIndex];
    else
        ch->data = ((uint32_t *)this->pCharactersBuffer)[iIndex];

    return *ch;

    //return ((STRCHAR *)this->pCharactersBuffer)[iIndex];

    //return *this;
/*    
    if ( (iIndex>=this->iCharactersCount) ||
         (iIndex<0) ||
         (this->iCharactersCount==0) )
        return (STRCHAR &)this->szTmp;

    if (this->iCharactersWordSize == STRING_MODE_BYTE)
    {
        //return (uint32_t)(((uint8_t *)this->pCharactersBuffer)[iIndex]);
        return (STRCHAR &)this->szTmp;
    }
    else
    {
        return ((STRCHAR *)this->pCharactersBuffer)[iIndex];
    }


}
/*
STRCHAR STRING::operator [] ( int iIndex ) const
{
    //Operation B
    STRCHAR ch;

    iIndex = 0;

    if ( (iIndex>=this->iCharactersCount) ||
         (iIndex<0) ||
         (this->iCharactersCount==0) )
        return ch;

    if (this->iCharactersWordSize == STRING_MODE_BYTE)
        ch.data = ((uint8_t *)this->pCharactersBuffer)[iIndex];
    else
        ch.data = ((uint32_t *)this->pCharactersBuffer)[iIndex];


    return ch;
/*

    if (this->iCharactersWordSize == STRING_MODE_BYTE)
        return (STRCHAR)(((uint8_t *)this->pCharactersBuffer)[iIndex])();
    else
        return ((STRCHAR *)this->pCharactersBuffer)[iIndex]();
*/

    //return (STRCHAR)(((uint8_t *)this->pCharactersBuffer)[iIndex]);
//}

    /*case  8: reg_EAX = sprintf ( psz, "%o", lValue ); break;
        case 10: reg_EAX = sprintf ( psz, "%d", lValue ); break;
        case 16: reg_EAX = sprintf ( psz, "%x", lValue ); break;*/


/*
STRING &    STRING::operator =              ( STRCHAR ch )
{


}
*/


//STRING & STRING::operator = ( STRING const & that )
/*
uint32_t STRING::operator = ( int iIndex )
{
    //if (*this != that) 
    {
    
    }
    return *this;
}

*/



void        eSTRING::charSet                     ( int iIndex, uint32_t cValue )
{
    if ( (iIndex>=this->iCharactersCount) ||
         (iIndex<0) ||
         (this->iCharactersCount==0) )
        return;

    if (this->iCharactersWordSize == STRING_MODE_BYTE)
        ((uint8_t *)this->pCharactersBuffer)[iIndex] = cValue;
    else
        ((uint32_t *)this->pCharactersBuffer)[iIndex] = cValue;

}

uint32_t    eSTRING::charGet                     ( int iIndex )
{
    uint32_t uiChar;
    void * pChar;

    if ( (iIndex>=this->iCharactersCount) ||
         (iIndex<0) ||
         (this->iCharactersCount==0) )
    {
        uiChar = 0;
        pChar  = NULL;
    }
    else if (this->iCharactersWordSize == STRING_MODE_BYTE)
    {
        pChar = ((uint8_t *)this->pCharactersBuffer) + iIndex;
        uiChar = *((uint8_t *)pChar);
    }
    else
    {
        pChar = ((uint32_t *)this->pCharactersBuffer) + iIndex;
        uiChar = *((uint32_t *)pChar);
    }

    return uiChar;
}

void        eSTRING::charConcat              ( uint32_t cValue )
{
    eSTRING strTemp;
    uint32_t uiChar;
    uiChar = cValue;
    strTemp.Buffer_NewAlloc ( 1, STRING_MODE_UNICODE );
    strTemp.Buffer_Replace ( (uint8_t *)&uiChar, 1 );

    this->_strcat ( strTemp );
}

void        eSTRING::charStrcpy              ( uint32_t cValue )
{
    eSTRING strTemp;
    uint32_t uiChar;
    uiChar = cValue;
    strTemp.Buffer_NewAlloc ( 1, STRING_MODE_UNICODE );
    strTemp.Buffer_Replace ( (uint8_t *)&uiChar, 1 );

    this->_strcpy ( strTemp );
}


/*
int         eSTRING::set                     ( int iIndex, eCHAR cValue )
{
    if ( (iIndex>=this->iCharactersCount) ||
         (iIndex<0) ||
         (this->iCharactersCount==0) )
        return 0;

    if (this->iCharactersWordSize == STRING_MODE_BYTE)
        ((uint8_t *)this->pCharactersBuffer)[iIndex] = cValue.data;
    else
        ((uint32_t *)this->pCharactersBuffer)[iIndex] = cValue.data;

    return 0;
}

eCHAR & eSTRING::get                     ( int iIndex )
{
    eCHAR * cValue;
    uint32_t uiChar;
    void * pChar;

    if ( (iIndex>=this->iCharactersCount) ||
         (iIndex<0) ||
         (this->iCharactersCount==0) )
    {
        uiChar = 0;
        pChar  = NULL;
    }
    else if (this->iCharactersWordSize == STRING_MODE_BYTE)
    {
        pChar = ((uint8_t *)this->pCharactersBuffer) + iIndex;
        uiChar = *((uint8_t *)pChar);
    }
    else
    {
        pChar = ((uint32_t *)this->pCharactersBuffer) + iIndex;
        uiChar = *((uint32_t *)pChar);
    }

    cValue                  = new eCHAR ( uiChar );
    cValue->iWriteMode      = this->iCharactersWordSize;
    cValue->pWriteAccess    = pChar;

    return *cValue;
}

eCHAR &     eSTRING::operator []             ( int iIndex )
{
    return this->get ( iIndex );
}
*/