#include "string.h"
#include "string-convert.h"

int eSTRING::exportBytes ( void * pData, int iDataBytesCount, int iStringEncoding, bool bEnableStringWithZero )
{
    int iOutputBytes;

    if (iStringEncoding==-1)
        iStringEncoding = STRING_ENCODING_ISO8859_15;

    /*
        Get output length in byte
    */
    switch (this->iCharactersWordSize)
    {
        case STRING_MODE_UNICODE:
            switch (iStringEncoding)
            {
                case STRING_ENCODING_UTF32:
                    iOutputBytes = this->iCharactersCount << 2;
                    break;

                case STRING_ENCODING_UTF8:
                    iOutputBytes = string_UTF8_encodeFromUTF32 (
                        (uint32_t *)this->pCharactersBuffer,
                        this->iCharactersCount,
                        NULL );
                    break;

                default:
                    iOutputBytes = this->iCharactersCount;
                    break;
            }
            break;

        case STRING_MODE_BYTE:
            switch (iStringEncoding)
            {
                case STRING_ENCODING_UTF32:
                    iOutputBytes = this->iCharactersCount << 2;
                    break;

                default:
                    iOutputBytes = this->iCharactersCount;
                    break;

            }
            break;
    }

    /*
        Output data
    */
    if ( (iOutputBytes<iDataBytesCount) && (pData) )
    {
        switch (this->iCharactersWordSize)
        {
            case STRING_MODE_UNICODE:
                string_UTF32toBYTE ( 
                    iStringEncoding,
                    (uint32_t *)this->pCharactersBuffer,
                    this->iCharactersCount,
                    (char *)pData );
                break;

            case STRING_MODE_BYTE:
                switch (iStringEncoding)
                {
                    case STRING_ENCODING_UTF32:
                        string_BYTEtoUTF32 (
                            STRING_ENCODING_NONE,
                            (char *)this->pCharactersBuffer,
                            this->iCharactersCount,
                            (uint32_t *)pData );
                        break;

                    case STRING_ENCODING_UTF8:
                        string_BYTEtoUTF8 ( 
                            (char *)this->pCharactersBuffer,
                            this->iCharactersCount,
                            (uint8_t *)pData );
                        break;

                    default:
                        memcpy ( 
                            pData,
                            this->pCharactersBuffer,
                            iOutputBytes );
                        break;
                }
                break;
        }

        if (bEnableStringWithZero)
        {
            ((uint8_t *)pData)[iOutputBytes] = 0;
        }
    }

    if (bEnableStringWithZero) iOutputBytes += 1; 

    return iOutputBytes;
}


void eSTRING::enableUnicode ( int iStringEncodingFrom )
{
    int     iCharactersCountSource;
    int     iCharactersCountTarget;
    void *  pPreviousBuffer;

    switch (this->iCharactersWordSize)
    {
        case STRING_MODE_UNICODE:
            //Don't do anything
            break;

        case STRING_MODE_BYTE:
            if (iStringEncodingFrom == STRING_ENCODING_UTF32)
            {
                //Enable UTF-32 and force boundary of 4
                this->iCharactersCount      >>= 2;
                this->iCharactersAllocated  >>= 2;
                this->iCharactersWordSize   = STRING_MODE_UNICODE;
                this->length                = this->iCharactersCount;


                if (this->iCharactersCount)
                {
                    if ( *((uint32_t *)this->pCharactersBuffer) == 0xFFFE0000 ) //Big endian
                    {

                    }
                    else if ( *((uint32_t *)this->pCharactersBuffer) == 0x0000FEFF ) //Little endian
                    {
                        //BOM detected !
                        //Move the input buffer and remove the BOM
                        memmove (
                            (uint8_t *)this->pCharactersBuffer,
                            (uint8_t *)this->pCharactersBuffer + 4,
                            (this->iCharactersCount<<2) - 4 );

                        this->iCharactersCount -= 1;
                        this->length = this->iCharactersCount;
                    }
                }
                 
                 
            }
            else
            {
                //Use UTF-8, PageCode or NONE
                if (iStringEncodingFrom == STRING_ENCODING_UTF8)
                {
                    if (this->iCharactersCount>=3)
                    {
                        if ( ( ((uint8_t *)this->pCharactersBuffer)[0] == 0xEF ) &&
                             ( ((uint8_t *)this->pCharactersBuffer)[1] == 0xBB ) &&
                             ( ((uint8_t *)this->pCharactersBuffer)[2] == 0xBF ) )
                        {
                            //BOM detected !
                            //Move the input buffer and remove the BOM
                            memmove (
                                (uint8_t *)this->pCharactersBuffer,
                                (uint8_t *)this->pCharactersBuffer + 3,
                                this->iCharactersCount - 3 );

                            this->iCharactersCount -= 3;
                            this->length = iCharactersCount;
                        }
                    }

                    //Variable-length of UTF-8 encoding
                    iCharactersCountSource = this->iCharactersCount;
                    iCharactersCountTarget = string_UTF8_decodeLength (
                        (char *)this->pCharactersBuffer,
                        iCharactersCountSource );
                }
                else
                {
                    //Fixed-length of BYTE encoding
                    iCharactersCountSource = this->iCharactersCount;
                    iCharactersCountTarget = this->iCharactersCount;;
                }

                //Get a previous copy
                pPreviousBuffer = (void *)malloc ( iCharactersCountSource );
                memcpy ( pPreviousBuffer, this->pCharactersBuffer, iCharactersCountSource );

                //New area
                this->Buffer_NewAlloc ( iCharactersCountTarget, STRING_MODE_UNICODE );

                //Convert
                string_BYTEtoUTF32 (
                    iStringEncodingFrom,
                    (char *)pPreviousBuffer,
                    iCharactersCountSource,
                    (uint32_t *)this->pCharactersBuffer );

                Security_Whiteness ( pPreviousBuffer, iCharactersCountSource );
                free ( pPreviousBuffer );
            }
            break;
    }
}

void eSTRING::setCharLocale           ( int iStringEncodingChar )
{
    this->iCharLocaleEncoding = iStringEncodingChar;
}

//Lower / Upper / Lower no accent / Upper no accent
uint32_t string_LowercaseToUppercaseList[] = {
    0x0061, 0x0041, 'a', 'A', 
    0x0062, 0x0042, 'b', 'B',
    0x0063, 0x0043, 'c', 'C',
    0x0064, 0x0044, 'd', 'D',
    0x0065, 0x0045, 'e', 'E',
    0x0066, 0x0046, 'f', 'F',
    0x0067, 0x0047, 'g', 'G',
    0x0068, 0x0048, 'h', 'H',
    0x0069, 0x0049, 'i', 'I',
    0x006a, 0x004a, 'j', 'J',
    0x006b, 0x004b, 'k', 'K',
    0x006c, 0x004c, 'l', 'L',
    0x006d, 0x004d, 'm', 'M',
    0x006e, 0x004e, 'n', 'N',
    0x006f, 0x004f, 'o', 'O',
    0x0070, 0x0050, 'p', 'P',
    0x0071, 0x0051, 'q', 'Q',
    0x0072, 0x0052, 'r', 'R',
    0x0073, 0x0053, 's', 'S',
    0x0074, 0x0054, 't', 'T',
    0x0075, 0x0055, 'u', 'U',
    0x0076, 0x0056, 'v', 'V',
    0x0077, 0x0057, 'w', 'W',
    0x0078, 0x0058, 'x', 'X',
    0x0079, 0x0059, 'y', 'Y',
    0x007a, 0x005a, 'z', 'Z',
    0x00e0, 0x00c0, 'a', 'A',
    0x00e1, 0x00c1, 'a', 'A',
    0x00e2, 0x00c2, 'a', 'A',
    0x00e3, 0x00c3, 'a', 'A',
    0x00e4, 0x00c4, 'a', 'A',
    0x00e5, 0x00c5, 'a', 'A',
    0x00e6, 0x00c6, 0x00e6, 0x00c6,     //ae, AE
    0x00e7, 0x00c7, 'c', 'C',
    0x00e8, 0x00c8, 'e', 'E',
    0x00e9, 0x00c9, 'e', 'E',
    0x00ea, 0x00ca, 'e', 'E',
    0x00eb, 0x00cb, 'e', 'E',
    0x00ec, 0x00cc, 'i', 'I',
    0x00ed, 0x00cd, 'i', 'I',
    0x00ee, 0x00ce, 'i', 'I',
    0x00ef, 0x00cf, 'i', 'I',
    0x00f1, 0x00d1, 'n', 'N',

    0x00f2, 0x00d2, 'o', 'O',
    0x00f3, 0x00d3, 'o', 'O',
    0x00f4, 0x00d4, 'o', 'O',
    0x00f5, 0x00d5, 'o', 'O',
    0x00f6, 0x00d6, 'o', 'O',
    0x00f8, 0x00d8, 'o', 'O',

    0x00f9, 0x00d9, 'u', 'U',
    0x00fa, 0x00da, 'u', 'U',
    0x00fb, 0x00db, 'u', 'U',
    0x00fc, 0x00dc, 'u', 'U',



    0x0101, 0x0100, 'a', 'A',
    0x0103, 0x0102, 'a', 'A',
    0x0105, 0x0104, 'a', 'A',

    0x0107, 0x0106, 'c', 'C',
    0x0109, 0x0108, 'c', 'C',
    0x010b, 0x010a, 'c', 'C',
    0x010d, 0x010c, 'c', 'C',

    0x010f, 0x010e, 'd', 'D',
    0x0111, 0x0110, 'd', 'D',

    0x0113, 0x0112, 'e', 'E',
    0x0115, 0x0114, 'e', 'E',
    0x0117, 0x0116, 'e', 'E',
    0x0119, 0x0118, 'e', 'E',
    0x011b, 0x011a, 'e', 'E',

    0x011d, 0x011c, 'g', 'G',
    0x011f, 0x011e, 'g', 'G',
    0x0121, 0x0120, 'g', 'G',
    0x0123, 0x0122, 'g', 'G',
    0x0125, 0x0124, 'h', 'H',
    0x0127, 0x0126, 'h', 'H',

    0x0129, 0x0128, 'i', 'I',
    0x012b, 0x012a, 'i', 'I',
    0x012d, 0x012c, 'i', 'I',
    0x012f, 0x012e, 'i', 'I',
    0x0131, 0x0130, 'i', 'I',

    0x0133, 0x0132, 0x0133, 0x0132, //ij, IJ
    0x0135, 0x0134, 'j', 'J',
    0x0137, 0x0136, 'k', 'K',
    0x013a, 0x0139, 'l', 'L',
    0x013c, 0x013b, 'l', 'L',
    0x013e, 0x013d, 'l', 'L',
    0x0140, 0x013f, 'l', 'L',
    0x0142, 0x0141, 'l', 'L',
    0x0144, 0x0143, 'n', 'N',
    0x0146, 0x0145, 'n', 'N',
    0x0148, 0x0147, 'n', 'N',
    0x014b, 0x014a, 0x014b, 0x014a, //small Eng, capital Eng
    0x014d, 0x014c, 'o', 'O',
    0x014f, 0x014e, 'o', 'O',
    0x0151, 0x0150, 'o', 'O',
    0x0153, 0x0152, 0x0153, 0x0152, // oe, OE

    0x0155, 0x0154, 'r', 'R',
    0x0157, 0x0156, 'r', 'R',
    0x0159, 0x0158, 'r', 'R',

    0x015b, 0x015a, 's', 'S',
    0x015d, 0x015c, 's', 'S',
    0x015f, 0x015e, 's', 'S',
    0x0161, 0x0160, 's', 'S',

    0x0163, 0x0162, 't', 'T',
    0x0165, 0x0164, 't', 'T',
    0x0167, 0x0166, 't', 'T',

    0x0169, 0x0168, 'u', 'U',
    0x016b, 0x016a, 'u', 'U',
    0x016d, 0x016c, 'u', 'U',
    0x016f, 0x016e, 'u', 'U',
    0x0171, 0x0170, 'u', 'U',
    0x0173, 0x0172, 'u', 'U',

    0x0175, 0x0174, 'w', 'W',
    0x0177, 0x0176, 'y', 'Y',
    0x00ff, 0x0178, 'y', 'Y',
    0x00fd, 0x00dd, 'y', 'Y',
    0x017a, 0x0179, 'z', 'Z',
    0x017c, 0x017b, 'z', 'Z',
    0x017e, 0x017d, 'z', 'Z',
    0, 0, 0, 0,
};

eSTRING & eSTRING::toUpperCase ( bool bRemoveAccent )
{
    int iIndex;
    int iIndexMap, iIndexMapOffset;
    uint32_t uiCharPrev;
    uint32_t uiCharNext;

    iIndexMapOffset = (bRemoveAccent)?(3):(1);

    if (this->iCharactersWordSize == STRING_MODE_UNICODE)
    {
        for (iIndex=0;iIndex<this->iCharactersCount;iIndex++)
        {
            uiCharPrev = ((uint32_t *)this->pCharactersBuffer)[iIndex];
            uiCharNext = uiCharPrev;

            //lower to upper
            for (iIndexMap=0;string_LowercaseToUppercaseList[iIndexMap];iIndexMap+=4)
            {
                if (uiCharPrev == string_LowercaseToUppercaseList[iIndexMap])
                {
                    uiCharNext = string_LowercaseToUppercaseList[iIndexMap+iIndexMapOffset];
                    break;
                }
            }

            //upper without accent
            if (bRemoveAccent)
            {
                for (iIndexMap=0;string_LowercaseToUppercaseList[iIndexMap];iIndexMap+=4)
                {
                    if (uiCharPrev == string_LowercaseToUppercaseList[iIndexMap+1])
                    {
                        uiCharNext = string_LowercaseToUppercaseList[iIndexMap+3];
                        break;
                    }
                }
            
            }

            //Change the character
            if (uiCharPrev != uiCharNext)
                ((uint32_t *)this->pCharactersBuffer)[iIndex] = uiCharNext;
        }
    }
    return *this;
}
eSTRING & eSTRING::toLowerCase ( bool bRemoveAccent )
{
    int iIndex;
    int iIndexMap, iIndexMapOffset;
    uint32_t uiCharPrev;
    uint32_t uiCharNext;

    iIndexMapOffset = (bRemoveAccent)?(2):(0);

    if (this->iCharactersWordSize == STRING_MODE_UNICODE)
    {
        for (iIndex=0;iIndex<this->iCharactersCount;iIndex++)
        {
            uiCharPrev = ((uint32_t *)this->pCharactersBuffer)[iIndex];
            uiCharNext = uiCharPrev;

            //upper to lower
            for (iIndexMap=0;string_LowercaseToUppercaseList[iIndexMap];iIndexMap+=4)
            {
                if (uiCharPrev == string_LowercaseToUppercaseList[iIndexMap+1])
                {
                    uiCharNext = string_LowercaseToUppercaseList[iIndexMap+iIndexMapOffset];
                    break;
                }
            }

            //lower without accent
            if (bRemoveAccent)
            {
                for (iIndexMap=0;string_LowercaseToUppercaseList[iIndexMap];iIndexMap+=4)
                {
                    if (uiCharPrev == string_LowercaseToUppercaseList[iIndexMap])
                    {
                        uiCharNext = string_LowercaseToUppercaseList[iIndexMap+2];
                        break;
                    }
                }
            
            }

            //Change the character
            if (uiCharPrev != uiCharNext)
                ((uint32_t *)this->pCharactersBuffer)[iIndex] = uiCharNext;
        }
    }
    return *this;
}



