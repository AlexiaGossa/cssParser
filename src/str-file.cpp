#include "str.h"

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

#define strBOM_UNKNOW      0x00
#define strBOM_UTF8        0x01
#define strBOM_UTF32_BE    0x02
#define strBOM_UTF32_LE    0x03
#define strBOM_UTF16_BE    0x04
#define strBOM_UTF16_LE    0x06

int strDetectBOM ( void * pData, int * piTypeBOM );

uint32_t strEndianReorder32 ( uint32_t ui32Value );
uint16_t strEndianReorder16 ( uint16_t ui16Value );


int         strNewMemoryGetContents         ( str * pstr, void * pSource, int iSourceByteCount )
{
    int         iBOMSize;
    int         iBOMType;
    char *      pbBuffer;

    pbBuffer = (char *)pSource;


    if (iSourceByteCount==0)
    {
        strNew ( 
            pstr, 
            STRING_ENCODING_SIMPLEBYTE );
        return 0;
    }

    //Autodetect BOM
    iBOMSize = 0;
    iBOMType = strBOM_UNKNOW;
    if (iSourceByteCount>4)
        iBOMSize = strDetectBOM ( pbBuffer, &iBOMType );

    switch (iBOMType)
    {
        case strBOM_UNKNOW:

            //Alloc str
            strNew ( 
                pstr, 
                STRING_ENCODING_SIMPLEBYTE );

            strCopy ( 
                pstr, 
                STRING_ENCODING_SIMPLEBYTE, 
                (char *)(pbBuffer+iBOMSize),
                (int)(iSourceByteCount-iBOMSize) );

            break;

        //Convert UTF-32 BigEndian to LittleEndian
        case strBOM_UTF32_BE:
            int iIndex, iCount;
            uint32_t * pdwBuffer;
            iCount = (int)((iSourceByteCount-iBOMSize)>>2);
            pdwBuffer = (uint32_t *)(pbBuffer+iBOMSize);
            for (iIndex=0;iIndex<iCount;iIndex++)
            {
                *pdwBuffer = strEndianReorder32 ( *pdwBuffer );
                pdwBuffer++;
            }
            //And go to LittleEndian processing
        case strBOM_UTF32_LE:
            //Alloc str
            strNew ( 
                pstr, 
                STRING_ENCODING_UTF32 );

            strCopy ( 
                pstr, 
                STRING_ENCODING_UTF32, 
                (char *)(pbBuffer+iBOMSize),
                (int)(iSourceByteCount-iBOMSize) );

            break;

        case strBOM_UTF16_BE:
        case strBOM_UTF16_LE:
            free ( pbBuffer );
            return -1;

        case strBOM_UTF8:
            //Alloc str
            strNew ( 
                pstr, 
                STRING_ENCODING_UTF32 );

            strCopy ( 
                pstr, 
                STRING_ENCODING_UTF8, 
                (char *)(pbBuffer+iBOMSize),
                (int)(iSourceByteCount-iBOMSize) );

            break;

    }

    return strLength(pstr);
}

/*
* Return characters count read (not byte)
* -1 if error or file size read
*/
int      strNewFileGetContents ( str * pstr, char * pszFilename )
{
    FILE *      pFile;
    uint8_t *   pbBuffer;
    uint64_t    qwFileSize;
    int         iReturnValue;

#ifdef _WIN32
    _finddata_t _FindData;
    intptr_t    itSearchHandle;
    itSearchHandle = _findfirst(pszFilename, &_FindData);
    if (itSearchHandle == -1)
#else
    struct stat statFile;

    if (stat(szBaseFileName, &statFile))
#endif
        return -1;


    //Get file size
#ifdef _WIN32
    qwFileSize = (uint64_t)_FindData.size;
    _findclose(itSearchHandle);
#else
    qwFileSize = (uint64_t)statFile.st_size;
#endif

    if (qwFileSize==0)
    {
        strNew ( 
            pstr, 
            STRING_ENCODING_SIMPLEBYTE );
        return 0;
    }

    //Open file
    pFile = fopen ( pszFilename, "rb" );
    if (pFile)
    {
        //Load file
        pbBuffer = (uint8_t *)malloc ( (size_t)qwFileSize );
        fread ( pbBuffer, 1, (size_t)qwFileSize, pFile );
        fclose ( pFile );


        iReturnValue = strNewMemoryGetContents ( 
            pstr,
            pbBuffer,
            (int)qwFileSize );

        free ( pbBuffer );

        return iReturnValue;
    }

    return -1;

}

uint32_t strEndianReorder32 ( uint32_t ui32Value )
{
    //Fast butterfly method like FFT (like this https://forum.hardware.fr/hfr/Programmation/ASM/comment-inverser-octet-sujet_57169_1.htm#t842562)
    ui32Value = ((ui32Value<<8) & 0xFF00FF00) | ((ui32Value>>8) & 0x00FF00FF );       // ABCD => B0D0 | 0A0C = BADC
    return (ui32Value<<16) | (ui32Value>>16);                                         // BADC => DC00 | 00BA = DCBA
}

uint16_t strEndianReorder16 ( uint16_t ui16Value )
{
    return (ui16Value<<8) | (ui16Value>>8);
}




/*
 *  Also return the BOM size   
 */
int strDetectBOM ( void * pData, int * piTypeBOM )
{
    uint8_t * pbData;

    pbData = (uint8_t *)pData;

    /*
     *  UTF-8
     */

    if ( (pbData[0] == 0xEF) && (pbData[1] == 0xBB) && (pbData[2] == 0xBF) )
    {
        *piTypeBOM = strBOM_UTF8;
        return 3;
    }

    /*
     *  UTF-32
     */

    if ( (pbData[0] == 0x00) && (pbData[1] == 0x00) && (pbData[2] == 0xFE) && (pbData[3] == 0xFF) )
    {
        *piTypeBOM = strBOM_UTF32_BE;
        return 4;
    }

    if ( (pbData[0] == 0xFF) && (pbData[1] == 0xFE) && (pbData[2] == 0x00) && (pbData[3] == 0x00) )
    {
        *piTypeBOM = strBOM_UTF32_LE;
        return 4;
    }

    /*
     *  UTF-16
     */

    if ( (pbData[0] == 0xFE) && (pbData[1] == 0xFF) )
    {
        *piTypeBOM = strBOM_UTF16_BE;
        return 2;
    }

    if ( (pbData[0] == 0xFF) && (pbData[1] == 0xFE) )
    {
        *piTypeBOM = strBOM_UTF16_LE;
        return 2;
    }


    *piTypeBOM = strBOM_UNKNOW;
    return 0;
}
