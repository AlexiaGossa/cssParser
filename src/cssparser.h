#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#include "strz.h"

/*
    css

        selector

            property: value;
                        subValue
                            subSubValue

*/

typedef int                 zRETURN;


//All CSS token








typedef struct {

    union {
        int32_t     iValue;
        uint32_t    uiValue;
    };
    float       fValue;
    bool        bInteger;
    bool        bFloat;

    int         iValueType;

} cssVALUE;

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


typedef struct {
    int         iPropertyType;
    int         iSuperRule; //Important...

    cssVALUE    value;
} cssPROPERTY;

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





typedef struct {
    char            sz[128];

    cssPROPERTY *   pPropertyArray;
    int             iPropertyCount;

} cssSELECTOR;


typedef struct {

    cssSELECTOR *   pSelectorArray;
    int             iSelectorCount;

    //int iObjectCount;


} cssOBJECT;


typedef struct {

    int iCurrentLineElement;
    int iCurrentLineContent;

    int * piLineOffset;
    int   iLineCount;

} cssDEBUG;


enum cssMODE {
    cssMODE_BASE = 0,


};






zRETURN cssParse (
    char *          pszText,
    int             iTextLength,
    cssOBJECT *     pcssObject,
    cssDEBUG *      pcssDebug );

zRETURN cssParseElementStyle (
    char *          pszText,
    int             iTextLength,
    int             TextElementIndex_iStart,        //Element text start
    int             TextElementIndex_iStop,         //Element text stop
    int             TextContentIndex_iStart,        //Element style text start
    int             TextContentIndex_iStop,         //Element style text stop
    cssOBJECT *     pcssObject,         //Objects data
    cssDEBUG *      pcssDebug );


/*
zRETURN cssParseElementStyle ( 
    eSTRING & strElementText,
    eSTRING & strContentStyleText,
    cssOBJECT * pcssObject,
    cssDEBUG * pcssDebug );
*/
cssNODE * cssParseContentStyle ( 
    eSTRING & strContent,
    cssDEBUG * pcssDebug );

zRETURN cssParseSelector ( 
    cssSELECTOR * pSelector, 
    cssNODE * nodeSelector );





void cssParseSendError ( cssDEBUG * pcssDebug, int iTextOffset, char * pszErrorMessage );


/*
    Other functions
*/
uint32_t cssParseHSLtoRGB ( int iH, int iS, int iL );
uint32_t cssParseXtoU ( char * psz, int iCharCount, int iEnableDoubleValue );