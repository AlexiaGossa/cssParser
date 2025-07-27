#include "cssparse.h"
#include <math.h>




float cssParseHUEtoRGB ( float fP, float fQ, float fT)
{
    if(fT < 0.0000000000F) fT += 1.0F;
    if(fT > 1.0000000000F) fT -= 1.0F;
    if(fT < 0.1666666667F) return fP + (fQ - fP) * 6.0F * fT;
    if(fT < 0.5000000000F) return fQ;
    if(fT < 0.6666666667F) return fP + (fQ - fP) * (0.66666667F - fT) * 6.0F;
    return fP;
}
/*
    iH = 0...360
    iS = 0...100%
    iL = 0...100%
*/
uint32_t cssParseHSLtoRGB ( int iH, int iS, int iL )
{
    int     iR, iG, iB;
    float   fH, fS, fL;
    float   fQ, fP;

    if (iS==0)
    {
        iR = (iL*653)>>8; //*2.55
        iG = iR;
        iB = iR;
    }
    else
    {
        fH = (float)iH / 360.0F;
        fS = (float)iS * 0.01F;
        fL = (float)iL * 0.01F;

        if (fL<0.5F)
            fQ = fL * (1.0F + fS);
        else
            fQ = fL + fS - ( fL * fS );

        fP = (2.0F * fL) - fQ;

        iR = (int)(0.5F + 255.0F * cssParseHUEtoRGB ( fP, fQ, fH + 0.33333333F ) );
        iG = (int)(0.5F + 255.0F * cssParseHUEtoRGB ( fP, fQ, fH ) );
        iB = (int)(0.5F + 255.0F * cssParseHUEtoRGB ( fP, fQ, fH - 0.33333333F ) );
    }

    if (iR>255) iR = 255;
    if (iR<  0) iR =   0;
    if (iG>255) iG = 255;
    if (iG<  0) iG =   0;
    if (iB>255) iB = 255;
    if (iB<  0) iB =   0;

    return (iR<<16) | (iG<<8) | iB;
}



uint32_t cssParseXtoU ( char * psz, int iCharCount, int iEnableDoubleValue )
{
    int iIndex;
    uint32_t uiValue;

    uiValue = 0;
    for (iIndex=0;iIndex<iCharCount;iIndex++)
    {
        uiValue <<= 4;
        if ( psz[iIndex]>='0' && psz[iIndex]<='9' )
            uiValue += (uint32_t)(psz[iIndex]-'0');
        else if ( psz[iIndex]>='a' && psz[iIndex]<='f' )
            uiValue += (uint32_t)(psz[iIndex]-'a'+10);
        else if ( psz[iIndex]>='A' && psz[iIndex]<='F' )
            uiValue += (uint32_t)(psz[iIndex]-'A'+10);
    }

    if (iEnableDoubleValue)
        uiValue = (uiValue<<4)+uiValue;

    return uiValue;
}