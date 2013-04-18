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

#ifndef __FLOOD_TRACKING_PASSIVE_H__
#define __FLOOD_TRACKING_PASSIVE_H__
#include "FLOOD_TRACKING_config.h"
#include "FLOOD_TRACKING_message.h"
namespace wiselib
{
	template<	typename Os_P,
				typename Node_P,
				typename FLOOD_TRACKING_Trace_P,
				typename FLOOD_TRACKING_TraceList_P,
				typename Timer_P,
				typename Radio_P,
				typename Rand_P,
				typename Clock_P,
				typename Debug_P>
	class FLOOD_TRACKING_PassiveType
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Rand_P Rand;
		typedef typename Rand::rand_t rand_t;
		typedef Debug_P Debug;
		typedef Node_P Node;
		typedef FLOOD_TRACKING_Trace_P FLOOD_TRACKING_Trace;
		typedef FLOOD_TRACKING_TraceList_P FLOOD_TRACKING_TraceList;
		typedef typename FLOOD_TRACKING_TraceList::iterator FLOOD_TRACKING_TraceListIterator;
		typedef typename Node::Position Position;
		typedef typename Node::Position::CoordinatesNumber CoordinatesNumber;
		typedef Timer_P Timer;
		typedef Clock_P Clock;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Radio::ExtendedData ExtendedData;
		typedef typename Radio::TxPower TxPower;
		typedef typename Timer::millis_t millis_t;
		typedef FLOOD_TRACKING_MessageType<Os, Radio> Message;
		typedef FLOOD_TRACKING_PassiveType<Os, Node, FLOOD_TRACKING_Trace, FLOOD_TRACKING_TraceList, Timer, Radio, Rand, Clock, Debug> self_type;
		// -----------------------------------------------------------------------
		FLOOD_TRACKING_PassiveType()
		: 	radio_callback_id_  				( 0 )
		{}
		// -----------------------------------------------------------------------
		~FLOOD_TRACKING_PassiveType()
		{
		}
		// -----------------------------------------------------------------------
		FLOOD_TRACKING_PassiveType( int16_t _db )
		{
			transmission_dB = _db;
		}
		// -----------------------------------------------------------------------
		void enable( void )
		{
			radio().enable_radio();
			radio_callback_id_ = radio().template reg_recv_callback<self_type, &self_type::receive>( this );
			clean_traces();
#ifdef PASSIVE_NODE
			debug().debug(" Passive %x boot", self.get_id() );
#endif
#ifdef TRACKER_NODE
			debug().debug(" Trackers %x boot", self.get_id() );
			targets_position();
#endif
		}
		// -----------------------------------------------------------------------
		void disable( void )
		{
			radio().unreg_recv_callback( radio_callback_id_ );
			radio().disable();
		}
		// -----------------------------------------------------------------------
		void send( node_id_t destination, size_t len, block_data_t *data, message_id_t msg_id )
		{
			Message message;
			message.set_msg_id( msg_id );
			message.set_payload( len, data );
			TxPower power;
			power.set_dB( transmission_dB );
			radio().set_power( power );
			radio().send( destination, message.buffer_size(), ( uint8_t* )&message );
		}
		// -----------------------------------------------------------------------
		void receive( node_id_t from, size_t len, block_data_t *data, const ExtendedData& exdata )
		{
			message_id_t msg_id = *data;
			Message *message = ( Message* )data;
			if ( msg_id == SPREAD_ID )
			{
				FLOOD_TRACKING_Trace t = FLOOD_TRACKING_Trace( message->payload() );
				FLOOD_TRACKING_TraceListIterator traces_iterator = traces.begin();
				FLOOD_TRACKING_TraceListIterator single_traces_iterator = single_traces.begin();
#ifdef PASSIVE_NODE
				//debug().debug(" Passive %x receive spread trace of target %x from passive %x with start_time %i and detection point %x, [%i,%i] and lqi %i", self.get_id(), t.get_target_id(), from, t.get_start_time(), t.get_current().get_id(), t.get_current().get_position().get_x(), t.get_current().get_position().get_y(), t.get_detection_LQI() );
#endif
#ifdef TRACKER_NODE
				//debug().debug(" Tracker %x receive spread trace of target %x from passive %x with start_time %i and detection point %x, [%i,%i] and lqi %i", self.get_id(), t.get_target_id(), from, t.get_start_time(), t.get_current().get_id(), t.get_current().get_position().get_x(), t.get_current().get_position().get_y(), t.get_detection_LQI() );
				uint8_t single_store_flag = 1;
#endif
				uint8_t store_flag = 1;

				while ( traces_iterator!=traces.end() )
				{
					if (	traces_iterator->get_target_id() == t.get_target_id() &&
							traces_iterator->get_current().get_id() == t.get_current().get_id() &&
							traces_iterator->get_start_time() == t.get_start_time() )
					{
						store_flag = 0;
						traces_iterator = traces.end();
#ifdef PASSIVE_NODE
						//debug().debug(" Passive %x trace was not stored", self.get_id() );
#endif
#ifdef TRACKER_NODE
						//debug().debug(" Tracker %x trace was not stored", self.get_id() );
#endif
					}
					else
					{
						++traces_iterator;
					}
				}
				if ( store_flag )
				{
					traces.push_back( t );
#ifdef PASSIVE_NODE
					//debug().debug(" Passive %x trace was stored and broadcasted", self.get_id() );
					send( Radio::BROADCAST_ADDRESS, t.get_buffer_size(),  message->payload(), SPREAD_ID );
#endif
#ifdef TRACKER_NODE
					//debug().debug(" Tracker %x trace was stored with size %i", self.get_id(), traces.size() );
					while ( single_traces_iterator != single_traces.end() && store_flag )
					{
						if ( single_traces_iterator->get_target_id() == t.get_target_id() )
						{
							//int16_t lqi_refinement = ( ( 255 - t.get_detection_LQI() ) * 100 ) / 255;
							//uint8_t lqi_refined_x = lqi_refinement * t.get_current().get_position().get_x() / 100;
							//uint8_t lqi_refined_y = lqi_refinement * t.get_current().get_position().get_y() / 100;
							//debug().debug(" lqi refined [%i, %i]", lqi_refined_x, lqi_refined_y );
							uint8_t x = ( single_traces_iterator->get_current().get_position().get_x() + t.get_current().get_position().get_x() ) / 2;
							uint8_t y = ( single_traces_iterator->get_current().get_position().get_y() + t.get_current().get_position().get_y() ) / 2;
							single_traces_iterator->set_current( Node( single_traces_iterator->get_current().get_id(), Position( x, y ) ) );
							single_store_flag = 0;
							single_traces_iterator = single_traces.end();
						}
						else
						{
							++single_traces_iterator;
						}
					}
					if ( single_store_flag )
					{
						single_traces.push_back( t );
					}
#endif
				}
			}
			else if ( msg_id == DETECTION_ID )
			{
				FLOOD_TRACKING_Trace t = FLOOD_TRACKING_Trace( message->payload() );
				FLOOD_TRACKING_TraceListIterator traces_iterator = traces.begin();
#ifdef PASSIVE_NODE
				t.set_current( self );
				t.set_detection_LQI( exdata.link_metric() );
				traces.push_back( t );
				debug().debug(" Passive %x received detection trace of target %x from %x with start_time %i and point %x, [%i,%i] and lqi %i", self.get_id(), t.get_target_id(), from, t.get_start_time(), t.get_current().get_id(), t.get_current().get_position().get_x(), t.get_current().get_position().get_y(), t.get_detection_LQI() );
				block_data_t buff[Radio::MAX_MESSAGE_LENGTH];
				block_data_t* buf = buff;
				t.set_buffer_from( buf );
				//if ( exdata.link_metric() < 231 )
				//{
					send( Radio::BROADCAST_ADDRESS, t.get_buffer_size(), buf, SPREAD_ID );
				//}
				//debug().debug(" Passive %x broadcasted detection trace", self.get_id() );
#endif
#ifdef TRACKER_NODE
				//debug().debug(" Tracker %x received detection trace of target %x from passive %x with start_time %i and detection point %x, [%i,%i] and lqi %i", self.get_id(), t.get_target_id(), from, t.get_start_time(), t.get_current().get_id(), t.get_current().get_position().get_x(), t.get_current().get_position().get_y(), t.get_detection_LQI() );
				//while ( traces_iterator != traces.end() )
				//{
				//	if ( t.get_target_id() == self.get_id() &&
				//		traces_iterator->get_current().get_id() == t.get_current().get_id() &&
				//		traces_iterator->get_start_time() == t.get_start_time() )
				//	{
				//		//debug().debug(" Tracker %x detection trace (%i,%i) vs (%i,%i) = (%i, %i)", self.get_id() , traces_iterator->get_current().get_position().get_x(), traces_iterator->get_current().get_position().get_y(), t.get_current().get_position().get_x(), t.get_current().get_position().get_y(), ( traces_iterator->get_current().get_position().get_x() + t.get_current().get_position().get_x() ) / 2, ( traces_iterator->get_current().get_position().get_y() + t.get_current().get_position().get_y() ) / 2 );
				//		//uint8_t y = ( traces_iterator->get_current().get_position().get_y() + t.get_current().get_position().get_y() ) / 2;
				//		//uint8_t x = ( traces_iterator->get_current().get_position().get_x() + t.get_current().get_position().get_x() ) / 2;
				//		//traces_iterator->set_current( Node( traces_iterator->get_current().get_id(), Position( x, y ) ) );
				//		//debug().debug(" Tracker %x refining detection trace (%i,%i) ", self.get_id() , traces_iterator->get_current().get_position().get_x(), traces_iterator->get_current().get_position().get_y() );
				//	}
				//	++traces_iterator;
				//}
#endif
			}
		}
		void targets_position( void* userdata = NULL )
		{
			//debug().debug(" sizeof target list = %i %i", single_traces.size(), traces.size() );
			for ( FLOOD_TRACKING_TraceListIterator i = single_traces.begin(); i != single_traces.end(); ++i )
			{
				debug().debug("%i:%i:%i", i->get_target_id(), i->get_current().get_position().get_x(), i->get_current().get_position().get_y() );
			}
			timer().template set_timer<self_type, &self_type::targets_position>( 1000, this, 0 );
		}
		// -----------------------------------------------------------------------
		void clean_traces( void* userdata = NULL )
		{
			traces.clear();
			timer().template set_timer<self_type, &self_type::clean_traces>( 15000, this, 0);
		}
		// -----------------------------------------------------------------------
		void init( Radio& radio, Timer& timer, Debug& debug, Rand& rand, Clock& clock )
		{
			radio_ = &radio;
			timer_ = &timer;
			debug_ = &debug;
			rand_ = &rand;
			clock_ = &clock;
		}
		// -----------------------------------------------------------------------
		FLOOD_TRACKING_TraceList* get_traces()
		{
			return &traces;
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
		Rand& rand()
		{
			return *rand_;
		}
		Clock& clock()
		{
			return *clock_;
		}
		Radio * radio_;
		Timer * timer_;
		Debug * debug_;
		Rand * rand_;
		Clock * clock_;
		enum MessageIds
		{
			DETECTION_ID = 11,
			SPREAD_ID = 22,
		};
		uint32_t radio_callback_id_;
		FLOOD_TRACKING_TraceList traces;
		FLOOD_TRACKING_TraceList single_traces;
		Node self;
		uint16_t transmission_dB;
   	};
}
#endif
