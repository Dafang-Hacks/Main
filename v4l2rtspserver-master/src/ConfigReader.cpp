#include "ConfigReader.h"



void ConfigReader::readConfig() {

    // Ini file to override some path when /system/sdcard won't exit

    char dirNameBuffer[1024 + 1] = {0};
    // Read the symbolic link '/proc/self/exe'.
    const char *linkName = "/proc/self/exe";
    readlink(linkName, dirNameBuffer, sizeof(dirNameBuffer) - 1);

    // Read the same exe file + ini
    strncat(dirNameBuffer, ".ini", sizeof(dirNameBuffer) - 1);
    LOG_S(INFO) <<  "Reading Config:" << dirNameBuffer;
    reader = new INIReader(dirNameBuffer);
    if (reader->ParseError() < 0) {
        LOG_S(ERROR) << "Cant read Config";
        exit(-1);
    }
}

char* ConfigReader::getSensorName() {
    return strdup(reader->Get("Hardware", "SensorName", "").c_str());
}
int ConfigReader::getSensorAddr(){
    return reader->GetInteger("Hardware", "SensorAddr", 0);
}

int ConfigReader::getWidth(){
    return reader->GetInteger("Video", "Width", 0);
}

int ConfigReader::getHeight(){
    return reader->GetInteger("Video", "Height", 0);
}

int ConfigReader::getFps(){
    return reader->GetInteger("Video", "Fps", 0);
}


bool ConfigReader::getAudioEnabled(){
    return reader->GetBoolean("Audio","Enabled",false);
}

