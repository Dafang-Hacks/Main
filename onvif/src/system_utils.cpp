#include "system_utils.h"

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <stdarg.h>
#include <fstream>
#include <string.h>
#include "PTZController.hpp"
#include "smacros.h"

#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/writer.h>

using namespace std;
using namespace boost;

bool system_file_exists(const string & name)
{
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}

string system_exec(const char * _cmd, ...)
{
	// printf("%s\n", _cmd);

	va_list args;
	char cmd[512];

	va_start(args, _cmd);
	vsprintf(cmd, _cmd, args);
	va_end(args);

	// printf("--%s\n", cmd);

	char buffer[512];
	string result = "";
	FILE* pipe = popen(cmd, "r");

	if (!pipe)
		throw std::runtime_error("popen() failed!");

	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	boost::trim(result);
	// printf("----%s\n", result.c_str());
	return result;
}

void system_gpio( int pin, int value )
{
	system_exec("echo %i > /sys/class/gpio/gpio%i/value", value, pin);
}

int system_gpio( int pin )
{
	return atoi( system_exec("cat /sys/class/gpio/gpio%i/value", pin).c_str() );
}

string system_read_config( string config, string key )
{
	return system_exec("source /system/sdcard/config/%s.conf; echo $%s;", config.c_str(), key.c_str());
}

void system_write_config( string config, string key, string value )
{
	char buffer [124];
	sprintf(buffer, "rewrite_config /system/sdcard/config/%s.conf %s \"\\\"%s\\\"\"", config.c_str(), key.c_str(), value.c_str());
	system_common_function(buffer);
}

string system_common_function( string args )
{
	string cmd = string("source ") + SCRIPTS_PATH + "/common_functions.sh; %s;";
	return system_exec( cmd.c_str(), args.c_str() );
}

void system_common_function( string func, bool on )
{
	string val = (on) ? "on" : "off";
	string cmd = string("source ") + SCRIPTS_PATH + "/common_functions.sh; %s %s;";
	system_exec( cmd.c_str(), func.c_str(), val.c_str() );
}

string system_set_conf( char key, string value )
{
	string cmd = string(BIN_PATH) + "/setconf.sh -k %c -v %s;";
	return system_exec(cmd.c_str(), key, value.c_str());
}

string system_get_conf( char key )
{
	string cmd = string(BIN_PATH) + "/setconf -g %s;";
	return system_exec(cmd.c_str(), key);
}

string system_get_manufacturer()
{
	return "Wyze / Xiaomi";
}

string system_get_model()
{
	if (system_file_exists("/driver/8189es.ko"))
		return MODEL_WYZE_PAN;
	else if(system_file_exists("/driver/8189fs.ko"))
		return MODEL_XIAFANG_S1;
	else
		return MODEL_WYZE_V2;
}

string system_get_fw()
{
	string cmd = string(BIN_PATH) + "/busybox strings /dev/mtd0 | grep \"U-Boot 2\" | awk -F ' ' '{print $2}'";
	return system_exec(cmd.c_str());
}

string system_get_mac()
{
	return system_exec("cat /sys/class/net/wlan0/address");
}

int system_get_rtsp_port()
{
	return atoi( system_read_config("rtspserver", "PORT").c_str() );
}

float system_get_rtsp_framerate()
{
	string framerate = system_read_config("rtspserver", "FRAMERATE_NUM");
	return atof( framerate.c_str() );
}

int system_get_rtsp_video_bitrate()
{
	string bitrate = system_read_config("rtspserver", "BITRATE");
	return atoi( bitrate.c_str() );
}

VideoResolution system_get_rtsp_video_resolution()
{
	string config_result = system_read_config("rtspserver", "RTSPH264OPTS");
	boost::regex pattern("^-W(\\d{3,4}).*-H(\\d{3,4}).*");
	boost::smatch smatches;

	if( ! boost::regex_match( config_result, smatches, pattern ) )
	{
		return { 0, 0 };
	}

	int width = atoi( smatches.str(1).c_str() );
	int height = atoi( smatches.str(2).c_str() );

	return { width, height };
}

int system_get_rtsp_audio_in_samplerate()
{
	string samplerate = system_read_config("rtspserver", "AUDIOINBR");
	return atoi( samplerate.c_str() );
}

int system_get_rtsp_audio_out_samplerate()
{
	string samplerate = system_read_config("rtspserver", "AUDIOOUTBR");
	return atoi( samplerate.c_str() );
}

int system_get_rtsp_audio_hw_volume()
{
	return atoi( system_read_config("rtspserver", "HWVOLUME").c_str() );
}

string system_get_rtsp_stream_username()
{
	return system_read_config("rtspserver", "USERNAME");
}

string system_get_rtsp_stream_password()
{
	return system_read_config("rtspserver", "USERPASSWORD");
}

void system_restart_rtsp(soap * soap)
{
	string path = string(CONTROLSCRIPTS_PATH) + "/rtsp-h264";

	DEBUG_MSG("Stopping rtsp service Process...\n");
	system( (path + " stop").c_str() );

	pid_t pid1 = fork();
	if(pid1 == 0)
	{

		// Child Process
		pid_t pid2 = fork();
		if(pid2 == 0)
		{
			// Child Process
			setsid();

			// Here we release resources
			// soap_closesock(soap);
			soap_destroy(soap);
			soap_end(soap);
			soap_free(soap);

			sleep(1);

			DEBUG_MSG("Starting rtsp service Process...\n");
			execl(path.c_str(), path.c_str(), "start", (char*) NULL);
		}
		else if(pid2 > 0)
		{
			exit(0);
		}
		else
		{
			DEBUG_MSG("Error with Fork() second");
		}
	} else if(pid1 < 0) {
		DEBUG_MSG("Error with Fork() first");
	}
}

int system_get_http_port()
{
	return 80;
}

void system_store_presets( std::map<std::string, PTPosition *> presets )
{
	rapidjson::Document root;
	rapidjson::Document::AllocatorType & a = root.GetAllocator();

	root.SetObject();

	rapidjson::Value jsonPresets;
	jsonPresets.SetObject();

	for( auto & p : presets )
	{
		rapidjson::Value values;
		values.SetObject();
		values.AddMember("x", rapidjson::Value(p.second->x), a);
		values.AddMember("y", rapidjson::Value(p.second->y), a);

		jsonPresets.AddMember(rapidjson::StringRef(p.first.c_str()), values, a);
	}

	root.AddMember("presets", jsonPresets, a);

	string file = string(CONFIG_PATH) + "/ptz_presets.conf";
	FILE * fp = fopen(file.c_str(), "wb");

	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	root.Accept(writer);

	fclose(fp);
}

std::map<std::string, PTPosition *> system_load_presets()
{
	std::map<std::string, PTPosition *> presets;

	string file = string(CONFIG_PATH) + "/ptz_presets.conf";
	FILE * fp = fopen(file.c_str(), "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	fclose(fp);
	for(auto & p : d["presets"].GetObject())
	{
		const auto & o = p.value.GetObject();
		PTPosition * position = new PTPosition{ o["x"].GetInt(), o["y"].GetInt() };
		presets[p.name.GetString()] = position;
	}
	return presets;
}

bool system_irled() { return system_common_function("ir_led status") == "ON"; }
bool system_ircut() { return system_common_function("ir_cut status") == "ON"; }
bool system_auto_night_mode() { return system_common_function("auto_night_mode status") == "ON"; }
bool system_night_mode() { return system_common_function("night_mode status") == "ON"; }

void system_irled( bool on ) { system_gpio(49, on); }
void system_ircut( bool on ) { system_common_function("ir_cut", on); }
void system_auto_night_mode( bool on ) { system_common_function("auto_night_mode", on); }
void system_night_mode( bool on ) { system_common_function("night_mode", on); }
