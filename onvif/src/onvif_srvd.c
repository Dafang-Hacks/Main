/**
 * @Author: Sian Croser <Sian-Lee-SA>
 * @Date:   2020-03-03T11:37:52+10:30
 * @Email:  CQoute@gmail.com
 * @Filename: onvif_srvd.c
 * @Last modified by:   Sian Croser <Sian-Lee-SA>
 * @Last modified time: 2020-05-12T07:14:34+09:30
 * @License: GPL-3
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <getopt.h>

#include "daemon.h"
#include "smacros.h"

#include "services/ServiceContext.h"

// ---- gsoap ----
// #include "wsddapi.h"
#include "DeviceBinding.nsmap"

// #include "soapwsddService.h"

// Onvif Services
#include "soapDeviceBindingService.h"
#include "soapDeviceIOBindingService.h"
#include "soapImagingBindingService.h"
#include "soapMediaBindingService.h"
#include "soapPTZBindingService.h"
#include "soapReceiverBindingService.h"
#include "soapPullPointSubscriptionBindingService.h"
#include "soapRemoteDiscoveryBindingService.h"

static const char *help_str =
				" ===============  Help  ===============\n"
				" Daemon name:  " DAEMON_NAME          "\n"
				" Daemon  ver:  " DAEMON_VERSION_STR   "\n"
#ifdef  DEBUG
				" Build  mode:  debug\n"
#else
				" Build  mode:  release\n"
#endif
				" Build  date:  " __DATE__ "\n"
				" Build  time:  " __TIME__ "\n\n"
				"Options:                      description:\n\n"
				"       --no_chdir             Don't change the directory to '/'\n"
				"       --no_fork              Don't do fork\n"
				"       --no_close             Don't close standart IO files\n"
				"       --pid_file     [value] Set pid file name\n"
				"       --log_file     [value] Set log file name\n\n"
				"       --port         [value] Set socket port for Services   (default = 8080)\n"
				"       --user         [value] Set user name for Services     (default = root)\n"
				"       --password     [value] Set user password for Services (default = password)\n"
				"       --model        [value] Set model device for Services  (default = (Auto detected))\n"
				"       --ifs          [value] Set Net interfaces for work    (default = wlan0)\n"
				"       --hardware_id  [value] Set Hardware ID of device      (default = (Auto detected))\n"
				"       --serial_num   [value] Set Serial number of device    (default = (Auto detected))\n"
				"       --firmware_ver [value] Set firmware version of device (default = (Auto detected))\n"
				"       --manufacturer [value] Set manufacturer for Services  (default = (Auto detected))\n\n"
				"  -v,  --version              Display daemon version\n"
				"  -h,  --help                 Display this help\n\n";



namespace LongOpts
{
	enum
	{
		version = 'v',
		help    = 'h',

		//daemon options
		no_chdir = 1,
		no_fork,
		no_close,
		pid_file,
		log_file,

		//ONVIF Service options (context)
		port,
		user,
		password,
		manufacturer,
		model,
		firmware_ver,
		serial_num,
		hardware_id,
		scope,
		ifs
	};
}

static const char *short_opts = "hv";

static const struct option long_opts[] =
{
	{ "version",      no_argument,       NULL, LongOpts::version       },
	{ "help",         no_argument,       NULL, LongOpts::help          },

	//daemon options
	{ "no_chdir",     no_argument,       NULL, LongOpts::no_chdir      },
	{ "no_fork",      no_argument,       NULL, LongOpts::no_fork       },
	{ "no_close",     no_argument,       NULL, LongOpts::no_close      },
	{ "pid_file",     required_argument, NULL, LongOpts::pid_file      },
	{ "log_file",     required_argument, NULL, LongOpts::log_file      },

	//ONVIF Service options (context)
	{ "port",         required_argument, NULL, LongOpts::port          },
	{ "user",         required_argument, NULL, LongOpts::user          },
	{ "password",     required_argument, NULL, LongOpts::password      },
	{ "manufacturer", required_argument, NULL, LongOpts::manufacturer  },
	{ "model",        required_argument, NULL, LongOpts::model         },
	{ "firmware_ver", required_argument, NULL, LongOpts::firmware_ver  },
	{ "serial_num",   required_argument, NULL, LongOpts::serial_num    },
	{ "hardware_id",  required_argument, NULL, LongOpts::hardware_id   },
	{ "scope",        required_argument, NULL, LongOpts::scope         },
	{ "ifs",          required_argument, NULL, LongOpts::ifs           },

	{ NULL,           no_argument,       NULL,  0                      }
};


static struct soap *soap;
ServiceContext * service_ctx = ServiceContext::CreateInstance();

#define FOREACH_SERVICE(APPLY, soap)             			\
		APPLY(DeviceBindingService, soap)               	\
		APPLY(DeviceIOBindingService, soap)             	\
		APPLY(ImagingBindingService, soap)					\
		APPLY(MediaBindingService, soap)                	\
		APPLY(PTZBindingService, soap)                		\
		APPLY(ReceiverBindingService, soap)             	\
		APPLY(PullPointSubscriptionBindingService, soap)	\
		APPLY(RemoteDiscoveryBindingService, soap)


#define DECLARE_SERVICE(service, soap) service service ## _inst(soap);

#define DISPATCH_SERVICE(service, soap)                                  \
				else if (service ## _inst.dispatch() != SOAP_NO_METHOD) {\
						soap_send_fault(soap);                           \
						soap_stream_fault(soap, std::cerr);              \
				}

void daemon_exit_handler(int sig)
{
	soap_destroy(soap);
	soap_end(soap);
	soap_free(soap);

	unlink(daemon_info.pid_file);

	DEBUG_MSG("Exited Onvif Server...\n");

	exit(EXIT_SUCCESS);
}


void processing_cmd(int argc, char *argv[])
{
	int opt;

	while( (opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1 )
	{
		switch( opt )
		{
			case LongOpts::help:
				puts(help_str);
				exit_if_not_daemonized(EXIT_SUCCESS);
				break;

			case LongOpts::version:
				puts(DAEMON_NAME "  version  " DAEMON_VERSION_STR "\n");
				exit_if_not_daemonized(EXIT_SUCCESS);
				break;

			 //daemon options
			case LongOpts::no_chdir:
				daemon_info.no_chdir = 1;
				break;

			case LongOpts::no_fork:
				daemon_info.no_fork = 1;
				break;

			case LongOpts::no_close:
				daemon_info.no_close_stdio = 1;
				break;

			case LongOpts::pid_file:
				daemon_info.pid_file = optarg;
				break;

			case LongOpts::log_file:
				daemon_info.log_file = optarg;
				break;

			case LongOpts::port:
				service_ctx->set_port(atoi(optarg));
				break;

			case LongOpts::user:
				service_ctx->set_user(optarg);
				break;

			case LongOpts::password:
				service_ctx->set_password(optarg);
				break;

			case LongOpts::manufacturer:
				service_ctx->get_device_info().Manufacturer = optarg;
				break;

			case LongOpts::model:
				service_ctx->get_device_info().Model = optarg;
				break;

			case LongOpts::firmware_ver:
				service_ctx->get_device_info().FirmwareVersion = optarg;
				break;

			case LongOpts::serial_num:
				service_ctx->get_device_info().SerialNumber = optarg;
				break;

			case LongOpts::hardware_id:
				service_ctx->get_device_info().HardwareId = optarg;
				break;

			case LongOpts::ifs:
				if( ! service_ctx->add_net_interface(optarg) )
					daemon_error_exit("Can't open network interface: %s - %m\n", optarg);
				break;

			default:
				puts("for more detail see help\n\n");
				exit_if_not_daemonized(EXIT_FAILURE);
				break;
		}
	}
}


void init_signals(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = daemon_exit_handler;
	if( sigaction(SIGTERM, &sa, NULL) != 0 )
		daemon_error_exit("Can't set daemon_exit_handler: %m\n");

	signal(SIGCHLD, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP,  SIG_IGN);
}

void init_gsoap(void)
{
	soap = soap_new();

	if(!soap)
		daemon_error_exit("Can't get mem for SOAP\n");

	soap->bind_flags = SO_REUSEADDR;

	if( !soap_valid_socket(soap_bind(soap, NULL, service_ctx->get_port(), 100)) )
	{
		soap_stream_fault(soap, std::cerr);
		exit(EXIT_FAILURE);
	}

	soap->send_timeout = 3; // timeout in sec
	soap->recv_timeout = 3; // timeout in sec

	service_ctx->set_soap(soap);
}

void init(void *data)
{
	init_signals();

	if(service_ctx->get_net_interfaces().empty())
	{
		if( ! service_ctx->add_net_interface("wlan0") )
			daemon_error_exit("Can't open network interface: wlan0\n", optarg);
	}

	init_gsoap();
}

int main(int argc, char *argv[])
{
	processing_cmd(argc, argv);
	daemonize2(init, NULL);

	FOREACH_SERVICE(DECLARE_SERVICE, soap)

	while( true )
	{
		if( !soap_valid_socket(soap_accept(soap)) )
		{
			soap_stream_fault(soap, std::cerr);
			return EXIT_FAILURE;
		}

		if( soap_begin_serve(soap) )
		{
			soap_stream_fault(soap, std::cerr);
		}
		// Foreach implements } else if { to flow with pre and post code
		// else if {
		FOREACH_SERVICE(DISPATCH_SERVICE, soap)
		// }
		else
		{
			DEBUG_MSG("Unknown service\n");
			soap_stream_fault(soap, std::cerr);
			soap_send_fault(soap);

		}
	}

	return EXIT_SUCCESS;
}
