/**
 * @Author: Sian Croser <SianLee>
 * @Date:   2020-03-03T11:37:51+10:30
 * @Email:  CQoute@gmail.com
 * @Filename: PTZController.hpp
 * @Last modified by:   Sian Croser <Sian-Lee-SA>
 * @Last modified time: 2020-05-12T07:43:29+09:30
 * @License: GPL-3
 */


#ifndef PTZ_CONTROLLER_HPP
#define PTZ_CONTROLLER_HPP

#include <vector>
#include "soap_helpers.h"
#include "system_utils.h"
#include "smacros.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <sys/stat.h>
#include <fcntl.h>
/* ioctl cmd */
#define MOTOR_STOP					0x1
#define MOTOR_RESET					0x2
#define MOTOR_MOVE					0x3
#define MOTOR_GET_STATUS			0x4
#define MOTOR_SPEED					0x5
#define MOTOR_GOBACK				0x6
#define MOTOR_CRUISE				0x7
/* directional_attr */
#define MOTOR_DIRECTIONAL_UP    	0x0
#define MOTOR_DIRECTIONAL_DOWN    	0x1
#define MOTOR_DIRECTIONAL_LEFT    	0x2
#define MOTOR_DIRECTIONAL_RIGHT    	0x3
#define MOTOR_INIT_SPEED    		1000    /* unit :ns */

#define AXIS_HORIZONAL 				1
#define AXIS_VERTICAL 				2
#define AXIS_FORCIBLY 				3

#include <arpa/inet.h>
#include <string.h>

struct PTPosition {
	int x;
	int y;
};

class PTMotorDriver
{
	public:
		enum motor_status {
			MOTOR_IS_STOP,
			MOTOR_IS_RUNNING,
		};

	private:

		int fd;

		typedef struct {
			int x;
			int y;
		} motors_steps;

		typedef struct {
			int x;
			int y;
			enum motor_status status;
			int speed;
		} motor_message;

		typedef struct  {
			unsigned int x_max_steps;
			unsigned int y_max_steps;
			unsigned int x_cur_step;
			unsigned int y_cur_step;
		} motor_reset_data;

		void sendCommand( int cmd, void *buffer )
		{
			if( ! this->fd )
				this->fd = open("/dev/motor", O_WRONLY);

			ioctl(this->fd, cmd, buffer);
		}

	public:

		struct MotorState {
			int x;
			int y;
			enum motor_status status;
			int speed;
		};

		bool isRunning()
		{
			if( ! this->fd )
				return false;
			return this->getState( false ).status == MOTOR_IS_RUNNING;
		}

		void release()
		{
			if( this->fd )
			{
				close(this->fd);
				this->fd = 0;
				system_common_function("update_axis");
			}
		}

		void setSpeed(uint8_t speed)
		{
			uint16_t motorspeed = speed;
			sendCommand(MOTOR_SPEED, &motorspeed);
		}

		void setStop()
		{
			this->sendCommand(MOTOR_STOP, 0);
			this->release();
		}

		void setMove( int x, int y )
		{
			motors_steps steps = { x, y };
			this->sendCommand(MOTOR_MOVE, &steps);
		}

		void reset()
		{
			motor_reset_data motor_move;
			motor_move.x_max_steps = 0;
			motor_move.y_max_steps = 0;
			motor_move.x_cur_step = 0;
			motor_move.y_cur_step = 0;
			sendCommand(MOTOR_RESET, &motor_move);
		}

		MotorState getState( bool release = true )
		{
			motor_message status;
			this->sendCommand(MOTOR_GET_STATUS, &status);

			if( release )
				this->release();

			return { status.x, status.y, status.status, status.speed };
		}
};

class PTZController
{
	private:
		std::map<std::string, PTPosition *> 	m_presets;
		PTMotorDriver *							m_pt_motor = new PTMotorDriver();

		void load_presets()
		{
			this->m_presets = system_load_presets();
		}

		void save_presets()
		{
			system_store_presets( this->m_presets );
		}

	public:

		PTZController()
		{
			this->load_presets();
		}

		/**
		* Synology sends continuous move for their ptz control
		* then  send stop. We cannot block the thread for the motor
		* to arrive at position as stop would never get received.
		*/
		void continuous_move( float x, float y )
		{
			int speed_x = static_cast<int>(x * 1000.0);
			int speed_y = static_cast<int>(y * 1000.0);

			if(x != 0) {
				this->m_pt_motor->setSpeed(abs(speed_x / 2));
				x = (x > 0) ? 2600 : -2600;
			}
			if(y != 0) {
				if( ! x )
					this->m_pt_motor->setSpeed(abs(speed_y / 2));
				y = (y > 0) ? 2600 : -2600;
			}

			// We expect a stop command to stop and release
			this->m_pt_motor->setMove( x, y );
		}


		/**
		* When relative_move move is used, we can block the thread
		* as we have a known destination and closing the fd
		* would halt the movement before its complete. While this
		* will prevent incoming commands, it's only momentarilly
		* otherwise would have tolook at threading.
		*/
		void relative_move( float x, float y )
		{
			PTMotorDriver::MotorState state = this->m_pt_motor->getState( false );
			PTPosition target = PTPosition{ state.x + (int)x, state.y + (int)y };
			this->m_pt_motor->setMove( x, y );

			// We use an i counter for fail safe
			int i = 0;

			DEBUG_MSG("Target: %i %i\n", target.x, target.y);
			while( (state.x != target.x || state.y != target.y) && i < 2500 )
			{
				DEBUG_MSG("Current: %i %i\n", state.x, state.y);
				state = this->m_pt_motor->getState( false );
				i++;
			}

			this->stop();
		}

		/**
		* We let relative_move handle the logic for releasing the motor
		*/
		void absolute_move( float x, float y )
		{
			PTMotorDriver::MotorState state = this->m_pt_motor->getState( false );
			this->relative_move( (int)(x - state.x), (int)(y - state.y) );
		}

		void stop()
		{
			this->m_pt_motor->setStop();
		}

		void go_home_position()
		{
			if( ! this->m_presets.count("home") )
			{
				this->set_preset("home");
			}
			this->absolute_move(this->m_presets["home"]->x, this->m_presets["home"]->y);
		}

		void go_preset_position(std::string token)
		{
			if( ! this->m_presets.count(token) )
				return;

			this->absolute_move(this->m_presets[token]->x, this->m_presets[token]->y);
		}

		PTMotorDriver::MotorState get_motor_state()
		{
			return this->m_pt_motor->getState();
		}

		std::map<std::string, PTPosition *> & presets()
		{
			return this->m_presets;
		}

		void set_home_preset()
		{
			this->set_preset("home");
		}

		void set_preset(const std::string & name)
		{
			PTMotorDriver::MotorState state = this->m_pt_motor->getState();
			printf("Setting preset state %s %i %i", name.c_str(), state.x, state.y);

			this->m_presets[name] = new PTPosition{ state.x, state.y };

			this->save_presets();
		}

		void remove_preset(const std::string & token)
		{
			this->m_presets.erase(token);

			this->save_presets();
		}
};
#endif
