#include<stdio.h>
#include <getopt.h>
#include <cstdlib>
#include "sharedmem.h"

#define SETGETSHAREDMEMORYINT(INT) if (get) printf("%d\n",  INT); else INT = atoi(value);
#define SETGETSHAREDMEMORYLONG(LONG) if (get) printf("%ld\n",  LONG); else LONG = atol(value);
#define SETGETSHAREDMEMORYSTRING(STR) if (get) printf("%s\n",  STR); else  strcpy(STR,value);
#define SETGETSHAREDMEMORYBOOL(INT) if (get) printf("%s\n",  INT?"true":"false"); else INT= strToBool(value);

int stringToInts(char *str, int val[], int size)
{
    int i = 0;
    char *pt = strtok (str,",");
    while ((pt != NULL) &&
            i < size) {
        int a = atoi(pt);
        val[i] = a;
        i++;
        pt = strtok (NULL, ",");
    }
    return (i == size);
}

bool strToBool(char *str)
{
    if (strcasecmp(str, "true") == 0) return(true);
    if (strcasecmp(str, "on") == 0) return(true);
    if (strcasecmp(str, "false") == 0) return(false);
    if (strcasecmp(str, "off") == 0) return(false);
    if (atoi(str) == 1) return (true);
    return false;
}

void usage(char *command)
{
    fprintf(stderr, "Usage %s -g -k KEY -v VALUE\n", command);
    fprintf(stderr, "Where k to set value, g to get the value\n");
    fprintf(stderr, "\t'f' flip mode set to\n");
    fprintf(stderr, "\t\t'1' -> flip on\n");
    fprintf(stderr, "\t\t'0' -> flip off\n");

    fprintf(stderr, "\t'n' night mode set to\n");
    fprintf(stderr, "\t\t'1' -> night mode on\n");
    fprintf(stderr, "\t\t'0' -> night mode off\n");

    fprintf(stderr, "\t'b' set bitrate to VALUE\n");
    fprintf(stderr, "\t'o' OSD text set to VALUE\n");
    fprintf(stderr, "\t'c' OSD color set VALUE to\n");
    fprintf(stderr, "\t\t'0' for White\n");
    fprintf(stderr, "\t\t'1' for Black\n");
    fprintf(stderr, "\t\t'2' for Red\n");
    fprintf(stderr, "\t\t'3' for Green\n");
    fprintf(stderr, "\t\t'4' for Blue\n");
    fprintf(stderr, "\t\t'5' for Cyan\n");
    fprintf(stderr, "\t\t'6' for Yellow\n");
    fprintf(stderr, "\t\t'7' for Purple\n");

    fprintf(stderr, "\t'x' OSD position Y pos is set to VALUE\n");
    fprintf(stderr, "\t's' OSD size\n");
    fprintf(stderr, "\t'e' OSD font name (ttf)\n");
    fprintf(stderr, "\t'p' OSD space between char is set to VALUE (can be negative)\n");
    fprintf(stderr, "\t'w' fixed text width (0 variable, 1 fixed)\n");

    fprintf(stderr, "\t'm' motion sensitivity (0 to 4) -1 to deactivate motion\n");
    fprintf(stderr, "\t'z' display a circle when motion detected (-1 deactivated, use the OSD color numbers)\n");
    fprintf(stderr, "\t'r' set detection region (shall be: x0,y0,x1,y1) the server need to be restarted to take into account the new value\n");
    fprintf(stderr, "\t't' set tracking on/off (detection region is not taken into account anymore)\n");
    fprintf(stderr, "\t'u' set time before launching script after no motion (to restore camera position) -1 to deactivate\n");

    fprintf(stderr, "\t'h' set hardware volume (from mic)\n");
    fprintf(stderr, "\t'i' set software volume percentage (X will add X% to the data, from 0 to xxx, -1 to do nothing)\n");
    fprintf(stderr, "\t'q' set set filter number (1 or 2, 0 no filter)\n");
    fprintf(stderr, "\t'l' set set highpass filter on/off\n");
    fprintf(stderr, "\t'a' set set aec filter on/off\n");

    fprintf(stderr, "\t'd' set frame rate (shall be FrmRateNum,FrmRateDen (example: 25,1 to get 25 images per second)\n");

    fprintf(stderr, "Example: to set osd text: %s -k o -v OSDTEXT\n", command);
    fprintf(stderr, "         to get osd text: %s -g o\n", command);

}

int main(int argc, char *argv[]) {

    char key = 0;
    char *value = NULL;
    bool get = false;

    // Parse Arguments:
    int c;
    while ((c = getopt(argc, argv, "g:k:v:")) != -1) {
        switch (c) {
            case 'g':
                    get = true;
                    // no break on purpose, execute 'k' case
            case 'k':
                if (key == 0) {
                    key = optarg[0];
                } else {
                       printf("Can not use g and k values at the same time\n", c);
                       usage(argv[0]);
                       exit(EXIT_FAILURE);
                }
                break;

            case 'v':
                value = optarg;
                break;


            default:
                printf("Invalid Argument %c\n", c);
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    SharedMem &mem = SharedMem::instance();
    shared_conf *conf = mem.getConfig();
    //printf("%d,%d\n", conf->nightmode, conf->flip);
    mem.readConfig();
    //printf("%d,%d\n", conf->nightmode, conf->flip);
    switch (key) {
        case 'f':
            SETGETSHAREDMEMORYINT(conf->flip );
            break;
        case 'n':
            SETGETSHAREDMEMORYINT(conf->nightmode );
            break;
        case 'b':
            SETGETSHAREDMEMORYINT(conf->bitrate );
            break;

        // OSD configuration
        case 'o':
            SETGETSHAREDMEMORYSTRING(conf->osdTimeDisplay );
            break;
        case 'e':
            SETGETSHAREDMEMORYSTRING(conf->osdFontName );
            break;
        case 'c':
            SETGETSHAREDMEMORYLONG(conf->osdColor);
            break;
        case 's':
            SETGETSHAREDMEMORYINT(conf->osdSize);
            break;
        case 'x':
            SETGETSHAREDMEMORYINT(conf->osdPosY);
            break;
        case 'p':
            SETGETSHAREDMEMORYINT(conf->osdSpace);
            break;
        case 'w':
            SETGETSHAREDMEMORYBOOL(conf->osdFixedWidth);
            break;
        // Motion configuration
        case 'm':
            SETGETSHAREDMEMORYINT(conf->sensitivity);
            break;
        case 'z':
           SETGETSHAREDMEMORYINT(conf->motionOSD);
           break;
        case 'r':
            if (get)
                printf("%d,%d,%d,%d\n", conf->detectionRegion[0], conf->detectionRegion[1],conf->detectionRegion[2],conf->detectionRegion[3]);
            else
                stringToInts(value, conf->detectionRegion, 4);
            break;
        case 't':
            SETGETSHAREDMEMORYBOOL(conf->motionTracking);
            break;
        case 'u':
            SETGETSHAREDMEMORYINT(conf->motionTimeout);
            break;
        case 'h':
            SETGETSHAREDMEMORYINT(conf->hardVolume);
            break;
        case 'i':
            SETGETSHAREDMEMORYINT(conf->softVolume);
            break;
        case 'q':
            SETGETSHAREDMEMORYINT(conf->filter);
            break;
        case 'l':
            SETGETSHAREDMEMORYBOOL(conf->highfilter);
            break;
        case 'a':
            SETGETSHAREDMEMORYBOOL(conf->aecfilter);
            break;
        case 'd':
            if (get)
                printf("%d,%d\n", conf->frmRateConfig[0], conf->frmRateConfig[1]);
            else
                stringToInts(value, conf->frmRateConfig, 2);
            break;

    default:
        printf("Invalid Argument %c\n", key);
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    mem.setConfig();
    //mem.readConfig();
    //printf("%d,%d\n", conf->nightmode, conf->flip);

    return 0;


}

