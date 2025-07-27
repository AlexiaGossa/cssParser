#include "str.h"

/*
 *
 *
 *  Security part
 *
 *
 */

void        strSecurityInit            ( str * pstr )
{
    pstr->ui64SecuritySeed[0] = 18446744073709551557ULL;
    pstr->ui64SecuritySeed[1] = 0;
    strSecurityUpdate ( pstr, (uint64_t)pstr );
    
}

void        strSecurityUpdate          ( str * pstr, uint64_t qwValue )
{
    uint64_t x, y;
    uint64_t a, b;

    a = pstr->ui64SecuritySeed[0] ^ qwValue;
    b = pstr->ui64SecuritySeed[1] ^ qwValue;

    x = (6364136223846793005 * b) + 1442695040888963407; //MMIX by Donald Knuth    
    y = (6364136223846793005 * a) + 1442695040888963407; //MMIX by Donald Knuth    

    pstr->ui64SecuritySeed[0] = (6364136223846793005 * x) + 1442695040888963407; //MMIX by Donald Knuth    
    pstr->ui64SecuritySeed[1] = (6364136223846793005 * y) + 1442695040888963407; //MMIX by Donald Knuth
}

void        strSecurityWhitness        ( str * pstr, void * p, size_t nByteCount )
{
    int iLength, iIndex;
    uint32_t    ui32Random;
    uint8_t *   pui8Data;
    uint64_t    x;

    iLength     = (int)nByteCount;
    pui8Data    = (uint8_t *)p;
    ui32Random  = 0;

    for (iIndex=0;iIndex<iLength;iIndex++)
    {
        if ( (iIndex&3) == 0)
        {
            x = pstr->ui64SecuritySeed[0];
            x ^= x >> 12;
            x ^= x << 25;
            x ^= x >> 27;
            pstr->ui64SecuritySeed[0] = x;
            ui32Random = (uint32_t)((x * 0x2545F4914F6CDD1D)>>32);
        }
        *pui8Data = (uint8_t)(ui32Random&0xFF);
        pui8Data++;
        ui32Random>>=8;
    }
}
