#include "str-convert.h"


unsigned int * string_CodePage_GetTable ( int iCodePage )
{
    int iIndex;

    for (iIndex=0;StringCodePage[iIndex].pCodePageTable128;iIndex++)
    {
        if (StringCodePage[iIndex].iCodePage == iCodePage)
            return StringCodePage[iIndex].pCodePageTable128;
    }

    return NULL;
}


int string_PageCode_decodeToUTF32 ( int iPageCode, char * pszInput, int iInputLength, uint32_t * pdwOutputUTF32 )
{
    int             iIndex;
    unsigned int *  puiCodePageTable128;
    uint32_t *      pdwOutput;
    uint8_t *       pbInput;

    if (!pdwOutputUTF32)
        return iInputLength;

    puiCodePageTable128 = string_CodePage_GetTable ( iPageCode );
    pbInput             = (uint8_t *)pszInput;
    pdwOutput           = (uint32_t *)pdwOutputUTF32;

    for (iIndex=0;iIndex<iInputLength;iIndex++)
    {
        if ((*pbInput)&0x80)
            *pdwOutput = puiCodePageTable128[(*pbInput)&0x7F];
        else
            *pdwOutput = *pbInput;

        pbInput++;
        pdwOutput++;
    }

    return iInputLength;
}

int string_PageCode_encodeFromUTF32 ( int iPageCode, uint32_t * pdwInputUTF32, int iInputLength, char * pszOutput )
{
    int             iIndex, iIndexCharacter, iIndexFound;
    unsigned int *  puiCodePageTable128;
    uint32_t *      pdwInput;
    uint8_t *       pbOutput;

    if (!pszOutput)
        return iInputLength;
    
    puiCodePageTable128 = string_CodePage_GetTable ( iPageCode );
    pdwInput            = (uint32_t *)pdwInputUTF32;
    pbOutput            = (uint8_t *)pszOutput;
    
    for (iIndex=0;iIndex<iInputLength;iIndex++)
    {
        if ((*pdwInput)<128)
            *pbOutput = (uint8_t)(*pdwInput);
        else
        {
            iIndexFound = 32;
            for (iIndexCharacter=0;iIndexCharacter<128;iIndexCharacter++)
            {
                if (puiCodePageTable128[iIndexCharacter]==*pdwInput)
                {
                    iIndexFound = iIndexCharacter+128;
                    break;
                }
            }
            *pbOutput = iIndexFound;
        }

        pdwInput++;
        pbOutput++;
    }

    *pbOutput = 0;

    return iInputLength;
}

int string_PageCode_to_PageCode ( char * pcTarget, int iTargetPageCode, char * pcSource, int iSourcePageCode, int iCharLength )
{
    uint8_t *       pbSource;
    uint8_t *       pbTarget;
    int             iIndex, iIndexCharacter, iIndexFound;
    uint32_t        ui32Char;
    unsigned int *  puiCodePageTableSource128;
    unsigned int *  puiCodePageTableTarget128;

    puiCodePageTableSource128 = string_CodePage_GetTable ( iSourcePageCode );
    puiCodePageTableTarget128 = string_CodePage_GetTable ( iTargetPageCode );

    pbSource = (uint8_t *)pcSource;
    pbTarget = (uint8_t *)pcTarget;

    for (iIndex=0;iIndex<iCharLength;iIndex++)
    {
        if ( (*pbSource) < 128 )
        {
            *pbTarget = *pbSource;
        }
        else
        {
            ui32Char = puiCodePageTableSource128[(*pbSource)&0x7F];
            iIndexFound = 32;
            for (iIndexCharacter=0;iIndexCharacter<128;iIndexCharacter++)
            {
                if (puiCodePageTableTarget128[iIndexCharacter]==ui32Char)
                {
                    iIndexFound = iIndexCharacter+128;
                    break;
                }
            }
            *pbTarget = iIndexFound;
        }

        pbSource++;
        pbTarget++;
    }
    return 0;
}