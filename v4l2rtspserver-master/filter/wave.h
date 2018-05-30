/******************************************************************************/
/* WAVE-FILES UTILITES                                                        */
/* wave.h                                                                     */
/* (c) Sergei Mashkin, 2015                                                   */
/******************************************************************************/

#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>

#include "types.h"

/******************************************************************************/
/* DEFINITIONS                                                                */
/******************************************************************************/

#define  ABS_PATH_MAX                 1024

//Types of wave-files
#define  WAVETYPE_NOTSET              0 //set wavetype automatically on first write operation
#define  WAVETYPE_MONO_8000HZ_PCM16   1
#define  WAVETYPE_MONO_8000HZ_PCMA    2
#define  WAVETYPE_MONO_8000HZ_PCMU    3
#define  WAVETYPE_MONO_8000HZ_GSM610  4
#define  WAVETYPE_DEFAULT             WAVETYPE_MONO_8000HZ_PCM16

#define  WAVEFILE_RWMODE_NOTSET       0
#define  WAVEFILE_RWMODE_READ         1
#define  WAVEFILE_RWMODE_WRITE        2

#define  WAVEFILE_BUFF_SIZE           320  //>=320

//------------------------------------------------------------------------------
//Canonical WAVE format sructure
typedef struct waveheader_t waveheader_t;
struct waveheader_t
{
    //==== RIFF Header for WAVE-file
    char          riff[4];              //'RIFF'
    uint32_t      file_length;          //(file size-8) bytes
    char          wave[4];              //'WAVE'

    //==== fmt chunk
    char          fmt[4];               //'fmt '
    uint32_t      fmt_length;           //length of fmt data = (18+extra format bytes), bytes
    uint16_t      type;                 //0x0001=PCM, 0x0006=alaw, 0x0007=ulaw, 0x0031=GSM610
    uint16_t      channels;             //1=mono,2=stereo
    uint32_t      samples_per_second;   //samples per second (44100,8000,16000,..)
    uint32_t      bytes_per_second;     //samples_per_second * block_align
    uint16_t      block_align;          //channels * bits_per_sample / 8
    uint16_t      bits_per_sample;      //8 or 16
    uint16_t      extra_format_bytes;   //number of extra format bytes
    char          extra_format_data[2]; //set maximum supported extra format bytes here

    //==== fact chunk
    char          fact[4];              //'fact'
    uint32_t      fact_length;          //length of the fact block (bytes) = 4
    char          fact_data[4];         //set maximum supported fact bytes here

    //==== data chunk
    char          data[4];              //'data'
    uint32_t      data_length;          //length of the data block (bytes, must be x2)
};

//------------------------------------------------------------------------------
//WAVE-FILE structure
typedef struct wavefile_t wavefile_t;
struct wavefile_t
{
    unsigned char  wavetype;               //WAVETYPE_MONO_8000HZ_PCM16 / WAVETYPE_MONO_8000HZ_PCMA / ...
    char           filename[ABS_PATH_MAX]; //name of currently used wave-file ("" = not opened)
    uint8_t        rwmode;

    waveheader_t * header;
    FILE         * fp;                     //pointer to opened file
    long           pos;                    //current read/write  pos

    uint32_t       end_samples;            //samples from beginig to end mark
    uint32_t       end_bytes;              //number of data-bytes from beginig to end mark
    uint32_t       total_samples;          //samples from beginig to end of file
    uint32_t       total_bytes;            //number of data-bytes from beginig to end of file

    //==== vocoder
    void *         vocoder;

    uint8_t        data8[WAVEFILE_BUFF_SIZE];
    int            data8_bytes;

    int16_t        voice16[WAVEFILE_BUFF_SIZE];
    int            voice16_samples;
};

//NOTE:
//For multi-channel data, samples are interleaved between channels, like this:
// sample 0 for channel 0
// sample 0 for channel 1
// sample 1 for channel 0
// sample 1 for channel 1
//...

/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

waveheader_t * waveheader_create           ( void );
void           waveheader_destroy          ( waveheader_t * header );
int            waveheader_set_default      ( waveheader_t * header, uint8_t wavetype );
int            waveheader_write            ( FILE * fid, waveheader_t * header );
int            waveheader_read             ( FILE * fid, waveheader_t * header );
int            waveheader_get_default_size ( uint8_t wavetype );


wavefile_t   * wavefile_create             ( void );
void           wavefile_destroy            ( wavefile_t * wavefile );

int            wavefile_write_open         ( wavefile_t * wavefile, char * filename, uint8_t wavetype );
int            wavefile_write_data         ( wavefile_t * wavefile, uint8_t * data, int bytes, int samples, uint8_t wavetype );
int            wavefile_write_voice        ( wavefile_t * wavefile, int16_t * voice, int samples );
int            wavefile_write_set_end      ( wavefile_t * wavefile );

int            wavefile_read_open          ( wavefile_t * wavefile, char * filename );
int            wavefile_read_data          ( wavefile_t * wavefile, uint8_t * data, int bytes, uint8_t * wavetype );
int            wavefile_read_voice         ( wavefile_t * wavefile, int16_t * voice, int samples );

int            wavefile_close              ( wavefile_t * wavefile );

int            wavefile_set_wavetype       ( wavefile_t * wavefile, uint8_t wavetype );
uint8_t        wavefile_get_wavetype       ( wavefile_t * wavefile );

uint32_t       wavefile_get_bytes          ( wavefile_t * wavefile );
uint32_t       wavefile_get_samples        ( wavefile_t * wavefile );
uint32_t       wavefile_get_seconds        ( wavefile_t * wavefile );

#endif //WAVE_H
