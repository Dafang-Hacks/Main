/******************************************************************************/
/* SUPER-FAST G.711 UTILITES (PCMU,PCMA)                                      */
/*                                                                            */
/* (c) TODO: find authors                                                     */
/******************************************************************************/

#ifndef G711SUPER_H
#define G711SUPER_H

#include "types.h"

//------------------------------------------------------------
inline void  init_g711(void);

//------------------------------------------------------------
extern short MuLawDecompressTable[256];
extern short ALawDecompressTable[256];
#define mulaw2linear(a) (MuLawDecompressTable[a])
#define alaw2linear(a)  (ALawDecompressTable[a])

//------------------------------------------------------------
extern unsigned char MuLawCompressTable[16384];
extern unsigned char ALawCompressTable[16384];
#define linear2mulaw(a) (MuLawCompressTable[((unsigned short)(a)) >> 2])
#define linear2alaw(a)  (ALawCompressTable[((unsigned short)(a)) >> 2])

//------------------------------------------------------------
extern unsigned char MuLawALawRecompressTable[256];
extern unsigned char ALawMuLawRecompressTable[256];
#define mulaw2alaw(a) (MuLawALawRecompressTable[a])
#define alaw2mulaw(a) (ALawMuLawRecompressTable[a])


#endif //G711SUPER_H

