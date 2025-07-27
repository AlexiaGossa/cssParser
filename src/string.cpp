#include "string.h"
#include "string-convert.h"

void eSTRING::Buffer_Init ( void )
{
    this->iCharactersCount          = 0;
    this->length                    = 0;
    this->iCharactersAllocated      = 0;
    this->pCharactersBuffer         = NULL;
    this->iCharactersWordSize       = 1;
    this->iTempAlloc                = 2;
}



void eSTRING::Buffer_Free ( void )
{
    free ( this->pCharactersBuffer );

    this->iCharactersCount          = 0;
    this->length                    = 0;
    this->iCharactersAllocated      = 0;
    this->pCharactersBuffer         = NULL;
    this->iCharactersWordSize       = 1;
    this->iTempAlloc                = 0;
}

void eSTRING::Buffer_NewAlloc ( int iCharactersCountNew, int iCharactersWordSizeNew )
{
    //Clean memory area
    Security_Whiteness (
        this->pCharactersBuffer,
        this->iCharactersCount * this->iCharactersWordSize );

    //Free buffer
    free ( this->pCharactersBuffer );


    //The allocation size
    this->iCharactersWordSize   = iCharactersWordSizeNew;
    this->iCharactersAllocated  = (iCharactersCountNew + 1024)&0xFFFC00;
    this->iCharactersCount      = iCharactersCountNew;
    this->length                = iCharactersCountNew;

    //Allocation of the new buffer
    this->pCharactersBuffer = 
        (void *)malloc ( 
            this->iCharactersAllocated * this->iCharactersWordSize );
}

void eSTRING::Buffer_Realloc ( int iCharactersCountNew, bool bDisableCopy )
{
    int     iCharactersAllocatedNew; // iNewReallocSize;
    void *  pNewBuffer;

    //The reallocation size
    iCharactersAllocatedNew = (iCharactersCountNew + 1024)&0xFFFC00;

    //Do we need a realloc ?
    if (iCharactersAllocatedNew != this->iCharactersAllocated)
    {
        //Allocation of the buffer
        pNewBuffer = (void *)malloc ( iCharactersAllocatedNew * this->iCharactersWordSize );

        //Buffer copy
        if (!bDisableCopy)
            memcpy (
                pNewBuffer,
                this->pCharactersBuffer,
                (iCharactersAllocatedNew<this->iCharactersCount)?(iCharactersAllocatedNew*this->iCharactersWordSize):(this->iCharactersCount*this->iCharactersWordSize) );

        //On nettoie l'ancienne zone mémoire
        Security_Whiteness (
            this->pCharactersBuffer,
            this->iCharactersCount * this->iCharactersWordSize );

        //Free buffer
        free ( this->pCharactersBuffer );

        //Transfert variables
        this->pCharactersBuffer       = pNewBuffer;
    }
    else if (bDisableCopy)
    {
        Security_Whiteness (
            this->pCharactersBuffer,
            this->iCharactersCount * this->iCharactersWordSize );
    }

    //Transfert variables
    this->iCharactersAllocated    = iCharactersAllocatedNew;
    this->iCharactersCount        = iCharactersCountNew;
    this->length                  = iCharactersCountNew;

    Security_SeedGenerator ( this->iCharactersCount );
}



void eSTRING::Buffer_Replace ( unsigned char * pData, int iCharactersCountNew )
{
    //A new allocation !
    Buffer_Realloc ( 
        iCharactersCountNew,
        true );

    //Copy the new data
    memcpy (
        this->pCharactersBuffer,
        pData,
        iCharactersCountNew * this->iCharactersWordSize );
}



void eSTRING::Buffer_Concat ( unsigned char * pData, int iCharactersCountNew )
{
    int iCharactersCountPrevious;

    iCharactersCountPrevious = this->iCharactersCount;

    Buffer_Realloc ( 
        iCharactersCountPrevious + iCharactersCountNew,
        false );

    memcpy (
        ((uint8_t *)this->pCharactersBuffer) + (iCharactersCountPrevious * this->iCharactersWordSize),
        pData,
        iCharactersCountNew * this->iCharactersWordSize );
}


/**
 *
 *
 *
 */
void eSTRING::_strcpy ( eSTRING & stringExt )
{
    int iConvertMode;

    iConvertMode  = (this->iCharactersWordSize      == STRING_MODE_UNICODE)?(1):(0);
    iConvertMode |= (stringExt.iCharactersWordSize  == STRING_MODE_UNICODE)?(2):(0);

    switch (iConvertMode)
    {
        case 0: //convert BYTE  => BYTE
        case 3: //convert UTF32 => UTF32

            this->Buffer_Replace ( 
                (unsigned char *)stringExt.pCharactersBuffer, 
                stringExt.iCharactersCount );

            break;

        case 1: //convert BYTE  => UTF32

            this->Buffer_Realloc ( 
                stringExt.iCharactersCount, 
                true );

            string_BYTEtoUTF32 (
                STRING_ENCODING_NONE,
                (char *)stringExt.pCharactersBuffer,
                stringExt.iCharactersCount,
                (uint32_t *)this->pCharactersBuffer );

            break;

        case 2: //convert UTF32 => BYTE

            this->Buffer_Realloc ( 
                stringExt.iCharactersCount, 
                true );

            string_UTF32toBYTE (
                STRING_ENCODING_NONE,
                (uint32_t *)stringExt.pCharactersBuffer,
                stringExt.iCharactersCount,
                (char *)this->pCharactersBuffer );

            break;
    }
}

/**
 *
 *
 *
 */
void eSTRING::_strcpy ( char * pszStringExt, int iStringEncodingChar )
{
    int iLength;

    if (iStringEncodingChar == STRING_ENCODING_UTF8)
        iLength = string_UTF8_decodeLength ( pszStringExt, (int)strlen(pszStringExt) );
    else 
        iLength = (int)strlen(pszStringExt);


    if (this->iCharactersWordSize == STRING_MODE_UNICODE)
    {
        this->Buffer_Realloc ( 
            iLength, 
            true );

        string_BYTEtoUTF32 (
            iStringEncodingChar,
            (char *)pszStringExt,
            iLength,
            (uint32_t *)this->pCharactersBuffer );
    }
    else
    {
        this->Buffer_Replace ( 
            (unsigned char *)pszStringExt, 
            iLength );
    }
}

void eSTRING::_strcpyraw ( uint8_t * pbString, uint32_t dwStringLength )
{
    int iLength;

    if (this->iCharLocaleEncoding == STRING_ENCODING_UTF8)
        iLength = string_UTF8_decodeLength ( (char *)pbString, (int)dwStringLength );
    else 
        iLength = (int)dwStringLength;


    if (this->iCharactersWordSize == STRING_MODE_UNICODE)
    {
        this->Buffer_Realloc ( 
            iLength, 
            true );

        string_BYTEtoUTF32 (
            this->iCharLocaleEncoding,
            (char *)pbString,
            iLength,
            (uint32_t *)this->pCharactersBuffer );
    }
    else
    {
        this->Buffer_Replace ( 
            (unsigned char *)pbString, 
            iLength );
    }
}

void eSTRING::_strcat ( eSTRING & stringExt )
{
    int iConvertMode;
    int iOffset;

    iConvertMode  = (this->iCharactersWordSize      == STRING_MODE_UNICODE)?(1):(0);
    iConvertMode |= (stringExt.iCharactersWordSize  == STRING_MODE_UNICODE)?(2):(0);

    switch (iConvertMode)
    {
        case 0: //convert BYTE  => BYTE
        case 3: //convert UTF32 => UTF32

            this->Buffer_Concat ( 
                (unsigned char *)stringExt.pCharactersBuffer, 
                stringExt.iCharactersCount );

            break;

        case 1: //convert BYTE => UTF32

            iOffset = this->iCharactersCount;
            this->Buffer_Realloc ( 
                this->iCharactersCount + stringExt.iCharactersCount, 
                false );

            string_BYTEtoUTF32 (
                STRING_ENCODING_NONE,
                (char *)stringExt.pCharactersBuffer,
                stringExt.iCharactersCount,
                ((uint32_t *)this->pCharactersBuffer) + iOffset );
            
            break;

        case 2: //convert UTF32 => BYTE

            iOffset = this->iCharactersCount;
            this->Buffer_Realloc ( 
                this->iCharactersCount + stringExt.iCharactersCount, 
                false );

            string_UTF32toBYTE (
                STRING_ENCODING_NONE,
                (uint32_t *)stringExt.pCharactersBuffer,
                stringExt.iCharactersCount,
                ((char *)this->pCharactersBuffer)+iOffset );

            break;
    }
}

void eSTRING::_strcat ( char * pszStringExt, int iStringEncodingChar )
{
    int iOffset;
    int iLength;


    if (this->iCharactersWordSize == STRING_MODE_UNICODE)
    {
        if (iStringEncodingChar == STRING_ENCODING_UTF8)
            iLength = string_UTF8_decodeLength ( pszStringExt, (int)strlen(pszStringExt) );
        else 
            iLength = (int)strlen(pszStringExt);

        iOffset = this->iCharactersCount;


        this->Buffer_Realloc ( 
            this->iCharactersCount + iLength, 
            false );

        string_BYTEtoUTF32 (
            iStringEncodingChar,
            (char *)pszStringExt,
            iLength,
            ((uint32_t *)this->pCharactersBuffer) + iOffset );
    }
    else
    {
        this->Buffer_Concat (
            (unsigned char *)pszStringExt,
            (int)strlen(pszStringExt) );
    }
}

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <sys/utime.h>
#include <sys/stat.h>
#else
#include <unistd.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

size_t  eSTRING::fileGetContents ( char * pszFilename )
{
    FILE *      pFile;
    uint8_t *   pbBuffer;
    uint64_t    qwFileSize;
#ifdef _WIN32
    _finddata_t _FindData;
    intptr_t    itSearchHandle;

    itSearchHandle = _findfirst(pszFilename, &_FindData);
    if (itSearchHandle == -1)
#else
    struct stat statFile;

    if (stat(szBaseFileName, &statFile))
#endif
        return 0;
    else
    {
#ifdef _WIN32
        qwFileSize = (uint64_t)_FindData.size;
        _findclose(itSearchHandle);
#else
        qwFileSize = (uint64_t)statFile.st_size;
#endif
        pFile = fopen ( pszFilename, "rb" );
        if (pFile)
        {
            pbBuffer = (uint8_t *)malloc ( qwFileSize );
            fread ( pbBuffer, 1, qwFileSize, pFile );
            fclose ( pFile );

            this->iCharactersWordSize = STRING_MODE_BYTE;
            this->iCharLocaleEncoding = STRING_ENCODING_NONE;
            this->_strcpyraw ( pbBuffer, (uint32_t)qwFileSize );

            free ( pbBuffer );

            return qwFileSize;
        }

    }


    return 0;

}
