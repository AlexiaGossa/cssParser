#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#include "str.h"
#include "cssnode.h"

#ifndef __H_CSSPARSE__
#define __H_CSSPARSE__

typedef int zRETURN;

/*
 *
 *
 *
 */

enum token_t
{
    _cssEOL,

    _cssTYPE_STRING,
    _cssTYPE_VALUE,
    _cssTYPE_COLORHEX,

    _cssPX,
    _cssPERCENT,
    _cssVMIN,
    _cssVMAX,
    _cssVW,
    _cssVH,
    _cssRGB,
    _cssRGBA,
    _cssHSL,
    _cssHSLA,
    _cssINHERIT,
    _cssINITIAL,
    _cssDEFAULT,
    _cssUNSET,
    _cssNONE,
    _cssINLINE,
    _cssINLINE_BLOCK,
    _cssBLOCK,
    _cssTABLE,
    _cssTABLE_CELL,
    _cssSTATIC,
    _cssABSOLUTE,
    _cssRELATIVE,
    _cssFIXED,

    _cssVISIBLE,
    _cssHIDDEN,

    _cssREPEAT,
    _cssSPACE,
    _cssNO_REPEAT,
    _cssROUND,

    _cssCOVER,
    _cssCONTAIN,
    _cssAUTO,

    _cssMIDDLE,
    _cssNORMAL,
    _cssANYWHERE,
    _cssBREAK_WORD,
    _cssCENTER,
    _cssJUSTIFY,

    _cssBORDER_BOX,
    _cssPADDING_BOX,
    _cssCONTENT_BOX,


    _cssLEFT,                                               // single-value
    _cssRIGHT,                                              // single-value
    _cssTOP,                                                // single-value
    _cssBOTTOM,                                             // single-value
    _cssWIDTH,                                              // 1 or 2 values => min-width et max-width
    _cssHEIGHT,                                             // 1 or 2 values => min-height et max-height
    _cssMIN_WIDTH,                                          // single-value
    _cssMAX_WIDTH,                                          // single-value
    _cssMIN_HEIGHT,                                         // single-value
    _cssMAX_HEIGHT,                                         // single-value

    _cssPADDING,
    _cssPADDING_LEFT,                                       // single-value
    _cssPADDING_RIGHT,                                      // single-value
    _cssPADDING_TOP,                                        // single-value
    _cssPADDING_BOTTOM,                                     // single-value

    _cssMARGIN,                                             
    _cssMARGIN_LEFT,                                        // single-value
    _cssMARGIN_RIGHT,                                       // single-value
    _cssMARGIN_TOP,                                         // single-value
    _cssMARGIN_BOTTOM,                                      // single-value

    _cssBORDER,
    _cssBORDER_LEFT_COLOR,                                  // single-value
    _cssBORDER_LEFT_WIDTH,                                  // single-value
    _cssBORDER_RIGHT_COLOR,                                 // single-value
    _cssBORDER_RIGHT_WIDTH,                                 // single-value
    _cssBORDER_TOP_COLOR,                                   // single-value
    _cssBORDER_TOP_WIDTH,                                   // single-value
    _cssBORDER_BOTTOM_COLOR,                                // single-value
    _cssBORDER_BOTTOM_WIDTH,                                // single-value

    _cssBORDER_RADIUS,
    _cssBORDER_TOP_LEFT_RADIUS,                             // 1 or 2 values (limited to 1 value)
    _cssBORDER_TOP_RIGHT_RADIUS,                            // 1 or 2 values (limited to 1 value)
    _cssBORDER_BOTTOM_LEFT_RADIUS,                          // 1 or 2 values (limited to 1 value)
    _cssBORDER_BOTTOM_RIGHT_RADIUS,                         // 1 or 2 values (limited to 1 value)

    _cssVISIBILITY,                                         // single-value
    _cssPOSITION,                                           // single-value
    _cssDISPLAY,                                            // 1, 2 or 3 values (limited to 1 value)
    _cssCOLOR,                                              // single-value
    _cssOPACITY,                                            // single-value
                                                            
    _cssBACKGROUND_COLOR,                                   // single-value
    _cssBACKGROUND_ORIGIN,                                  // single-value
    _cssBACKGROUND_POSITION_X,                              // 1 or 2 values (limited to 1 value) 
    _cssBACKGROUND_POSITION_Y,                              // 1 or 2 values (limited to 1 value)
    _cssBACKGROUND_REPEAT_X,                                // single-value
    _cssBACKGROUND_REPEAT_Y,                                // single-value
    _cssBACKGROUND_SIZE_X,                                  // 1 or 2 values (size and "auto", "contain" or "cover") (limited to 1 value)
    _cssBACKGROUND_SIZE_Y,                                  // 1 or 2 values (size and "auto", "contain" or "cover") (limited to 1 value)

    _cssFONT_SIZE,                                          // single-value
    _cssFONT_WEIGHT,                                        // single-value
    _cssFONT_FAMILY,                                        // single or multiple values (the font name and font name alternatives...)
    _cssLINE_HEIGHT,                                        // single-value

    _cssTEXT_ALIGN,                                         // single-value
    _cssVERTICAL_ALIGN,                                     // single-value
    _cssOVERFLOW,                                           // single-value
    _cssOVERFLOW_WRAP,                                      // single-value

    _cssZINDEX,                                             // single-value    

};

/*
 *
 *  css-VALUE
 *
 */

typedef struct {

    union {
        int32_t     iValue;
        uint32_t    uiValue;
    };
    float       fValue;
    char *      pszValue;
    bool        bInteger;
    bool        bFloat;
    bool        bString;

    int         iValueType;

} cssVALUE;
/*
enum cssVALUE_TYPE {
    cssPX,
    cssPERCENT,
    cssVMIN,
    cssVMAX,
    cssVW,
    cssVH,
    cssRGB,
    cssRGBA,
    cssAUTO,

    cssINHERIT,
    cssINITIAL,
    cssDEFAULT,
    cssUNSET,

    cssDISPLAY_NONE,
    cssDISPLAY_INLINE,
    cssDISPLAY_BLOCK,

    cssPOSITION_STATIC,
    cssPOSITION_ABSOLUTE,
    cssPOSITION_RELATIVE,
    cssPOSITION_FIXED,

    cssVISIBILITY_VISIBLE,
    cssVISIBILITY_HIDDEN,

    cssBACKGROUND_REPEAT_REPEAT,
    cssBACKGROUND_REPEAT_SPACE,
    cssBACKGROUND_REPEAT_NO_REPEAT,
    cssBACKGROUND_REPEAT_ROUND,

    cssBACKGROUND_SIZE_COVER,
    cssBACKGROUND_SIZE_CONTAIN,
    cssBACKGROUND_SIZE_AUTO,
};
*/

/*
 *
 *  css-PROPERTY
 *
 */

typedef struct {
    int         iPropertyType;
    int         iSuperRule; //Important...

    cssVALUE    value;
} cssPROPERTY;

/*
enum cssPROPERTY_TYPE {
    cssLEFT,
    cssRIGHT,
    cssTOP,
    cssBOTTOM,
    cssWIDTH,
    cssHEIGHT,
    cssMIN_WIDTH,
    cssMAX_WIDTH,
    cssMIN_HEIGHT,
    cssMAX_HEIGHT,

    cssPADDING_LEFT,
    cssPADDING_RIGHT,
    cssPADDING_TOP,
    cssPADDING_BOTTOM,

    cssMARGIN_LEFT,
    cssMARGIN_RIGHT,
    cssMARGIN_TOP,
    cssMARGIN_BOTTOM,

    cssBORDER_LEFT_COLOR,
    cssBORDER_LEFT_WIDTH,
    cssBORDER_RIGHT_COLOR,
    cssBORDER_RIGHT_WIDTH,
    cssBORDER_TOP_COLOR,
    cssBORDER_TOP_WIDTH,
    cssBORDER_BOTTOM_COLOR,
    cssBORDER_BOTTOM_WIDTH,

    cssBORDER_TOP_LEFT_RADIUS,
    cssBORDER_TOP_RIGHT_RADIUS,
    cssBORDER_BOTTOM_LEFT_RADIUS,
    cssBORDER_BOTTOM_RIGHT_RADIUS,

    cssVISIBILITY,
    cssPOSITION,
    cssDISPLAY,

    cssBACKGROUND_COLOR,
    cssBACKGROUND_ORIGIN,
    cssBACKGROUND_POSITION_X,
    cssBACKGROUND_POSITION_Y,
    cssBACKGROUND_REPEAT_X,
    cssBACKGROUND_REPEAT_Y,
    cssBACKGROUND_SIZE_X,
    cssBACKGROUND_SIZE_Y,

};
*/

/*
 *
 *  css-SELECTOR
 *
 */

typedef struct {
    char            sz[128];
    int             iElementHandle; //-1 or zero if not used

    cssPROPERTY *   pPropertyArray;
    int             iPropertyCount;

} cssSELECTOR;

/*
 *
 *  css-OBJECT
 *
 */

typedef struct {

    cssSELECTOR *   pSelectorArray;
    int             iSelectorCount;

    //int iObjectCount;


} cssOBJECT;


/*
 *
 *  DEBUG line of css file/text
 *
 */
typedef struct {

    int iCurrentLineElement;
    int iCurrentLineContent;

    int * piLineOffset;
    int   iLineCount;

} cssDEBUG;



enum cssMODE {
    cssMODE_BASE = 0,


};

zRETURN cssParseInit (
    cssOBJECT *     pcssObject );

zRETURN cssParseFree ( 
    cssOBJECT *     pcssObject );

zRETURN cssParseStr (
    str *          pstr,
    cssOBJECT *     pcssObject );

zRETURN cssParseFile (
    char *          pszFilename,
    cssOBJECT *     pcssObject );

zRETURN cssParseText (
    char *          pszText,
    cssOBJECT *     pcssObject );



zRETURN cssParseElementContent (
    str *           pstrElement,
    str *           pstrContent,
    cssOBJECT *     pcssObject );

cssNODE * cssParseContentStyle (
    str *           pstrContent );

zRETURN cssParseSelector ( 
    cssSELECTOR * pSelector, 
    cssNODE * nodeSelector );

zRETURN cssParseElementInline (
    int iHandleElement,
    str * pstrContent,
    cssOBJECT *     pcssObject );

zRETURN cssParseTreePrintf ( 
    cssOBJECT *     pcssObject );


char * cssParseSelectorFromToken ( int iToken );
cssPROPERTY * cssParseSelectorAlloc ( cssSELECTOR * pSelector, int iNewPropertyCount );

float    cssParseHUEtoRGB ( float fP, float fQ, float fT);
uint32_t cssParseHSLtoRGB ( int iH, int iS, int iL );
uint32_t cssParseXtoU ( char * psz, int iCharCount, int iEnableDoubleValue );


void * cssParseMalloc ( size_t size );



#endif