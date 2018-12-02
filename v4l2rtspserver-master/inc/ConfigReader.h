 #ifndef DEF_ConfigReader
 #define DEF_ConfigReader

#include <stdio.h>


#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

#include "logger.h"
#include <sstream>
#include "INIReader.h"



 class ConfigReader
 {
 public:
    static ConfigReader& instance()
    {
       static ConfigReader _instance;
       return _instance;
    }
    ~ConfigReader() {}
    void readConfig();
    char* getSensorName();
    int getSensorAddr();
    int getWidth();
    int getHeight();
    int getFps();
    int getEncoderMode();
    int getBitrate();
    bool getAudioEnabled();
    INIReader *reader;


 private:
    ConfigReader() {}           // verhindert, dass ein Objekt von außerhalb von ConfigReader erzeugt wird.
                    // protected, wenn man von der Klasse noch erben möchte
    ConfigReader( const ConfigReader& ); /* verhindert, dass eine weitere Instanz via
 Kopier-Konstruktor erstellt werden kann */
    ConfigReader & operator = (const ConfigReader &); //Verhindert weitere Instanz durch Kopie
 };


#endif