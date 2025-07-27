#include "strz.h"

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

size_t  strzFileGetContents ( strz * pstrz, char * pszFilename )
{
    FILE *      pFile;
    uint8_t *   pbBuffer;
    uint64_t    qwFileSize;
#ifdef _WIN32
    _finddata_t _FindData;
    intptr_t    itSearchHandle;
    uint64_t    qwSkeepBytesCount;

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
            pbBuffer = (uint8_t *)malloc ( (size_t)qwFileSize );
            fread ( pbBuffer, 1, (size_t)qwFileSize, pFile );
            fclose ( pFile );

            //this->iCharactersWordSize = STRING_MODE_BYTE;
            //this->iCharLocaleEncoding = STRING_ENCODING_NONE;
            //this->_strcpyraw ( pbBuffer, (uint32_t)qwFileSize );

            qwSkeepBytesCount = 0;

            if (qwFileSize>3)
            {
                if ( (pbBuffer[0]==0xEF) && (pbBuffer[1]==0xBB) && (pbBuffer[2]==0xBF) )
                    qwSkeepBytesCount = 3;
            }

            strzCopy ( pstrz, (char *)(pbBuffer+qwSkeepBytesCount), (int)(qwFileSize-qwSkeepBytesCount) );

            free ( pbBuffer );

            return (size_t)qwFileSize;
        }

    }


    return 0;

}

#define strzBOM_UNKNOW      0x00
#define strzBOM_UTF8        0x01
#define strzBOM_UTF32_BE    0x02
#define strzBOM_UTF32_LE    0x03
#define strzBOM_UTF16_BE    0x04
#define strzBOM_UTF16_LE    0x06

int strzDetectBOM ( void * pData, int * piTypeBOM )
{
    uint8_t * pbData;

    pbData = (uint8_t *)pData;

    /*
     *  UTF-8
     */

    if ( (pbData[0] == 0xEF) && (pbData[1] == 0xBB) && (pbData[2] == 0xBF) )
    {
        *piTypeBOM = strzBOM_UTF8;
        return 3;
    }

    /*
     *  UTF-32
     */

    if ( (pbData[0] == 0x00) && (pbData[1] == 0x00) && (pbData[2] == 0xFE) && (pbData[3] == 0xFF) )
    {
        *piTypeBOM = strzBOM_UTF32_BE;
        return 4;
    }

    if ( (pbData[0] == 0xFF) && (pbData[1] == 0xFE) && (pbData[2] == 0x00) && (pbData[3] == 0x00) )
    {
        *piTypeBOM = strzBOM_UTF32_LE;
        return 4;
    }

    /*
     *  UTF-16
     */

    if ( (pbData[0] == 0xFE) && (pbData[1] == 0xFF) )
    {
        *piTypeBOM = strzBOM_UTF16_BE;
        return 2;
    }

    if ( (pbData[0] == 0xFF) && (pbData[1] == 0xFE) )
    {
        *piTypeBOM = strzBOM_UTF16_LE;
        return 2;
    }


    *piTypeBOM = strzBOM_UNKNOW;
    return 0;
}
