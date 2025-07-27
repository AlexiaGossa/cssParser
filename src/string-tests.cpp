#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

#include "string.h"


#ifdef nono
/*

Les tests

string => string
strchar => string
string[] => strchar
string[] => uint
strchar => string[]
uint => string[]


*/
int stringTest00 ( void )
{
    eSTRING strFirst, strSecond;
    //STRCHAR ch;
    uint32_t c32;
    int iIndex;



    for (iIndex=0;iIndex<100000;iIndex++)
    {
        strFirst = "hello world";
        strFirst = strFirst.substring ( 0 );
    }

    for (iIndex=0;iIndex<100000;iIndex++)
    {
        strFirst = "hello world";
        strFirst = strFirst.substring ( 0 );
    }

    for (iIndex=0;iIndex<1000000;iIndex++)
    {
        strFirst = "  hello world  ";
        strFirst = strFirst.trim ( );

        strFirst = "hello world  ";
        strFirst = strFirst.trim ( );

        strFirst = "  hello world";
        strFirst = strFirst.trim ( );

        strFirst = "hello world";
        strFirst = strFirst.trim ( );
    }

    strFirst.setCharLocale ( STRING_ENCODING_UTF8 );
    strFirst.enableUnicode ( STRING_ENCODING_UTF8 );

    //string[] => strchar
    // STRCHAR & STRING::operator [] ( int iIndex )
    //ch = strFirst[0];               

    //string[]() => uint
    // STRCHAR & STRING::operator [] ( int iIndex )
    // uint32_t operator() ( void )
//    c32 = strFirst[0]();

    c32 = -1;
    c32 = strFirst.charGet ( 0 );
    c32 = -1;
    c32 = strFirst.charGet(0);

    strFirst.charSet ( 0, 104 );

    strFirst.charSet(0,105);

    c32 = 106;
    strFirst.charSet(0,c32);

    iIndex = 1;
    strFirst.charSet(iIndex,107);

    iIndex = 2;
    c32 = 108;
    strFirst.charSet(iIndex,c32);
    
    //psz => strchar
    //ch = "x";

    //char => strchar
    //ch = 'y';

    //strchar => string[]
    //strFirst[0] = ch;

    

    


    return 0;
}

/*
 *  RAW only with copy and concat
 */
int stringTest01 ( void )
{
    eSTRING strFirst, strSecond;
    
    
    //printf ( "cssParser test\n");

    // strFirst = "hello world !"
    strFirst = "hello";
    strFirst += " world";
    strFirst = strFirst + " !";


    // strSecond = "hello world ! Oups"
    strSecond = strFirst + " Oups";

    // strSecond = "Yellow ?"
    strSecond = "Yellow ? ";

    // strFirst = "Yellow ? hello world !"
    strFirst = strSecond + strFirst;

    return 0;
}

#endif