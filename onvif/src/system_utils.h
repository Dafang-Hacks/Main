#ifndef SYS_UTILS_HEADER
#define SYS_UTILS_HEADER

#include <string>
#include <map>
#include "soapH.h"

#define MODEL_WYZE_PAN		"Wyzecam Pan"
#define MODEL_XIAFANG_S1	"Xiaomi Xiaofang S1"
#define MODEL_WYZE_V2		"Wyzecam V2"

#define BASE_PATH       "/system/sdcard"
#define BIN_PATH        BASE_PATH "/bin"
#define SCRIPTS_PATH    BASE_PATH "/scripts"
#define CONTROLSCRIPTS_PATH		BASE_PATH "/controlscripts"
#define CONFIG_PATH		BASE_PATH "/config"
#define WWW_PATH        BASE_PATH "/www"


using namespace std;

struct VideoResolution {
	int width;
	int height;
};

bool system_file_exists(const string & name);

string system_exec(const char * _cmd, ...);

void system_gpio( int pin, int value );

int system_gpio( int pin );

string system_read_config( string config, string key );

void system_write_config( string config, string key, string value );

// detect_model:
// getgpio: pin
// setgpio: pin value
// get_config: path key
// rewrite_config: path key value
// blue_led: on, off, status
// yellow_led: on, off, status
// ir_led: on, off, status
// ir_cut: on, off, status
// motor: up [steps], down [steps], left [steps], right [steps], reset_pos_count [steps], status [horizontal, vertical]
// update_motor_pos: coords
// ldr: status          # Read the hw light sensor (hw in mqtt.conf)
// exposure: status
// http_server: on, off, restart, status
// rtsp_h264_server: on, off, status
// rtsp_mjpeg_server: on, off, status
// recording:  on, off, status
// ftp_server: on, off, status
// timelapse: on, off, status
// motion_detection: on, off, status
// motion_send_telegram: on, off, status
// motion_tracking: on, off, status
// night_mode: on, off, status
// auto_night_mode: on, off, status
// snapshot:
// update_axis:
// set_timezone:
// reboot_system:
string system_common_function( string args );

void system_common_function( string func, bool on );

// 'f' flip mode set to
//         '1' -> flip on
//         '0' -> flip off
// 'n' night mode set to
//         '1' -> night mode on
//         '0' -> night mode off
// 'b' set bitrate to VALUE
// 'o' OSD text set to VALUE
// 'c' OSD color set VALUE to
//         '0' for White
//         '1' for Black
//         '2' for Red
//         '3' for Green
//         '4' for Blue
//         '5' for Cyan
//         '6' for Yellow
//         '7' for Purple
// 'x' OSD position Y pos is set to VALUE
// 's' OSD size
// 'e' OSD font name (ttf)
// 'p' OSD space between char is set to VALUE (can be negative)
// 'w' fixed text width (0 variable, 1 fixed)
// 'm' motion sensitivity (0 to 4) -1 to deactivate motion
// 'z' display a circle when motion detected (-1 deactivated, use the OSD color numbers)
// 'r' set detection region (shall be: x0,y0,x1,y1) the server need to be restarted to take into account the new value
// 't' set tracking on/off (detection region is not taken into account anymore)
// 'u' set time before launching script after no motion (to restore camera position) -1 to deactivate
// 'h' set hardware volume (from mic)
// 'i' set software volume percentage (X will add X44 the data, from 0 to xxx, -1 to do nothing)
// 'q' set set filter number (1 or 2, 0 no filter)
// 'l' set set highpass filter on/off
// 'a' set set aec filter on/off
// 'd' set frame rate (shall be FrmRateNum,FrmRateDen (example: 25,1 to get 25 images per second)
string system_set_conf( char key, string value );
string system_get_conf( char key );

string system_get_manufacturer();
string system_get_model();
string system_get_fw();
string system_get_mac();

int system_get_rtsp_port();
float system_get_rtsp_framerate();
int system_get_rtsp_video_bitrate();
VideoResolution system_get_rtsp_video_resolution();
int system_get_rtsp_audio_in_samplerate();
int system_get_rtsp_audio_out_samplerate();
int system_get_rtsp_audio_hw_volume();
string system_get_rtsp_stream_username();
string system_get_rtsp_stream_password();
void system_restart_rtsp(soap * soap);

int system_get_http_port();

struct PTPosition;
void system_store_presets( std::map<std::string, PTPosition *> );
std::map<std::string, PTPosition *> system_load_presets();

bool system_irled();
bool system_ircut();
bool system_auto_night_mode();
bool system_night_mode();

void system_irled( bool on );
void system_ircut( bool on );
void system_auto_night_mode( bool on );
void system_night_mode( bool on );

#endif
