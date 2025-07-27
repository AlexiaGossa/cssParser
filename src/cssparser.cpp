#include "cssparser.h"
#include <math.h>

/*
cssLEXICALENTRY cssLexicalEntry[] = {
    { "left",                                   CSS_LEFT                                            },
    { "right",                                  CSS_RIGHT                                           },
    { "top",                                    CSS_TOP                                             },
    { "bottom",                                 CSS_BOTTOM                                          },
    { "width",                                  CSS_WIDTH                                           },
    { "height",                                 CSS_HEIGHT                                          },
    { "min-width",                              CSS_MIN_WIDTH                                       },
    { "min-height",                             CSS_MIN_HEIGHT                                      },
    { "padding",                                CSS_PADDING                                         },
    { "padding-left",                           CSS_PADDING_LEFT                                    },
    { "padding-right",                          CSS_PADDING_RIGHT                                   },
    { "padding-top",                            CSS_PADDING_TOP                                     },
    { "padding-bottom",                         CSS_PADDING_BOTTOM                                  },
    { "margin",                                 CSS_MARGIN                                          },
    { "margin-left",                            CSS_MARGIN_LEFT                                     },
    { "margin-right",                           CSS_MARGIN_RIGHT                                    },
    { "margin-top",                             CSS_MARGIN_TOP                                      },
    { "margin-bottom",                          CSS_MARGIN_BOTTOM                                   },
    { "border",                                 CSS_BORDER                                          },
    { "border-left",                            CSS_BORDER_LEFT                                     },
    { "border-right",                           CSS_BORDER_RIGHT                                    },
    { "border-top",                             CSS_BORDER_TOP                                      },
    { "border-bottom",                          CSS_BORDER_BOTTOM                                   },
    { "z-index",                                CSS_ZINDEX                                          },

    { "auto",                                   CSS_UNIT_AUTO                                       },
    { "px",                                     CSS_UNIT_PIXEL                                      },
    { "%",                                      CSS_UNIT_PERCENT                                    },

    { "font-size",                              CSS_FONT_SIZE                                       },
    { "font-weight",                            CSS_FONT_WEIGHT                                     },
    { "line-height",                            CSS_LINE_HEIGHT                                     },
    { "font-style",                             CSS_FONT_STYLE                                      },
    { "color",                                  CSS_COLOR                                           },

    { "background",                             CSS_BACKGROUND                                      },
    { "background-color",                       CSS_BACKGROUND_COLOR                                },
    { "background-image",                       CSS_BACKGROUND_IMAGE                                },
    { "background-size",                        CSS_BACKGROUND_SIZE                                 },
    { "background-position",                    CSS_BACKGROUND_POSITION                             },
    { "background-repeat",                      CSS_BACKGROUND_REPEAT                               },

    { "display",                                CSS_DISPLAY                                         },
    { "none",                                   CSS_DISPLAY_NONE                                    },
    { "inline",                                 CSS_DISPLAY_INLINE                                  },
    { "block",                                  CSS_DISPLAY_BLOCK                                   },

    { "position",                               CSS_POSITION                                        },
    { "static",                                 CSS_POSITION_STATIC                                 },
    { "absolute",                               CSS_POSITION_ABSOLUTE                               },
    { "relative",                               CSS_POSITION_RELATIVE                               },
    { "fixed",                                  CSS_POSITION_FIXED                                  },

    { "inherit",                                CSS_UNIVERSAL_VALUE_INHERIT                         },
    { "initial",                                CSS_UNIVERSAL_VALUE_INITIAL                         },
    { "default",                                CSS_UNIVERSAL_VALUE_DEFAULT                         },
    
    { "[",                                      CSS_SEPARATOR_BRACKET_OPEN                          },
    { "]",                                      CSS_SEPARATOR_BRACKET_CLOSE                         },
    { "{",                                      CSS_SEPARATOR_CURLY_BRACKET_OPEN                    },
    { "}",                                      CSS_SEPARATOR_CURLY_BRACKET_CLOSE                   },
    { "(",                                      CSS_SEPARATOR_ROUND_BRACKET_OPEN                    },
    { ")",                                      CSS_SEPARATOR_ROUND_BRACKET_CLOSE                   },

    { ".",                                      CSS_ELEMENT_DOT_CLASS                               },
    { "#",                                      CSS_ELEMENT_HASH_ID                                 },

    { "body",                                   CSS_ELEMENT_BODY                                    },
    { "div",                                    CSS_ELEMENT_DIV                                     },
    { "p",                                      CSS_ELEMENT_P                                       },
    { "span",                                   CSS_ELEMENT_SPAN                                    },

    { ":",                                      CSS_COLON,                                          },

    { "",                                       -1,                                                 },
};
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
    _cssBORDER_TOP_LEFT_RADIUS,                             // 1 or 2 values
    _cssBORDER_TOP_RIGHT_RADIUS,                            // 1 or 2 values
    _cssBORDER_BOTTOM_LEFT_RADIUS,                          // 1 or 2 values
    _cssBORDER_BOTTOM_RIGHT_RADIUS,                         // 1 or 2 values

    _cssVISIBILITY,                                         // single-value
    _cssPOSITION,                                           // single-value
    _cssDISPLAY,                                            // 1, 2 or 3 values
    _cssCOLOR,                                              // single-value
    _cssOPACITY,                                            // single-value
                                                            
    _cssBACKGROUND_COLOR,                                   // single-value
    _cssBACKGROUND_ORIGIN,                                  // single-value
    _cssBACKGROUND_POSITION_X,                              // 1 or 2 values
    _cssBACKGROUND_POSITION_Y,                              // 1 or 2 values
    _cssBACKGROUND_REPEAT_X,                                // single-value
    _cssBACKGROUND_REPEAT_Y,                                // single-value
    _cssBACKGROUND_SIZE_X,                                  // 1 or 2 values (size and "auto", "contain" or "cover")
    _cssBACKGROUND_SIZE_Y,                                  // 1 or 2 values (size and "auto", "contain" or "cover")

    _cssFONT_SIZE,                                          // single-value
    _cssFONT_WEIGHT,                                        // single-value
    _cssFONT_FAMILY,                                        // single or multiple values (the font name and font name alternatives...)
    _cssLINE_HEIGHT,                                        // single-value

    _cssTEXT_ALIGN,                                         // single-value
    _cssVERTICAL_ALIGN,                                     // single-value
    _cssOVERFLOW,                                           // single-value
    _cssOVERFLOW_WRAP,                                      // single-value


} ;

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
    { _cssTYPE_STRING,                      "",                                 0, 0  { _cssEOL } },
    { _cssTYPE_VALUE,                       "",                                 0, 0  { _cssEOL } },
    { _cssTYPE_COLORHEX,                    "#",                                0, 0  { _cssEOL } },
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

    { _cssBORDER_BOX,                       "border-box",                       0, 0  { _cssEOL } },
    { _cssPADDING_BOX,                      "padding-box",                      0, 0  { _cssEOL } },
    { _cssCONTENT_BOX,                      "content-box",                      0, 0  { _cssEOL } },




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
    { _cssBORDER_TOP_LEFT_RADIUS,           "border-top-left-radius",           1, 2, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_TOP_RIGHT_RADIUS,          "border-top-right-radius",          1, 2, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_BOTTOM_LEFT_RADIUS,        "border-bottom-left-radius",        1, 2, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBORDER_BOTTOM_RIGHT_RADIUS,       "border-bottom-right-radius",       1, 2, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
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
    { _cssBACKGROUND_SIZE_X,                "background-size-x",                1, 2, { _cssAUTO, _cssCOVER, _cssCONTAIN, _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssBACKGROUND_SIZE_Y,                "background-size-y",                1, 2, { _cssAUTO, _cssCOVER, _cssCONTAIN, _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },

    { _cssFONT_SIZE,                        "font-size",                        1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssFONT_WEIGHT,                      "font-weight",                      1, 1, { _cssTYPE_VALUE, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssFONT_FAMILY,                      "font-family",                      1, 8, { _cssTYPE_STRING, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssLINE_HEIGHT,                      "line-height",                      1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssTEXT_ALIGN,                       "text-align",                       1, 1, { _cssLEFT, _cssRIGHT, _cssCENTER, _cssJUSTIFY, _cssVW, _cssVH, _cssMIDDLE, _cssTOP, _cssBOTTOM, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssVERTICAL_ALIGN,                   "vertical-align",                   1, 1, { _cssPX, _cssPERCENT, _cssVMIN, _cssVMAX, _cssVW, _cssVH, _cssMIDDLE, _cssTOP, _cssBOTTOM, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssOVERFLOW,                         "overflow",                         1, 1, { _cssVISIBLE, _cssHIDDEN, _cssAUTO, _cssINHERIT, _cssINITIAL, _cssUNSET } },
    { _cssOVERFLOW_WRAP,                    "overflow-wrap",                    1, 1, { _cssNORMAL, _cssANYWHERE, _cssBREAK_WORD, _cssINHERIT, _cssINITIAL, _cssUNSET } },


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








zRETURN cssParsePropertyValue ( cssPROPERTY * pProperty, cssNODE * nodeValue )
{
    char szValue[256], szNumber[64];
    char szNumberTable[]="0123456789.";
    int iNumberStart, iNumberStop, iAsciiIndex, iIndex, iCount;
    uint32_t uiValue;
    uint8_t uiComponent[4];
    uint32_t ui32Component[4];

    token_t tokenPostfix, tokenValue;
    
    nodeValue->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
    iCount          = strlen(szValue);

    //First step : detect a color hexa
    if (szValue[0] == '#')
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
                pProperty->value.bFloat     = false;
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
                pProperty->value.bFloat     = false;
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
                pProperty->value.bFloat     = false;
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
                pProperty->value.bFloat     = false;
                pProperty->value.iValueType = _cssRGBA;

                return 0;

            default:
                return -1;
        }
    }
    else
    {
        //Try to detect a lexer value
        tokenValue = cssParseSelectorGetLexer ( szValue + iAsciiIndex );
        switch (tokenValue)
        {
            case _cssRGB:
                if (nodeValue->getChildCount()==3)
                {
                    //R-component
                    nodeValue->getChildIndex(0)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    uiComponent[2] = atoi ( szValue );

                    //G-component
                    nodeValue->getChildIndex(1)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    uiComponent[1] = atoi ( szValue );

                    //B-component
                    nodeValue->getChildIndex(2)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    uiComponent[0] = atoi ( szValue );

                    //Alpha
                    uiComponent[3] = 0xFF;

                    //Store value
                    pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                    pProperty->value.bInteger   = true;
                    pProperty->value.bFloat     = false;
                    pProperty->value.iValueType = _cssRGB;

                    return 0;
                }
                break;

            case _cssRGBA:
                if (nodeValue->getChildCount()==4)
                {
                    //R-component
                    nodeValue->getChildIndex(0)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    uiComponent[2] = atoi ( szValue );

                    //G-component
                    nodeValue->getChildIndex(1)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    uiComponent[1] = atoi ( szValue );

                    //B-component
                    nodeValue->getChildIndex(2)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    uiComponent[0] = atoi ( szValue );

                    //Alpha
                    nodeValue->getChildIndex(3)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    uiComponent[3] = (uint8_t)floor ( (atof(szValue)*255.0F) + 0.5F);

                    //Store value
                    pProperty->value.uiValue    = *((uint32_t *)uiComponent);
                    pProperty->value.bInteger   = true;
                    pProperty->value.bFloat     = false;
                    pProperty->value.iValueType = _cssRGBA;

                    return 0;
                }
                break;

            case _cssHSL:
                if (nodeValue->getChildCount()==3)
                {
                    //H-component
                    nodeValue->getChildIndex(0)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    ui32Component[2] = atoi ( szValue );

                    //S-component
                    nodeValue->getChildIndex(1)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    ui32Component[1] = atoi ( szValue );

                    //L-component
                    nodeValue->getChildIndex(2)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    ui32Component[0] = atoi ( szValue );

                    //Store value
                    pProperty->value.uiValue    = 0xFF000000 | cssParseHSLtoRGB ( ui32Component[2], ui32Component[1], ui32Component[0] );
                    pProperty->value.bInteger   = true;
                    pProperty->value.bFloat     = false;
                    pProperty->value.iValueType = _cssRGB;

                    return 0;
                }
                break;
            case _cssHSLA:
                if (nodeValue->getChildCount()==4)
                {
                    //H-component
                    nodeValue->getChildIndex(0)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    ui32Component[2] = atoi ( szValue );

                    //S-component
                    nodeValue->getChildIndex(1)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    ui32Component[1] = atoi ( szValue );

                    //L-component
                    nodeValue->getChildIndex(2)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    ui32Component[0] = atoi ( szValue );

                    //Alpha
                    nodeValue->getChildIndex(3)->str.exportBytes ( szValue, 256, STRING_ENCODING_ISO8859_15, true );
                    ui32Component[3] = (uint32_t)floor ( (atof(szValue)*255.0F) + 0.5F);

                    //Store value
                    pProperty->value.uiValue    = (ui32Component[3]<<24) | cssParseHSLtoRGB ( ui32Component[2], ui32Component[1], ui32Component[0] );
                    pProperty->value.bInteger   = true;
                    pProperty->value.bFloat     = false;
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
                            pProperty->value.fValue     = atof ( szNumber );
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
    uint32_t    uiPropertyIndex, uiPropertyCount;
    token_t     iToken;
    char        szToken[256];
    cssNODE *   nodeProperty;
    int         iCount, iIndex;

    uint32_t    uiValueIndex,uiValueCount;

    //On énumère toutes les "property"...
    uiPropertyCount = nodeSelector->getChildCount();
    for (uiPropertyIndex=0;uiPropertyIndex<uiPropertyCount;uiPropertyIndex++)
    {
        //L'élément "property" sélectionné        
        nodeProperty = nodeSelector->getChildIndex ( uiPropertyCount );
        nodeProperty->str.exportBytes ( szToken, 256, STRING_ENCODING_ISO8859_15, true );

        //On énumère toutes les "value"...
        uiValueCount = nodeProperty->getChildCount();
        iToken = cssParseSelectorGetLexer ( szToken );

        //On vérifie que les "value" correspondent bien à la "property"
        if (cssParseSelector_CheckPropertyAndValue(nodeProperty,iToken))
            iToken = _cssEOL;


        switch (iToken)
        {
            case _cssBORDER_RADIUS:
                pSelector->pPropertyArray = (cssPROPERTY *)malloc ( sizeof(cssPROPERTY) * 4 );
                pSelector->iPropertyCount = 4;
                pSelector->pPropertyArray[0].iPropertyType = _cssBORDER_TOP_LEFT_RADIUS;
                pSelector->pPropertyArray[1].iPropertyType = _cssBORDER_TOP_RIGHT_RADIUS;
                pSelector->pPropertyArray[2].iPropertyType = _cssBORDER_BOTTOM_LEFT_RADIUS;
                pSelector->pPropertyArray[3].iPropertyType = _cssBORDER_BOTTOM_RIGHT_RADIUS;
                switch (uiValueCount)
                {
                    case 1: //Single to 4
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(0) );
                        break;

                    case 2: //top-left/bottom-right ; top-right/bottom-left
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(1) );
                        break;

                    case 3: //top-left ; top-right/bottom-left ; bottom-right
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(2) );
                        break;

                    case 4: //top-left ; top-right ; bottom-right ; bottom-left
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(2) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(3) );
                        break;
                }
                break;

            case _cssBORDER:
                //Value detection of only Colors and Width
                //border-style is bypassed !!!

                pSelector->pPropertyArray = (cssPROPERTY *)malloc ( sizeof(cssPROPERTY) * 8 );
                pSelector->iPropertyCount = 8;

                {
                    cssPROPERTY pPropertyArray[3];

                    pPropertyArray[0].value.iValueType = _cssEOL;
                    pPropertyArray[1].value.iValueType = _cssEOL;
                    pPropertyArray[2].value.iValueType = _cssEOL;

                    iCount = 0;

                    switch (uiValueCount)
                    {
                        case 3:
                            iCount += 1;
                            cssParsePropertyValue ( pPropertyArray + 2, nodeProperty->getChildIndex(2) );
                        case 2:
                            iCount += 1;
                            cssParsePropertyValue ( pPropertyArray + 1, nodeProperty->getChildIndex(1) );
                        case 1:
                            iCount += 1;
                            cssParsePropertyValue ( pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                            break;

                        default:
                            break;
                    }

                    for (iIndex=0;iIndex<iCount;iIndex++)
                    {
                        switch (pPropertyArray[iIndex].value.iValueType)
                        {
                            case cssPX:
                            case cssPERCENT:
                            case cssVMIN:
                            case cssVMAX:
                            case cssVW:
                            case cssVH:
                                memcpy ( pSelector->pPropertyArray + 0, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pSelector->pPropertyArray + 1, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pSelector->pPropertyArray + 2, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pSelector->pPropertyArray + 3, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                pSelector->pPropertyArray[0].iPropertyType = _cssBORDER_LEFT_WIDTH;
                                pSelector->pPropertyArray[1].iPropertyType = _cssBORDER_RIGHT_WIDTH;
                                pSelector->pPropertyArray[2].iPropertyType = _cssBORDER_TOP_WIDTH;
                                pSelector->pPropertyArray[3].iPropertyType = _cssBORDER_BOTTOM_WIDTH;
                                break;

                            case cssRGB:
                            case cssRGBA:
                                memcpy ( pSelector->pPropertyArray + 4, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pSelector->pPropertyArray + 5, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pSelector->pPropertyArray + 6, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                memcpy ( pSelector->pPropertyArray + 7, pPropertyArray + iIndex, sizeof(cssPROPERTY) );
                                pSelector->pPropertyArray[4].iPropertyType = _cssBORDER_LEFT_COLOR;
                                pSelector->pPropertyArray[5].iPropertyType = _cssBORDER_RIGHT_COLOR;
                                pSelector->pPropertyArray[6].iPropertyType = _cssBORDER_TOP_COLOR;
                                pSelector->pPropertyArray[7].iPropertyType = _cssBORDER_BOTTOM_COLOR;
                                break;
                        }
                    }
                }
                
                break;

            case _cssPADDING:
            case _cssMARGIN:
            
                pSelector->pPropertyArray = (cssPROPERTY *)malloc ( sizeof(cssPROPERTY) * 4 );
                pSelector->iPropertyCount = 4;

                switch (iToken)
                {
                    case _cssPADDING:
                        pSelector->pPropertyArray[0].iPropertyType = _cssPADDING_TOP;
                        pSelector->pPropertyArray[1].iPropertyType = _cssPADDING_RIGHT;
                        pSelector->pPropertyArray[2].iPropertyType = _cssPADDING_BOTTOM;
                        pSelector->pPropertyArray[3].iPropertyType = _cssPADDING_LEFT;
                        break;
                    case _cssMARGIN:
                        pSelector->pPropertyArray[0].iPropertyType = _cssMARGIN_TOP;
                        pSelector->pPropertyArray[1].iPropertyType = _cssMARGIN_RIGHT;
                        pSelector->pPropertyArray[2].iPropertyType = _cssMARGIN_BOTTOM;
                        pSelector->pPropertyArray[3].iPropertyType = _cssMARGIN_LEFT;
                        break;
                }

                switch (uiValueCount)
                {
                    case 1: //Single to 4
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(0) );
                        break;

                    case 2: //top/bottom ; left/right
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(1) );
                        break;

                    case 3: //top ; left/right : bottom
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(2) );
                        break;

                    case 4: //top ; right ; bottom ; left
                        cssParsePropertyValue ( pSelector->pPropertyArray + 0, nodeProperty->getChildIndex(0) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 1, nodeProperty->getChildIndex(1) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 2, nodeProperty->getChildIndex(2) );
                        cssParsePropertyValue ( pSelector->pPropertyArray + 3, nodeProperty->getChildIndex(3) );
                        break;
                }
                break;

            /*
            All properties except :
            
                _cssBORDER_RADIUS
                _cssBORDER
                _cssPADDING
                _cssMARGIN
                
            The used properies :

                all remaining properties
             

            */
            default:


        switch (uiValueCount)
        {
            case 0:
                break;

            case 1: //Only one value

                pSelector->pPropertyArray = (cssPROPERTY *)malloc ( sizeof(cssPROPERTY) );
                pSelector->iPropertyCount = 1;

                pSelector->pPropertyArray->iPropertyType    = iToken;
                pSelector->pPropertyArray->iSuperRule       = 0;

                cssParsePropertyValue ( 
                    pSelector->pPropertyArray, 
                    nodeProperty->getChildIndex(0) );

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


                pSelector->pPropertyArray = (cssPROPERTY *)malloc ( sizeof(cssPROPERTY) * uiValueCount );
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

    return 0;
}