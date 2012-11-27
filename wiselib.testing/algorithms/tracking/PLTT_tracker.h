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

#ifndef __PLTT_TRACKER_H__
#define __PLTT_TRACKER_H__

#include "PLTT_default_values_config.h"
#include "PLTT_source_config.h"
#include "../../internal_interface/message/message.h"

namespace wiselib
{
	template<	typename Os_P,
				typename PLTT_Agent_P,
				typename Node_P,
				typename Position_P,
				typename IntensityNumber_P,
				typename Timer_P,
				typename Radio_P,
				typename ReliableRadio_P,
				typename Rand_P,
				typename Clock_P,
				typename Debug_P>
	class PLTT_TrackerType
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef ReliableRadio_P ReliableRadio;
		typedef Debug_P Debug;
		typedef Node_P Node;
		typedef Position_P Position;
		typedef IntensityNumber_P IntensityNumber;
		typedef Rand_P Rand;
		typedef Clock_P Clock;
		typedef PLTT_Agent_P PLTT_Agent;
		typedef typename PLTT_Agent::AgentID AgentID;
		typedef Timer_P Timer;
		typedef PLTT_TrackerType<Os, PLTT_Agent, Node, Position, IntensityNumber, Timer, Radio, ReliableRadio, Rand, Clock, Debug> self_type;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Timer::millis_t millis_t;
		typedef typename Radio::TxPower TxPower;
		typedef typename Radio::ExtendedData ExtendedData;
		typedef typename Clock::time_t time_t;
		typedef Message_Type<Os, Radio, Debug> Message;
		void init( Radio& _radio, ReliableRadio& _reliable_radio, Timer& _timer, Rand& _rand, Clock& _clock, Debug& _debug )
		{
			radio_ = &_radio;
			timer_ = &_timer;
			rand_ = &_rand;
			debug_ = &_debug;
			clock_ = &_clock;
			reliable_radio_ = &_reliable_radio;
		}
		// -----------------------------------------------------------------------
		Node* get_self()
		{
			return &self;
		}
		// -----------------------------------------------------------------------
		void set_self( Node _n )
		{
			self = _n;
		}
		// -----------------------------------------------------------------------
		PLTT_TrackerType() :
			radio_callback_id					( 0 ),
			reliable_radio_callback_id			( 0 ),
			transmission_power_dB				( PLTT_TRACKER_H_TRANSMISSION_POWER_DB ),
			current_link_metric 				( 255 ),
			status								( WAITING_STATUS ),
			generate_agent_period				( PLTT_TRACKER_H_GENERATE_AGENT_PERIOD ),
			generate_agent_period_offset_ratio	( PLTT_TRACKER_H_GENERATE_AGENT_PERIOD_OFFSET_RATIO ),
			init_tracking_millis				( PLTT_TRACKER_H_INIT_TRACKING_MILLIS )
#ifdef CONFIG_PLTT_TRACKER_H_MINI_RUN
			,tracker_mini_run_times				( PLTT_TRACKER_H_MINI_RUN_TIMES ),
			tracker_mini_run_counter			( 0 )
#endif
		{}
		// -----------------------------------------------------------------------
		PLTT_TrackerType( node_id_t _tid, IntensityNumber _tar_max_inten, uint8_t _tp_db, millis_t _itm, millis_t _gap, uint16_t _gapor, uint32_t _tmrt ) :
			radio_callback_id					( 0 ),
			reliable_radio_callback_id			( 0 ),
			current_link_metric 				( 255 ),
			status								( WAITING_STATUS ),
#ifdef CONFIG_PLTT_TRACKER_H_MINI_RUN
			tracker_mini_run_counter			( 0 )
#endif
		{
			target_id = _tid;
			target_max_inten = _tar_max_inten;
			transmission_power_dB = _tp_db;
			init_tracking_millis = _itm;
			generate_agent_period = _gap;
			generate_agent_period_offset_ratio = _gapor;
			tracker_mini_run_times = _tmrt;
		}
		// -----------------------------------------------------------------------
		~PLTT_TrackerType()
		{}
		// -----------------------------------------------------------------------
		void enable( void )
		{
			radio().enable_radio();
			reliable_radio().enable_radio();
			set_status( ACTIVE_STATUS );
#ifdef DEBUG_PLTT_TRACKER_H_ENABLE
			debug().debug( "PLTT_Tracker - Enable - Tracking target of id %x.\n", target_id );
#endif
			radio_callback_id = radio().template reg_recv_callback<self_type, &self_type::receive> ( this );
			reliable_radio_callback_id = reliable_radio().template reg_recv_callback<self_type, &self_type::receive> ( this );
			timer().template set_timer<self_type, &self_type::send_echo> ( init_tracking_millis, this, 0 );
		}
		// -----------------------------------------------------------------------
		void disable( void )
		{
			set_status( WAITING_STATUS );
			radio().unreg_recv_callback( radio_callback_id );
		}
		// -----------------------------------------------------------------------
		void send( node_id_t _destination, size_t _len, block_data_t* _data, message_id_t _msg_id  )
		{
			trans_power.set_dB( transmission_power_dB );
			radio().set_power( trans_power );
			Message message;
			message.set_message_id( _msg_id );
			message.set_payload( _len, _data );
			radio().send( _destination, message.serial_size(), (block_data_t*)&message );
		}
		// -----------------------------------------------------------------------
		void send_query( void* _userdata )
		{
#ifdef DEBUG_PLTT_TRACKER_H_SEND_QUERY
			debug().debug( "PLTT_Tracker - send_query - Entering.\n" );
#endif
			if ( current_link_metric != 255 )
			{
				agent = PLTT_Agent( current_agent_id, target_id, radio().id(), target_max_inten );
				agent.set_start_millis( clock().milliseconds( clock().time() ) + clock().seconds( clock().time() ) * 1000 );
				block_data_t buff[ReliableRadio::MAX_MESSAGE_LENGTH];
				trans_power.set_dB( transmission_power_dB );
				reliable_radio().set_power( trans_power );
				Message message;
				message.set_message_id( PLTT_AGENT_QUERY_ID );
				message.set_payload(  agent.serial_size(), agent.serialize( buff ) );
				//printf("XXXXXXXXX\n");
				//agent.print( debug(), radio() );
				//message.print( debug(), radio() );
				//PLTT_Agent agent2;
				//agent2.de_serialize( buff );
				//printf("XXXXXXXXX\n");
				//agent2.print( debug(), radio() );
				reliable_radio().send( current_query_destination, message.serial_size(), message.serialize() );
				current_link_metric = 255;
#ifdef DEBUG_PLTT_TRACKER_H_SEND_QUERY
				debug().debug( "PLTT_Tracker - send_query - A track query was routed to %x.\n", current_query_destination );
#endif
			}
			else
			{
#ifdef DEBUG_PLTT_TRACKER_H_SEND_QUERY
				debug().debug( "PLTT_Tracker - send_query - No echo replies for %x query.\n", current_agent_id );
#endif
			}
#ifdef DEBUG_PLTT_TRACKER_H_SEND_QUERY
			debug().debug( "PLTT_Tracker - send_query - Exiting.\n" );
#endif
		}
		// -----------------------------------------------------------------------
		void send_echo( void* _userdata )
		{
#ifdef DEBUG_PLTT_TRACKER_H_SEND_ECHO
			debug().debug( "PLTT_Tracker - send_echo %x - Entering.\n", radio().id() );
#endif
#ifdef CONFIG_PLTT_TRACKER_H_MINI_RUN
			if ( tracker_mini_run_counter < tracker_mini_run_times )
			{
				debug().debug(" tmc vs tmr [%d vs %d]\n", tracker_mini_run_counter, tracker_mini_run_times );
#endif
				current_agent_id = ( rand()() % ( 0xffffffff -1 ) + 1 );
#ifdef DEBUG_PLTT_TRACKER_H_SEND_ECHO
				debug().debug( "PLTT_Tracker - send_echo - Generated a new rand id : %x \n", current_agent_id );
#endif
				size_t len = sizeof( AgentID );
				block_data_t buf[Radio::MAX_MESSAGE_LENGTH];
				block_data_t* buff = buf;
				write<Os, block_data_t, AgentID> ( buff, current_agent_id );
				send( Radio::BROADCAST_ADDRESS, len, buff, PLTT_TRACKER_ECHO_ID );
				debug().debug("XXXXXXXXXXXXXXXXXtracker data %d : %d ", generate_agent_period, ( generate_agent_period_offset_ratio * generate_agent_period ) / 100 );
				timer().template set_timer<self_type, &self_type::send_echo> ( generate_agent_period, this, 0);
				timer().template set_timer<self_type, &self_type::send_query> ( ( generate_agent_period_offset_ratio * generate_agent_period ) / 100, this, 0);
#ifdef CONFIG_PLTT_TRACKER_H_MINI_RUN
				tracker_mini_run_counter = tracker_mini_run_counter + 1;
			}
#endif
#ifdef DEBUG_PLTT_TRACKER_H_SEND_ECHO
			debug().debug( "PLTT_Tracker - send_echo %x - Exiting.\n", radio().id() );
#endif
		}
		// -----------------------------------------------------------------------
		void receive( node_id_t _from, size_t _len, block_data_t* _data, const ExtendedData& _exdata )
		{
			message_id_t msg_id = *_data;
			Message *message = (Message*)_data;
			if ( msg_id == PLTT_AGENT_REPORT_ID )
			{

//#ifdef DEBUG_PLTT_TRACKER_H_RECEIVE
				debug().debug( "PLTT_Tracker - receive - Received agent from %x.\n", _from );
				agent.print( debug(), radio() );
//#endif
			}
			else if( msg_id == PLTT_TRACKER_ECHO_REPLY_ID )
			{
				AgentID aid = read<Os, block_data_t, AgentID>( message->get_payload() );
#ifdef DEBUG_PLTT_TRACKER_H_RECEIVE
				debug().debug( "PLTT_Tracker - receive - Received echo reply %x from %x.\n", _from, aid );
#endif
				if ( ( aid == current_agent_id ) && ( _exdata.link_metric() < current_link_metric ) )
				{
#ifdef DEBUG_PLTT_TRACKER_H_RECEIVE
					debug().debug( "PLTT_Tracker - receive - Node %x was chosen for the final candidate.\n", _from );
#endif
					current_query_destination = _from;
					current_link_metric = _exdata.link_metric();
				}
			}
		}
		// -----------------------------------------------------------------------
		uint8_t get_status()
		{
			return status;
		}
		// -----------------------------------------------------------------------
		void set_status( int _st )
		{
			status = _st;
		}
		// -----------------------------------------------------------------------
		millis_t get_init_tracking_millis()
		{
			return init_tracking_millis;
		}
		// -----------------------------------------------------------------------
		void set_init_tracking_millis( millis_t _itm )
		{
			init_tracking_millis = _itm;
		}
		// -----------------------------------------------------------------------
	private:
		Radio& radio()
		{
			return *radio_; 
		}
		// -----------------------------------------------------------------------
		ReliableRadio& reliable_radio()
		{
			return *reliable_radio_;
		}
		// -----------------------------------------------------------------------
		Timer& timer()
		{
			return *timer_; 
		}
		// -----------------------------------------------------------------------
		Debug& debug()
		{ 
			return *debug_; 
		}
		// -----------------------------------------------------------------------
		Rand& rand()
		{
			return *rand_;
		}
		// -----------------------------------------------------------------------
		Clock& clock()
		{
			return *clock_;
		}
		// -----------------------------------------------------------------------
		Clock* clock_;
		Radio* radio_;
		ReliableRadio* reliable_radio_;
		Timer* timer_;
		Rand* rand_;
		Debug* debug_;
		enum MessageIds
		{
			PLTT_TRACKER_ECHO_ID = 21,
			PLTT_TRACKER_ECHO_REPLY_ID = 31,
			PLTT_AGENT_QUERY_ID = 41,
			PLTT_AGENT_REPORT_ID = 51
		};
		enum pltt_tracker_status
		{
			ACTIVE_STATUS,
			WAITING_STATUS,
			PLTT_tracker_STATUS_NUM_VALUES
		};
		uint32_t radio_callback_id;
		uint32_t reliable_radio_callback_id;
		node_id_t target_id;
		TxPower trans_power;
		int8_t transmission_power_dB;
		PLTT_Agent agent;
		IntensityNumber target_max_inten;
		AgentID current_agent_id;
		node_id_t current_query_destination;
		uint8_t current_link_metric;
		uint8_t status;
		millis_t generate_agent_period;
		millis_t generate_agent_period_offset_ratio;
		Node self;
		millis_t init_tracking_millis;
#ifdef CONFIG_PLTT_TRACKER_H_MINI_RUN
		uint32_t tracker_mini_run_times;
		uint32_t tracker_mini_run_counter;
#endif
	};
}
#endif
