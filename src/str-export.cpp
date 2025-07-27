#include "str.h"
#if !_WIN32 && !__APPLE__
    #define __LINUX__
    #include <locale.h>
#endif

static int  strLocale_iPageCode = -1;

int strFindPageCode ( char * pszPageCode );



void strDetectAutoPageCode ( void )
{
    char *  pszLocaleInfo, szLocale[256];
        
    if (strLocale_iPageCode!=-1)
        return;

#ifdef __APPLE__
    strLocale_iPageCode = STRING_ENCODING_MACROMAN;
#elif __LINUX__
    strcpy ( szLocale, setlocale(LC_CTYPE,"") );
    strLocale_iPageCode = strFindPageCode ( szLocale );
#else
    strLocale_iPageCode = STRING_ENCODING_WINDOWS1252;
#endif
}



int         strExport                   ( str * pstr, int iEncoding, void * pTarget, int iTargetBufferSizeByteCount, bool bEnableZeroEnding )
{
    int iTargetCharactersCount;
    int iTargetBytesCount;
    uint32_t * pdwBuffer;

    switch (pstr->iEncoding)
    {
        // ################################
        // From UTF-32 to ...
        case STRING_ENCODING_UTF32:
            switch (iEncoding)
            {
                // ************************
                // ... to UTF-32
                case STRING_ENCODING_UTF32:

                    iTargetCharactersCount = iTargetBufferSizeByteCount>>2;

                    //Add a zero ending
                    if (bEnableZeroEnding)
                        iTargetCharactersCount -= 1;

                    //Limit characters count
                    if (iTargetCharactersCount>pstr->iCharacterCount)
                        iTargetCharactersCount = pstr->iCharacterCount;

                    if (iTargetCharactersCount<0)
                        return 0;

                    memcpy (
                        pTarget,
                        pstr->pdw,
                        iTargetCharactersCount << 2 );

                    if (bEnableZeroEnding)
                        ((uint32_t *)pTarget)[iTargetCharactersCount] = 0;
                    
                    return (iTargetCharactersCount + ((bEnableZeroEnding)?(1):(0))) << 2;

                // ************************
                // ... to UTF-8
                case STRING_ENCODING_UTF8:

                    iTargetBytesCount = string_UTF8_encodeFromUTF32 (
                        pstr->pdw,
                        pstr->iCharacterCount,
                        (char *)pTarget,
                        iTargetBufferSizeByteCount-((bEnableZeroEnding)?(1):(0)) );

                    if (bEnableZeroEnding)
                        ((char *)pTarget)[iTargetBytesCount] = 0;

                    return iTargetBytesCount + ((bEnableZeroEnding)?(1):(0));

                // ************************
                // ... to PageCode
                default:

                    iTargetCharactersCount = iTargetBufferSizeByteCount;

                    //Add a zero ending
                    if (bEnableZeroEnding)
                        iTargetCharactersCount -= 1;

                    //Limit characters count
                    if (iTargetCharactersCount>pstr->iCharacterCount)
                        iTargetCharactersCount = pstr->iCharacterCount;

                    if (iTargetCharactersCount<0)
                        return 0;

                    string_PageCode_encodeFromUTF32 ( 
                        iEncoding,
                        pstr->pdw,
                        iTargetCharactersCount,
                        (char *)pTarget );

                    if (bEnableZeroEnding)
                        ((uint8_t *)pTarget)[iTargetCharactersCount] = 0;
                    
                    return iTargetCharactersCount + ((bEnableZeroEnding)?(1):(0));
            }
            break;

        // ################################
        // From UTF-8 to ... nothing !!!
        case STRING_ENCODING_UTF8:
            return 0;

        // ################################
        // From PageCode to ...
        default:
            switch (iEncoding)
            {
                // ************************
                // ... to UTF-32
                case STRING_ENCODING_UTF32:

                    iTargetCharactersCount = iTargetBufferSizeByteCount>>2;

                    //Add a zero ending
                    if (bEnableZeroEnding)
                        iTargetCharactersCount -= 1;

                    //Limit characters count
                    if (iTargetCharactersCount>pstr->iCharacterCount)
                        iTargetCharactersCount = pstr->iCharacterCount;

                    if (iTargetCharactersCount<0)
                        return 0;

                    string_PageCode_decodeToUTF32 ( 
                        pstr->iEncoding,
                        pstr->psz,
                        iTargetCharactersCount,
                        (uint32_t *)pTarget );

                    if (bEnableZeroEnding)
                        ((uint32_t *)pTarget)[iTargetCharactersCount] = 0;
                    
                    return (iTargetCharactersCount + ((bEnableZeroEnding)?(1):(0))) << 2;

                // ************************
                // ... to UTF-8
                case STRING_ENCODING_UTF8:

                    //Alloc temporary buffer
                    pdwBuffer = (uint32_t *)malloc ( sizeof(uint32_t) * pstr->iCharacterCount );

                    //PageCode to UTF-32
                    string_PageCode_decodeToUTF32 ( 
                        pstr->iEncoding,
                        pstr->psz,
                        pstr->iCharacterCount,
                        pdwBuffer );

                    //UTF-32 to UTF-8
                    iTargetBytesCount = string_UTF8_encodeFromUTF32 (
                        pdwBuffer,
                        pstr->iCharacterCount,
                        (char *)pTarget,
                        iTargetBufferSizeByteCount-((bEnableZeroEnding)?(1):(0)) );

                    //Free temporary buffer
                    free ( pdwBuffer );

                    if (bEnableZeroEnding) ((char *)pTarget)[iTargetBytesCount] = 0;
                    return iTargetBytesCount + ((bEnableZeroEnding)?(1):(0));

                // ************************
                // ... to PageCode
                default:

                    iTargetCharactersCount = iTargetBufferSizeByteCount;

                    //Add a zero ending
                    if (bEnableZeroEnding)
                        iTargetCharactersCount -= 1;

                    //Limit characters count
                    if (iTargetCharactersCount>pstr->iCharacterCount)
                        iTargetCharactersCount = pstr->iCharacterCount;

                    if (iTargetCharactersCount<0)
                        return 0;

                    string_PageCode_to_PageCode ( 
                        (char *)pTarget,
                        iEncoding,
                        pstr->psz,
                        pstr->iEncoding,
                        iTargetCharactersCount );

                    if (bEnableZeroEnding)
                        ((uint8_t *)pTarget)[iTargetCharactersCount] = 0;
                    
                    return iTargetCharactersCount + ((bEnableZeroEnding)?(1):(0));
            }
            break;

    }
    return 0;
}


int         strExport                   ( str * pstr, char * pszTarget, int iTargetBufferSizeByteCount, bool bEnableZeroEnding )
{
    return strExport ( 
        pstr, 
        strLocale_iPageCode, 
        pszTarget, 
        iTargetBufferSizeByteCount, 
        bEnableZeroEnding );   
}



typedef struct {
    char sz[16];
    int  iPageCode;
} strLocalePageCodeItem;

strLocalePageCodeItem pLPCI[] = {

    {"ISO-8859-1",      STRING_ENCODING_ISO8859_1},
    {"ISO-8859-2",      STRING_ENCODING_ISO8859_2},
    {"ISO-8859-3",      STRING_ENCODING_ISO8859_3},
    {"ISO-8859-4",      STRING_ENCODING_ISO8859_4},
    {"ISO-8859-15",     STRING_ENCODING_ISO8859_15},

    {"ISO8859-1",       STRING_ENCODING_ISO8859_1},
    {"ISO8859-2",       STRING_ENCODING_ISO8859_2},
    {"ISO8859-3",       STRING_ENCODING_ISO8859_3},
    {"ISO8859-4",       STRING_ENCODING_ISO8859_4},
    {"ISO8859-15",      STRING_ENCODING_ISO8859_15},

    {"UTF-8",           STRING_ENCODING_UTF8},
    {"UTF8",            STRING_ENCODING_UTF8},

    {"MAC-ROMAN",       STRING_ENCODING_MACROMAN},
    {"MACROMAN",        STRING_ENCODING_MACROMAN},

    {"WINDOWS1250",     STRING_ENCODING_WINDOWS1250 },
    {"WINDOWS-1250",    STRING_ENCODING_WINDOWS1250 },
    {"cp-1250",         STRING_ENCODING_WINDOWS1250 },
    {"cp1250",          STRING_ENCODING_WINDOWS1250 },

    {"WINDOWS1252",     STRING_ENCODING_WINDOWS1252 },
    {"WINDOWS-1252",    STRING_ENCODING_WINDOWS1252 },
    {"cp-1252",         STRING_ENCODING_WINDOWS1252 },
    {"cp1252",          STRING_ENCODING_WINDOWS1252 },

    {"IBM437",          STRING_ENCODING_IBM437 },
    {"cp437",           STRING_ENCODING_IBM437 },
    {"cp-437",          STRING_ENCODING_IBM437 },
    {"oem437",          STRING_ENCODING_IBM437 },
    {"oem-437",         STRING_ENCODING_IBM437 },

    {"IBM850",          STRING_ENCODING_IBM850 },
    {"cp850",           STRING_ENCODING_IBM850 },
    {"cp-850",          STRING_ENCODING_IBM850 },
    {"oem850",          STRING_ENCODING_IBM850 },
    {"oem-850",         STRING_ENCODING_IBM850 },

    {"IBM852",          STRING_ENCODING_IBM852 },
    {"cp852",           STRING_ENCODING_IBM852 },
    {"cp-852",          STRING_ENCODING_IBM852 },
    {"oem852",          STRING_ENCODING_IBM852 },
    {"oem-852",         STRING_ENCODING_IBM852 },

    {"", -1 } };

int strFindPageCode ( char * pszPageCode )
{
    int iIndex;

    for (iIndex=0;pLPCI[iIndex].iPageCode!=-1;iIndex++)
    {
        if (strstr(pszPageCode,pLPCI[iIndex].sz))
        {
            return pLPCI[iIndex].iPageCode;
            break;
        }
    }

    return -1;
}