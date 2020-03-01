#ifndef SERVICECONTEXT_HPP
#define SERVICECONTEXT_HPP

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <arpa/inet.h>

#include "../PTZController.hpp"

#include "smacros.h"
#include "eth_dev_param.h"
#include "soapH.h"
#include "../soap_helpers.h"

using namespace std;

class ServiceContext
{
	private:
		static ServiceContext * Instance;

		struct soap * m_soap;

		vector<Eth_Dev_Param> m_net_ifs; //ethernet interfaces
		vector<string> m_scopes;

		bool m_has_ptz =				system_get_model() == MODEL_WYZE_PAN;

		int m_port =                  	8080;
		string m_user =               	"root";
		string m_password =           	"password";
		PTZController * m_ptzcontroller = nullptr;

		ServiceContext()
		{
			if( this->m_has_ptz )
				this->m_ptzcontroller = new PTZController();

			this->m_scopes.push_back("onvif://www.onvif.org/Profile/S");
			this->m_scopes.push_back("onvif://www.onvif.org/Profile/G");
			this->m_scopes.push_back("onvif://www.onvif.org/Profile/Streaming");
		}

	public:

		static ServiceContext * CreateInstance();
		static ServiceContext * GetInstance();

		struct DeviceInformation {
			string Manufacturer = system_get_manufacturer();
			string Model = system_get_model();
			string FirmwareVersion = system_get_fw();
			string SerialNumber = system_get_mac();
			string HardwareId = "unknown";
		} m_device_information;

		void set_port( int _port ) { this->m_port = _port; }
		void set_user( string _user ) { this->m_user = _user; }
		void set_password( string _password ) { this->m_password = _password; }
		void set_soap( struct soap * _soap )
		{
			this->m_soap = _soap;
		}

		int get_port() { return this->m_port; }
		string get_user() { return this->m_user; }
		string get_password() { return this->m_password; }
		bool has_ptz() { return this->m_has_ptz; }

		PTZController * getPTZController() { return this->m_ptzcontroller; }

		struct soap * get_soap() { return this->m_soap; }

		std::vector<Eth_Dev_Param> & get_net_interfaces() { return this->m_net_ifs; }

		DeviceInformation & get_device_info() { return this->m_device_information; }

	public:
		~ServiceContext() {}

		bool add_net_interface(const char * dev_if)
		{
			this->m_net_ifs.push_back( Eth_Dev_Param() );

			if( this->m_net_ifs.back().open(dev_if) != 0 )
				return false;

			return true;
		}

		string getServerIpFromClient()
		{
			return getServerIpFromClientIp(htonl(this->m_soap->ip));
		}

		string getServerIpFromClientIp(uint32_t client_ip) const
		{
			char server_ip[INET_ADDRSTRLEN];

			for(size_t i = 0; i < this->m_net_ifs.size(); ++i)
			{
				uint32_t if_ip, if_mask;
				this->m_net_ifs[i].get_ip(&if_ip);
				this->m_net_ifs[i].get_mask(&if_mask);

				if( (if_ip & if_mask) == (client_ip & if_mask) )
				{
					this->m_net_ifs[i].get_ip(server_ip);
					return server_ip;
				}
			}

			return "127.0.0.1";  //localhost
		}

		string getXAddr(soap *soap) const
		{
			std::ostringstream os;

			os << "http://" << getServerIpFromClientIp(htonl(soap->ip)) << ":" << this->m_port << "/onvif/device_service";

			return os.str();
		}

		vector<string> get_scopes()
		{
			return this->m_scopes;
		}
};
#endif
