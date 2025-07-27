/*
 *  Similar C++ implementation of JS string
 */

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdint.h>

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4028)
#pragma warning(disable : 4996)
#endif

#ifndef zM_STRING_HEADER
#define zM_STRING_HEADER

#define STRING_MODE_BYTE         1
#define STRING_MODE_UNICODE      4

#define STRING_ENCODING_UNASSIGNED      0xFFFFFFFF
#define STRING_ENCODING_MACROMAN        0x30000001
#define STRING_ENCODING_WINDOWS1250     0x20001250
#define STRING_ENCODING_WINDOWS1252     0x20001252
#define STRING_ENCODING_IBM437          0x10000437
#define STRING_ENCODING_IBM850          0x10000850
#define STRING_ENCODING_IBM852          0x10000852
#define STRING_ENCODING_ISO8859_1       0x08859001
#define STRING_ENCODING_ISO8859_2       0x08859002
#define STRING_ENCODING_ISO8859_3       0x08859003
#define STRING_ENCODING_ISO8859_4       0x08859004
#define STRING_ENCODING_ISO8859_15      0x08859015
#define STRING_ENCODING_UTF32           0x00000020
#define STRING_ENCODING_UTF8            0x00000008
#define STRING_ENCODING_NONE            0x00000000




/*



A la création d'une string, le mode "auto-detect" est actif, mais char-size est en mode BYTE.

Pour savoir 
Sous MacOS, la pagecode est par défaut MACROMAN
Sous Windows, la pagecode est Windows 1252









En mode RAW :
- les fonctions liées à la locale ne fonctionnent pas
- 

*/
/*
class eCharStringGeneric {
    protected:
        //virtual void Destroy() = 0;

    public:
        void * pWriteAccess;
        int iWriteMode;

        eCharStringGeneric () { pWriteAccess = NULL; }
        virtual ~eCharStringGeneric () { }
};

class eCHAR : public eCharStringGeneric {
    protected:
    
    public:    
        uint32_t data;

        eCHAR () { data = 0; }
        eCHAR ( uint32_t cValue ) { data = cValue; }
        ~eCHAR ( ) { }


        //eCHAR => uint32_t
        operator uint32_t() {
        return this->data;
        }

        eCHAR & operator = (const uint32_t cValue )
        {
            if (this->pWriteAccess)
            {
                if (this->iWriteMode==1)
                    *((uint8_t *)this->pWriteAccess) = cValue;
                else if (this->iWriteMode==4)
                    *((uint32_t *)this->pWriteAccess) = cValue;

                this->pWriteAccess = NULL;
            }
            this->data = cValue;
            return *this;
        }

        eCHAR & operator = ( eCHAR & cValue )
        {
            this->pWriteAccess = NULL;
            return *this;
        }

    
};
*/
#ifdef nono
class STRCHAR {
    protected:
        

    public:
        uint32_t data;

        STRCHAR ()
        {
            data = 0;
        }

        ~STRCHAR ( )
        {
        
        }

        /*
        CHAR &    operator = ( uint32_t cValue )
        {
            this->data = cValue;
            return *this;
        }
        */

        void    operator = ( uint32_t cValue )
        {
            this->data = cValue;
        }

        void    operator = ( size_t cValue )
        {
            this->data = (uint32_t)cValue;
        }

        uint32_t operator() ( void )
        {
            return this->data;
        }

        void    operator = ( char * pszData )
        {
            this->data = *pszData;
        }

        void    operator = ( char cChar )
        {
            this->data = cChar;
        }

};
#endif

class eSTRING/* : public eCharStringGeneric*/ {


    protected:

        void        Buffer_Init                     ( void );
        void        Buffer_Free                     ( void );
        void        Buffer_NewAlloc                 ( int iCharactersCountNew, int iCharactersWordSizeNew );
        void        Buffer_Realloc                  ( int iCharactersCountNew, bool bDisableCopy );
        void        Buffer_Replace                  ( unsigned char * pData, int iCharactersCountNew );
        void        Buffer_Concat                   ( unsigned char * pData, int iCharactersCountNew );



        void        Security_Init                   ( void );
        void        Security_Whiteness              ( void * p, size_t nByteCount );
        void        Security_SeedGenerator          ( int iValue );
        uint32_t    Security_XorShift64Star32       ( void );
        uint64_t    Security_ui64Seed[2];

        void        setInternalEncoding             ( int iStringEncoding );


        void        _strcpy                         ( eSTRING & stringExt );
        void        _strcpy                         ( char * pszStringExt, int iStringEncodingChar );
        void        _strcat                         ( eSTRING & stringExt );
        void        _strcat                         ( char * pszStringExt, int iStringEncodingChar );

        void        _strcpyraw                      ( uint8_t * pbString, uint32_t dwStringLength );

        size_t      _indexOfParams                  ( eSTRING & strValue, size_t indexStart, bool bEnableFirst );
        size_t      _indexOfParams                  ( char * pszData, size_t indexStart, bool bEnableFirst );


    private:

        int         iCharactersCount;           //In number of characters (not in byte or dword)
        int         iCharactersAllocated;       //In number of characters allocated (not in byte or dword)
        void *      pCharactersBuffer;
        int         iCharactersWordSize;        //1 or 4
        int         iTempAlloc;                 //0 = alloc false, 1 = temporary alloc, 2 = stable alloc


        int         iCharLocaleEncoding;
        char        szTmp[32];

        //int         iEncodingImport;            //UTF-8 by default
        //int         iEncodingLocale;            //NONE by default

        /*
        int         iCharacterCount;
        int         iCharacterCountAlloc;
        void *      pBuffer;
        bool        bAllocated;
        int         iBufferCharSize;    //1 = BYTE, 4 = UTF32
        char        szTmp[32];
        */




    public:

        int length;

        eSTRING ( )
        {
            Buffer_Init ( );
            Security_Init ( );
            this->iCharLocaleEncoding = STRING_ENCODING_NONE;
        }

        eSTRING ( int iStringEncodingChar )
        {
            Buffer_Init ( );
            Security_Init ( );

            //Switch to UNICODE
            this->iCharLocaleEncoding = iStringEncodingChar;
            this->iCharactersWordSize = STRING_MODE_UNICODE;
        }

        eSTRING ( eSTRING & strBase )
        {
            Buffer_Init ( );
            Security_Init ( );

            //Switch to UNICODE
            this->iCharLocaleEncoding = strBase.iCharLocaleEncoding;
            this->iCharactersWordSize = strBase.iCharactersWordSize;
        }

        ~eSTRING ( )
        {
            Buffer_Free ( );
        }

        eSTRING &    operator =             ( eSTRING & strValue );
        eSTRING &    operator +=            ( eSTRING & strValue );
        eSTRING &    operator +             ( eSTRING & strValue );

        eSTRING &    operator =             ( char * pszData );
        eSTRING &    operator +=            ( char * pszData );
        eSTRING &    operator +             ( char * pszData );

        eSTRING &    operator =             ( int int32Value );
        eSTRING &    operator +=            ( int int32Value );
        eSTRING &    operator +             ( int int32Value );

        eSTRING &    operator =             ( unsigned int uint32Value );
        eSTRING &    operator +=            ( unsigned int uint32Value );
        eSTRING &    operator +             ( unsigned int uint32Value );
/*
        eSTRING &    operator =              ( eCHAR cValue );
        eSTRING &    operator +=             ( eCHAR cValue );
        eSTRING &    operator +              ( eCHAR cValue );
*/
        eSTRING &    operator =             ( char int8Value );
        eSTRING &    operator +=            ( char int8Value );
        eSTRING &    operator +             ( char int8Value );

        eSTRING &    operator =             ( unsigned char uint8Value );
        eSTRING &    operator +=            ( unsigned char uint8Value );
        eSTRING &    operator +             ( unsigned char uint8Value );

        //STRCHAR &   operator []             ( int iIndex );

        //uint32_t    operator []             ( int iIndex );

        void        charSet                 ( int iIndex, uint32_t cValue );
        uint32_t    charGet                 ( int iIndex );
        void        charConcat              ( uint32_t cValue );
        void        charStrcpy              ( uint32_t cValue );

        //int         set                     ( int iIndex, eCHAR cValue );
        //eCHAR &     get                     ( int iIndex );
        //eCHAR &     operator []             ( int iIndex );
        //operator int()         get                     ( int iIndex );

        //operator    int()                   {   return 0; }



        //STRCHAR const & operator []         ( int iIndex ) const;
        //STRCHAR     operator []             ( int iIndex ) const;

        //STRING const &    operator =              ( int iIndex ) const;

        //STRING &    operator =              ( STRCHAR ch );

        //eSTRING &   substring operator =    ( size_t indexStart );


        eSTRING &   substring               ( size_t indexStart );                             //Alloc new eSTRING
        eSTRING &   substring               ( size_t indexStart, size_t indexEnd );            //Alloc new eSTRING
        eSTRING &   substr                  ( size_t indexStart );                             //Alloc new eSTRING
        eSTRING &   substr                  ( size_t indexStart, size_t length );              //Alloc new eSTRING
        eSTRING &   charAt                  ( size_t indexAt );                                //Alloc new eSTRING

        size_t      indexOf                 ( eSTRING & strValue );
        size_t      indexOf                 ( char * pszData );
        size_t      indexOf                 ( eSTRING & strValue, size_t indexStart );
        size_t      indexOf                 ( char * pszData, size_t indexStart );

        size_t      lastIndexOf             ( eSTRING & strValue );
        size_t      lastIndexOf             ( char * pszData );
        size_t      lastIndexOf             ( eSTRING & strValue, size_t indexStart );
        size_t      lastIndexOf             ( char * pszData, size_t indexStart );

        eSTRING &   trim                    ( void );
        eSTRING &   trimStart               ( void );
        eSTRING &   trimLeft                ( void );
        eSTRING &   trimEnd                 ( void );
        eSTRING &   trimRight               ( void );

        eSTRING &   toUpperCase             ( bool bRemoveAccent );
        eSTRING &   toLowerCase             ( bool bRemoveAccent );

        void        enableUnicode           ( int iStringEncodingFrom );
        void        setCharLocale           ( int iStringEncodingChar );

        int         exportBytes             ( void * pData, int iDataBytesCount, int iStringEncoding, bool bEnableStringWithZero );
        

        size_t      fileGetContents         ( char * pszFilename );

        void        print                   ( void )
        {
            char szTmp[8192];

            szTmp[0] = 0;
            this->exportBytes ( szTmp, 8192, STRING_ENCODING_ISO8859_15, true );
            printf ( "%s\n", szTmp );
        
        }









};


#endif