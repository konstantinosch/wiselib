/***************************************************************************
** This file is part of the generic algorithm library Wiselib.           **
** Copyright (C) 2008,2009 by the Wisebed (www.wisebed.eu) project.      **
**                                                                       **
** The Wiselib is free software: you can redistribute it and/or modify   **
** it under the terms of the GNU Lesser General Public License as        **
** published by the Free Software Foundation, either version 3 of the    **
** License, or (at your option) any later version.                       **
**                                                                       **
** The Wiselib is distributed in the hope that it will be useful,        **
** but WITHOUT ANY WARRANTY; without even the implied warranty of        **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
** GNU Lesser General Public License for more details.                   **
**                                                                       **
** You should have received a copy of the GNU Lesser General Public      **
** License along with the Wiselib.                                       **
** If not, see <http://www.gnu.org/licenses/>.                           **
***************************************************************************/

#ifndef __FLOOD_TRACKING_TARGET_H__
#define __FLOOD_TRACKING_TARGET_H__

#include "FLOOD_TRACKING_config.h"
#include "FLOOD_TRACKING_message.h"

namespace wiselib
{
	template<typename Os_P,
		typename FLOOD_TRACKING_Trace_P,
		typename Node_P,
		typename Timer_P,
		typename Radio_P,
		typename Clock_P,
		typename Debug_P>
	class FLOOD_TRACKING_TargetType
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Debug_P Debug;
		typedef Node_P Node;
		typedef FLOOD_TRACKING_Trace_P FLOOD_TRACKING_Trace;
		typedef Timer_P Timer;
		typedef Clock_P Clock;
		typedef FLOOD_TRACKING_TargetType<Os, FLOOD_TRACKING_Trace, Node, Timer, Radio, Clock, Debug> self_type;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Timer::millis_t millis_t;
		typedef typename FLOOD_TRACKING_Trace::TimesNumber TimesNumber;
		typedef typename Radio::TxPower TxPower;
		typedef FLOOD_TRACKING_MessageType<Os, Radio> Message;
		void init( Radio& radio, Timer& timer, Clock& clock, Debug& debug )
		{
			radio_ = &radio;
			timer_ = &timer;
			debug_ = &debug;
			clock_ = &clock;
		}
		Node* get_self()
		{
			return &self;
		}
		void set_self( Node _n )
		{
			self = _n;
		}
		// -----------------------------------------------------------------------
		FLOOD_TRACKING_TargetType()
		{}
		// -----------------------------------------------------------------------
		FLOOD_TRACKING_TargetType( FLOOD_TRACKING_Trace _t, millis_t _s, int16_t _db )
		{
			target_trace = _t;
			spread_milis = _s;
			transmission_dB = _db;
			target_trace.set_start_time( 0 );
		}
		// -----------------------------------------------------------------------
		~FLOOD_TRACKING_TargetType()
		{}
		// -----------------------------------------------------------------------
		void enable( void )
		{
			//debug().debug(" Target %x boot", self.get_id() );
			radio().enable_radio();
			target_trace.set_target_id( self.get_id() );
			send_trace();
		}
		// -----------------------------------------------------------------------
		void disable( void )
		{
			radio().disable();
		}
		// -----------------------------------------------------------------------
		void send_trace( void* userdata = NULL)
		{
			Message message;
			message.set_msg_id( DETECTION_ID );
			block_data_t buffer[Radio::MAX_MESSAGE_LENGTH];
			block_data_t* buff = buffer;
			message.set_payload( target_trace.get_buffer_size(), target_trace.set_buffer_from( buff ) );
			TxPower power;
			power.set_dB( transmission_dB );
			radio().set_power( power );
			radio().send( Radio::BROADCAST_ADDRESS, message.buffer_size(), (block_data_t*)&message );
			debug().debug(" Target %x send trace %d with transmission_db = %i", self.get_id(), target_trace.get_start_time(), transmission_dB );
			//debug().debug("%i:%i:%i",self.get_id(), 150, 150);
			//int id_1 = 0xf43e;
			//int id_2 = 0x4567;
			//int id_3 = 0xf742;
			//debug().debug("%i:%i:%i",id_1, 0, 0);
			//debug().debug("%i:%i:%i",id_2, 3, 5);
			//debug().debug("%i:%i:%i",id_3, 65, 23);
			target_trace.update_start_time();
			timer().template set_timer<self_type, &self_type::send_trace>( spread_milis, this, 0 );
		}
		// -----------------------------------------------------------------------
	private:
		Radio& radio()
		{
			return *radio_; 
		}
		Timer& timer()
		{
			return *timer_; 
		}
		Debug& debug()
		{
			return *debug_; 
		}
		Clock& clock()
		{
			return *clock_;
		}
		Radio * radio_;
		Timer * timer_;
		Debug * debug_;
		Clock * clock_;
		enum MessageIds
		{
			DETECTION_ID = 11
		};
		uint32_t radio_callback_id_;
		FLOOD_TRACKING_Trace target_trace;
		millis_t spread_milis;
		int16_t transmission_dB;
		Node self;

	};

}
#endif
