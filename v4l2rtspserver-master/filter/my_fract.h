/******************************************************************************/
/* FRACTIONAL ARITHMETICS UTILITES                                            */
/* my_fract.h                                                                 */
/* (c) Sergei Mashkin, 2015                                                   */
/******************************************************************************/

#ifndef MY_FRACT_H
#define MY_FRACT_H

/******************************************************************************/
/* Type definitions                                                           */
/******************************************************************************/

typedef   unsigned char         u8;
typedef   unsigned short        u16;
typedef   unsigned int          u32;
typedef   unsigned long long    u64;
typedef   signed char           s8;
typedef   signed short          s16;
typedef   signed int            s32;
typedef   signed long long      s64;

typedef   short                 fract16;
typedef   long                  fract32;

/******************************************************************************/
/* FRACT 16 UTILITES                                                          */
/******************************************************************************/

#define  FRACT16_TO_FLOAT(a)  ((float)((fract16)(a))/(32768.0))
#define  FLOAT_TO_FRACT16(a)  ((fract16)((a)*32768))

#ifdef _bfin_

#define  MULT16(a,b)    __builtin_bfin_multr_fr1x16(a,b)
#define  ADD16(a,b)     __builtin_bfin_add_fr1x16(a,b)
#define  SUB16(a,b)     __builtin_bfin_sub_fr1x16(a,b)
#define  ABS16(a)       __builtin_bfin_abs_fr1x16(a)
#define  NEG16(a)       __builtin_bfin_negate_fr1x16(a)
#define  ASHL16(a,b)    __builtin_bfin_shl_fr1x16(a,b)  //b>=0, arithmetical left shift
#define  ASHR16(a,b)    __builtin_bfin_shl_fr1x16(a,-b) //b>=0  arithmetical right shift
#define  ASHIFT16(a,b)  __builtin_bfin_shl_fr1x16(a,b)  //arithmetical shift
#define  MIN16(a,b)     __builtin_bfin_min_fr1x16(a,b)
#define  MAX16(a,b)     __builtin_bfin_max_fr1x16(a,b)
#define  DIV16(a,b)     BF_DIV16(a,b)

#else

#define  MULT16(a,b)    (fract16)((((s32)a)*((s32)b))>>15)  //(1.15)*(1.15)=(2.30) -> convert into (1.15)
#define  ADD16(a,b)     (a+b)                               //no saturation
#define  SUB16(a,b)     (a-b)                               //no saturation
#define  ABS16(a)       ((a>=0)?(a):(-a))                   //no saturation
#define  NEG16(a)       (-(a))                              //no saturation
#define  ASHL16(a,b)    (a<<b)  //b>=0  arithmetical left shift,  no saturation
#define  ASHR16(a,b)    (a>>b)  //b>=0  arithmetical right shift, no saturation
#define  ASHIFT16(a,b)  ((b>0)?(a<<b):(a>>(-b)))  //arithmetical shift, no saturation
#define  MIN16(a,b)     ((a<b)?(a):(b))
#define  MAX16(a,b)     ((a>b)?(a):(b))
#define  DIV16(a,b)     ((((s32)a)<<15)/((s32)b))           //(1.15)/(1.15)=(2.30)/(1.15)=(1.15)

#endif

/******************************************************************************/
/* FRACT 32 UTILITES                                                          */
/******************************************************************************/

#define  FRACT32_TO_FLOAT(a)  ((float)((fract32)(a))/(32768.0*65536.0))
#define  FLOAT_TO_FRACT32(a)  ((fract32)((a)*32768*65536))

#ifdef _bfin_

#define  MULT32(a,b)   BF_MULT32(a,b)
#define  ADD32(a,b)    BF_ADD32(a,b)
#define  SUB32(a,b)    BF_SUB32(a,b)
#define  ABS32(a)      BF_ABS32(a)
#define  NEG32(a)      BF_NEG32(a)
#define  ASHL32(a,b)   BF_ASHIFT32(a,b)
#define  ASHR32(a,b)   BF_ASHIFT32(a,-(b))
#define  ASHIFT32(a,b) BF_ASHIFT32(a,b)
#define  MIN32(a,b)    BF_MIN32(a,b)
#define  MAX32(a,b)    BF_MAX32(a,b)
#define  DIV3216(a,b)  DIV32_16(a,b)

#else /* #ifdef _bfin_ */

#define  MULT32(a,b)   (fract32)((((s64)a)*((s64)b))>>31) //(1.31)*(1.31)=(2.62) -> convert into (1.31)
#define  ADD32(a,b)    (a+b)                              //no saturation
#define  SUB32(a,b)    (a-b)                              //no saturation
#define  ABS32(a)      ((a>=0)?(a):(-a))                  //no saturation
#define  NEG32(a)      (-(a))                             //no saturation
#define  ASHL32(a,b)   (a<<b)  //b>=0  arithmetical left shift,  no saturation
#define  ASHR32(a,b)   (a>>b)  //b>=0  arithmetical right shift, no saturation
#define  ASHIFT32(a,b) ((b>0)?(a<<b):(a>>(-b)))  //arithmetical shift, no saturation
#define  MIN32(a,b)    ((a<b)?(a):(b))
#define  MAX32(a,b)    ((a>b)?(a):(b))
#define  DIV3216(a,b)  ((s16)((((s32)a)>>1)/((s32)b)))    //(1.31)/(1.15) = (2.30)/(1.15) = (1.15)

#endif /* #ifdef _bfin_ */

/******************************************************************************/
/* FRACT 16/32 UTILITES                                                       */
/******************************************************************************/

volatile static union {
        struct {
                fract16 lo;
                fract16 hi;
        } two16;
        fract32 one32;
} convert;

#define  F32    convert.one32   /* Usage: F32=x_fract32; x_fract16=F16HI; y_fract16=F16LO; */
#define  F16LO  convert.two16.lo
#define  F16HI  convert.two16.hi

/******************************************************************************/
/* UTILITES IMPLEMENTATIONS                                                   */
/******************************************************************************/

#ifdef _bfin_

static inline fract16 DIV32_16(fract32 a, fract16 b)
{
        fract32 res, bb;
        bb = b;
        __asm__("R0 = %1;"
                "R1 = %2;"
        /*      "R0 <<= 1;" */
                "DIVS(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "DIVQ(R0,R1);"
                "R0 = R0.L;"
                "%0 = R0;":"=m"(res)
                :"m"(a), "m"(bb)
                :"R0", "R1", "cc");
        return res;
}

static inline fract16 BF_DIV16(fract16 a, fract16 b)
{
        fract32 a32;
        F16HI = a;
        a32 = F32;
        return DIV32_16(a32, b);
}

static inline fract32 BF_MAX32(fract32 a, fract32 b)
{
        fract32 res;
        __asm__ __volatile__("%0 = MAX(%1,%2);":"=d"(res)
                             :"d"(a), "d"(b)
            );
        return res;
}

static inline fract32 BF_MIN32(fract32 a, fract32 b)
{
        fract32 res;
        __asm__ __volatile__("%0 = MIN(%1,%2);":"=d"(res)
                             :"d"(a), "d"(b)
            );
        return res;
}

static inline fract32 BF_ADD32(fract32 a, fract32 b)
{
        fract32 res;
        __asm__ __volatile__("%0 = %1 + %2 (S);":"=d"(res)
                             :"d"(a), "d"(b)
            );
        return res;
}

static inline fract32 BF_SUB32(fract32 a, fract32 b)
{
        fract32 res;
        __asm__ __volatile__("%0 = %1 - %2 (S);":"=d"(res)
                             :"d"(a), "d"(b)
            );
        return res;
}

static inline fract32 BF_ABS32(fract32 a)
{
        fract32 res;
        __asm__ __volatile__("%0 = ABS %1;":"=d"(res)
                             :"d"(a)
            );
        return res;
}

static inline fract32 BF_NEG32(fract32 a)
{
        fract32 res;
        __asm__ __volatile__("%0 = - %1 (S);":"=d"(res)
                             :"d"(a)
            );
        return res;
}

static inline fract32 BF_MULT32(fract32 a, fract32 b)
{
        fract32 res;
        __asm__ __volatile__("A1 = A0 = 0;"
                             "R2 = ( A0 += %1.H * %2.H ),"
                             "A1 += %1.H * %2.L (M);"
                             "%0 = ( A1 += %2.H * %1.L ) (M);"
                             "%0 = %0 >>> 15;" "%0 = R2 + %0 (S);":"=W"(res)
                             :"d"(a), "d"(b)
                             :"A0", "A1", "R2");
        return res;
}

static inline fract32 BF_ASHIFT32(fract32 a, signed short b)
{
        fract32 res;
        __asm__ __volatile__("%0 = ASHIFT %1 BY %2.L (S);":"=d"(res)
                             :"d"(a), "d"(b)
            );
        return res;
}

#endif /* #ifdef _bfin_ */

#endif /* MY_FRACT_H */
