/*
 *  Similar C++ implementation of JS string
 */
#ifndef zM_STR_HEADER
#define zM_STR_HEADER

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include "str-convert.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4028)
#pragma warning(disable : 4996)
#endif


#define STRING_MODE_BYTE         1
#define STRING_MODE_UNICODE      4

#define STRING_ENCODING_SIMPLEBYTE      0xFFFFFFFF
#define STRING_ENCODING_MACROMAN        0x30000001
#define STRING_ENCODING_WINDOWS1250     0x20001250
#define STRING_ENCODING_WINDOWS1252     0x20001252
#define STRING_ENCODING_IBM437          0x10000437
#define STRING_ENCODING_IBM850          0x10000850
#define STRING_ENCODING_IBM852          0x10000852
#define STRING_ENCODING_ISO8859_1       0x08859001
#define STRING_ENCODING_ISO8859_2       0x08859002
#define STRING_ENCODING_ISO8859_3       0x08859003
#define STRING_ENCODING_ISO8859_4       0x08859004
#define STRING_ENCODING_ISO8859_15      0x08859015
#define STRING_ENCODING_UTF32           0x00000020
#define STRING_ENCODING_UTF8            0x00000008
#define STRING_ENCODING_NONE            0x00000000


typedef struct {
    union {
        char *              psz;
        uint32_t *          pdw;
        void *              p;
    };

    int                 iCharacterCount;          //Source length in characters (could be byte or dword)

    

    //int                 iUsedOffset;            //Offset in characters
    //int                 iUsedLength;            //Length in characters

    //bool                bAllocated;

    int                 iEncoding;
    uint64_t            ui64SecuritySeed[2];
} str;



/*
 *  str-security
 */
void        strSecurityInit                 ( str * pstr );
void        strSecurityUpdate               ( str * pstr, uint64_t qwValue );
void        strSecurityWhitness             ( str * pstr, void * p, size_t nByteCount );

/*
 *  str-base
 */

int         strNew                          ( str * pstr );
int         strNew                          ( str * pstr, int iEncoding );
int         strNew                          ( str * pstr, str * pstrOrigin );
int         strDelete                       ( str * pstr );


int         strEncodingGetCharSize          ( int iEncoding );


int         strCopy                         ( str * pstr, char * pszSource );                                                
int         strCopy                         ( str * pstr, char * pszSource, int iSourceByteCount );                          
int         strCopy                         ( str * pstr, int iEncoding, char * pszSource );                                 
int         strCopy                         ( str * pstr, int iEncoding, void * pSource, int iSourceByteCount );           
int         strCopy                         ( str * pstr, uint32_t * pdwSource, int iSourceByteCount );                      
int         strCopy                         ( str * pstr, str * pstrSource );                                                

int         strLength                       ( str * pstr );
int         strByteCount                    ( str * pstr );
int         strZero                         ( str * pstr );
int         strTruncate                     ( str * pstr, size_t length );


//New str and read file or memory. Return the read characters count
int         strNewFileGetContents           ( str * pstr, char * pszFilename );
int         strNewMemoryGetContents         ( str * pstr, void * pSource, int iSourceByteCount );

int         strExport                       ( str * pstr, char * pszTarget, int iTargetBufferSizeByteCount, bool bEnableZeroEnding );
int         strExport                       ( str * pstr, int iEncoding, void * pTarget, int iTargetBufferSizeByteCount, bool bEnableZeroEnding );


size_t      strIndexOf                      ( str * pstr, str * pstrValue);
size_t      strIndexOf                      ( str * pstr, str * pstrValue, size_t indexStart);
size_t      strIndexOf                      ( str * pstr, char * pszData );
size_t      strIndexOf                      ( str * pstr, char * pszData, size_t indexStart );

size_t      strLastIndexOf                  ( str * pstr, str * pstrValue);
size_t      strLastIndexOf                  ( str * pstr, str * pstrValue, size_t indexStart);
size_t      strLastIndexOf                  ( str * pstr, char * pszData );
size_t      strLastIndexOf                  ( str * pstr, char * pszData, size_t indexStart);

int         strCompare                      ( str * pstr, char * pszData );
int         strCompare                      ( str * pstr, char * pszData, size_t indexStart );
int         strCompare                      ( str * pstr, str * pstrValue );
int         strCompare                      ( str * pstr, str * pstrValue, size_t indexStart );


int         strSubstring                    ( str * pstr, str * pstrSource, size_t indexStart );
int         strSubstring                    ( str * pstr, str * pstrSource, size_t indexStart, size_t indexEnd );
int         strSubstr                       ( str * pstr, str * pstrSource, size_t indexStart );
int         strSubstr                       ( str * pstr, str * pstrSource, size_t indexStart, size_t length );

int         strCharAt                       ( str * pstr, str * pstrSource, size_t indexAt );
uint32_t    strCharGet                      ( str * pstr, int iIndex );
void        strCharConcat                   ( str * pstr, uint32_t uiChar );


void        strTrim                         ( str * pstr );
void        strTrim                         ( str * pstr, str * pstrSource );
void        strTrimStart                    ( str * pstr );
void        strTrimStart                    ( str * pstr, str * pstrSource );
void        strTrimEnd                      ( str * pstr );
void        strTrimEnd                      ( str * pstr, str * pstrSource );



/*
int         strzCopy                        ( strz * pstrzTarget, strz * pstrzSource );

int         strzCopy                    ( strz * pstrz, char * pszSource );
int         strzCopy                    ( strz * pstrz, char * pszSource, int iSourceLength );
int         strzCopy                    ( strz * pstrz, uint32_t * pqwSource, int iSourceLength );

int         strzFree                    ( strz * pstrz );
int         strzClear                   ( strz * pstrz );

int         strzLength                  ( strz * pstrz );
int         strzExport                  ( strz * pstrz, char * pszTarget, int iTargetLength, bool bEnableZeroEnding );
size_t      strzFileGetContents         ( strz * pstrz, char * pszFilename );


//Search
size_t      strzIndexOf                 ( strz * pstrz, char * pszData, size_t indexStart );
size_t      strzIndexOf                 ( strz * pstrz, char * pszData );

size_t      strzLastIndexOf             ( strz * pstrz, char * pszData, size_t indexStart);
size_t      strzLastIndexOf             ( strz * pstrz, char * pszData );

//Sub
strz        strzSubstringCopy           ( strz * pstrz, size_t indexStart );
strz        strzSubstringCopy           ( strz * pstrz, size_t indexStart, size_t indexEnd );
strz        strzSubstringLink           ( strz * pstrz, size_t indexStart );
strz        strzSubstringLink           ( strz * pstrz, size_t indexStart, size_t indexEnd );

strz        strzSubstrCopy              ( strz * pstrz, size_t indexStart );
strz        strzSubstrCopy              ( strz * pstrz, size_t indexStart, size_t length );
strz        strzSubstrLink              ( strz * pstrz, size_t indexStart );
strz        strzSubstrLink              ( strz * pstrz, size_t indexStart, size_t length );

strz        strzCharAtCopy              ( strz * pstrz, size_t indexAt );
strz        strzCharAtLink              ( strz * pstrz, size_t indexAt );
uint32_t    strzCharGet                 ( strz * pstrz, int iIndex );

void        strzCharConcatCopy          ( strz * pstrz, uint32_t uiChar );

//Trim
strz        strzTrimCopy                ( strz * pstrz );
strz        strzTrimLink                ( strz * pstrz );
void        strzTrim                    ( strz * pstrz );

strz        strzTrimStartCopy           ( strz * pstrz );
strz        strzTrimStartLink           ( strz * pstrz );
void        strzTrimStart               ( strz * pstrz );

strz        strzTrimEndCopy             ( strz * pstrz );
strz        strzTrimEndLink             ( strz * pstrz );
void        strzTrimEnd                 ( strz * pstrz );
*/


























#endif