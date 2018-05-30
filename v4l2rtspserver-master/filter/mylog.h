/******************************************************************************/
/* LOG-OUT UTILITES                                                           */
/* mylog.h                                                                    */
/* (c) Sergei Mashkin, 2015                                                   */
/******************************************************************************/

#ifndef MYLOG_H
#define MYLOG_H

#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/types.h>

//---- FUNCTIONS ---------------------------------------------

#define MYLOGDEVICE_NOLOGS     0
#define MYLOGDEVICE_STDOUT     1  //printf()

#ifndef MYLOGDEVICE
#define MYLOGDEVICE MYLOGDEVICE_NOLOGS
#endif

    //////////////////////////////////////////////////////////////
#if MYLOGDEVICE == MYLOGDEVICE_NOLOGS
    //////////////////////////////////////////////////////////////

    #define MYLOG_SCREEN(mess...) \
        do{                       \
            printf(mess);         \
            printf("\n");         \
        }while(0)

    #define MYLOG_USER(mess...)
    #define MYLOG_ERROR(mess...)
    #define MYLOG_WARNING(mess...)
    #define MYLOG_EVENT(mess...)
    #define MYLOG_MESSAGE(mess...)
    #define MYLOG_STATE(mess...)
    #define MYLOG_DEBUG(mess...)
    #define MYLOG_TRACE(mess...)
    #define MYLOG_GOD(mess...)
    #define MYLOG_REALTIME(mess...)

    //////////////////////////////////////////////////////////////
#elif (MYLOGDEVICE == MYLOGDEVICE_STDOUT)
    //////////////////////////////////////////////////////////////

    #define MYLOG_SCREEN(mess...)                                 \
        do{                                                       \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_USER(mess...)                                   \
        do{                                                       \
            printf("USE %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_ERROR(mess...)                                  \
        do{                                                       \
            printf("ERR %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_WARNING(mess...)                                \
        do{                                                       \
            printf("WAR %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_EVENT(mess...)                                  \
        do{                                                       \
            printf("EVE %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_MESSAGE(mess...)                                \
        do{                                                       \
            printf("MES %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_STATE(mess...)                                  \
        do{                                                       \
            printf("STA %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_DEBUG(mess...)                                  \
        do{                                                       \
            printf("USE %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_TRACE(mess...)                                  \
        do{                                                       \
            printf("TRA %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_GOD(mess...)                                    \
        do{                                                       \
            printf("GOD %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    #define MYLOG_REALTIME(mess...)                               \
        do{                                                       \
            printf("REA %d:%s: ",__LINE__,__FUNCTION__);          \
            printf(mess);                                         \
            printf("\n");                                         \
        }while(0)

    //////////////////////////////////////////////////////////////
#endif // MYLOGDEVICE
    //////////////////////////////////////////////////////////////


#endif //MYLOG_H


