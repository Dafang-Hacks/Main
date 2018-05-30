/******************************************************************************/
/* NOISE REMOVER                                                              */
/* noise_remover.h                                                            */
/* (c) Sergei Mashkin, 2015                                                   */
/******************************************************************************/

#ifndef NOISE_REMOVER_H
#define NOISE_REMOVER_H

#include  "my_fract.h"

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

#define SUBBANDS    11
#define FILTER_LEN  25

struct fir_s {
    const fract16 * coeff;
    fract16   delay[2*FILTER_LEN]; /* see OSLEC optimization of FIR filters */
    int       pos;
};

struct energy_s {
    int     counter;
    fract32 sum;
    fract16 out;
};

struct noise_remover_s {
    fract32          hf100_xdelay;
    fract32          hf100_ydelay;
    struct fir_s     filter   [SUBBANDS];
    struct energy_s  energy   [SUBBANDS];
    fract32          signal_e [SUBBANDS];
    fract32          noise_e  [SUBBANDS];
    int              incrtime [SUBBANDS];
    fract16          alpha    [SUBBANDS];
};

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

int      noise_remover_init ( struct noise_remover_s * nrm );
fract16  noise_remover      ( struct noise_remover_s * nrm, fract16 x, int training );

#endif /* NOISE_REMOVER_H */
