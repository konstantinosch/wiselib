/**************************************************************************
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

#ifndef __ATP_H__
#define __ATP_H__

#include "ATP_source_config.h"
#include "ATP_default_values_config.h"
#include "../../internal_interface/message/message.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename NeighborDiscovery_P,
				typename Timer_P,
				typename Rand_P,
				typename Clock_P,
				typename Debug_P
			>
	class ATP_Type
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Rand_P Rand;
		typedef typename Rand::rand_t rand_t;
		typedef Debug_P Debug;
		typedef NeighborDiscovery_P NeighborDiscovery;
		typedef Timer_P Timer;
		typedef Clock_P Clock;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Radio::ExtendedData ExtendedData;
		typedef typename Timer::millis_t millis_t;
		typedef typename Radio::TxPower TxPower;
		typedef typename Clock::time_t time_t;
		typedef Message_Type<Os, Radio, Debug> Message;
		typedef ATP_Type<Os, Radio, NeighborDiscovery, Timer, Rand, Clock, Debug> self_type;
		typedef typename NeighborDiscovery::ProtocolSettings ProtocolSettings;
		typedef typename NeighborDiscovery::Neighbor Neighbor;
		typedef typename NeighborDiscovery::ProtocolPayload ProtocolPayload;
		typedef typename NeighborDiscovery::Protocol Protocol;
		typedef typename NeighborDiscovery::Beacon Beacon;
		typedef typename NeighborDiscovery::Neighbor_vector Neighbor_vector;
		typedef typename NeighborDiscovery::Neighbor_vector_iterator Neighbor_vector_iterator;
		typedef typename NeighborDiscovery::ProtocolPayload_vector ProtocolPayload_vector;
		typedef typename NeighborDiscovery::ProtocolPayload_vector_iterator ProtocolPayload_vector_iterator;
		typedef wiselib::ATP_Type<Os, Radio, NeighborDiscovery, Timer, Rand, Clock, Debug> ATP;
		// -----------------------------------------------------------------------
		ATP_Type() :
			radio_callback_id						( 0 ),
			transmission_power_dB					( ATP_H_TRANSMISSION_POWER_DB ),
			nb_convergence_time						( ATP_H_CONVERGENCE_TIME ),
			nb_convergence_time_counter				( 0 ),
			nb_convergence_time_max_counter			( ATP_H_MAX_MONITORING_PHASES ),
			nb_connections_high						( ATP_H_SCL_DMAX ),
			nb_connections_low						( ATP_H_SCL_DMIN ),
			random_enable_timer_range				( ATP_H_RANDOM_ENABLE_TIMER_RANGE ),
			status									( WAITING_STATUS )
		{
		}
		// -----------------------------------------------------------------------
		~ATP_Type()
		{
		}
		// -----------------------------------------------------------------------
		void enable( void )
		{
#ifdef DEBUG_ATP_H_ENABLE
			debug().debug( "ATP - enable %x - Entering.\n", radio().id() );
#endif
			radio().enable_radio();
			set_status( ACTIVE_STATUS );
#ifndef CONFIG_ATP_H_RANDOM_BOOT
			neighbor_discovery_enable_task();
#else
			millis_t r = rand()() % random_enable_timer_range;
			timer().template set_timer<self_type, &self_type::neighbor_discovery_enable_task> ( r, this, 0 );
#endif
#ifdef DEBUG_ATP_H_ENABLE
			debug().debug( "ATP - enable %x - Exiting.\n", radio().id() );
#endif
		}
		// -----------------------------------------------------------------------
		void neighbor_discovery_enable_task( void* _userdata = NULL )
		{
#ifdef DEBUG_ATP_H_NEIGHBOR_DISCOVERY_ENABLE_TASK
			debug().debug( "ATP - neighbor_discovery_enable_task %x - Entering.\n", radio().id() );
#endif
			block_data_t buff[100];
			ProtocolPayload pp( NeighborDiscovery::ATP_PROTOCOL_ID, 0, buff );
			uint8_t ef = ProtocolSettings::NEW_PAYLOAD|ProtocolSettings::LOST_NB|ProtocolSettings::NB_REMOVED|ProtocolSettings::NEW_PAYLOAD;
			ProtocolSettings ps( /*255, 0, 255, 0, 255, 0, 255* 0,*/ 100, 90, 100, 90, ef, -18, 100, 3000, 100, ProtocolSettings::RATIO_DIVIDER, 2, ProtocolSettings::MEAN_DEAD_TIME_PERIOD, 100, 100, ProtocolSettings::R_NR_WEIGHTED, 1, 1, pp );
#ifdef CONFIG_ATP_H_RANDOM_DB
			transmission_power_dB = (rand()()%5)*(-6);
			debug().debug("RAND_DB:%x:%d\n", radio().id(), transmission_power_dB );
#endif
			neighbor_discovery().set_transmission_power_dB( transmission_power_dB );
			uint8_t result = 0;
			result = neighbor_discovery(). template register_protocol<self_type, &self_type::sync_neighbors>( NeighborDiscovery::ATP_PROTOCOL_ID, ps, this  );
			Protocol* prot_ref = neighbor_discovery().get_protocol_ref( NeighborDiscovery::ATP_PROTOCOL_ID );
			if ( prot_ref != NULL )
			{
				neighbor_discovery().enable();
#ifdef DEBUG_ATP_H_STATS
				#ifdef	DEBUG_ATP_H_STATS_SHAWN
						debug().debug("COORD:%d:%d:%f:%f\n", nb_convergence_time_max_counter, radio().id(), neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
				#endif
				#ifdef	DEBUG_ATP_H_STATS_ISENSE
						debug().debug("COORD:%d:%x:%d:%d\n", nb_convergence_time_max_counter, radio().id(), neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
				#endif
#ifdef DEBUG_ATP_H_STATS_SHAWN
				debug().debug("CON:%d:%d:%d:%d:%d:%d:%d:%f:%f\n", nb_convergence_time_counter, radio().id(), prot_ref->get_neighborhood_active_size(), prot_ref->get_neighborhood_ref()->size(), transmission_power_dB, nb_convergence_time, nb_convergence_time_max_counter, neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug("CON:%d:%x:%d:%d:%i:%d:%d:%d:%d\n", nb_convergence_time_counter, radio().id(), prot_ref->get_neighborhood_active_size(), prot_ref->get_neighborhood_ref()->size(), transmission_power_dB, nb_convergence_time, nb_convergence_time_max_counter, neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
#endif
#endif
				timer().template set_timer<self_type, &self_type::neighbor_discovery_inter_task> ( nb_convergence_time/nb_convergence_time_max_counter, this, 0 );
			}
#ifdef DEBUG_ATP_H_NEIGHBOR_DISCOVERY_ENABLE_TASK
			debug().debug( "ATP - neighbor_discovery_enable_task - Exiting.\n" );
#endif
		}
		// -----------------------------------------------------------------------
		void neighbor_discovery_inter_task(void* _userdata = NULL )
		{
			Protocol* prot_ref = neighbor_discovery().get_protocol_ref( NeighborDiscovery::ATP_PROTOCOL_ID );
			Protocol* prot_ref_ND = neighbor_discovery().get_protocol_ref( NeighborDiscovery::ND_PROTOCOL_ID );
#ifdef DEBUG_ATP_H_STATS_SHAWN
				debug().debug("CON:%d:%d:%d:%d:%d:%d:%d:%f:%f\n", nb_convergence_time_counter, radio().id(), prot_ref->get_neighborhood_active_size(), prot_ref->get_neighborhood_ref()->size(), transmission_power_dB, nb_convergence_time, nb_convergence_time_max_counter, neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug("CON:%d:%x:%d:%d:%i:%d:%d:%d:%d\n", nb_convergence_time_counter, radio().id(), prot_ref->get_neighborhood_active_size(), prot_ref->get_neighborhood_ref()->size(), transmission_power_dB, nb_convergence_time, nb_convergence_time_max_counter, neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
#endif
			if ( prot_ref->get_neighborhood_active_size() < nb_connections_low )
			{
				int8_t old_transmission_power_dB = transmission_power_dB;
#ifdef CONFIG_ATP_H_FLEXIBLE_DB
				transmission_power_dB = transmission_power_dB + 6;
#endif
				if ( transmission_power_dB > ATP_H_NB_INTER_TASK_MAX_DB )
				{
					transmission_power_dB = ATP_H_NB_INTER_TASK_MAX_DB;
				}
				if ( transmission_power_dB != old_transmission_power_dB )
				{
#ifdef DEBUG_ATP_H_NEIGHBOR_DISCOVERY_STATS
					debug().debug("%x - increasing radius from %i to %i\n", radio().id(), old_transmission_power_dB, transmission_power_dB );
#endif
				}
			}
			else if ( prot_ref->get_neighborhood_active_size() > nb_connections_high )
			{
				int8_t old_transmission_power_dB = transmission_power_dB;
#ifdef CONFIG_ATP_H_FLEXIBLE_DB
				transmission_power_dB = transmission_power_dB - 6;
#endif
				if ( transmission_power_dB < ATP_H_NB_INTER_TASK_MIN_DB )
				{
					transmission_power_dB = ATP_H_NB_INTER_TASK_MIN_DB;
				}
				if ( transmission_power_dB != old_transmission_power_dB )
				{
#ifdef DEBUG_ATP_H_NEIGHBOR_DISCOVERY_STATS
					debug().debug("%x - decreasing radius from %i to %i\n", radio().id(), old_transmission_power_dB, transmission_power_dB );
#endif
				}
			}
			for ( Neighbor_vector_iterator i = prot_ref->get_neighborhood_ref()->begin(); i != prot_ref->get_neighborhood_ref()->end(); ++i )
			{
#ifdef	DEBUG_ATP_H_STATS_SHAWN
				debug().debug( "NB:%d:%d:%d:%f:%f\n", nb_convergence_time_counter+1, radio().id(), i->get_node().get_id(), neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
				debug().debug( "NB:%d:%d:%d:%f:%f\n", nb_convergence_time_counter+1, radio().id(), i->get_node().get_id(),i->get_position().get_x(), i->get_position().get_y() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "NB:%d:%x:%x:%d:%d\n", nb_convergence_time_counter+1, radio().id(), i->get_id(), neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
				debug().debug( "NB:%d:%x:%x:%d:%d\n", nb_convergence_time_counter+1, radio().id(), i->get_id(), i->get_position().get_x(), i->get_position().get_y() );
#endif
			}
#ifdef DEBUG_ATP_H_STATS
			if ( prot_ref->get_neighborhood_active_size() < nb_connections_low )
			{
#ifdef	DEBUG_ATP_H_STATS_SHAWN
				debug().debug( "LOCAL_MINIMUM:%d:%d:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "LOCAL_MINIMUM:%d:%x:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
			}
			else if (prot_ref->get_neighborhood_active_size() > nb_connections_high )
			{
#ifdef	DEBUG_ATP_H_STATS_SHAWN
				debug().debug( "LOCAL_MAXIMUM:%d:%d:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "LOCAL_MAXIMUM:%d:%x:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
			}
#endif
			neighbor_discovery().set_transmission_power_dB( transmission_power_dB );
			nb_convergence_time_counter = nb_convergence_time_counter + 1;
#ifdef CONFIG_ATP_H_MEMORYLESS_STATISTICS
			prot_ref_ND->get_protocol_settings_ref()->set_beacon_weight( nb_convergence_time_counter );
			prot_ref_ND->get_protocol_settings_ref()->set_lost_beacon_weight( nb_convergence_time_counter );
			prot_ref->get_protocol_settings_ref()->set_beacon_weight( nb_convergence_time_counter );
			prot_ref->get_protocol_settings_ref()->set_lost_beacon_weight( nb_convergence_time_counter );
#endif
			if ( nb_convergence_time_counter < nb_convergence_time_max_counter )
			{
				timer().template set_timer<self_type, &self_type::neighbor_discovery_inter_task> ( nb_convergence_time/nb_convergence_time_max_counter, this, 0 );
			}
			else
			{
				timer().template set_timer<self_type, &self_type::neighbor_discovery_disable_task> ( nb_convergence_time/nb_convergence_time_max_counter, this, 0 );
			}
		}
		// -----------------------------------------------------------------------
		void neighbor_discovery_disable_task( void* _userdata = NULL )
		{
#ifdef DEBUG_ATP_H_NEIGHBOR_DISCOVERY_DISABLE_TASK
			debug().debug( "ATP - neighbor_discovery_unregister_task %x - Entering.\n", radio().id() );
#endif
			Protocol* prot_ref = neighbor_discovery().get_protocol_ref( NeighborDiscovery::ATP_PROTOCOL_ID );
			for ( Neighbor_vector_iterator i = prot_ref->get_neighborhood_ref()->begin(); i != prot_ref->get_neighborhood_ref()->end(); ++i )
			{
#ifdef	DEBUG_ATP_H_STATS_SHAWN
				debug().debug( "NB:%d:%d:%d:%f:%f\n", nb_convergence_time_counter+1, radio().id(), i->get_node().get_id(), neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
				debug().debug( "NB:%d:%d:%d:%f:%f\n", nb_convergence_time_counter+1, radio().id(), i->get_node().get_id(),i->get_position().get_x(), i->get_position().get_y() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "NB:%d:%x:%x:%d:%d\n", nb_convergence_time_counter+1, radio().id(), i->get_id(), neighbor_discovery().get_position().get_x(),  neighbor_discovery().get_position().get_y() );
				debug().debug( "NB:%d:%x:%x:%d:%d\n", nb_convergence_time_counter+1, radio().id(), i->get_id(), i->get_position().get_x(), i->get_position().get_y() );
#endif
			}
#ifdef DEBUG_ATP_H_STATS
			if ( prot_ref->get_neighborhood_active_size() < nb_connections_low )
			{
#ifdef	DEBUG_ATP_H_STATS_SHAWN
				debug().debug( "LOCAL_MINIMUM:%d:%d:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "LOCAL_MINIMUM:%d:%x:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
			}
			else if (prot_ref->get_neighborhood_active_size() > nb_connections_high )
			{
#ifdef	DEBUG_ATP_H_STATS_SHAWN
				debug().debug( "LOCAL_MAXIMUM:%d:%d:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
#ifdef	DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "LOCAL_MAXIMUM:%d:%x:%d\n", nb_convergence_time_counter, radio().id(),  prot_ref->get_neighborhood_active_size() );
#endif
			}
#endif
#ifdef CONFIG_ATP_H_DISABLE_NEIGHBOR_DISCOVERY
			neighbor_discovery().disable();
#endif
#ifdef DEBUG_ATP_H_NEIGHBOR_DISCOVERY_DISABLE_TASK
			debug().debug( "ATP - neighbor_discovery_unregister_task %x - Exiting.\n", radio().id() );
#endif
#ifdef CONFIG_ATP_H_END_EXP
			timer().template set_timer<self_type, &self_type::end_exp> ( ATP_H_END_EXP_TIMER, this, 0 );
#endif
		}
		// -----------------------------------------------------------------------
#ifdef CONFIG_ATP_H_END_EXP
		void end_exp(void* user_data = NULL )
		{
			radio().disable_radio();
		}
#endif
		// -----------------------------------------------------------------------
		void disable( void )
		{
			set_status( WAITING_STATUS );
			radio().unreg_recv_callback( radio_callback_id );
		}
		// -----------------------------------------------------------------------
		void sync_neighbors( uint8_t _event, node_id_t _from, size_t _len, uint8_t* _data )
		{
#ifdef DEBUG_ATP_H_STATS_ISENSE
			debug().debug( "ATP - sync_neighbors %x - Entering.\n", radio().id() );
#endif
			if ( _event & ProtocolSettings::NEW_PAYLOAD )
			{
#ifdef DEBUG_ATP_H_STATS_ISENSE
			debug().debug( "ATP - sync_neighbors %x - NEW_PAYLOAD.\n", radio().id() );
#endif
				return;
			}
			else if ( _event & ProtocolSettings::LOST_NB )
			{

#ifdef DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "ATP - sync_neighbors %x - LOST_NB %x.\n", radio().id(), _from );
#endif
				return;
			}
			else if ( _event & ProtocolSettings::NB_REMOVED )
			{

#ifdef DEBUG_ATP_H_STATS_ISENSE
				debug().debug( "ATP - sync_neighbors %x - NB_REMOVED %x.\n", radio().id(), _from );
#endif
				return;
			}
		}
		// -----------------------------------------------------------------------
		void init( Radio& radio, Timer& timer, Debug& debug, Rand& rand, Clock& clock, NeighborDiscovery& neighbor_discovery )
		{
			_radio = &radio;
			_timer = &timer;
			_debug = &debug;
			_rand = &rand;
			_clock = &clock;
			_neighbor_discovery = &neighbor_discovery;
		}
		// -----------------------------------------------------------------------
		void set_status( int _st )
		{
			status = _st;
		}
		// -----------------------------------------------------------------------
	private:
		Radio& radio()
		{
			return *_radio;
		}
		// -----------------------------------------------------------------------
		Timer& timer()
		{
			return *_timer;
		}
		// -----------------------------------------------------------------------
		Debug& debug()
		{
			return *_debug;
		}
		// -----------------------------------------------------------------------
		Rand& rand()
		{
			return *_rand;
		}
		// -----------------------------------------------------------------------
		Clock& clock()
		{
			return *_clock;
		}
		// -----------------------------------------------------------------------
		NeighborDiscovery& neighbor_discovery()
		{
			return *_neighbor_discovery;
		}
		// -----------------------------------------------------------------------
		Radio* _radio;
		Timer* _timer;
		Debug* _debug;
		Rand* _rand;
		Clock* _clock;
		NeighborDiscovery* _neighbor_discovery;
		enum atp_status
		{
			ACTIVE_STATUS,
			WAITING_STATUS,
			ATP_STATUS_NUM_VALUES
		};
		uint32_t radio_callback_id;
		int8_t transmission_power_dB;
		millis_t nb_convergence_time;
		uint32_t nb_convergence_time_counter;
		uint32_t nb_convergence_time_max_counter;
		uint16_t nb_connections_high;
		uint16_t nb_connections_low;
		uint32_t random_enable_timer_range;
		uint8_t status;
	};

}
#endif
