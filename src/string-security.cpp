#include "string.h"

void eSTRING::Security_Init ( void )
{
    Security_ui64Seed[0] = 18446744073709551557;
    Security_ui64Seed[1] = 0;
}

void eSTRING::Security_Whiteness ( void * p, size_t nByteCount )
{
    int iLength, iIndex;
    uint32_t    ui32Random;
    uint8_t *   pui8Data;

    iLength     = (int)nByteCount;
    pui8Data    = (uint8_t *)p;
    ui32Random  = 0;

    for (iIndex=0;iIndex<iLength;iIndex++)
    {
        if ( (iIndex&3) == 0) ui32Random = Security_XorShift64Star32 ( );

        *pui8Data = (uint8_t)(ui32Random&0xFF);

        pui8Data++;
        ui32Random>>=8;
    }
}

void eSTRING::Security_SeedGenerator ( int iValue )
{
    uint64_t x, y;
    uint64_t a, b;
    
    a = Security_ui64Seed[0] ^ ((uint64_t)iValue);
    b = Security_ui64Seed[1] ^ ((uint64_t)iValue);

    x = (6364136223846793005 * b) + 1442695040888963407; //MMIX by Donald Knuth    
    y = (6364136223846793005 * a) + 1442695040888963407; //MMIX by Donald Knuth    

    Security_ui64Seed[0] = (6364136223846793005 * x) + 1442695040888963407; //MMIX by Donald Knuth    
    Security_ui64Seed[1] = (6364136223846793005 * y) + 1442695040888963407; //MMIX by Donald Knuth
}

uint32_t eSTRING::Security_XorShift64Star32 ( void )
{
    uint64_t x = Security_ui64Seed[0];
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    Security_ui64Seed[0] = x;
    return (x * 0x2545F4914F6CDD1D)>>32;
}
