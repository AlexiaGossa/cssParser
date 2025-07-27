#ifndef zM_STR_CONVERT_HEADER
#define zM_STR_CONVERT_HEADER

#include "str.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4028)
#pragma warning(disable : 4996)
#endif


#define UTF8_CODEPOINT_1BYTE    0x00
#define UTF8_CODEPOINT_2BYTE    0xC0
#define UTF8_CODEPOINT_3BYTE    0xE0
#define UTF8_CODEPOINT_4BYTE    0xF0
#define UTF8_CODEPOINT_5BYTE    0xF8
#define UTF8_CODEPOINT_6BYTE    0xFC
#define UTF8_CODEPOINT_NBYTE    0x80

#define UNUSED                  0xFFFFFFFF


//Diacritical marks (non-spacing) : 0x0300 - 0x036F (0xcc80 - 0xcdaf)
typedef struct {
    char            cPreviousCharacter;
    unsigned int    iDiacriticalCode;
    unsigned int    iLatinCode;
} stringDiacriticalItem_t;


//CodePages...
typedef struct {
    char *          szCodePageName;
    int             iCodePage;
    unsigned int *  pCodePageTable128;
} stringCodePage_t;


extern stringCodePage_t StringCodePage[];
extern stringDiacriticalItem_t StringDiacriticalItem[];

/*
 *
 *  @RETURN return the effective length of the UTF8 (with diacritical support)
 */
int string_UTF8_decodeLength            ( char * pszInputUTF8,          int             iInputLength );
int string_UTF8_decodeToUTF32           ( char * pszInputUTF8,          int             iInputLength,           uint32_t * pdwOutputUTF32 );
int string_UTF8_encodeFromUTF32         ( uint32_t * pdwInputUTF32,     int             iInputLength,           char *  pszOutputUTF8,          int iOutputByteCount );
int string_PageCode_decodeToUTF32       ( int iPageCode,                char *          pszInput,               int     iInputLength,           uint32_t *      pdwOutputUTF32 );
int string_PageCode_encodeFromUTF32     ( int iPageCode,                uint32_t *      pdwInputUTF32,          int     iInputLength,           char *          pszOutput );
int string_PageCode_to_PageCode         ( char * pcTarget,              int iTargetPageCode,                    char * pcSource,                int iSourcePageCode,            int iCharLength );

/*
int string_BYTEtoUTF32                  ( int iSourceCodePage,          char *          pszInputDataRAW,        int     iInputLengthRAW,        uint32_t *      pdwOutputDataUTF32 );
int string_BYTEtoUTF8                   (                               char *          pszInputDataRAW,        int     iInputLengthRAW,        uint8_t *       pbOutputBYTE );
int string_UTF32toBYTE                  ( int iTargetCodePage,          uint32_t *      pdwInputDataUTF32,      int     iInputLengthUTF32,      char *          pszOutputDataRAW );
*/
#endif
