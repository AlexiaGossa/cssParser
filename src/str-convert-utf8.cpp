#include "str-convert.h"

void string_UTF8_characterDecode ( unsigned char * pbInputData, int * plInputLen, unsigned int *pulValue )
{

    if (!(*pbInputData&0x80)) // 1 byte (7-bit)
    {
        *pulValue = pbInputData[0];
        *plInputLen = 1;
    }
    else if ((*pbInputData&0xE0)==UTF8_CODEPOINT_2BYTE) // 2 byte (11-bit)
    {
        *pulValue = (pbInputData[1]&0x3F) | ((pbInputData[0]&0x1F)<<6);
        *plInputLen = 2;
    }
    else if ((*pbInputData&0xF0)==UTF8_CODEPOINT_3BYTE) // 3 byte (16-bit)
    {
        *pulValue = (pbInputData[2]&0x3F) | ((pbInputData[1]&0x3F)<<6) | ((pbInputData[0]&0x0F)<<12);
        *plInputLen = 3;
    }
    else if ((*pbInputData&0xF8)==UTF8_CODEPOINT_4BYTE) // 4 byte (21-bit)
    {
        *pulValue = (pbInputData[3]&0x3F) | ((pbInputData[2]&0x3F)<<6) | ((pbInputData[1]&0x3F)<<12) | ((pbInputData[0]&0x07)<<18);
        *plInputLen = 4;
    }
    else if ((*pbInputData&0xFC)==UTF8_CODEPOINT_5BYTE) // 5 byte (26-bit) - non-standard : FSS-UTF(1992) / UTF-8 (1993)
    {
        *pulValue = (pbInputData[4]&0x3F) | ((pbInputData[3]&0x3F)<<6) | ((pbInputData[2]&0x3F)<<12) | ((pbInputData[1]&0x3F)<<18) | ((pbInputData[0]&0x03)<<24);
        *plInputLen = 4;
    }
    else if ((*pbInputData&0xFE)==UTF8_CODEPOINT_6BYTE) // 6 byte (31-bit) - non-standard : FSS-UTF(1992) / UTF-8 (1993)
    {
        *pulValue = (pbInputData[5]&0x3F) | ((pbInputData[4]&0x3F)<<6) | ((pbInputData[3]&0x3F)<<12) | ((pbInputData[2]&0x3F)<<18) | ((pbInputData[1]&0x3F)<<24) | ((pbInputData[0]&0x01)<<30);
        *plInputLen = 4;
    }
    else //Invalid UTF8 CODEPOINT
    {
        *pulValue = 0;
        *plInputLen = 1; //Go to next byte...
    }
}


void string_UTF8_characterEncode ( unsigned int ulValue, unsigned char * pbOutputData, int * plOutputLen )
{
    ulValue &= 0x7FFFFFFF;
    
    if (ulValue<128)
    {
        pbOutputData[0] = ulValue | UTF8_CODEPOINT_1BYTE;
        *plOutputLen = 1;
    }
    else if (ulValue<2048)
    {
        pbOutputData[1] =  (ulValue     &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[0] =  (ulValue>> 6)       | UTF8_CODEPOINT_2BYTE;
        *plOutputLen = 2;
    }
    else if (ulValue<65536)
    {
        pbOutputData[2] = ( ulValue     &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[1] = ((ulValue>> 6)&0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[0] =  (ulValue>>12)       | UTF8_CODEPOINT_3BYTE;
        *plOutputLen = 3;
    }
    else if (ulValue<2097152)
    {    
        pbOutputData[3] = ( ulValue     &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[2] = ( ulValue>> 6 &0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[1] = ((ulValue>>12)&0x3F) | UTF8_CODEPOINT_NBYTE;
        pbOutputData[0] =  (ulValue>>16)       | UTF8_CODEPOINT_4BYTE;
        *plOutputLen = 4;
    }
    else *plOutputLen = 0; //Invalid UTF8 code
}

void string_UTF8_characterEncodeLength ( unsigned int ulValue, int * plOutputLen )
{
    ulValue &= 0x7FFFFFFF;
    
    if (ulValue<128)
    {
        *plOutputLen = 1;
    }
    else if (ulValue<2048)
    {
        *plOutputLen = 2;
    }
    else if (ulValue<65536)
    {
        *plOutputLen = 3;
    }
    else if (ulValue<2097152)
    {    
        *plOutputLen = 4;
    }
    else *plOutputLen = 0; //Invalid UTF8 code
}

int string_UTF8_decodeLength ( char * pszInputUTF8, int iInputByteCount )
{
    int             lIndex, lCount, lSizeUTF8;
    int             lIndexOutput;
    unsigned int    ulValue, ulLastValue;
    unsigned int    ulDiacriticalMarkValue;
    int             iDIndex;

    if (!pszInputUTF8)
        return 0;

    lCount          = iInputByteCount;
    lIndexOutput    = 0;
    lSizeUTF8       = 1;
    for (lIndex=0;lIndex<lCount;lIndex+=lSizeUTF8)
    {
        string_UTF8_characterDecode ( (unsigned char *)pszInputUTF8+lIndex, &lSizeUTF8, &ulValue );
        
        if (lSizeUTF8!=-1)
        {
            if ((ulValue>=0x0300)&&(ulValue<=0x036F))
            {
                //Diacritical mark detected !
                ulDiacriticalMarkValue = ulValue;
                for (iDIndex=0;StringDiacriticalItem[iDIndex].cPreviousCharacter!=0;iDIndex++)
                {
                    if ( (StringDiacriticalItem[iDIndex].cPreviousCharacter == ulLastValue) &&
                         (StringDiacriticalItem[iDIndex].iDiacriticalCode == ulValue) )
                    {
                        ulLastValue = 0;
                        break;
                    }
                }
            }
            else
            {
                lIndexOutput++;
                ulDiacriticalMarkValue = 0;
                ulLastValue = ulValue;
            }
        }
        else lSizeUTF8 = 1;
    }
    return lIndexOutput;
}

/*
 *  
 */
int string_UTF8_decodeToUTF32 ( char * pszInputUTF8, int iInputLength, uint32_t * pdwOutputUTF32 )
{
    int             lIndex, lCount, lSizeUTF8;
    int             lIndexOutput;
    unsigned int    ulValue, ulLastValue;
    unsigned int    ulDiacriticalMarkValue;
    int             iDIndex;

    if (!pszInputUTF8)
        return 0;
    
    lCount          = iInputLength;
    lIndexOutput    = 0;
    lSizeUTF8       = 1;
    for (lIndex=0;lIndex<lCount;lIndex+=lSizeUTF8)
    {
        string_UTF8_characterDecode ( (unsigned char *)pszInputUTF8+lIndex, &lSizeUTF8, &ulValue );
        
        if (lSizeUTF8!=-1)
        {
            if ((ulValue>=0x0300)&&(ulValue<=0x036F))
            {
                //Diacritical mark detected !
                ulDiacriticalMarkValue = ulValue;
                for (iDIndex=0;StringDiacriticalItem[iDIndex].cPreviousCharacter!=0;iDIndex++)
                {
                    if ( (StringDiacriticalItem[iDIndex].cPreviousCharacter == ulLastValue) &&
                         (StringDiacriticalItem[iDIndex].iDiacriticalCode == ulValue) )
                    {
                        if (pdwOutputUTF32)
                            pdwOutputUTF32[lIndexOutput-1] = StringDiacriticalItem[iDIndex].iLatinCode;

                        ulLastValue = 0;
                        break;
                    }
                }
            }
            else
            {
                //Transcoding
                if (pdwOutputUTF32)
                    pdwOutputUTF32[lIndexOutput] = ulValue;

                lIndexOutput++;
                ulDiacriticalMarkValue = 0;
                ulLastValue = ulValue;
            }
        }
        else lSizeUTF8 = 1;
    }
    return lIndexOutput;
}


int string_UTF8_encodeFromUTF32 ( uint32_t * pdwInputUTF32, int iInputLength, char * pszOutputUTF8, int iOutputByteCount )
{
    int iIndex, iIndexOutput;
    int iCharacterLength;

    //La sortie
    if (pszOutputUTF8)
        *pszOutputUTF8 = 0;

    //Il n'y a pas d'entrée
    if (!pdwInputUTF32)
        return 0;
    
    if (pszOutputUTF8)
    {
        for (
            iIndex=0,iIndexOutput=0;
            iIndex<iInputLength;
            iIndex++)
        {
            //Detect end of output buffer without truncating utf-8 code
            if (iOutputByteCount<=8)
            {
                string_UTF8_characterEncodeLength ( 
                    pdwInputUTF32[iIndex],
                    &iCharacterLength );

                if (iCharacterLength>iOutputByteCount)
                    break;
            }

            string_UTF8_characterEncode ( 
                pdwInputUTF32[iIndex],
                (unsigned char *)pszOutputUTF8 + iIndexOutput,
                &iCharacterLength );

            iIndexOutput        += iCharacterLength;
            iOutputByteCount    -= iCharacterLength;
        }
    }
    else
    {
        for (
            iIndex=0,iIndexOutput=0;
            iIndex<iInputLength;
            iIndex++)
        {
            string_UTF8_characterEncodeLength ( 
                pdwInputUTF32[iIndex],
                &iCharacterLength );

            iIndexOutput += iCharacterLength;
        }
    }
    return iIndexOutput;
}


