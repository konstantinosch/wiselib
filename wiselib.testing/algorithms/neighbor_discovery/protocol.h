#ifndef PROTOCOL_H
#define	PROTOCOL_H

#include "neighbor.h"
#include "protocol_settings.h"
#include "protocol_payload.h"
#include "util/pstl/vector_static.h"
#include "util/delegates/delegate.hpp"

namespace wiselib
{
	template< 	typename Os_P,
				typename Radio_P,
				typename Clock_P,
				typename Timer_P,
				typename Debug_P>
	class Protocol
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Clock_P Clock;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef Neighbor<Os, Radio, Clock, Timer, Debug> Neighbor;
		typedef ProtocolPayload<Os, Radio, Debug> ProtocolPayload;
		typedef ProtocolSettings<Os, Radio, Timer, Debug> ProtocolSettings;
		typedef vector_static<Os, Neighbor, NB_MAX_NEIGHBORS> Neighbor_vector;
		typedef typename Neighbor_vector::iterator Neighbor_vector_iterator;
		typedef vector_static<Os, ProtocolPayload, NB_MAX_REGISTERED_PROTOCOLS> ProtocolPayload_vector;
		typedef typename ProtocolPayload_vector::iterator ProtocolPayload_vector_iterator;
		typedef delegate4<void, uint8_t, node_id_t, uint8_t, uint8_t*> event_notifier_delegate_t;
		typedef Protocol<Os, Radio, Clock, Timer, Debug> self_type;
		// --------------------------------------------------------------------
		Protocol() :
			protocol_id					( 0 ),
			event_notifier_callback		( event_notifier_delegate_t::template from_method<Protocol, &Protocol::null_callback > ( this ) )
		{}
		~Protocol()
		{}
		// --------------------------------------------------------------------
		void set_protocol_id( uint8_t _pid )
		{
			protocol_id = _pid;
			settings.get_protocol_payload_ref()->set_protocol_id( _pid );
		}
		// --------------------------------------------------------------------
		uint8_t get_protocol_id()
		{
			return protocol_id;
		}
		// --------------------------------------------------------------------
		event_notifier_delegate_t get_event_notifier_callback()
		{
			return event_notifier_callback;
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)(uint8_t, node_id_t, uint8_t, uint8_t*) >
		void set_event_notifier_callback( T *_obj_pnt )
		{
			event_notifier_callback = event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt );
		}
		// --------------------------------------------------------------------
		void set_event_notifier_callback( event_notifier_delegate_t _enc )
		{
			event_notifier_callback = _enc;
		}
		// --------------------------------------------------------------------
		void reset_event_notifier_callback()
		{
			event_notifier_callback	= event_notifier_delegate_t::template from_method<Protocol, &Protocol::null_callback > ( this );
		}
		// --------------------------------------------------------------------
		ProtocolSettings get_protocol_settings()
		{
			return settings;
		}
		// --------------------------------------------------------------------
		ProtocolSettings* get_protocol_settings_ref()
		{
			return &settings;
		}
		// --------------------------------------------------------------------
		void set_protocol_settings( ProtocolSettings& _ps )
		{
			settings = _ps;
		}
		// --------------------------------------------------------------------
		Neighbor_vector* get_neighborhood_ref()
		{
			return &neighborhood;
		}
		// --------------------------------------------------------------------
		Neighbor_vector get_neighborhood()
		{
			return neighborhood;
		}
		// --------------------------------------------------------------------
		void set_neighborhood( Neighbor_vector& _nv )
		{
			neighborhood = _nv;
		}
		// --------------------------------------------------------------------
		Neighbor* get_neighbor_ref( node_id_t _nid )
		{
			for ( Neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
			{
				if (it->get_id() == _nid )
				{
					return &(*it);
				}
			}
			return NULL;
		}
		// --------------------------------------------------------------------
		void resolve_overflow_strategy( node_id_t _nid )
		{
			Neighbor* n_ref = get_neighbor_ref( _nid );
			if ( n_ref != NULL )
			{
				ProtocolSettings* p_ref = get_protocol_settings_ref();
				if ( n_ref->get_total_beacons() == 0xffffffff || n_ref->get_total_beacons_expected() == 0xffffffff )
				{
					if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_TOTAL_BEACONS )
					{
						n_ref->set_total_beacons( 0 );
					}
					if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_TOTAL_BEACONS_EXPECTED )
					{
						n_ref->set_total_beacons_expected( 0 );
					}
					if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_AVG_LQI )
					{
						n_ref->set_avg_LQI( 0 );
					}
					if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_AVG_LQI_INVERSE )
					{
						n_ref->set_avg_LQI_inverse( 0 );
					}
					if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_STAB )
					{
						n_ref->set_link_stab_ratio( 0 );
					}
					if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_STAB_INVERSE )
					{
						n_ref->set_link_stab_ratio_inverse( 0 );
					}
					if ( p_ref->get_overflow_strategy() == ProtocolSettings::RATIO_DIVIDER )
					{
						n_ref->set_total_beacons( n_ref->get_total_beacons() / p_ref->get_ratio_divider() );
						n_ref->set_total_beacons_expected( n_ref->get_total_beacons_expected() / p_ref->get_ratio_divider() );
					}
					else
					{
						n_ref->set_total_beacons( n_ref->get_total_beacons() / p_ref->get_ratio_divider() );
						n_ref->set_total_beacons_expected( n_ref->get_total_beacons_expected() / p_ref->get_ratio_divider() );
					}
				}
			}
		}
		// --------------------------------------------------------------------
		uint32_t resolve_beacon_weight( node_id_t _nid )
		{
			Neighbor* n_ref = get_neighbor_ref( _nid );
			if ( n_ref != NULL )
			{
				ProtocolSettings* p_ref = get_protocol_settings_ref();
				if ( p_ref->get_ratio_normalization_strategy() == ProtocolSettings::R_NR_NORMAL )
				{
					return 1;
				}
				else if ( p_ref->get_ratio_normalization_strategy() == ProtocolSettings::R_NR_WEIGHTED )
				{
					return p_ref->get_beacon_weight();
				}
				else if( p_ref->get_ratio_normalization_strategy() == ProtocolSettings::R_NR_WEIGHTED_PROPORTIONAL )
				{
					return p_ref->get_beacon_weight() * n_ref->get_total_beacons_expected() / 100;
				}
				return 1;
			}
			return 0;
		}
		// --------------------------------------------------------------------
		uint32_t resolve_lost_beacon_weight( node_id_t _nid )
		{
			Neighbor* n_ref = get_neighbor_ref( _nid );
			if ( n_ref != NULL )
			{
				ProtocolSettings* p_ref = get_protocol_settings_ref();
				if ( p_ref->get_ratio_normalization_strategy() == ProtocolSettings::R_NR_NORMAL )
				{
					return 1;
				}
				else if ( p_ref->get_ratio_normalization_strategy() == ProtocolSettings::R_NR_WEIGHTED )
				{
					return p_ref->get_lost_beacon_weight();
				}
				else if( p_ref->get_ratio_normalization_strategy() == ProtocolSettings::R_NR_WEIGHTED_PROPORTIONAL )
				{
					return p_ref->get_lost_beacon_weight() * n_ref->get_total_beacons_expected() / 100;
				}
				return 1;
			}
			return 0;
		}
		// --------------------------------------------------------------------
		Protocol& operator=( const Protocol& _p )
		{
			protocol_id = _p.protocol_id;
			event_notifier_callback = _p.event_notifier_callback;
			settings = _p.settings;
			neighborhood = _p.neighborhood;
			return *this;
		}
		// --------------------------------------------------------------------
		void print( Debug& debug )
		{
			debug.debug( "-------------------------------------------------------");
			debug.debug( "protocol :");
			debug.debug( "protocol_id : %d", protocol_id );
			debug.debug( "settings :");
			settings.print( debug );
			debug.debug( "neighborhood :");
			for ( Neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
			{
				it->print( debug );
			}
			debug.debug( "-------------------------------------------------------");
		}
		// --------------------------------------------------------------------
		void null_callback( uint8_t null_event, node_id_t null_node_id, uint8_t null_len, uint8_t* null_data )
		{}
		// --------------------------------------------------------------------
	private:
		uint8_t protocol_id;
		event_notifier_delegate_t event_notifier_callback;
		ProtocolSettings settings;
		Neighbor_vector neighborhood;
		ProtocolPayload protocol_payload;
	};
}
#endif
