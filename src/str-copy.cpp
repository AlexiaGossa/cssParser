#include "str.h"




int         strCopy                         ( str * pstr, char * pszSource )
{
    return strCopy ( pstr, STRING_ENCODING_SIMPLEBYTE, (void *)pszSource, (int)strlen(pszSource) );
}

int         strCopy                         ( str * pstr, char * pszSource, int iSourceByteCount )
{
    return strCopy ( pstr, STRING_ENCODING_SIMPLEBYTE, (void *)pszSource, iSourceByteCount );
}

int         strCopy                         ( str * pstr, int iEncoding, char * pszSource )
{
    return strCopy ( pstr, iEncoding, (void *)pszSource, (int)strlen(pszSource) );
}


/*
 *  Only this transcoding capability is available
 *  - UTF-8 to UTF-32
 *  - UTF-8 to PageCode
 *  - UTF-32 to UTF-32 (copy)
 *  - UTF-32 to PageCode
 *  - PageCode to UTF-32
 *  - PageCode to PageCode
 */
int         strCopy                         ( str * pstr, int iEncoding, void * pSource, int iSourceByteCount )
{
    int iInputCharactersCount;
    void * pInternalBuffer;

    //########################################
    //Step 1 - Can't transcode to UTF-8
    if (pstr->iEncoding==STRING_ENCODING_UTF8)
        return -1;

    //########################################
    //Step 2 - Get source Length
    switch (iEncoding)
    {
        case STRING_ENCODING_UTF8:
            iInputCharactersCount = string_UTF8_decodeLength ( (char *)pSource, iSourceByteCount );
            break;

        case STRING_ENCODING_UTF32:     //  Source UTF-32
            iInputCharactersCount = iSourceByteCount / strEncodingGetCharSize ( STRING_ENCODING_UTF32 );
            break;

        default:                        //  Source PageCode
            iInputCharactersCount = iSourceByteCount;
            break;
    }

    //########################################
    //Step 3 - Set target length
    pstr->iCharacterCount = iInputCharactersCount;

    //########################################
    //Step 4 - Memory allocation
    pstr->p = realloc (
        pstr->p,
        pstr->iCharacterCount * strEncodingGetCharSize ( pstr->iEncoding ) );





    switch (iEncoding)
    {
        case STRING_ENCODING_UTF8:      //  Source UTF-8
            switch (pstr->iEncoding)
            {
                //########################################
                // UTF-8 to UTF-32
                case STRING_ENCODING_UTF32:

                    //Decode UTF-8 to UTF-32
                    string_UTF8_decodeToUTF32 ( 
                        (char *)pSource, 
                        iSourceByteCount, 
                        pstr->pdw );

                    break;

                //########################################
                // UTF-8 to UTF-32 to PageCode
                default:

                    //Internal buffer
                    pInternalBuffer = malloc (
                        pstr->iCharacterCount * strEncodingGetCharSize ( STRING_ENCODING_UTF32 ) );

                    //Decode UTF-8 to UTF-32
                    string_UTF8_decodeToUTF32 ( 
                        (char *)pSource, 
                        iSourceByteCount, 
                        (uint32_t *)pInternalBuffer );

                    //UTF-32 to PageCode
                    string_PageCode_encodeFromUTF32 ( 
                        pstr->iEncoding, 
                        (uint32_t *)pInternalBuffer,
                        pstr->iCharacterCount,
                        pstr->psz );

                    //Free internal buffer
                    free ( 
                        pInternalBuffer );

                    break;
            }
            break;

        case STRING_ENCODING_UTF32:     //  Source UTF-32
            switch (pstr->iEncoding)
            {
                //########################################
                //Simple copy
                case STRING_ENCODING_UTF32:
                    //Simple copy
                    memmove (
                        pstr->p,
                        pSource,
                        pstr->iCharacterCount * strEncodingGetCharSize ( pstr->iEncoding ) );

                    break;

                //########################################
                // UTF-32 to PageCode
                default:
                    //UTF-32 to PageCode
                    string_PageCode_encodeFromUTF32 (        
                        pstr->iEncoding,
                        (uint32_t *)pSource,
                        pstr->iCharacterCount,
                        pstr->psz );

                    break;
            }
            break;

        default:                        //  Source PageCode
            switch (pstr->iEncoding)
            {
                //########################################
                // PageCode to UTF-32
                case STRING_ENCODING_UTF32:

                    //PageCode to UTF-32
                    string_PageCode_decodeToUTF32 (        
                        iEncoding,
                        (char *)pSource,
                        pstr->iCharacterCount,
                        pstr->pdw );

                    break;

                //########################################
                // PageCode to PageCode
                default:
                    if (pstr->iEncoding == iEncoding)
                    {
                        //Simple copy
                        memmove (
                            pstr->p,
                            pSource,
                            pstr->iCharacterCount );
                    }
                    else
                    {
                        //Transcoding
                        string_PageCode_to_PageCode ( 
                            pstr->psz,
                            pstr->iEncoding,
                            (char *)pSource,
                            iEncoding,
                            pstr->iCharacterCount );
                    }
                    break;
            }
            break;
    }

    return 0;
}


int         strCopy                         ( str * pstr, uint32_t * pdwSource, int iSourceByteCount )
{
    return strCopy ( pstr, STRING_ENCODING_UTF32, pdwSource, iSourceByteCount );
}

int         strCopy                         ( str * pstr, str * pstrSource )
{
    return strCopy ( 
        pstr,
        pstrSource->iEncoding,
        pstrSource->p,
        pstrSource->iCharacterCount * strEncodingGetCharSize ( pstrSource->iEncoding ) );
}


