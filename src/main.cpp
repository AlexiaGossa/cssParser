#include "main.h"

int stringTest00 ( void );
int stringTest01 ( void );

#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<sizeof(s)?sizeof(s)-1-(i):sizeof(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))

//#define HASH(s)    ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))

//#define HASH(s)     (sizeof(s)>4?1:0)

//#define HASH_1(s,o,h)   (((sizeof(s)-o)>0)?((h*0x1000193)^((uint32_t)s[o])):(h))
//#define HASH_1(s,o,h)   (((sizeof(s)-o)>0)?((h)+s[o]):h)
//#define HASH_4(s,o,h)   HASH_1(s,o,HASH_1(s,o+1,HASH_1(s,o+2,HASH_1(s,o+3,h))))
//#define HASH_16(s,o,h)  HASH_4(s,o,HASH_4(s,o+4,HASH_4(s,o+8,HASH_4(s,o+12,h))))
//#define HASH(s)         HASH_4(s,0,0)

/*
#define HASH_1(s,o,h)   (((sizeof(s)-o)>0)? (((h)^(uint32_t)s[o])*0x1000193):(h))

#define HASH_2(s,of,h)   HASH_1(s,(of+0),HASH_1(s,(of+1),h))
//#define HASH_4(s,of,h)   HASH_1(s,(of+0),HASH_1(s,(of+1),HASH_1(s,(of+2),HASH_1(s,(of+3),h))))

#define HASH(s)         HASH_2(s,0,HASH_2(s,2,0x811c9dc5))


ak ^ bk


*(0x01000193)

*/


#define HASH_1(s,o)   ((uint32_t)(((sizeof(s)-(o))>0)?(  0x1000193ULL * (0x811c9dc5ULL^((uint32_t)s[(o)]))  ):(0)))
#define HASH_4(s,o)   HASH_1(s,o+0) ^ HASH_1(s,o+1) ^ HASH_1(s,o+2) ^ HASH_1(s,o+3) ^ HASH_1(s,o+4) ^ HASH_1(s,o+5)
#define HASH(s)       HASH_4(s,0)

//#define HASH_1(s,o,h)       ((uint32_t)(((sizeof(s)-(o))>0)?(  ((h)*0x1000193ULL) ^ ((uint32_t)s[(o)])  ):(h)))
//#define HASH_4(s,o,h)       HASH_1(s,o+0,HASH_1(s,o+1,HASH_1(s,o+2,HASH_1(s,o+3,h))))
//#define HASH_16(s,o,h)      HASH_4(s,o+0,h)^HASH_4(s,o+4,h)^HASH_4(s,o+8,h)^HASH_4(s,o+12,h)
//#define HASH(s)             HASH_1(s,0,HASH_1(s,1,HASH_1(s,2,HASH_1(s,3,0x811c9dc5ULL)))) ^ HASH_1(s,4,HASH_1(s,5,HASH_1(s,6,HASH_1(s,7,0x811c9dc5ULL))))


// ^ HASH_1(s,1) ^ HASH_1(s,2) ^ HASH_1(s,3)
// ^ HASH_1(s,1,0x811c9dc5,0x1000193) ^ HASH_1(s,2,0x811c9dc5,0x1000193) ^ HASH_1(s,3,0x811c9dc5,0x1000193)

/*

    qwHash = 14695981039346656037ULL;
    dwHashA = (unsigned long)n;
    dwHashB = (unsigned long)((n*11)+(n*7)+(n*5));

    for (i=0;i<n;i++)
    {
        qwHash ^= (mDES_UID)psz[i];
        qwHash *= 1099511628211ULL;
*/


//#define HASH(s)         

int main ( int argc, char ** argv )
{
//    char *  FileCSS_psz;
//    int     FileCSS_iLength;
    /*STRING strMy, strSecond;
    
    
    printf ( "cssParser test\n");

    strMy = "hello";

    strMy += " world";

    strMy = strMy + " !";

    strSecond = strMy + " Oups";

    strSecond = "Yellow ?";

    strMy = strSecond + strMy;
    */

    //return stringTest00 ();


    //eSTRING strFileCSS;
    cssOBJECT cssObject;
    cssDEBUG  cssDebug;


/*
    strFileCSS.fileGetContents ( "sample1.css" );
    strFileCSS.setCharLocale ( STRING_ENCODING_UTF8 );
    strFileCSS.enableUnicode ( STRING_ENCODING_UTF8 );

    FileCSS_iLength = strFileCSS.length;
    FileCSS_psz = (char *)malloc ( FileCSS_iLength + 1 );
    strFileCSS.exportBytes ( FileCSS_psz, FileCSS_iLength, STRING_ENCODING_ISO8859_15, true );
*/

/*
    {
        str strFile;
        htmlDOCUMENT htmlDocument;
        int iIndex;

        //for (iIndex=0;iIndex<100000;iIndex++)
        {

            strNewFileGetContents ( &strFile, "sample1.html" );

            htmlParseStr ( &strFile, &htmlDocument );

            strDelete ( &strFile );

            //htmlDocumentShowDebug ( &htmlDocument );


            htmlDocumentFree ( &htmlDocument );

        }

    }

    {
        int iIndex;


        for (iIndex=0;iIndex<100000;iIndex++)
        {
            if ((iIndex&0xFFF)==0)
                printf ( "%d\n", iIndex );
            cssParseInit ( 
                &cssObject );


            cssParseFile (
                "sample1.css",
                &cssObject );

//            cssParseTreePrintf (
//                &cssObject );

            cssParseFree (
                &cssObject );
        }
    }
*/

    
    /*
    {
        str strFile;
        htmlDOCUMENT htmlDocument;
        int iIndex;

        for (iIndex=0;iIndex<1000000;iIndex++)
        {
            if ((iIndex&0xFFF)==0)
                printf ( "%d/1000000\n", iIndex );

            cssParseInit ( &cssObject );
            cssParseFile ( "sample1.css", &cssObject );
            cssParseFree ( &cssObject );

            strNewFileGetContents ( &strFile, "sample1.html" );
            htmlParseStr ( &strFile, &htmlDocument );
            strDelete ( &strFile );
            htmlDocumentFree ( &htmlDocument );
        }

    }

    */
    {
        domCONTEXT domContext;

        domContextInit ( &domContext );
        
        domContextLoad ( &domContext, "sample1.html" );

        htmlDocumentShowDebug ( &domContext.htmlDocument );

        domContextFree ( &domContext );
    }


    printf ( "end !" );

    {} while (!kbhit());

/*
    cssParse ( 
        FileCSS_psz,
        FileCSS_iLength,
        &cssObject, 
        &cssDebug );
*/
    /*cssParse ( strFileCSS, &cssObject, &cssDebug );
    cssParse ( strFileCSS, &cssObject, &cssDebug );
    cssParse ( strFileCSS, &cssObject, &cssDebug );
    cssParse ( strFileCSS, &cssObject, &cssDebug );
    cssParse ( strFileCSS, &cssObject, &cssDebug );*/

//    free ( FileCSS_psz );


    return 0;
}