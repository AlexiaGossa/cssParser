#include "cssparse.h"
#include <math.h>



//All CSS token/string
typedef struct 
{
    token_t         iToken;
    const char *    psz;

    int             iMinimalValueCount;
    int             iMaximalValueCount;
    token_t         iParametersToken[16];

    
} lexer_t;

lexer_t lexTable[]  = {
    { _cssTYPE_STRING,                      "",                                 0, 0, { _cssEOL } },
    { _cssTYPE_VALUE,                       "",                                 0, 0, { _cssEOL } },
    { _cssTYPE_COLORHEX,                    "#",                                0, 0, { _cssEOL } },
    { _cssPX,                               "px",                               0, 0, { _cssEOL } },
    { _cssPERCENT,                          "%",                                0, 0, { _cssEOL } },
    { _cssVMIN,                             "vmin",                             0, 0, { _cssEOL } },
    { _cssVMAX,                             "vmax",                             0, 0, { _cssEOL } },
    { _cssVW,                               "vw",                               0, 0, { _cssEOL } },
    { _cssVH,                               "vh",                               0, 0, { _cssEOL } },
    { _cssRGB,                              "rgb",                              0, 0, { _cssEOL } },
    { _cssRGBA,                             "rgba",                             0, 0, { _cssEOL } },
    { _cssHSL,                              "hsl",                              0, 0, { _cssEOL } },
    { _cssHSLA,                             "hsla",                             0, 0, { _cssEOL } },
    { _cssINHERIT,                          "inherit",                          0, 0, { _cssEOL } },
    { _cssINITIAL,                          "initial",                          0, 0, { _cssEOL } },
    { _cssDEFAULT,                          "default",                          0, 0, { _cssEOL } },
    { _cssUNSET,                            "unset",                            0, 0, { _cssEOL } },

    { _cssNONE,                             "none",                             0, 0, { _cssEOL } },
    { _cssINLINE,                           "inline",                           0, 0, { _cssEOL } },
    { _cssINLINE_BLOCK,                     "inline-block",                     0, 0, { _cssEOL } },
    { _cssTABLE,                            "table",                            0, 0, { _cssEOL } },
    { _cssTABLE_CELL,                       "table-cell",                       0, 0, { _cssEOL } },
    { _cssBLOCK,                            "block",                            0, 0, { _cssEOL } },
    { _cssSTATIC,                           "static",                           0, 0, { _cssEOL } },
    { _cssABSOLUTE,                         "absolute",                         0, 0, { _cssEOL } },
    { _cssRELATIVE,                         "relative",                         0, 0, { _cssEOL } },
    { _cssFIXED,                            "fixed",                            0, 0, { _cssEOL } },

    { _cssVISIBLE,                          "visible",                          0, 0, { _cssEOL } },
    { _cssHIDDEN,                           "hidden",                           0, 0, { _cssEOL } },

    { _cssREPEAT,                           "repeat",                           0, 0, { _cssEOL } },
    { _cssSPACE,                            "space",                            0, 0, { _cssEOL } },
    { _cssNO_REPEAT,                        "no-repeat",                        0, 0, { _cssEOL } },
    { _cssROUND,                            "round",                            0, 0, { _cssEOL } },

    { _cssCOVER,                            "cover",                            0, 0, { _cssEOL } },
    { _cssCONTAIN,                          "contain",                          0, 0, { _cssEOL } },
    { _cssAUTO,                             "auto",                             0, 0, { _cssEOL } },

    { _cssMIDDLE,                           "middle",                           0, 0, { _cssEOL } },
    { _cssNORMAL,                           "normal",                           0, 0, { _cssEOL } },
    { _cssANYWHERE,                         "anywhere",                         0, 0, { _cssEOL } },
    { _cssBREAK_WORD,                       "break-word",                       0, 0, { _cssEOL } },
    { _cssCENTER,                           "center",                           0, 0, { _cssEOL } },
    { _cssJUSTIFY,                          "justify",                          0, 0, { _cssEOL } },

    { _cssBORDER_BOX,                       "border-box",                       0, 0, { _cssEOL } },
    { _cssPADDING_BOX,                      "padding-box",                      0, 0, { _cssEOL } },
    { _cssCONTENT_BOX,                      "content-box",                      0, 0, { _cssEOL } },




    { _cssLEFT,                             "left",                             1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssRIGHT,                            "right",                            1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssTOP,                              "top",                              1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBOTTOM,                           "bottom",                           1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssWIDTH,                            "width",                            1, 2, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssHEIGHT,                           "height",                           1, 2, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMIN_WIDTH,                        "min-width",                        1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMAX_WIDTH,                        "max-width",                        1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMIN_HEIGHT,                       "min-height",                       1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMAX_HEIGHT,                       "max-height",                       1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssPADDING,                          "padding",                          1, 4, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssPADDING_LEFT,                     "padding-left",                     1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssPADDING_RIGHT,                    "padding-right",                    1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssPADDING_TOP,                      "padding-top",                      1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssPADDING_BOTTOM,                   "padding-bottom",                   1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMARGIN,                           "margin",                           1, 4, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMARGIN_LEFT,                      "margin-left",                      1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMARGIN_RIGHT,                     "margin-right",                     1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMARGIN_TOP,                       "margin-top",                       1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssMARGIN_BOTTOM,                    "margin-bottom",                    1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER,                           "border",                           1, 3, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssTYPE_COLORHEX, _cssRGB, _cssRGBA, _cssHSL, _cssHSLA, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_LEFT_COLOR,                "border-left-color",                1, 1, { _cssTYPE_COLORHEX, _cssRGB, _cssRGBA, _cssHSL, _cssHSLA, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_LEFT_WIDTH,                "border-left-width",                1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_RIGHT_COLOR,               "border-right-color",               1, 1, { _cssTYPE_COLORHEX, _cssRGB, _cssRGBA, _cssHSL, _cssHSLA, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_RIGHT_WIDTH,               "border-right-width",               1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_TOP_COLOR,                 "border-top-color",                 1, 1, { _cssTYPE_COLORHEX, _cssRGB, _cssRGBA, _cssHSL, _cssHSLA, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_TOP_WIDTH,                 "border-top-width",                 1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_BOTTOM_COLOR,              "border-bottom-color",              1, 1, { _cssTYPE_COLORHEX, _cssRGB, _cssRGBA, _cssHSL, _cssHSLA, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_BOTTOM_WIDTH,              "border-bottom-width",              1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_RADIUS,                    "border-radius",                    1, 4, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_TOP_LEFT_RADIUS,           "border-top-left-radius",           1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_TOP_RIGHT_RADIUS,          "border-top-right-radius",          1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_BOTTOM_LEFT_RADIUS,        "border-bottom-left-radius",        1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_BOTTOM_RIGHT_RADIUS,       "border-bottom-right-radius",       1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssVISIBILITY,                       "visibility",                       1, 1, { _cssVISIBLE, _cssHIDDEN, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssPOSITION,                         "position",                         1, 1, { _cssSTATIC, _cssABSOLUTE, _cssRELATIVE, _cssFIXED, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssDISPLAY,                          "display",                          1, 1, { _cssNONE, _cssINLINE, _cssINLINE_BLOCK, _cssBLOCK, _cssTABLE, _cssTABLE_CELL, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssCOLOR,                            "color",                            1, 1, { _cssTYPE_COLORHEX, _cssRGB, _cssRGBA, _cssHSL, _cssHSLA, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssOPACITY,                          "opacity",                          1, 1, { _cssTYPE_VALUE, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_COLOR,                 "background-color",                 1, 1, { _cssTYPE_COLORHEX, _cssRGB, _cssRGBA, _cssHSL, _cssHSLA, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_ORIGIN,                "background-origin",                1, 1, { _cssBORDER_BOX, _cssPADDING_BOX, _cssCONTENT_BOX, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_POSITION_X,            "background-position-x",            1, 1, { _cssLEFT, _cssRIGHT, _cssCENTER, _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_POSITION_Y,            "background-position-y",            1, 1, { _cssTOP, _cssBOTTOM, _cssCENTER, _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_REPEAT_X,              "background-repeat-x",              1, 1, { _cssREPEAT, _cssNO_REPEAT, _cssSPACE, _cssROUND, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_REPEAT_Y,              "background-repeat-y",              1, 1, { _cssREPEAT, _cssNO_REPEAT, _cssSPACE, _cssROUND, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_SIZE_X,                "background-size-x",                1, 1, { _cssAUTO, _cssCOVER, _cssCONTAIN, _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_SIZE_Y,                "background-size-y",                1, 1, { _cssAUTO, _cssCOVER, _cssCONTAIN, _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },

    { _cssFONT_SIZE,                        "font-size",                        1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssFONT_WEIGHT,                      "font-weight",                      1, 1, { _cssTYPE_VALUE, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssFONT_FAMILY,                      "font-family",                      1, 8, { _cssTYPE_STRING, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssLINE_HEIGHT,                      "line-height",                      1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssTEXT_ALIGN,                       "text-align",                       1, 1, { _cssLEFT, _cssRIGHT, _cssCENTER, _cssJUSTIFY, _cssVW, _cssVH, _cssMIDDLE, _cssTOP, _cssBOTTOM, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssVERTICAL_ALIGN,                   "vertical-align",                   1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssMIDDLE, _cssTOP, _cssBOTTOM, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssOVERFLOW,                         "overflow",                         1, 1, { _cssVISIBLE, _cssHIDDEN, _cssAUTO, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssOVERFLOW_WRAP,                    "overflow-wrap",                    1, 1, { _cssNORMAL, _cssANYWHERE, _cssBREAK_WORD, _cssINHERIT, _cssINITIAL, _cssUNSET } },

    { _cssZINDEX,                           "z-index",                          1, 1, { _cssTYPE_VALUE } },


    { _cssEOL, "" },





};


token_t cssParseSelectorGetLexer ( char * pszText )
{
    int iIndex;

    for (iIndex=0;lexTable[iIndex].iToken!=_cssEOL;iIndex++)
    {
        if (stricmp(pszText,lexTable[iIndex].psz)==0)
        {
            return lexTable[iIndex].iToken;
        }
    }
    return _cssEOL;
}

char * cssParseSelectorFromToken ( int iToken )
{
    int iIndex;

    for (iIndex=0;lexTable[iIndex].iToken!=_cssEOL;iIndex++)
    {
        if (lexTable[iIndex].iToken == iToken)
            return (char *)lexTable[iIndex].psz;
    }
    return NULL;
}

zRETURN cssParsePropertyValue ( cssPROPERTY * pProperty, cssNODE * nodeValue, bool bStringOnly )
{
    char szValue[256], szNumber[64];
    char szNumberTable[]="0123456789.-";
    int iNumberStart, iNumberStop, iAsciiIndex, iIndex, iCount;
    uint8_t uiComponent[4];
    uint32_t ui32Component[4];

    token_t tokenPostfix, tokenValue;
    
    strExport ( &(nodeValue->strText), szValue, 256, true );
    iCount          = strlen(szValue);

    pProperty->value.bInteger   = false;
    pProperty->value.bFloat     = false;
    pProperty->value.bString    = false;

    if (bStringOnly)
    {
        pProperty->value.bString    = true;
        pProperty->value.pszValue   = (char *)cssParseMalloc ( strlen(szValue)+1 );
        strcpy ( pProperty->value.pszValue, szValue );
    }
    else if (szValue[0] == '#') //First step : detect a color hexa
    {
        switch (iCount)
        {
            case 4: //3-value RGB (auto convert to 6-value RGB)
                uiComponent[3] = 0xFF;
                uiComponent[2] = cssParseXtoU ( szValue+1, 1, 1 );
                uiComponent[1] = cssParseXtoU ( szValue+2, 1, 1 );
                uiComponent[0] = cssParseXtoU ( szValue+3, 1, 1 );

                //Store value
                pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                pProperty->value.bInteger   = true;
                pProperty->value.iValueType = _cssRGB;

                return 0;

            case 5: //4-value RGBA (auto-convert to 8-value RGBA)
                uiComponent[3] = cssParseXtoU ( szValue+4, 1, 1 );
                uiComponent[2] = cssParseXtoU ( szValue+1, 1, 1 );
                uiComponent[1] = cssParseXtoU ( szValue+2, 1, 1 );
                uiComponent[0] = cssParseXtoU ( szValue+3, 1, 1 );

                //Store value
                pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                pProperty->value.bInteger   = true;
                pProperty->value.iValueType = _cssRGBA;

                return 0;

            case 7: //6-value RGB
                uiComponent[3] = 0xFF;
                uiComponent[2] = cssParseXtoU ( szValue+1, 2, 0 );
                uiComponent[1] = cssParseXtoU ( szValue+3, 2, 0 );
                uiComponent[0] = cssParseXtoU ( szValue+5, 2, 0 );

                //Store value
                pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                pProperty->value.bInteger   = true;
                pProperty->value.iValueType = _cssRGB;

                return 0;
                
            case 9: //8-value RGBA
                uiComponent[3] = cssParseXtoU ( szValue+7, 2, 0 );
                uiComponent[2] = cssParseXtoU ( szValue+1, 2, 0 );
                uiComponent[1] = cssParseXtoU ( szValue+3, 2, 0 );
                uiComponent[0] = cssParseXtoU ( szValue+5, 2, 0 );

                //Store value
                pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                pProperty->value.bInteger   = true;
                pProperty->value.iValueType = _cssRGBA;

                return 0;

            default:
                return -1;
        }
    }
    else
    {
        //Try to detect a lexer value
        tokenValue = cssParseSelectorGetLexer ( szValue );//+ iAsciiIndex );
        switch (tokenValue)
        {
            case _cssRGB:
                if (nodeValue->getChildCount()==3)
                {
                    //R-component
                    strExport ( &(nodeValue->getChildIndex(0)->strText), szValue, 256, true );
                    uiComponent[2] = atoi ( szValue );

                    //G-component
                    strExport ( &(nodeValue->getChildIndex(1)->strText), szValue, 256, true );
                    uiComponent[1] = atoi ( szValue );

                    //B-component
                    strExport ( &(nodeValue->getChildIndex(2)->strText), szValue, 256, true );
                    uiComponent[0] = atoi ( szValue );

                    //Alpha
                    uiComponent[3] = 0xFF;

                    //Store value
                    pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                    pProperty->value.bInteger   = true;
                    pProperty->value.iValueType = _cssRGB;

                    return 0;
                }
                break;

            case _cssRGBA:
                if (nodeValue->getChildCount()==4)
                {
                    //R-component
                    strExport ( &(nodeValue->getChildIndex(0)->strText), szValue, 256, true );
                    uiComponent[2] = atoi ( szValue );

                    //G-component
                    strExport ( &(nodeValue->getChildIndex(1)->strText), szValue, 256, true );
                    uiComponent[1] = atoi ( szValue );

                    //B-component
                    strExport ( &(nodeValue->getChildIndex(2)->strText), szValue, 256, true );
                    uiComponent[0] = atoi ( szValue );

                    //Alpha
                    strExport ( &(nodeValue->getChildIndex(3)->strText), szValue, 256, true );
                    uiComponent[3] = (uint8_t)floor ( (atof(szValue)*255.0F) + 0.5F);

                    //Store value
                    pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                    pProperty->value.bInteger   = true;
                    pProperty->value.iValueType = _cssRGBA;

                    return 0;
                }
                break;

            case _cssHSL:
                if (nodeValue->getChildCount()==3)
                {
                    //H-component
                    strExport ( &(nodeValue->getChildIndex(0)->strText), szValue, 256, true );
                    ui32Component[2] = atoi ( szValue );

                    //S-component
                    strExport ( &(nodeValue->getChildIndex(1)->strText), szValue, 256, true );
                    ui32Component[1] = atoi ( szValue );

                    //L-component
                    strExport ( &(nodeValue->getChildIndex(2)->strText), szValue, 256, true );
                    ui32Component[0] = atoi ( szValue );

                    //Store value
                    pProperty->value.uiValue    = 0xFF000000 | cssParseHSLtoRGB ( ui32Component[2], ui32Component[1], ui32Component[0] );
                    pProperty->value.bInteger   = true;
                    pProperty->value.iValueType = _cssRGB;

                    return 0;
                }
                break;
            case _cssHSLA:
                if (nodeValue->getChildCount()==4)
                {
                    //H-component
                    strExport ( &(nodeValue->getChildIndex(0)->strText), szValue, 256, true );
                    ui32Component[2] = atoi ( szValue );

                    //S-component
                    strExport ( &(nodeValue->getChildIndex(1)->strText), szValue, 256, true );
                    ui32Component[1] = atoi ( szValue );

                    //L-component
                    strExport ( &(nodeValue->getChildIndex(2)->strText), szValue, 256, true );
                    ui32Component[0] = atoi ( szValue );

                    //Alpha
                    strExport ( &(nodeValue->getChildIndex(3)->strText), szValue, 256, true );
                    ui32Component[3] = (uint32_t)floor ( (atof(szValue)*255.0F) + 0.5F);

                    //Store value
                    pProperty->value.uiValue    = (ui32Component[3]<<24) | cssParseHSLtoRGB ( ui32Component[2], ui32Component[1], ui32Component[0] );
                    pProperty->value.bInteger   = true;
                    pProperty->value.iValueType = _cssRGB;

                    return 0;
                }
                break;

            case _cssPADDING:
            case _cssMARGIN:
            case _cssBORDER:
            case _cssBORDER_RADIUS:
                break;


            default:
                pProperty->value.iValue     = 0;
                pProperty->value.bInteger   = false;
                pProperty->value.bFloat     = false;
                pProperty->value.iValueType = tokenValue;

                return 0;

            case _cssEOL:
                //Detect the number and ascii text
                iAsciiIndex     = -1;
                iNumberStart    = -1;
                iNumberStop     = -1;        
                for (iIndex=0;iIndex<iCount;iIndex++)
                {
                    if (strchr(szNumberTable,szValue[iIndex]) != NULL )
                    {
                        if (iNumberStart==-1) iNumberStart = iIndex;
                        iNumberStop = iIndex;
                    }
                    else
                    {
                        //Detect an ascii character
                        iAsciiIndex = iIndex;
                        break;
                    }
                }

                if ( (iAsciiIndex!=-1) && (iNumberStart!=-1) && (iNumberStop!=-1) )
                {
                    //Detect the postfix
                    tokenPostfix = cssParseSelectorGetLexer ( szValue + iAsciiIndex );

                    //Detect the value
                    memset ( szNumber, 0, 64 );
                    strncpy ( szNumber, szValue+iNumberStart, iNumberStop - iNumberStart + 1 );

                    //Store value
                    if (tokenPostfix!=_cssEOL)
                    {
                
                        if (strchr(szNumber,'.')==NULL)
                        {
                            //If value is integer
                            pProperty->value.iValue     = atoi ( szNumber );
                            pProperty->value.bInteger   = true;
                            pProperty->value.bFloat     = false;
                            pProperty->value.iValueType = tokenPostfix;

                            return 0;
                        }
                        else
                        {
                            //If value is float
                            pProperty->value.fValue     = (float)atof ( szNumber );
                            pProperty->value.bInteger   = false;
                            pProperty->value.bFloat     = true;
                            pProperty->value.iValueType = tokenPostfix;

                            return 0;
                        }

                    }
                    else
                    {
                        //Error !
                        return -1;
                    }
                }
                else if ( (iNumberStart!=-1) && (iNumberStop!=-1) )
                {
                    memset ( szNumber, 0, 64 );
                    strncpy ( szNumber, szValue+iNumberStart, iNumberStop - iNumberStart + 1 );
                    if (strchr(szNumber,'.')==NULL)
                    {
                        //If value is integer
                        pProperty->value.iValue     = atoi ( szNumber );
                        pProperty->value.bInteger   = true;
                        pProperty->value.bFloat     = false;
                        pProperty->value.iValueType = _cssTYPE_VALUE;

                        return 0;
                    }
                    else
                    {
                        //If value is float
                        pProperty->value.fValue     = (float)atof ( szNumber );
                        pProperty->value.bInteger   = false;
                        pProperty->value.bFloat     = true;
                        pProperty->value.iValueType = _cssTYPE_VALUE;

                        return 0;
                    }
                    
                }
                else
                {
                    //Error !
                    return -1;
                }

                break;
        }
    }


    return -1;
}

zRETURN cssParseSelector_CheckPropertyAndValue ( cssNODE * pnodeProperty, token_t iTokenProperty )
{
    int iIndex;
    int iValueCount;

    //Nombre de valeurs de la "property"
    iValueCount = pnodeProperty->getChildCount();

    for (iIndex=0;lexTable[iIndex].iToken!=_cssEOL;iIndex++)
    {
        if (lexTable[iIndex].iToken == iTokenProperty)
        {
            if (iValueCount>=lexTable[iIndex].iMinimalValueCount && iValueCount <= lexTable[iIndex].iMaximalValueCount)
            return 0;
        }
    }
    
    return 0;
}


zRETURN cssParseSelector ( cssSELECTOR * pSelector, cssNODE * nodeSelector )
{
    uint32_t        uiPropertyIndex, uiPropertyCount;
    token_t         iToken;
    char            szToken[256];
    cssNODE *       nodeProperty;
    int             iCount, iIndex;
    cssPROPERTY *   pPropertyArray;

    uint32_t        uiValueIndex,uiValueCount;

    //On énumère toutes les "property"...
    uiPropertyCount = nodeSelector->getChildCount();
    for (uiPropertyIndex=0;uiPropertyIndex<uiPropertyCount;uiPropertyIndex++)
    {
        //L'élément "property" sélectionné        
        nodeProperty = nodeSelector->getChildIndex ( uiPropertyIndex );
        strExport ( &(nodeProperty->strText), szToken, 256, true );

        //On énumère toutes les "value"...
        uiValueCount = nodeProperty->getChildCount();
        iToken = cssParseSelectorGetLexer ( szToken );

        //On vérifie que les "value" correspondent bien à la "property"
        if (cssParseSelector_CheckPropertyAndValue(nodeProperty,iToken))
            iToken = _cssEOL;


        switch (iToken)
        {
            case _cssBORDER_RADIUS:
                pPropertyArray = cssParseSelectorAlloc ( pSelector, 4 );
                pPropertyArray[0].iPropertyType = _cssBORDER_TOP_LEFT_RADIUS;
                pPropertyArray[1].iPropertyType = _cssBORDER_TOP_RIGHT_RADIUS;
                pPropertyArray[2].iPropertyType = _cssBORDER_BOTTOM_LEFT_RADIUS;
                pPropertyArray[3].iPropertyType = _cssBORDER_BOTTOM_RIGHT_RADIUS;
                switch (uiValueCount)
                {
                    case 1: //Single to 4
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(0), false );
                        break;

                    case 2: //top-left/bottom-right ; top-right/bottom-left
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(1), false );
                        break;

                    case 3: //top-left ; top-right/bottom-left ; bottom-right
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(2), false );
                        break;

                    case 4: //top-left ; top-right ; bottom-right ; bottom-left
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(2), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(3), false );
                        break;
                }
                break;

            case _cssBORDER:
                //Value detection of only Colors and Width
                //border-style is bypassed !!!
                pPropertyArray = cssParseSelectorAlloc ( pSelector, 8 );
                {
                    cssPROPERTY pPropertyArrayTable[3];

                    memset ( pPropertyArrayTable, 0, sizeof(cssPROPERTY) * 3 );

                    pPropertyArrayTable[0].value.iValueType = _cssEOL;
                    pPropertyArrayTable[1].value.iValueType = _cssEOL;
                    pPropertyArrayTable[2].value.iValueType = _cssEOL;

                    iCount = 0;

                    switch (uiValueCount)
                    {
                        case 3:
                            iCount += 1;
                            cssParsePropertyValue ( pPropertyArrayTable + 2, nodeProperty->getChildIndex(2), false );
                        case 2:
                            iCount += 1;
                            cssParsePropertyValue ( pPropertyArrayTable + 1, nodeProperty->getChildIndex(1), false );
                        case 1:
                            iCount += 1;
                            cssParsePropertyValue ( pPropertyArrayTable + 0, nodeProperty->getChildIndex(0), false );
                            break;

                        default:
                            break;
                    }

                    for (iIndex=0;iIndex<iCount;iIndex++)
                    {
                        switch (pPropertyArrayTable[iIndex].value.iValueType)
                        {
                            case _cssPX:
                            case _cssPERCENT:
                            case _cssVMIN:
                            case _cssVMAX:
                            case _cssVW:
                            case _cssVH:
                                memcpy ( pPropertyArray + 0, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pPropertyArray + 1, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pPropertyArray + 2, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pPropertyArray + 3, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                pPropertyArray[0].iPropertyType = _cssBORDER_LEFT_WIDTH;
                                pPropertyArray[1].iPropertyType = _cssBORDER_RIGHT_WIDTH;
                                pPropertyArray[2].iPropertyType = _cssBORDER_TOP_WIDTH;
                                pPropertyArray[3].iPropertyType = _cssBORDER_BOTTOM_WIDTH;
                                break;

                            case _cssRGB:
                            case _cssRGBA:
                                memcpy ( pPropertyArray + 4, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pPropertyArray + 5, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pPropertyArray + 6, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pPropertyArray + 7, pPropertyArrayTable + iIndex, sizeof(cssPROPERTY) );
                                pPropertyArray[4].iPropertyType = _cssBORDER_LEFT_COLOR;
                                pPropertyArray[5].iPropertyType = _cssBORDER_RIGHT_COLOR;
                                pPropertyArray[6].iPropertyType = _cssBORDER_TOP_COLOR;
                                pPropertyArray[7].iPropertyType = _cssBORDER_BOTTOM_COLOR;
                                break;
                        }
                    }
                }
                
                break;

            case _cssPADDING:
            case _cssMARGIN:
            
                pPropertyArray = cssParseSelectorAlloc ( pSelector, 4 );

                switch (iToken)
                {
                    case _cssPADDING:
                        pPropertyArray[0].iPropertyType = _cssPADDING_TOP;
                        pPropertyArray[1].iPropertyType = _cssPADDING_RIGHT;
                        pPropertyArray[2].iPropertyType = _cssPADDING_BOTTOM;
                        pPropertyArray[3].iPropertyType = _cssPADDING_LEFT;
                        break;
                    case _cssMARGIN:
                        pPropertyArray[0].iPropertyType = _cssMARGIN_TOP;
                        pPropertyArray[1].iPropertyType = _cssMARGIN_RIGHT;
                        pPropertyArray[2].iPropertyType = _cssMARGIN_BOTTOM;
                        pPropertyArray[3].iPropertyType = _cssMARGIN_LEFT;
                        break;
                }

                switch (uiValueCount)
                {
                    case 1: //Single to 4
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(0), false );
                        break;

                    case 2: //top/bottom ; left/right
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(1), false );
                        break;

                    case 3: //top ; left/right : bottom
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(2), false );
                        break;

                    case 4: //top ; right ; bottom ; left
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1), false );
                        cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(2), false );
                        cssParsePropertyValue ( pPropertyArray + 3, nodeProperty->getChildIndex(3), false );
                        break;
                }
                break;

            case _cssWIDTH:
            case _cssHEIGHT:
                pPropertyArray = cssParseSelectorAlloc ( pSelector, 2 );

                switch (iToken)
                {
                    case _cssWIDTH:
                        pPropertyArray[0].iPropertyType = _cssMIN_WIDTH;
                        pPropertyArray[1].iPropertyType = _cssMAX_WIDTH;
                        break;
                    case _cssHEIGHT:
                        pPropertyArray[0].iPropertyType = _cssMIN_HEIGHT;
                        pPropertyArray[1].iPropertyType = _cssMAX_HEIGHT;
                        break;
                }

                switch (uiValueCount)
                {
                    case 1: //Single to 2
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(0), false );
                        break;

                    case 2: //top/bottom ; left/right
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1), false );
                        break;
                }
                break;

            case _cssFONT_FAMILY:
                iCount = (int)uiValueCount;
                pPropertyArray = cssParseSelectorAlloc ( pSelector, iCount );
                for (iIndex=0;iIndex<iCount;iIndex++)
                {
                    cssParsePropertyValue ( 
                        pPropertyArray + iIndex, 
                        nodeProperty->getChildIndex(iIndex),
                        true );
                }
                break;

            default:
                switch (uiValueCount)
                {
                    case 1:
                        pPropertyArray = cssParseSelectorAlloc ( pSelector, 1 );
                        cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0), false );
                        pPropertyArray->iPropertyType = iToken;
                        break;

                    default:
                        break;
                }
                break;
        }
    }




/*

        switch (uiValueCount)
        {
            case 0:
                break;

            case 1: //Only one value

                pSelector->pPropertyArray = (cssPROPERTY *)cssParseMalloc ( sizeof(cssPROPERTY) );
                pSelector->iPropertyCount = 1;

                pSelector->pPropertyArray->iPropertyType    = iToken;
                pSelector->pPropertyArray->iSuperRule       = 0;

                cssParsePropertyValue ( 
                    pSelector->pPropertyArray, 
                    nodeProperty->getChildIndex(0), 
                    false );

                break;

            case 2:
            case 3:
            case 4:

                switch (iToken)
                {
                    case _cssPADDING:
                    case _cssMARGIN:
                    case _cssBORDER:
                    case _cssBORDER_RADIUS:


                pSelector->pPropertyArray = (cssPROPERTY *)cssParseMalloc ( sizeof(cssPROPERTY) * uiValueCount );
                pSelector->iPropertyCount = uiValueCount;

                pSelector->pPropertyArray->iPropertyType = iToken;



                    case _cssLEFT:
                    case _cssRIGHT:
                    case _cssTOP:
                    case _cssBOTTOM:
                    case _cssWIDTH:
                    case _cssHEIGHT:
                    case _cssMIN_WIDTH:
                    case _cssMAX_WIDTH:
                    case _cssMIN_HEIGHT:
                    case _cssMAX_HEIGHT:

                        cssParsePropertyValue ( pSelector->pPropertyArray, nodeProperty->getChildIndex(0) );
                        break;
                    

                        break;
                    
                    case _cssPADDING_LEFT:
                    case _cssPADDING_RIGHT:
                    case _cssPADDING_TOP:
                    case _cssPADDING_BOTTOM:
                    
                    case _cssMARGIN_LEFT:
                    case _cssMARGIN_RIGHT:
                    case _cssMARGIN_TOP:
                    case _cssMARGIN_BOTTOM:
                    
                    case _cssBORDER_LEFT_COLOR:
                    case _cssBORDER_LEFT_WIDTH:
                    case _cssBORDER_RIGHT_COLOR:
                    case _cssBORDER_RIGHT_WIDTH:
                    case _cssBORDER_TOP_COLOR:
                    case _cssBORDER_TOP_WIDTH:
                    case _cssBORDER_BOTTOM_COLOR:
                    case _cssBORDER_BOTTOM_WIDTH:
                    
                    case _cssBORDER_TOP_LEFT_RADIUS:
                    case _cssBORDER_TOP_RIGHT_RADIUS:
                    case _cssBORDER_BOTTOM_LEFT_RADIUS:
                    case _cssBORDER_BOTTOM_RIGHT_RADIUS:
                    case _cssVISIBILITY:
                    case _cssPOSITION:
                    case _cssDISPLAY:
                    case _cssCOLOR:
                    case _cssOPACITY:
                    case _cssBACKGROUND_COLOR:
                    case _cssBACKGROUND_ORIGIN:
                    case _cssBACKGROUND_POSITION_X:
                    case _cssBACKGROUND_POSITION_Y:
                    case _cssBACKGROUND_REPEAT_X:
                    case _cssBACKGROUND_REPEAT_Y:
                    case _cssBACKGROUND_SIZE_X:
                    case _cssBACKGROUND_SIZE_Y:
                    case _cssFONT_SIZE:
                    case _cssFONT_WEIGHT:
                    case _cssFONT_FAMILY:
                    case _cssTEXT_ALIGN:
                    case _cssVERTICAL_ALIGN:
                    case _cssOVERFLOW:
                        break;
                }


                break;
        
        switch (iToken)
        {
            case _cssLEFT:
            case _cssRIGHT:
            case _cssTOP:
            case _cssBOTTOM:
            case _cssWIDTH:
                break;
            case _cssHEIGHT:
                

                break;
            case _cssMIN_WIDTH:
            case _cssMAX_WIDTH:
            case _cssMIN_HEIGHT:
            case _cssMAX_HEIGHT:
            case _cssPADDING:
            case _cssPADDING_LEFT:
            case _cssPADDING_RIGHT:
            case _cssPADDING_TOP:
            case _cssPADDING_BOTTOM:
            case _cssMARGIN:
            case _cssMARGIN_LEFT:
            case _cssMARGIN_RIGHT:
            case _cssMARGIN_TOP:
            case _cssMARGIN_BOTTOM:
            case _cssBORDER:
            case _cssBORDER_LEFT_COLOR:
            case _cssBORDER_LEFT_WIDTH:
            case _cssBORDER_RIGHT_COLOR:
            case _cssBORDER_RIGHT_WIDTH:
            case _cssBORDER_TOP_COLOR:
            case _cssBORDER_TOP_WIDTH:
            case _cssBORDER_BOTTOM_COLOR:
            case _cssBORDER_BOTTOM_WIDTH:
            case _cssBORDER_RADIUS:
            case _cssBORDER_TOP_LEFT_RADIUS:
            case _cssBORDER_TOP_RIGHT_RADIUS:
            case _cssBORDER_BOTTOM_LEFT_RADIUS:
            case _cssBORDER_BOTTOM_RIGHT_RADIUS:
            case _cssVISIBILITY:
            case _cssPOSITION:
            case _cssDISPLAY:
            case _cssCOLOR:
            case _cssOPACITY:
            case _cssBACKGROUND_COLOR:
            case _cssBACKGROUND_ORIGIN:
            case _cssBACKGROUND_POSITION_X:
            case _cssBACKGROUND_POSITION_Y:
            case _cssBACKGROUND_REPEAT_X:
            case _cssBACKGROUND_REPEAT_Y:
            case _cssBACKGROUND_SIZE_X:
            case _cssBACKGROUND_SIZE_Y:
            case _cssFONT_SIZE:
            case _cssFONT_WEIGHT:
            case _cssFONT_FAMILY:
            case _cssTEXT_ALIGN:
            case _cssVERTICAL_ALIGN:
            case _cssOVERFLOW:
                break;
        }
    
    }
*/
    return 0;
}