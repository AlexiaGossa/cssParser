#include "string-convert.h"


int string_BYTEextentUTF32 ( uint8_t * pbInputBYTE, int iInputBYTE, uint32_t * pdwOutputUTF32 )
{
    int iIndex;

    for (iIndex=0;iIndex<iInputBYTE;iIndex++)
    {
        *pdwOutputUTF32 = (uint32_t)(*pbInputBYTE);
        pdwOutputUTF32++;
        pbInputBYTE++;
    }
    return iInputBYTE;
}

int string_UTF32stretchBYTE ( uint32_t * pdwInputUTF32, int iInputUTF32, uint8_t * pbOutputBYTE )
{
    int iIndex;

    for (iIndex=0;iIndex<iInputUTF32;iIndex++)
    {
        if ((*pdwInputUTF32)>255) *pbOutputBYTE = 32;
        else *pbOutputBYTE = (uint8_t)(*pdwInputUTF32);

        pbOutputBYTE++;
        pdwInputUTF32++;
    }
    return iInputUTF32;
}

int string_BYTEtoUTF8 ( char * pszInputDataRAW, int iInputLengthRAW, uint8_t * pbOutputBYTE )
{
    int iIndex;

    for (iIndex=0;iIndex<iInputLengthRAW;iIndex++)
    {
        if ((*pszInputDataRAW)>127) *pbOutputBYTE = 127;
        else *pbOutputBYTE = (uint8_t)(*pszInputDataRAW);

        pbOutputBYTE++;
        pszInputDataRAW++;
    }
    return iInputLengthRAW;
}


int string_BYTEtoUTF32 ( int iSourceCodePage, char * pszInputDataRAW, int iInputLengthRAW, uint32_t * pdwOutputDataUTF32 )
{
    switch (iSourceCodePage)
    {
        case STRING_ENCODING_UTF8:
            return string_UTF8_decodeToUTF32 (
                pszInputDataRAW,
                iInputLengthRAW,
                pdwOutputDataUTF32 );
        
        case STRING_ENCODING_NONE:
            return string_BYTEextentUTF32 ( 
                (uint8_t *)pszInputDataRAW,
                iInputLengthRAW, 
                pdwOutputDataUTF32 );

        default:
            return string_PageCode_decodeToUTF32 (
                iSourceCodePage,
                pszInputDataRAW,
                iInputLengthRAW,
                pdwOutputDataUTF32 );
    }
}



int string_UTF32toBYTE ( int iTargetCodePage, uint32_t * pdwInputDataUTF32, int iInputLengthUTF32, char * pszOutputDataRAW )
{
    switch (iTargetCodePage)
    {
        case STRING_ENCODING_UTF8:
            return string_UTF8_encodeFromUTF32 (
                pdwInputDataUTF32,
                iInputLengthUTF32,
                pszOutputDataRAW );

        case STRING_ENCODING_NONE:
            return string_UTF32stretchBYTE (
                pdwInputDataUTF32,
                iInputLengthUTF32,
                (uint8_t *)pszOutputDataRAW ); 
            

        default:
            return string_PageCode_encodeFromUTF32 (
                iTargetCodePage,
                pdwInputDataUTF32,
                iInputLengthUTF32,
                pszOutputDataRAW );
    }
}



