/******************************************************************************/
/* NOISE REMOVER                                                              */
/* noise_remover.c                                                            */
/* (c) Sergei Mashkin, 2015                                                   */
/******************************************************************************/

#include  "noise_remover.h"

/*****************************************************************************/
/* DEFINITIONS                                                               */
/*****************************************************************************/

#define TR          0x66660000  // 0.8
#define TF          0x7F5C0000  // 0.995
#define INCRTIME1   16000
#define INCRTIME2   32000
#define INCR1       0x000000D6  // 0.0000001
#define INCR2       0x00000431  // 0.0000005
#define INCR3       0x000014F8  // 0.0000025

#define SENS        0x00000000  // 1.5 = 1.0 + 0.5  //sensitivity [0..2],
                                // 0=do not remove noise,
                                // 1..2=remove noise totally
#define SENS1       0x7FFFFFFF  // 1.0 part of SENS
#define SENS2       0x40000000  // 0.5 part of SENS

#define ATR         0x7333      // 0.9
#define ATF         0x7FDF      // 0.999
#define EMIN        0x00010000  // 0.0000305..
#define MINALPHA    0x0CCC      // 0.1
#define MAXALPHA    0x7FFF      // 1.0

/******************************************************************************/
/* CONSTANTS                                                                  */
/******************************************************************************/

/* filters coefficients in (1.15) format */
static const fract16 LF300_COEFF [FILTER_LEN] = {
     253,    471,    707,    955,   1207,   1455,   1691,   1907,   2095,
    2249,   2364,   2434,   2458,   2434,   2364,   2249,   2095,   1907,
    1691,   1455,   1207,    955,    707,    471,    253
    };

static const fract16 BF600_COEFF [FILTER_LEN] = {
    -733,  -1273,  -1706,  -1952,  -1953,  -1683,  -1162,   -447,    368,
    1172,   1848,   2299,   2458,   2299,   1848,   1172,    368,   -447,
   -1162,  -1683,  -1953,  -1952,  -1706,  -1273,   -733
    };

static const fract16 BF900_COEFF [FILTER_LEN] = {
     1142,   1700,   1706,   1086,      0,  -1191,  -2055,  -2250,  -1663,
     -468,    930,   2038,   2458,   2038,    930,   -468,  -1663,  -2250,
    -2055,  -1191,      0,   1086,   1706,   1700,   1142
    };

static const fract16 BF1200_COEFF [FILTER_LEN] = {
    -1439,  -1626,   -707,    818,   1953,   1870,    519,  -1274,  -2323,
    -1883,   -191,   1664,   2458,   1664,   -191,  -1883,  -2323,  -1274,
      519,   1870,   1953,    818,   -707,  -1626,  -1439
    };

static const fract16 BF1500_COEFF [FILTER_LEN] = {
     1595,   1073,   -707,  -1940,  -1207,    897,   2217,   1274,  -1068,
    -2396,  -1270,   1198,   2458,   1198,  -1270,  -2396,  -1068,   1274,
     2217,    897,  -1207,  -1940,   -707,   1073,   1595
    };

static const fract16 BF1800_COEFF [FILTER_LEN] = {
    -1595,   -204,   1706,   1210,  -1207,  -2011,    174,   2250,   1068,
    -1761,  -2073,    665,   2458,    665,  -2073,  -1761,   1068,   2250,
      174,  -2011,  -1207,   1210,   1706,   -204,  -1595
    };

static const fract16 BF2100_COEFF [FILTER_LEN] = {
     1439,   -726,  -1706,    676,   1953,   -582,  -2162,    447,   2323,
     -282,  -2423,     96,   2458,     96,  -2423,   -282,   2323,    447,
    -2162,   -582,   1953,    676,  -1706,   -726,   1439
    };

static const fract16 BF2500_COEFF [FILTER_LEN] = {
    -1479,    929,   1414,  -1886,   -746,   2593,   -433,  -2697,   1790,
     2042,  -2867,   -761,   3277,   -761,  -2867,   2042,   1790,  -2697,
     -433,   2593,   -746,  -1886,   1414,    929,  -1479
    };

static const fract16 BF3000_COEFF [FILTER_LEN] = {
     817,    292,  -1706,   1826,      0,  -2380,   2923,   -670,  -2590,
    3775,  -1525,  -2260,   4096,  -2260,  -1525,   3775,  -2590,   -670,
    2923,  -2380,      0,   1826,  -1706,    292,    817
    };

static const fract16 BF3500_COEFF [FILTER_LEN] = {
      817,  -1469,   1706,  -1220,      0,   1591,  -2923,   3367,  -2590,
      751,   1525,  -3382,   4096,  -3382,   1525,    751,  -2590,   3367,
    -2923,   1591,      0,  -1220,   1706,  -1469,    817
    };

static const fract16 HF4000_COEFF [FILTER_LEN] = {
     -817,    832,   -707,    429,      0,   -559,   1211,  -1907,   2590,
    -3200,   3681,  -3990,   4096,  -3990,   3681,  -3200,   2590,  -1907,
     1211,   -559,      0,    429,   -707,    832,   -817
    };

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

//------------------------------------------------------------------------------
static inline void filter_init( struct fir_s * filter, const fract16 * coeff )
{
    int i;
    filter->coeff = coeff;
    filter->pos = 0;
    for(i=0; i<2*FILTER_LEN; i++)
        filter->delay[i] = 0;
    return;
}

//------------------------------------------------------------------------------
//Double filter delay size optimization is based on OSLEC source code
static inline fract16 filter( struct fir_s * filter, fract16 x )
{
    int       i;
    fract16   y;
    fract16 * xptr;
    const fract16 * cptr;

    filter->delay[filter->pos             ] = x;
    filter->delay[filter->pos + FILTER_LEN] = x;

    filter->pos++;
    if(filter->pos >= FILTER_LEN)
        filter->pos = 0;

    cptr = filter->coeff;
    xptr = filter->delay + filter->pos;
    y = 0x0000;
    for(i=0; i<FILTER_LEN; i++) {
        y = ADD16( y, MULT16( *(cptr++), *(xptr++) ) );
    }
    return y;
}

#if 0
//------------------------------------------------------------------------------
static inline fract16 filter_hf100( struct noise_remover_s * nrm, fract16 x )
{
    fract32 x32;

    if(!nrm)
            return 0;

    /*******************************************/
    /* high pass filter                        */
    /* IIR-filter, Fs=8000 Hz, Fc=100 Hz       */
    /*                                         */
    /* y = k * (b1*x + b2*xdelay - a2*ydelay); */
    /* xdelay = x;                             */
    /* ydelay = y;                             */
    /*                                         */
    /* The same:                               */
    /*                                         */
    /* y = k * (x - xdelay - a2*ydelay);       */
    /* xdelay = x;                             */
    /* ydelay = y;                             */
    /*                                         */
    /*******************************************/

    #define  HF100_B1  (0x7FFFFFFF)  //b1 =  1.00000000 //100 Hz
    #define  HF100_B2  (0x80000000)  //b2 = -1.00000000
    #define  HF100_A2  (0x89AE0000)  //a2 = -0.9243905;
    #define  HF100_K   (0x7B290000)  //k  =  0.9621952

    F16LO = 0;
    F16HI = x;
    x32 = F32;

    F32 = MULT32( HF100_A2 , nrm->hf100_ydelay );
    F32 = SUB32 ( x32      , F32               );
    F32 = SUB32 ( F32      , nrm->hf100_xdelay );
    F32 = MULT32( HF100_K  , F32               );
    nrm->hf100_xdelay = x32;
    nrm->hf100_ydelay = F32;

    return F16HI;
}
#endif

//------------------------------------------------------------------------------
int noise_remover_init (struct noise_remover_s * nrm)
{
    int k;

    if(!nrm)
        return -1;

    /* init filters */
    nrm->hf100_xdelay = 0;
    nrm->hf100_ydelay = 0;

    filter_init ( &(nrm->filter[ 0]), LF300_COEFF  );
    filter_init ( &(nrm->filter[ 1]), BF600_COEFF  );
    filter_init ( &(nrm->filter[ 2]), BF900_COEFF  );
    filter_init ( &(nrm->filter[ 3]), BF1200_COEFF );
    filter_init ( &(nrm->filter[ 4]), BF1500_COEFF );
    filter_init ( &(nrm->filter[ 5]), BF1800_COEFF );
    filter_init ( &(nrm->filter[ 6]), BF2100_COEFF );
    filter_init ( &(nrm->filter[ 7]), BF2500_COEFF );
    filter_init ( &(nrm->filter[ 8]), BF3000_COEFF );
    filter_init ( &(nrm->filter[ 9]), BF3500_COEFF );
    filter_init ( &(nrm->filter[10]), HF4000_COEFF );

    /* init signal envelope estimator */
    for(k=0; k<SUBBANDS; k++)
        nrm->signal_e[k] = 0;

    /* init noise estimator */
    for(k=0; k<SUBBANDS; k++) {
        nrm->noise_e  [k] = 0;
        nrm->incrtime [k] = 0;
    }

    /* init filter bank coefficients */
    for(k=0; k<SUBBANDS; k++)
        nrm->alpha[k] = 0x7FFF; /* 1.0 */

    return 0;
}

//------------------------------------------------------------------------------
fract16 noise_remover (struct noise_remover_s * nrm, fract16 x, int training)
{
    int     k;
    fract16 x100;
    fract16 vy[SUBBANDS];
    fract16 ey[SUBBANDS];
    fract32 a;
    fract16 a16;
    fract32 y;

    if(!nrm)
        return 0;

    if(!training)
        return x;

    /* Remove DC (High pass filter 100 Hz) */
    #if 0
    x100 = filter_hf100( nrm, x );
    #else
    x100 = x;
    #endif

    /* filter bank */
    for(k=0; k<SUBBANDS; k++)
        vy[k] = filter( &(nrm->filter[k]), x100 );

    /* energies */
    for(k=0; k<SUBBANDS; k++)
        ey[k] = ABS16( vy[k] );

    /* estimate signal envelope */
    for(k=0; k<SUBBANDS; k++) {
        //MATLAB code:
        // TR = 0.8;    //Raise up factor
        // TF = 0.995;  //Fall down factor
        // for k=1:SUBBANDS
        //     if ey(k) > signal_e(k)
        //         signal_e(k) = ey(k) - (ey(k) - signal_e(k)) * TR;
        //     else
        //         signal_e(k) = signal_e(k) * TF;
        //     end
        // end
        F16LO = 0x0000;
        F16HI = ey[k];
        if(F32 > nrm->signal_e[k]) {
            nrm->signal_e[k] = SUB32 ( F32 , nrm->signal_e[k] );
            nrm->signal_e[k] = MULT32( TR  , nrm->signal_e[k] );
            nrm->signal_e[k] = SUB32 ( F32 , nrm->signal_e[k] );
        }
        else {
            nrm->signal_e[k] = MULT32( TF  , nrm->signal_e[k] );
        }
    }

    /* estimate noise energy */
    for(k=0; k<SUBBANDS; k++) {
        //MATLAB code:
        // INCRTIME1 = 16000;
        // INCRTIME2 = 32000;
        // INCR1 = 0.0000001;
        // INCR2 = 0.0000005;
        // INCR3 = 0.0000025;
        // for k=1:SUBBANDS
        //     if signal_e(k) < noise_e(k)
        //         incrtime(k) = 0;
        //         noise_e(k) = signal_e(k);
        //     else
        //         if incrtime(k) < INCRTIME1
        //             incrtime(k) = incrtime(k) + 1;
        //             noise_e(k) = noise_e(k) + INCR1;
        //         elseif incrtime(k) < INCRTIME2
        //             incrtime(k) = incrtime(k) + 1;
        //             noise_e(k) = noise_e(k) + INCR2;
        //         else
        //             noise_e(k) = noise_e(k) + INCR3;
        //         end
        //     end
        // end
        if(nrm->signal_e[k] < nrm->noise_e[k]) {
            nrm->incrtime[k] = 0;
            nrm->noise_e[k] = nrm->signal_e[k];
        }
        else {
            if(nrm->incrtime[k] < INCRTIME1) {
                nrm->incrtime[k] ++;
                nrm->noise_e[k] = ADD32( nrm->noise_e[k] , INCR1 );
            }
            else if(nrm->incrtime[k] < INCRTIME2) {
                nrm->incrtime[k] ++;
                nrm->noise_e[k] = ADD32( nrm->noise_e[k] , INCR2 );
            }
            else {
                nrm->noise_e[k] = ADD32( nrm->noise_e[k] , INCR3 );
            }
        }
    }

    /* calculate alpha-s */

    //MATLAB code:
    // SENS = 1.5;
    // EMIN = 0.00001*ATT;
    // MINALPHA = 0.1;
    // MAXALPHA = 1.0;
    // a = max(0,signal_e - SENS*noise_e) ./ (signal_e + EMIN);
    //
    // ATR = 0.9;
    // ATF = 0.999;
    // for k=1:SUBBANDS
    //     if a(k) > alpha(k)
    //         alpha(k) = a(k) - (a(k) - alpha(k)) * ATR;
    //     else
    //         alpha(k) = alpha(k) * ATF;
    //     end
    // end
    // alpha = max(alpha,0.1);
    // alpha = min(alpha,1.00);

    for(k=0; k<SUBBANDS; k++) {

        //for SENS <= 1.0 :
        // a = MULT32 ( nrm->noise_e[k]  , SENS );
        // a = SUB32  ( nrm->signal_e[k] , a    );
        // a = MAX32  ( 0                , a    );

        //for 2.0 >= SENS > 1.0 :
        // max(0,signal_e - SENS*noise_e) --->
        // max(0,signal_e - SENS1*noise_e - SENS2*noise_e), where SENS1+SENS2=SENS
        a   = MULT32 ( nrm->noise_e[k]  , SENS1 );
        a   = SUB32  ( nrm->signal_e[k] , a     );
        F32 = MAX32  ( 0                , a     );

        a   = MULT32 ( nrm->noise_e[k]  , SENS2 );
        a   = SUB32  ( F32              , a     );
        a   = MAX32  ( 0                , a     );

        F32 = ADD32  ( nrm->signal_e[k] , EMIN );
        a16 = DIV3216( a , F16HI );

        if(a16 > nrm->alpha[k]) {
            nrm->alpha[k] = SUB16 ( a16 , nrm->alpha[k] );
            nrm->alpha[k] = MULT16( ATR , nrm->alpha[k] );
            nrm->alpha[k] = SUB16 ( a16 , nrm->alpha[k] );
        }
        else {
            nrm->alpha[k] = MULT16( ATF , nrm->alpha[k] );
        }

        if(nrm->alpha[k] < MINALPHA)
            nrm->alpha[k] = MINALPHA;

        if(nrm->alpha[k] > MAXALPHA)
            nrm->alpha[k] = MAXALPHA;

        //test: alpha[k] = 1.0
        //nrm->alpha[k] = 0x7FFF;
    }

    /* calculate de-noised signal */
    y = 0;
    for(k=0; k<SUBBANDS; k++) {
        y = y + (fract32)( MULT16( nrm->alpha[k], vy[k] ) );
    }

    if(y > 32767)
        return  32767;
    if(y <-32767)
        return -32767;
    return (fract16)(y);
}

