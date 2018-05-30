/******************************************************************************/
/* WAVE-FILES UTILITES                                                        */
/* wave.c                                                                     */
/* (c) Sergei Mashkin, 2015                                                   */
/******************************************************************************/

#include "wave.h"

#define MYLOGDEVICE 0 //0=MYLOGDEVICE_NOLOGS
#include "mylog.h"

#include <string.h>
#include "g711super.h"
#include "gsm.h"      //GSM0610    (13 kbit/s)

//types of audio coding type (look for standards)
#define  WAV_PCM      0x0001  //PCM uncompressed
#define  WAV_PCMU     0x0007  //ITU G.711 u-law
#define  WAV_PCMA     0x0006  //ITU G.711 a-law
#define  WAV_GSM610   0x0031  //GSM 6.10
#define  WAV_ADPCM    0x0002  //Microsoft ADPCM

/*************************************************************/
/* WAVE-HEADER FUNCTIONS                                     */
/*************************************************************/

//------------------------------------------------------------
//Create waveheader structure
waveheader_t * waveheader_create ( void )
{
        MYLOG_TRACE("@ ()");
        waveheader_t * header;
        header = (waveheader_t *) malloc( sizeof(waveheader_t) );
        if(header == NULL) {
                MYLOG_ERROR("Could not allocate memory for waveheader_t");
                return NULL;
        }
        //set default values
        (void) waveheader_set_default( header, WAVETYPE_DEFAULT );
        return(header);
}

//------------------------------------------------------------
//Destroy waveheader structure
void waveheader_destroy ( waveheader_t * header )
{
        if(header) {
                free( header );
        }
        return;
}

//------------------------------------------------------------
//Set default values to waveheader
//inputs:  header   = pointer to wav-header structure
//         wavetype = type of wave-file: WAVTYPE_MONO_8000HZ_PCM16 etc...
//returns:  0 = ok
//         -1 = error
int waveheader_set_default( waveheader_t * header, uint8_t wavetype )
{
        int i;

        MYLOG_TRACE("@ (header=%p,wavetype=%u)",header,wavetype);

        if(header==NULL) {
                MYLOG_ERROR("Invalid input parameter: header=NULL");
                return(-1);
        }

        header->riff[0] = 'R';
        header->riff[1] = 'I';
        header->riff[2] = 'F';
        header->riff[3] = 'F';
        header->wave[0] = 'W';
        header->wave[1] = 'A';
        header->wave[2] = 'V';
        header->wave[3] = 'E';
        header->fmt[0]  = 'f';
        header->fmt[1]  = 'm';
        header->fmt[2]  = 't';
        header->fmt[3]  = ' ';
        for(i=0; i<sizeof(header->extra_format_data); i++) { header->extra_format_data[i] = 0; }
        header->fact[0] = 'f';
        header->fact[1] = 'a';
        header->fact[2] = 'c';
        header->fact[3] = 't';
        for(i=0; i<sizeof(header->fact_data); i++) { header->fact_data[i] = 0; }
        header->data[0] = 'd';
        header->data[1] = 'a';
        header->data[2] = 't';
        header->data[3] = 'a';

        switch(wavetype)
        {
        case WAVETYPE_MONO_8000HZ_PCM16:
                header->file_length        = 0;        //! (file_length-8) - must be calculated when close file
                header->fmt_length         = 16;
                header->type               = WAV_PCM;
                header->channels           = 1;
                header->samples_per_second = 8000;
                header->bytes_per_second   = 16000;
                header->block_align        = 2;
                header->bits_per_sample    = 16;
                header->extra_format_bytes = 0;
                header->fact_length        = 0;        //! do not use 'fact' chunk for uncompressed data
                header->fact_data[0]       = 0;        //! samples - must be calculated when close file
                header->fact_data[1]       = 0;        //!
                header->fact_data[2]       = 0;        //!
                header->fact_data[3]       = 0;        //!
                header->data_length        = 0;        //! (length of data) - must be calculated when close file
                return(0);

        case WAVETYPE_MONO_8000HZ_PCMA:
                header->file_length        = 0;        //! (file_length-8) - must be calculated when close file
                header->fmt_length         = 18;
                header->type               = WAV_PCMA;
                header->channels           = 1;
                header->samples_per_second = 8000;
                header->bytes_per_second   = 8000;
                header->block_align        = 1;
                header->bits_per_sample    = 8;
                header->extra_format_bytes = 0;
                header->fact_length        = 4;
                header->fact_data[0]       = 0;        //! samples - must be calculated when close file
                header->fact_data[1]       = 0;        //!
                header->fact_data[2]       = 0;        //!
                header->fact_data[3]       = 0;        //!
                header->data_length        = 0;        //! (length of data) - must be calculated when close file
                return(0);

        case WAVETYPE_MONO_8000HZ_PCMU:
                header->file_length        = 0;        //! (file_length-8) - must be calculated when close file
                header->fmt_length         = 18;
                header->type               = WAV_PCMU;
                header->channels           = 1;
                header->samples_per_second = 8000;
                header->bytes_per_second   = 8000;
                header->block_align        = 1;
                header->bits_per_sample    = 8;
                header->extra_format_bytes = 0;
                header->fact_length        = 4;
                header->fact_data[0]       = 0;        //! samples - must be calculated when close file
                header->fact_data[1]       = 0;        //!
                header->fact_data[2]       = 0;        //!
                header->fact_data[3]       = 0;        //!
                header->data_length        = 0;        //! (length of data) - must be calculated when close file
                return(0);

#ifdef SUPP_GSM
        case WAVETYPE_MONO_8000HZ_GSM610:
                header->file_length        = 0;        //! (file_length-8) - must be calculated when close file
                header->fmt_length         = 20;
                header->type               = WAV_GSM610;
                header->channels           = 1;
                header->samples_per_second = 8000;
                header->bytes_per_second   = 1625;
                header->block_align        = 65;
                header->bits_per_sample    = 0;
                header->extra_format_bytes = 2;
                header->extra_format_data[0] = 320        & 0xFF; //320 samples per block
                header->extra_format_data[1] = (320 >> 8) & 0xFF;
                header->fact_length        = 4;
                header->fact_data[0]       = 0;        //! samples - must be calculated when close file
                header->fact_data[1]       = 0;        //!
                header->fact_data[2]       = 0;        //!
                header->fact_data[3]       = 0;        //!
                header->data_length        = 0;        //! (length of data) - must be calculated when close file
                return(0);
#endif // SUPP_GSM

        default:
                MYLOG_ERROR("Invalid input parameter: wavtype=%d", wavetype);
                return(-1);
        }
        return(0);
}

//------------------------------------------------------------
//Write header to opened wav-file
//inputs:  fid    = file id
//         header = pointer to wav-header structure
//returns:  0 = ok
//         -1 = error
int waveheader_write( FILE * fp, waveheader_t * header )
{
        int err;

        MYLOG_TRACE("@ (fp=%p,header=%p)",fp,header);

        //check input parameters
        if(fp==NULL) {
                MYLOG_ERROR("File is not opened!");
                return(-1);
        }
        if(header==NULL) {
                MYLOG_ERROR("Invalid input parameter: header=NULL");
                return(-1);
        }
        //Go to the begining of file
        err = fseek(fp,0L,SEEK_SET);
        if(err) {
                MYLOG_ERROR("Could not go to the begining of file!");
                return(-1);
        }
        //write header to file element by element:
        if( 1 != fwrite( &(header->riff[0]), sizeof(header->riff), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->file_length), sizeof(header->file_length), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->wave[0]), sizeof(header->wave), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->fmt[0]), sizeof(header->fmt), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->fmt_length), sizeof(header->fmt_length), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->type), sizeof(header->type), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->channels), sizeof(header->channels), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->samples_per_second), sizeof(header->samples_per_second), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->bytes_per_second), sizeof(header->bytes_per_second), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->block_align), sizeof(header->block_align), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->bits_per_sample), sizeof(header->bits_per_sample), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( header->extra_format_bytes > 0 ) {
                if( 1 != fwrite( &(header->extra_format_bytes), sizeof(header->extra_format_bytes), 1, fp ) ) {
                        MYLOG_ERROR("Could not write data to file");
                        return(-1);
                }
                if( 1 != fwrite( &(header->extra_format_data[0]), header->extra_format_bytes, 1, fp ) ) {
                        MYLOG_ERROR("Could not write data to file");
                        return(-1);
                }
        }
        if( header->fact_length > 0 ) {
                if( 1 != fwrite( &(header->fact[0]), sizeof(header->fact), 1, fp ) ) {
                        MYLOG_ERROR("Could not write data to file");
                        return(-1);
                }
                if( 1 != fwrite( &(header->fact_length), sizeof(header->fact_length), 1, fp ) ) {
                        MYLOG_ERROR("Could not write data to file");
                        return(-1);
                }
                if( header->fact_length > 0 ) {
                        if( 1 != fwrite( &(header->fact_data[0]), header->fact_length, 1, fp ) ) {
                                MYLOG_ERROR("Could not write data to file");
                                return(-1);
                        }
                }
        }
        if( 1 != fwrite( &(header->data[0]), sizeof(header->data), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        if( 1 != fwrite( &(header->data_length), sizeof(header->data_length), 1, fp ) ) {
                MYLOG_ERROR("Could not write data to file");
                return(-1);
        }
        return(0);
}

//------------------------------------------------------------
//Read header of opened wav-file
//inputs:  fp    = file id
//         header = pointer to wav-header structure
//returns:  0 = ok
//         -1 = error
int waveheader_read( FILE * fp, waveheader_t * header )
{
        int err;
        int i;

        MYLOG_TRACE("@ (fp=%p,header=%p)",fp,header);

        //check input parameters
        if(fp==NULL) {
                MYLOG_ERROR("File is not opened!");
                return(-1);
        }
        if(header==NULL) {
                MYLOG_ERROR("Invalid input parameter: header=NULL");
                return(-1);
        }
        //Go to the begining of file
        err = fseek(fp,0L,SEEK_SET);
        if(err) {
                MYLOG_ERROR("Could not go to the begining of file!");
                return(-1);
        }
        //read header from file element by element:
        //RIFF
        if( 1 != fread( &(header->riff[0]), sizeof(header->riff), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( header->riff[0]!='R' ||
            header->riff[1]!='I' ||
            header->riff[2]!='F' ||
            header->riff[3]!='F'   ) {
                MYLOG_ERROR("Invalid format: no RIFF");
                return(-1);
        }
        if( 1 != fread( &(header->file_length), sizeof(header->file_length), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        //WAVE
        if( 1 != fread( &(header->wave[0]), sizeof(header->wave), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( header->wave[0]!='W' ||
            header->wave[1]!='A' ||
            header->wave[2]!='V' ||
            header->wave[3]!='E'   ) {
                MYLOG_ERROR("Invalid format: no WAVE");
                return(-1);
        }
        //fmt
        if( 1 != fread( &(header->fmt[0]), sizeof(header->fmt), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( header->fmt[0]!='f' ||
            header->fmt[1]!='m' ||
            header->fmt[2]!='t' ||
            header->fmt[3]!=' '   ) {
                MYLOG_ERROR("Invalid format: no fmt");
                return(-1);
        }
        if( 1 != fread( &(header->fmt_length), sizeof(header->fmt_length), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( header->fmt_length < 16 ) {
                MYLOG_ERROR("Invalid format: bad fmt_length=%d", header->fmt_length);
                return(-1);
        }
        if( 1 != fread( &(header->type), sizeof(header->type), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( 1 != fread( &(header->channels), sizeof(header->channels), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( 1 != fread( &(header->samples_per_second), sizeof(header->samples_per_second), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( 1 != fread( &(header->bytes_per_second), sizeof(header->bytes_per_second), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( 1 != fread( &(header->block_align), sizeof(header->block_align), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( 1 != fread( &(header->bits_per_sample), sizeof(header->bits_per_sample), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( header->fmt_length >= 18 ) {
                if( 1 != fread( &(header->extra_format_bytes), sizeof(header->extra_format_bytes), 1, fp ) ) {
                        MYLOG_ERROR("Could not read data from file");
                        return(-1);
                }
                if( header->extra_format_bytes > sizeof(header->extra_format_data) ) {
                        MYLOG_ERROR("Invalid format: bad extra_format_bytes=%d", header->extra_format_bytes);
                        return(-1);
                }
                if(header->extra_format_bytes > 0) {
                        if( 1 != fread( &(header->extra_format_data[0]), header->extra_format_bytes, 1, fp ) ) {
                                MYLOG_ERROR("Could not read data from file");
                                return(-1);
                        }
                }
        }
        //fact
        if( 1 != fread( &(header->fact[0]), sizeof(header->fact), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }
        if( (header->fact[0]=='f') &&
            (header->fact[1]=='a') &&
            (header->fact[2]=='c') &&
            (header->fact[3]=='t')   )
        {
                //if(header->type != WAV_PCM) {
                //        MYLOG_ERROR("Invalid file format: for compressed data 'fact' must exist!");
                //        return(-1);
                //}
                if( 1 != fread( &(header->fact_length), sizeof(header->fact_length), 1, fp ) ) {
                        MYLOG_ERROR("Could not read data from file");
                        return(-1);
                }
                if(header->fact_length!=4) {
                        MYLOG_ERROR("Invalid format: fact_length=%d (must be 4)!", header->fact_length);
                        return(-1);
                }
                if( 1 != fread( &(header->fact_data[0]), header->fact_length, 1, fp ) ) {
                        MYLOG_ERROR("Could not read data from file");
                        return(-1);
                }
                //data
                if( 1 != fread( &(header->data[0]), sizeof(header->data), 1, fp ) ) {
                        MYLOG_ERROR("Could not read data from file");
                        return(-1);
                }
        }
        else if( (header->fact[0]=='d') &&
                 (header->fact[1]=='a') &&
                 (header->fact[2]=='t') &&
                 (header->fact[3]=='a')   )
        {
                //data
                header->data[0]='d';
                header->data[1]='a';
                header->data[2]='t';
                header->data[3]='a';
                //fill 'fact' with default values
                header->fact[0] = 'f';
                header->fact[1] = 'a';
                header->fact[2] = 'c';
                header->fact[3] = 't';
                header->fact_length = 0;
                for(i=0; i<sizeof(header->fact_data); i++) { header->fact_data[i] = 0; }
        }
        //data
        if( 1 != fread( &(header->data_length), sizeof(header->data_length), 1, fp ) ) {
                MYLOG_ERROR("Could not read data from file");
                return(-1);
        }

        //check type and parameters are supported
        if( header->type==WAV_PCM ) {
                if(header->fmt_length<16) {
                        MYLOG_ERROR("Invalid format: invalid fmt_length=%d for PCM format", header->fmt_length);
                        return(-1);
                }
        }
        else if( header->type==WAV_PCMA ) {
                if(header->fmt_length<16) {
                        MYLOG_ERROR("Invalid format: invalid fmt_length=%d for PCMA format", header->fmt_length);
                        return(-1);
                }
        }
        else if( header->type==WAV_PCMU ) {
                if(header->fmt_length<16) {
                        MYLOG_ERROR("Invalid format: invalid fmt_length=%d for PCMU format", header->fmt_length);
                        return(-1);
                }
        }
#ifdef SUPP_GSM
        else if( header->type==WAV_GSM610 ) {
                if(header->fmt_length!=20) {
                        MYLOG_ERROR("Invalid format: invalid fmt_length=%d for GSM610 format", header->fmt_length);
                        return(-1);
                }
        }
#endif // SUPP_GSM
        else {
                MYLOG_ERROR("Unsupported type of wave: %d", header->type);
                return(-1);
        }

        MYLOG_TRACE(".header->fact_length = %u",header->fact_length);
        MYLOG_TRACE(".header->fact_data   = %02X %02X %02X %02X",header->fact_data[0],header->fact_data[1],header->fact_data[2],header->fact_data[3]);

        return(0);
}

//------------------------------------------------------------
//Get default size of waveheader
//inputs:   wavetype = type of wave-file: WAVTYPE_MONO_8000HZ_PCM16 etc...
//returns:  bytes    = size of waveheader
//         -1 = error
int waveheader_get_default_size(uint8_t wavetype)
{
        switch(wavetype)
        {
        case WAVETYPE_MONO_8000HZ_PCM16:   return(44);
        case WAVETYPE_MONO_8000HZ_PCMA:    return(58);
        case WAVETYPE_MONO_8000HZ_PCMU:    return(58);
#ifdef SUPP_GSM
        case WAVETYPE_MONO_8000HZ_GSM610:  return(60);
#endif //SUPP_GSM
        default:                           return(-1);
        }
}

/*************************************************************/
/* WAVE-FILE FUNCTIONS                                       */
/*************************************************************/

//------------------------------------------------------------
//Create wavefile_t structure
//returns: wavefile = pointer to created wavefile
//         NULL     = error
wavefile_t * wavefile_create ( void )
{
        wavefile_t * wavefile;

        MYLOG_TRACE("@ ()");

        //allocate memory
        wavefile = (wavefile_t *) malloc( sizeof(wavefile_t) );
        if(wavefile==NULL) {
                MYLOG_ERROR("Could not allocate memory for wavefile");
                return NULL;
        }

        //initialize variables
        wavefile->wavetype        = WAVETYPE_NOTSET;
        wavefile->filename[0]     = '\0';
        wavefile->rwmode          = WAVEFILE_RWMODE_NOTSET;

        wavefile->fp              = NULL;
        wavefile->pos             = 0;
        wavefile->end_samples     = 0;
        wavefile->end_bytes       = 0;
        wavefile->total_samples   = 0;
        wavefile->total_bytes     = 0;

        wavefile->header          = NULL;
        wavefile->vocoder         = NULL;

        wavefile->data8_bytes     = 0;
        wavefile->voice16_samples = 0;

        return wavefile;
}

//------------------------------------------------------------
//Destroy wavefile structure
void wavefile_destroy ( wavefile_t * wavefile )
{
        MYLOG_TRACE("@ (wavefile=%p)",wavefile);

        if(wavefile == NULL) {
                return; //nothing to destroy
        }
        //close wavefile
        if(wavefile->fp) {
                wavefile_close( wavefile );
        }
        //destroy wavefile
        if(wavefile->header) {
                free(wavefile->header);
        }
        free(wavefile);
        return;
}

//------------------------------------------------------------
int wavefile_close ( wavefile_t * wavefile )
{
        int fid;
        int err;

        MYLOG_TRACE("@ (wavefile=%p)",wavefile);

        //Check input parameters
        if(wavefile==NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        //Check if wavefile is already closed
        if(wavefile->fp==NULL) {
                MYLOG_WARNING("Wavefile is already closed");
                return(0);
        }

        //1.Destroy vocoder
        if(wavefile->vocoder) {
#ifdef SUPP_GSM
                if( wavefile->wavetype==WAVETYPE_MONO_8000HZ_GSM610 )
                        gsm_destroy(wavefile->vocoder);
                else
#endif //SUPP_GSM
                        free(wavefile->vocoder);
                wavefile->vocoder = NULL;
        }

        //2.Close file
        switch(wavefile->rwmode)
        {
        case WAVEFILE_RWMODE_READ:
                //Close file
                fclose(wavefile->fp);
                wavefile->fp = NULL;
                return(0);

        case WAVEFILE_RWMODE_WRITE:
                //Finalize file
                if( wavefile->end_samples != 0 ) {
                        //leave only end_samples in file (truncate silence samples)
                        wavefile->total_samples = wavefile->end_samples;
                        wavefile->total_bytes   = wavefile->end_bytes;
                }

                MYLOG_TRACE("!wavefile->end_samples   = %u",wavefile->end_samples  );
                MYLOG_TRACE("!wavefile->end_bytes     = %u",wavefile->end_bytes    );
                MYLOG_TRACE("!wavefile->total_samples = %u",wavefile->total_samples);
                MYLOG_TRACE("!wavefile->total_bytes   = %u",wavefile->total_bytes  );

                //Fill waveheader
                wavefile->header->file_length = waveheader_get_default_size(wavefile->wavetype) + wavefile->total_bytes - 8;
                if(wavefile->header->fact_length >= 4) {
                        wavefile->header->fact_data[0] = (uint8_t)((wavefile->total_samples >> 0 ) & 0x000000FFL);
                        wavefile->header->fact_data[1] = (uint8_t)((wavefile->total_samples >> 8 ) & 0x000000FFL);
                        wavefile->header->fact_data[2] = (uint8_t)((wavefile->total_samples >> 16) & 0x000000FFL);
                        wavefile->header->fact_data[3] = (uint8_t)((wavefile->total_samples >> 24) & 0x000000FFL);
                }
                wavefile->header->data_length = wavefile->total_bytes;

                //(re)Write waveheader
                waveheader_write( wavefile->fp, wavefile->header );

                //Truncate wavefile
                fid = fileno(wavefile->fp);
                if(fid==-1) {
                        fclose(wavefile->fp);
                        wavefile->fp = NULL;
                        MYLOG_ERROR("Could not get fid! Wavefile closed!");
                        return(-1);
                }
                err = ftruncate(fid, wavefile->header->file_length + 8);
                if(err) {
                        MYLOG_WARNING("Could not trancate wavefile: %d: %s",errno,strerror(errno));
                }
                //Close file
                fclose(wavefile->fp);
                wavefile->fp = NULL;
                return(0);

        default:
                MYLOG_ERROR("Unexpected rwmode: %d",wavefile->rwmode);
                //close file
                fclose(wavefile->fp);
                wavefile->fp = NULL;
                return(-1);
        }
}

//------------------------------------------------------------
int wavefile_write_open ( wavefile_t * wavefile, char * filename, uint8_t wavetype )
{
        int err;

        MYLOG_TRACE("@ (wavefile=%p,filename=%s,wavetype=%u",wavefile,filename,wavetype);

        //Check input arguments
        if(wavefile == NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        if(filename == NULL) {
                MYLOG_ERROR("Invalid value: filename=NULL");
                return(-1);
        }
        //Check if wavefile is already in process
        if(wavefile->fp) {
                MYLOG_ERROR("wavefile is already used (filename=%s)! Close it before!",wavefile->filename);
                return(-1);
        }
        //Set wave-file structure
        snprintf( wavefile->filename, sizeof(wavefile->filename), filename );
        wavefile->wavetype        = wavetype;
        wavefile->end_samples     = 0;
        wavefile->end_bytes       = 0;
        wavefile->total_samples   = 0;
        wavefile->total_bytes     = 0;
        wavefile->rwmode          = WAVEFILE_RWMODE_WRITE;
        wavefile->data8_bytes     = 0;
        wavefile->voice16_samples = 0;

        //Create/init vocoder if it is needed
        init_g711();
        switch(wavefile->wavetype)
        {
        case WAVETYPE_MONO_8000HZ_PCM16:
        case WAVETYPE_MONO_8000HZ_PCMA:
        case WAVETYPE_MONO_8000HZ_PCMU:
                //do nothing
                break;
#ifdef SUPP_GSM
        case WAVETYPE_MONO_8000HZ_GSM610:
                //create vocoder
                if(wavefile->vocoder==NULL) {
                        wavefile->vocoder = gsm_create();
                        if(wavefile->vocoder==NULL) {
                                MYLOG_ERROR("Could not create GSM0610 vocoder!");
                                fclose(wavefile->fp);
                                wavefile->fp = NULL;
                                return(-1);
                        }
                }
                //init vocoder
                gsm_init(wavefile->vocoder);
                //set WAV49 option
                {
                        int one;
                        one = 1;
                        gsm_option(wavefile->vocoder, GSM_OPT_WAV49, &one);
                }
                break;
#endif //SUPP_GSM
        default:
                MYLOG_ERROR("Invalid value: wavetype=%u",wavetype);
                wavefile_close(wavefile);
                return(-1);
        }

        //Set header
        wavefile->header = waveheader_create();
        if(wavefile->header == NULL) {
                MYLOG_ERROR("Could not allocate memory for wavefile->header");
                wavefile_close(wavefile);
                return(-1);
        }
        err = waveheader_set_default( wavefile->header, wavefile->wavetype );
        if(err) {
                MYLOG_ERROR("Function waveheader_set_default failed!");
                wavefile_close(wavefile);
                return(-1);
        }

        //Try to open file
        wavefile->fp = fopen(filename,"w");
        if(wavefile->fp==NULL) {
                MYLOG_ERROR("Could not open file (filename=%s) in write mode: %d: %s",filename,errno,strerror(errno));
                wavefile_close(wavefile);
                return(-1);
        }

        //Write header
        err = waveheader_write( wavefile->fp, wavefile->header );
        if(err) {
                MYLOG_ERROR("Could not write waveheader to file");
                wavefile_close(wavefile);
                return(-1);
        }
        MYLOG_TRACE("wavefile (filename=%s) was successfuly opened in write mode", wavefile->filename);
        return(0);
}

//------------------------------------------------------------
//Write encoded data to opened wavefile
//inputs: wavefile = pointer to wavefile
//        data     = pointer to data bytes
//        bytes    = number of data bytes
//        samples  = number of samples
//        wavetype = type of encoded data
//returns: 0 = ok
//        -1 = error
int wavefile_write_data ( wavefile_t * wavefile, uint8_t * data, int bytes, int samples, uint8_t wavetype )
{
        int n;

        MYLOG_TRACE("@ (wavefile=%p,data=%p,bytes=%d,samples=%d,wavetype=%u)",wavefile,data,bytes,samples,wavetype);

        //Check input parameters
        if(wavefile == NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        if(wavefile->fp == NULL) {
                MYLOG_ERROR("Wavefile is not opened!");
                return(-1);
        }
        if(wavefile->rwmode != WAVEFILE_RWMODE_WRITE) {
                MYLOG_ERROR("Wavefile is not opened in write mode!");
                return(-1);
        }
        if(data==NULL) {
                MYLOG_ERROR("Invalid value: data=NULL");
                return(-1);
        }
        if(bytes<0) {
                MYLOG_ERROR("Invalid value: bytes=%d",bytes);
                return(-1);
        }
        if(samples<0) {
                MYLOG_ERROR("Invalid value: samples=%d",samples);
                return(-1);
        }
        if(wavetype != wavefile->wavetype) {
                MYLOG_ERROR("Invalid wavetype of data to be written: wavefile->wavetype=%d, data->wavetype=%d",
                            wavefile->wavetype, wavetype);
                return(-1);
        }
        if(samples==0) {
                //write nothing
                return(0);
        }

        //Write data to file (without conversion)
        switch(wavefile->wavetype)
        {
        case WAVETYPE_MONO_8000HZ_PCM16:
                if( samples<<1 != bytes ) {
                        MYLOG_ERROR("Invalid value: PCM16 samples=%d,bytes=%d",samples,bytes);
                        return(-1);
                }
                break;

        case WAVETYPE_MONO_8000HZ_PCMA:
        case WAVETYPE_MONO_8000HZ_PCMU:
                if( samples != bytes ) {
                        MYLOG_ERROR("Invalid value: PCMU/PCMA samples=%d,bytes=%d",samples,bytes);
                        return(-1);
                }
                break;

#ifdef SUPP_GSM
        case WAVETYPE_MONO_8000HZ_GSM610:
                if( samples*65 != bytes*320 ) {
                        MYLOG_ERROR("Invalid value: GSM610 samples=%d,bytes=%d",samples,bytes);
                        return(-1);
                }
                break;
#endif //SUPP_GSM

        default:
                MYLOG_ERROR("Unsupported wavefile->wavetype=%d",wavefile->wavetype);
                return(-1);
        }

        //Write data to file (without conversion)
        n = fwrite( data, sizeof(uint8_t), bytes, wavefile->fp );
        if(n != bytes) {
                MYLOG_ERROR( "Could not write bytes! %d bytes of %d have been written: %d:%s",
                             n,bytes,errno,strerror(errno) );
                return(-1);
        }
        wavefile->total_samples += samples;
        wavefile->total_bytes   += bytes;
        return(0);
}

//------------------------------------------------------------
//Write samples to opened wavefile
//inputs: wavefile = pointer to wavefile
//        voice    = pointer to buffer with samples
//        samples  = number of samples
//returns: 0 = ok
//        -1 = error
int wavefile_write_voice ( wavefile_t * wavefile, int16_t * voice, int samples )
{
        int       i;
        int       n;

        //Check input parameters
        if(wavefile == NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        if(wavefile->fp == NULL) {
                MYLOG_ERROR("Wavefile is not opened!");
                return(-1);
        }
        if(wavefile->rwmode != WAVEFILE_RWMODE_WRITE) {
                MYLOG_ERROR("Wavefile is not opened in write mode!");
                return(-1);
        }
        if(voice==NULL) {
                MYLOG_ERROR("Invalid value: voice=NULL");
                return(-1);
        }
        if(samples<0) {
                MYLOG_ERROR("Invalid value: samples=%d",samples);
                return(-1);
        }
        if(samples==0) {
                //write nothing
                return(0);
        }

        //convert data and write to file
        switch(wavefile->wavetype)
        {
        case WAVETYPE_MONO_8000HZ_PCM16:
                n = fwrite( voice, sizeof(int16_t), samples, wavefile->fp );
                if(n != samples) {
                        MYLOG_ERROR( "Could not write PCM16 samples! %d samples of %d have been written: %d:%s",
                                     n,samples,errno,strerror(errno) );
                        return(-1);
                }
                wavefile->total_samples += samples;
                wavefile->total_bytes   += (samples << 1);
                return(0);

        case WAVETYPE_MONO_8000HZ_PCMA:
                for(i=0; i<samples; i++) {
                        wavefile->data8[0] = linear2alaw( voice[i] );
                        n = fwrite( wavefile->data8, sizeof(uint8_t), 1, wavefile->fp );
                        if(n != 1) {
                                MYLOG_ERROR( "Could not write PCMA sample: %d:%s",
                                             errno,strerror(errno) );
                                return(-1);
                        }
                }
                wavefile->total_samples += samples;
                wavefile->total_bytes   += samples;
                return(0);

        case WAVETYPE_MONO_8000HZ_PCMU:
                for(i=0; i<samples; i++) {
                        wavefile->data8[0] = linear2mulaw( voice[i] );
                        n = fwrite( wavefile->data8, sizeof(uint8_t), 1, wavefile->fp );
                        if(n != 1) {
                                MYLOG_ERROR( "Could not write PCMA sample: %d:%s",
                                             errno,strerror(errno) );
                                return(-1);
                        }
                }
                wavefile->total_samples += samples;
                wavefile->total_bytes   += samples;
                return(0);

#ifdef SUPP_GSM
        case WAVETYPE_MONO_8000HZ_GSM610:
                if(wavefile->vocoder == NULL) {
                        MYLOG_ERROR("wavefile->vocoder=NULL!");
                        return(-1);
                }
                for(i=0; i<samples; i++) {
                        if(wavefile->voice16_samples >= 320) {
                                //convert block of 320 samples in voice[] to
                                //65 compressed data bytes (MS GSM frame format) in data8[]
                                // encode 1st frame
                                gsm_encode( wavefile->vocoder, (gsm_signal *)(wavefile->voice16), (uint8_t *)(wavefile->data8) );
                                // encode 2nd frame
                                gsm_encode( wavefile->vocoder, (gsm_signal *)(wavefile->voice16+160), (uint8_t *)(wavefile->data8+32) );

                                //write 65 GSM bytes from data8[] to file
                                n = fwrite( wavefile->data8, sizeof(uint8_t), 65, wavefile->fp );
                                if(n!=65) {
                                        MYLOG_ERROR( "Could not write GSM610 frame: %d bytes of %d have been written: %d:%s",
                                                    n,65,errno,strerror(errno) );
                                }
                                wavefile->voice16_samples -= 320;
                                wavefile->total_bytes    += 65;
                                wavefile->total_samples  += 320;
                        }
                        wavefile->voice16[wavefile->voice16_samples++] = voice[i];
                }
                return(0);
#endif //SUPP_GSM

        default:
                MYLOG_ERROR("Invalid wavetype=%d",wavefile->wavetype);
                return(-1);
        }
}

//------------------------------------------------------------
int wavefile_write_set_end ( wavefile_t * wavefile )
{
        MYLOG_TRACE("@ (wavefile=%p)",wavefile);

        //Check input parameters
        if(wavefile==NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        if(wavefile->fp==NULL) {
                MYLOG_ERROR("wavefile is not opened yet!");
                return(-1);
        }
        //store info about last speech place
        wavefile->end_samples = wavefile->total_samples;
        wavefile->end_bytes   = wavefile->total_bytes;
        return(0);
}

//------------------------------------------------------------
int wavefile_read_open ( wavefile_t * wavefile, char * filename )
{
        int err;

        //Check input arguments
        if(wavefile == NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        if(filename == NULL) {
                MYLOG_ERROR("Invalid value: filename=NULL");
                return(-1);
        }
        //Check if wavefile is already in process
        if(wavefile->fp) {
                MYLOG_ERROR("wavefile is already used (filename=%s)! Close it before!",wavefile->filename);
                return(-1);
        }
        //Try to open file
        wavefile->fp = fopen(filename,"r");
        if(wavefile->fp==NULL) {
                MYLOG_ERROR("Could not open file (filename=%s) in read mode: %d: %s",filename,errno,strerror(errno));
                wavefile_close(wavefile);
                return(-1);
        }

        //Read header
        wavefile->header = waveheader_create();
        if(wavefile->header == NULL) {
                MYLOG_ERROR("Could not allocate memory for wavefile->header");
                wavefile_close(wavefile);
                return(-1);
        }
        err = waveheader_read( wavefile->fp, wavefile->header );
        if(err) {
                MYLOG_ERROR("Could not read waveheader from file");
                wavefile_close(wavefile);
                return(-1);
        }

        //Set wave-file structure
        snprintf( wavefile->filename, sizeof(wavefile->filename), filename );
        switch(wavefile->header->type)
        {
        case WAV_PCM:
                if( (wavefile->header->channels           != 1    ) ||
                    (wavefile->header->samples_per_second != 8000 ) ||
                    (wavefile->header->bytes_per_second   != 16000) ||
                    (wavefile->header->block_align        != 2    ) ||
                    (wavefile->header->bits_per_sample    != 16   ) ||
                    (wavefile->header->extra_format_bytes != 0    )   )
                {
                        MYLOG_ERROR( "Unsupported type of PCM wavefile: chans=%d sps=%d bps=%d blalign=%d bits=%d exfmtbytes=%d\n",
                                     wavefile->header->channels,
                                     wavefile->header->samples_per_second,
                                     wavefile->header->bytes_per_second,
                                     wavefile->header->block_align,
                                     wavefile->header->bits_per_sample,
                                     wavefile->header->extra_format_bytes );
                }
                wavefile->wavetype = WAVETYPE_MONO_8000HZ_PCM16;
                break;
        case WAV_PCMU:
                if( (wavefile->header->channels           != 1    ) ||
                    (wavefile->header->samples_per_second != 8000 ) ||
                    (wavefile->header->bytes_per_second   != 8000 ) ||
                    (wavefile->header->block_align        != 1    ) ||
                    (wavefile->header->bits_per_sample    != 8    ) ||
                    (wavefile->header->extra_format_bytes != 0    )   )
                {
                        MYLOG_ERROR( "Unsupported type of PCMU wavefile: chans=%d sps=%d bps=%d blalign=%d bits=%d exfmtbytes=%d\n",
                                     wavefile->header->channels,
                                     wavefile->header->samples_per_second,
                                     wavefile->header->bytes_per_second,
                                     wavefile->header->block_align,
                                     wavefile->header->bits_per_sample,
                                     wavefile->header->extra_format_bytes );
                }
                wavefile->wavetype = WAVETYPE_MONO_8000HZ_PCMU;
                break;
        case WAV_PCMA:
                if( (wavefile->header->channels           != 1    ) ||
                    (wavefile->header->samples_per_second != 8000 ) ||
                    (wavefile->header->bytes_per_second   != 8000 ) ||
                    (wavefile->header->block_align        != 1    ) ||
                    (wavefile->header->bits_per_sample    != 8    ) ||
                    (wavefile->header->extra_format_bytes != 0    )   )
                {
                        MYLOG_ERROR( "Unsupported type of PCMA wavefile: chans=%d sps=%d bps=%d blalign=%d bits=%d exfmtbytes=%d\n",
                                     wavefile->header->channels,
                                     wavefile->header->samples_per_second,
                                     wavefile->header->bytes_per_second,
                                     wavefile->header->block_align,
                                     wavefile->header->bits_per_sample,
                                     wavefile->header->extra_format_bytes );
                }
                wavefile->wavetype = WAVETYPE_MONO_8000HZ_PCMA;
                break;
#ifdef SUPP_GSM
        case WAV_GSM610:
                if( (wavefile->header->channels             != 1                   ) ||
                    (wavefile->header->samples_per_second   != 8000                ) ||
                    (wavefile->header->bytes_per_second     != 1625                ) ||
                    (wavefile->header->block_align          != 65                  ) ||
                    (wavefile->header->bits_per_sample      != 0                   ) ||
                    (wavefile->header->extra_format_bytes   != 2                   ) ||
                    (wavefile->header->extra_format_data[0] != (320        & 0xFF) ) ||  //samples per block
                    (wavefile->header->extra_format_data[1] != ((320 >> 8) & 0xFF) )   ) //samples per block
                {
                        MYLOG_ERROR( "Unsupported type of GSM610 wavefile: chans=%d sps=%d bps=%d blalign=%d bits=%d exfmtbytes=%d exfmtdata[0]=%d exfmtdata[1]=%d\n",
                                     wavefile->header->channels,
                                     wavefile->header->samples_per_second,
                                     wavefile->header->bytes_per_second,
                                     wavefile->header->block_align,
                                     wavefile->header->bits_per_sample,
                                     wavefile->header->extra_format_bytes,
                                     wavefile->header->extra_format_data[0],
                                     wavefile->header->extra_format_data[1]  );
                }
                wavefile->wavetype = WAVETYPE_MONO_8000HZ_GSM610;
                break;
#endif //SUPP_GSM

        default:
                MYLOG_ERROR("Unsupported type of wavefile: header->type=%d",wavefile->header->type);
                wavefile_close(wavefile);
                return(-1);
        }

        if( wavefile->header->fact_length >= 4 ) {
                wavefile->end_samples = ((uint32_t)(wavefile->header->fact_data[0]) << 0 ) +
                                        ((uint32_t)(wavefile->header->fact_data[1]) << 8 ) +
                                        ((uint32_t)(wavefile->header->fact_data[2]) << 16) +
                                        ((uint32_t)(wavefile->header->fact_data[3]) << 24) ;
        }
        else {
                wavefile->end_samples = wavefile->header->data_length >> 1;  //PCM: 2 bytes = 1 sample
        }
        wavefile->total_samples = wavefile->end_samples;
        wavefile->end_bytes     = wavefile->header->data_length;
        wavefile->total_bytes   = wavefile->end_bytes;
        wavefile->rwmode        = WAVEFILE_RWMODE_READ;

        MYLOG_TRACE("wavefile->end_samples  =%u",wavefile->end_samples  );
        MYLOG_TRACE("wavefile->total_samples=%u",wavefile->total_samples);
        MYLOG_TRACE("wavefile->end_bytes    =%u",wavefile->end_bytes    );
        MYLOG_TRACE("wavefile->total_bytes  =%u",wavefile->total_bytes  );

        wavefile->data8_bytes     = 0;
        wavefile->voice16_samples = 0;

        //Create/init vocoder if it is needed
        init_g711();
        switch(wavefile->wavetype)
        {
        case WAVETYPE_MONO_8000HZ_PCM16:
        case WAVETYPE_MONO_8000HZ_PCMA:
        case WAVETYPE_MONO_8000HZ_PCMU:
                //do nothing
                break;
#ifdef SUPP_GSM
        case WAVETYPE_MONO_8000HZ_GSM610:
                //create vocoder
                if(wavefile->vocoder==NULL) {
                        wavefile->vocoder = gsm_create();
                        if(wavefile->vocoder==NULL) {
                                MYLOG_ERROR("Could not create GSM0610 vocoder!");
                                fclose(wavefile->fp);
                                wavefile->fp = NULL;
                                return(-1);
                        }
                }
                //init vocoder
                gsm_init(wavefile->vocoder);
                //set WAV49 option
                {
                        int one;
                        one = 1;
                        gsm_option(wavefile->vocoder, GSM_OPT_WAV49, &one);
                }
                break;
#endif //SUPP_GSM

        default:
                MYLOG_ERROR("Invalid value: wavefile->wavetype=%u",wavefile->wavetype);
                wavefile_close(wavefile);
                return(-1);
        }

        MYLOG_TRACE("wavefile (filename=%s) was successfuly opened in read mode", wavefile->filename);
        return(0);
}

//------------------------------------------------------------
//Read encoded data from opened wavefile
//inputs:  wavefile = pointer to wavefile
//         data     = destination buffer for readed data bytes
//         bytes    = number of bytes to be read
//outputs: wavetype = actual wavetype of readed bytes
//returns: 0 = ok
//         1 = end of file
//        -1 = error
int wavefile_read_data ( wavefile_t * wavefile, uint8_t * data, int bytes, uint8_t * wavetype )
{
        int n;

        MYLOG_TRACE("@ (wavefile=%p,data=%p,bytes=%d,wavetype=%p)",wavefile,data,bytes,wavetype);

        //Check input parameters
        if(wavefile == NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        if(wavefile->fp == NULL) {
                MYLOG_ERROR("Wavefile is not opened!");
                return(-1);
        }
        if(wavefile->rwmode != WAVEFILE_RWMODE_READ) {
                MYLOG_ERROR("Wavefile is not opened in read mode!");
                return(-1);
        }
        if(data==NULL) {
                MYLOG_ERROR("Invalid value: data=NULL");
                return(-1);
        }
        if(wavetype==NULL) {
                MYLOG_ERROR("Invalid value: wavetype=NULL");
                return(-1);
        }
        if(bytes<0) {
                MYLOG_ERROR("Invalid value: bytes=%d",bytes);
                return(-1);
        }
        if(bytes==0) {
                //read nothing
                *wavetype = wavefile->wavetype;
                return(0);
        }

        //Read data from file (without conversion)
        *wavetype = wavefile->wavetype;
        n = fread( data, sizeof(uint8_t), bytes, wavefile->fp );
        if(n != bytes) {
                if( feof(wavefile->fp) ) {
                        return(1); //end of file
                }
                else {
                        MYLOG_ERROR( "Could not read bytes! %d bytes of %d have been read: %d:%s",
                                     n,bytes,errno,strerror(errno) );
                        return(-1);
                }
        }
        return(0);
}

//------------------------------------------------------------
//Read samples from opened wavefile
//inputs:  wavefile = pointer to wavefile
//         voice    = destination buffer for readed samples
//         samples  = number of samples to be read
//returns: 0 = ok
//         1 = end of file
//        -1 = error
int wavefile_read_voice ( wavefile_t * wavefile, int16_t * voice, int samples )
{
        int       i;
        int       n;

        //Check input parameters
        if(wavefile == NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(-1);
        }
        if(wavefile->fp == NULL) {
                MYLOG_ERROR("Wavefile is not opened!");
                return(-1);
        }
        if(wavefile->rwmode != WAVEFILE_RWMODE_READ) {
                MYLOG_ERROR("Wavefile is not opened in read mode!");
                return(-1);
        }
        if(voice==NULL) {
                MYLOG_ERROR("Invalid value: voice=NULL");
                return(-1);
        }
        if(samples<0) {
                MYLOG_ERROR("Invalid value: samples=%d",samples);
                return(-1);
        }
        if(samples==0) {
                //read nothing
                return(0);
        }

        //read data and convert to voice
        switch(wavefile->wavetype)
        {
        case WAVETYPE_MONO_8000HZ_PCM16:
                n = fread( voice, sizeof(int16_t), samples, wavefile->fp );
                if(n != samples) {
                        if( feof(wavefile->fp) ) {
                                return(1); //end of file
                        }
                        else {
                                MYLOG_ERROR( "Could not read PCM16 samples! %d bytes of %d have been read: %d:%s",
                                             n,samples,errno,strerror(errno) );
                                return(-1);
                        }
                }
                return(0);

        case WAVETYPE_MONO_8000HZ_PCMA:
                for(i=0; i<samples; i++) {
                        n = fread( wavefile->data8, sizeof(uint8_t), 1, wavefile->fp );
                        if(n != 1) {
                                if( feof(wavefile->fp) ) {
                                        return(1); //end of file
                                }
                                else {
                                        MYLOG_ERROR( "Could not read PCMA samples: %d:%s",
                                                     errno,strerror(errno) );
                                        return(-1);
                                }
                        }
                        voice[i] = alaw2linear( wavefile->data8[0] );
                }
                return(0);

        case WAVETYPE_MONO_8000HZ_PCMU:
                for(i=0; i<samples; i++) {
                        n = fread( wavefile->data8, sizeof(uint8_t), 1, wavefile->fp );
                        if(n != 1) {
                                if( feof(wavefile->fp) ) {
                                        return(1); //end of file
                                }
                                else {
                                        MYLOG_ERROR( "Could not read PCMA samples: %d:%s",
                                                     errno,strerror(errno) );
                                        return(-1);
                                }
                        }
                        voice[i] = mulaw2linear( wavefile->data8[0] );
                }
                return(0);

#ifdef SUPP_GSM
        case WAVETYPE_MONO_8000HZ_GSM610:
                if(wavefile->vocoder == NULL) {
                        MYLOG_ERROR("wavefile->vocoder=NULL!");
                        return(-1);
                }
                for(i=0; i<samples; i++) {
                        if(wavefile->voice16_samples<=0) {
                                //read 65 GSM bytes from file to data8[]
                                n = fread( wavefile->data8, sizeof(uint8_t), 65, wavefile->fp );
                                if(n!=65) {
                                        if( feof(wavefile->fp) ) {
                                                //end of file
                                                return(1);
                                        }
                                        else {
                                                MYLOG_ERROR( "Could not read GSM610 samples: %d bytes of %d have been written: %d:%s",
                                                            n,65,errno,strerror(errno) );
                                                return(-1);
                                        }
                                }
                                //convert block of 65 compressed data bytes (MS GSM frame format) in data8[]
                                //to 320 samples in voice16[]
                                // decode 1st frame
                                gsm_decode( wavefile->vocoder, (uint8_t*)(data8), (gsm_signal*)(wavefile->voice16) );
                                // decode 2nd frame
                                gsm_decode( wavefile->vocoder, (uint8_t*)(data8)+32, (gsm_signal*)(wavefile->voice16+160) );
                                wavefile->voice16_samples += 320;
                        }
                        voice[i] = wavefile->voice16[ wavefile->voice16_samples-- ];
                }
                return(0);
#endif //SUPP_GSM

        default:
                MYLOG_ERROR("Invalid wavetype=%d",wavefile->wavetype);
                return(-1);
        }
}

//------------------------------------------------------------
//Get wavetype of opened wave-file
uint8_t wavefile_get_wavetype ( wavefile_t * wavefile )
{
        if(wavefile==NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(WAVETYPE_NOTSET);
        }
        return(wavefile->wavetype);
}

//------------------------------------------------------------
//Get size of opened wave-file (in bytes)
//NOTE: when writing wave-file - call this function only after wavefile_write_close()
//      when reading wave-file - call this function any time after wavefile_read_start()
//inputs:   wavefile = pointer to wavefile structure
//returns:  filesize = bytes
//          0 = error
uint32_t wavefile_get_bytes( wavefile_t * wavefile )
{
        //Check input parameters
        if(wavefile==NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(0);
        }
        return(  wavefile->header->file_length + 8  );
}

//------------------------------------------------------------
//Get length of opened wave-file (in samples)
//NOTE: when writing wave-file - call this function only after wavefile_write_close()
//      when reading wave-file - call this function any time after wavefile_read_start()
//inputs:   wavefile = pointer to wavefile structure
//returns:  lenght  = samples
//          0 = error
uint32_t wavefile_get_samples( wavefile_t * wavefile )
{
        //Check input parameters
        if(wavefile==NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(0);
        }
        return(  wavefile->total_samples  );
}

//------------------------------------------------------------
//Get length of opened wave-file (in seconds)
//NOTE: when writing wave-file - call this function only after wavefile_write_close()
//      when reading wave-file - call this function any time after wavefile_read_start()
//inputs:   wavefile = pointer to wavefile structure
//returns:  length   = seconds
//          0 = error
uint32_t wavefile_get_seconds( wavefile_t * wavefile )
{
        //Check input parameters
        if(wavefile==NULL) {
                MYLOG_ERROR("Invalid value: wavefile=NULL");
                return(0);
        }

        return(  wavefile->total_samples / wavefile->header->samples_per_second );
}




