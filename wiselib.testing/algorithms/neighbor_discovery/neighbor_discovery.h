#ifndef NEIGHBOR_DISCOVERY_H
#define	NEIGHBOR_DISCOVERY_H

#include "util/pstl/vector_static.h"
#include "util/delegates/delegate.hpp"
#include "neighbor_discovery_config.h"
#include "neighbor_discovery_message.h"
#include "neighbor.h"
#include "protocol_settings.h"
#include "protocol_payload.h"
#include "protocol.h"
#include "beacon.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Clock_P,
				typename Timer_P,
				typename Debug_P>
	class NeighborDiscovery
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef Clock_P Clock;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Clock::time_t time_t;
		typedef typename Radio::ExtendedData ExData;
		typedef typename Radio::TxPower TxPower;
		typedef typename Timer::millis_t millis_t;
		typedef NeighborDiscovery<Os, Radio, Clock, Timer, Debug> self_t;
		typedef NeighborDiscovery_MessageType<Os, Radio> Message;
		typedef Neighbor<Os, Radio, Clock, Timer, Debug> Neighbor;
		typedef ProtocolPayload< Os, Radio, Debug> ProtocolPayload;
		typedef ProtocolSettings<Os, Radio, Timer, Debug> ProtocolSettings;
		typedef Protocol<Os, Radio, Clock, Timer, Debug> Protocol;
		typedef Beacon<Os, Radio, Clock, Timer, Debug> Beacon;
		typedef vector_static<Os, Neighbor, NB_MAX_NEIGHBORS> Neighbor_vector;
		typedef typename Neighbor_vector::iterator Neighbor_vector_iterator;
		typedef vector_static<Os, ProtocolPayload, NB_MAX_REGISTERED_PROTOCOLS> ProtocolPayload_vector;
		typedef typename ProtocolPayload_vector::iterator ProtocolPayload_vector_iterator;
		typedef vector_static<Os, Protocol, NB_MAX_REGISTERED_PROTOCOLS> Protocol_vector;
		typedef typename Protocol_vector::iterator Protocol_vector_iterator;
		typedef delegate4<void, uint8_t, node_id_t, uint8_t, uint8_t*> event_notifier_delegate_t;
		// --------------------------------------------------------------------
		NeighborDiscovery()	:
			status								( WAITING_STATUS ),
			beacon_period						( NB_BEACON_PERIOD ),
			channel								( NB_CHANNEL ),
			transmission_power_dB				( NB_TRANSMISSION_POWER_DB ),
			protocol_max_payload_size			( NB_MAX_PROTOCOL_PAYLOAD_SIZE ),
			transmission_power_dB_strategy		( FIXED_PERIOD ),
			protocol_max_payload_size_strategy	( FIXED_PAYLOAD_SIZE ),
			beacon_period_strategy				( FIXED_TRANSM ),
			relax_millis						( NB_RELAX_MILLIS )
		{};
		// --------------------------------------------------------------------
		~NeighborDiscovery()
		{};
		// --------------------------------------------------------------------
		void enable()
		{
			Protocol p;
			Neighbor n;
			n.set_id( radio().id() );
			Neighbor_vector neighbors;
			neighbors.push_back( n );
			ProtocolPayload pp;
			pp.set_protocol_id( NB_PROTOCOL_ID );
			pp.set_payload_size( 0 );
			uint8_t ef = 			ProtocolSettings::NEW_NB_NO_BIDI|ProtocolSettings::NEW_NB_BIDI|ProtocolSettings::NEW_PAYLOAD_NO_BIDI|ProtocolSettings::NEW_PAYLOAD_BIDI|
									ProtocolSettings::LOST_NB_NO_BIDI|ProtocolSettings::LOST_NB_BIDI|ProtocolSettings::TRANS_DB_UPDATE|ProtocolSettings::BEACON_PERIOD_UPDATE;
			ProtocolSettings ps( 255, 0, 255, 0, 100, 0, 100, 0, 5, 5, ef, -6, 100, 3000, 100, ProtocolSettings::RATIO_DIVIDER, 2, pp );
			p.set_protocol_id( NB_PROTOCOL_ID );
			p.set_neighborhood( neighbors );
			p.set_protocol_settings( ps );
			protocols.push_back( p );
			set_status( ACTIVE_STATUS );
			radio().enable_radio();
			recv_callback_id_ = radio().template reg_recv_callback<self_t, &self_t::receive>( this );
			beacons();
		};
		// --------------------------------------------------------------------
		void disable()
		{
			set_status( WAITING_STATUS );
			protocols.clear();
			radio().template unreg_recv_callback( recv_callback_id_ );
		};
		// --------------------------------------------------------------------
		void send( node_id_t _dest, size_t _len, block_data_t* _data, message_id_t _msg_id )
		{
			Message message;
			message.set_msg_id( _msg_id );
			message.set_payload( _len, _data );
			TxPower power;
			power.set_dB( get_transmission_power_dB() );
			radio().set_channel( get_channel() );
			radio().set_power( power );
			radio().send( _dest, message.buffer_size(), (uint8_t*) &message );
		}
		// --------------------------------------------------------------------
		void beacons( void* _data = NULL )
		{
#ifdef NB_DEBUG_BEACONS
			debug().debug("NeighborDiscovery-beacons %x - Entering", radio().id() );
#endif
			if ( get_status() == ACTIVE_STATUS )
			{
				Protocol* p_ptr = get_protocol_ref( NB_PROTOCOL_ID );
				if ( p_ptr != NULL )
				{
#ifdef NB_DEBUG_BEACONS
					debug().debug("NeighborDiscovery-beacons %x - Protocol exists.", radio().id() );
#endif
					millis_t bp = get_beacon_period();
					Neighbor* n = p_ptr->get_neighbor_ref( radio().id() );
					if ( n != NULL )
					{
#ifdef NB_DEBUG_BEACONS
						debug().debug("NeighborDiscovery-beacons %x - Neighbor exists.", radio().id() );
#endif
						n->inc_beacon_period_update_counter();
						n->set_beacon_period( bp );
						Beacon beacon;
						beacon.set_protocol_payloads( protocols );
						Neighbor_vector nv = p_ptr->get_neighborhood();
						beacon.set_neighborhood( nv, radio().id() );
						beacon.set_beacon_period( bp );
						beacon.set_beacon_period_update_counter( n->get_beacon_period_update_counter() );
						block_data_t buff[Radio::MAX_MESSAGE_LENGTH];
						beacon.serialize( buff );
						size_t beacon_size = beacon.serial_size();
						send( Radio::BROADCAST_ADDRESS, beacon_size, buff, NB_MESSAGE );
#ifdef NB_DEBUG_BEACONS
						debug().debug("NeighborDiscovery-beacons %x - Sending beacon.", radio().id() );
						beacon.print( debug() );
#endif
						timer().template set_timer<self_t, &self_t::beacons> ( bp, this, 0 );
					}
					else
					{
#ifdef NB_DEBUG_BEACONS
						debug().debug("NeighborDiscovery-beacons %x - Neighbor does not exist.", radio().id() );
#endif
					}
				}
				else
				{
#ifdef NB_DEBUG_BEACONS
					debug().debug("NeighborDiscovery-beacons %x - Protocol does not exist.", radio().id() );
#endif
				}
			}
#ifdef NB_DEBUG_BEACONS
			debug().debug("NeighborDiscovery-beacons %x - Exiting", radio().id() );
#endif
		}
		// --------------------------------------------------------------------
		void receive( node_id_t _from, size_t _len, block_data_t * _msg, ExData const &_ex )
		{
#ifdef NB_DEBUG_RECEIVE
			debug().debug("NeighborDiscovery-receive %x - Entering", radio().id() );
#endif
			message_id_t msg_id = *_msg;
			if ( msg_id == NB_MESSAGE )
			{
				Message *message = (Message*) _msg;
				Beacon beacon;
				beacon.de_serialize( message->payload() );
#ifdef NB_DEBUG_RECEIVE
				debug().debug("NeighborDiscovery-receive %x - Received beacon message.", radio().id() );
				beacon.print( debug() );
#endif
				time_t current_time = clock().time();
				uint8_t beacon_lqi = _ex.link_metric();
				uint8_t found_flag = 0;
				for ( Protocol_vector_iterator pit = protocols.begin(); pit != protocols.end(); ++pit )
				{
					Neighbor new_neighbor;
					for ( Neighbor_vector_iterator nit = pit->get_neighborhood_ref()->begin(); nit != pit->get_neighborhood_ref()->end(); ++nit )
					{
						if ( _from == nit->get_id() )
						{
#ifdef NB_DEBUG_RECEIVE
							debug().debug("NeighborDiscovery-receive %x - Neighbor %x is known.", radio().id(), _from );
#endif
							found_flag = 1;
						}
					}
					if ( found_flag == 0 )
					{
#ifdef NB_DEBUG_RECEIVE
						debug().debug("NeighborDiscovery-receive %x - Neighbor %x is unknown.", radio().id(), _from );
#endif
						new_neighbor.set_id( _from );
						new_neighbor.set_total_beacons( 1 );
						new_neighbor.set_total_beacons_expected( 1 );
						new_neighbor.set_avg_LQI( beacon_lqi );
						new_neighbor.set_link_stab_ratio();
						new_neighbor.set_consecutive_beacons( 1 );
						new_neighbor.set_consecutive_beacons_lost( 0 );
						new_neighbor.set_beacon_period( beacon.get_beacon_period() );
						new_neighbor.set_beacon_period_update_counter( beacon.get_beacon_period_update_counter() );
						new_neighbor.set_last_beacon( current_time );
						for ( Neighbor_vector_iterator nit = beacon.get_neighborhood_ref()->begin(); nit != beacon.get_neighborhood_ref()->end(); ++nit )
						{
#ifdef NB_DEBUG_RECEIVE
							debug().debug("NeighborDiscovery-receive %x - Neighbor %x was aware of node.", radio().id(), _from );
#endif
							new_neighbor.set_avg_LQI_inverse( nit->get_avg_LQI() );
							new_neighbor.set_link_stab_ratio_inverse( nit->get_link_stab_ratio() );
						}
					}
					uint8_t events_flag = 0;
					uint8_t payload_flag = 0;
					for ( ProtocolPayload_vector_iterator ppit = beacon.get_protocol_payloads_ref()->begin(); ppit != beacon.get_protocol_payloads_ref()->end(); ++ppit )
					{
						if ( ppit->get_protocol_id() == pit->get_protocol_id() )
						{
#ifdef NB_DEBUG_RECEIVE
							debug().debug("NeighborDiscovery-receive %x - Beacon carried a payload for protocol %x.", radio().id(), pit->get_protocol_id() );
#endif
							payload_flag = 1;
						}
						else
						{
#ifdef NB_DEBUG_RECEIVE
							debug().debug("NeighborDiscovery-receive %x - Beacon did not carry a payload for protocol %x.", radio().id(), pit->get_protocol_id() );
#endif
						}
					}
					if	(	( ( new_neighbor.get_avg_LQI() <= pit->get_protocol_settings_ref()->get_max_avg_LQI_threshold() ) || ( new_neighbor.get_avg_LQI() >= pit->get_protocol_settings_ref()->get_min_avg_LQI_threshold() ) ) &&
							( ( new_neighbor.get_link_stab_ratio() <= pit->get_protocol_settings_ref()->get_max_link_stab_ratio_threshold() ) || ( new_neighbor.get_link_stab_ratio() >= pit->get_protocol_settings_ref()->get_min_link_stab_ratio_threshold() ) ) &&
							( ( ( ( new_neighbor.get_consecutive_beacons() == 0 ) && ( new_neighbor.get_consecutive_beacons_lost() <= pit->get_protocol_settings_ref()->get_consecutive_beacons_lost_threshold() ) ) ) ||
							( ( ( new_neighbor.get_consecutive_beacons() == pit->get_protocol_settings_ref()->get_consecutive_beacons_threshold() ) && ( new_neighbor.get_consecutive_beacons_lost() == 0 ) ) ) ) )
					{
						events_flag = events_flag | ProtocolSettings::NEW_NB_NO_BIDI;
						if (	( ( new_neighbor.get_avg_LQI_inverse() <= pit->get_protocol_settings_ref()->get_max_avg_LQI_inverse_threshold() ) || ( new_neighbor.get_avg_LQI_inverse() >= pit->get_protocol_settings_ref()->get_min_avg_LQI_inverse_threshold() ) ) &&
								( ( new_neighbor.get_link_stab_ratio_inverse() <= pit->get_protocol_settings_ref()->get_max_link_stab_ratio_inverse_threshold() ) || ( new_neighbor.get_link_stab_ratio_inverse() >= pit->get_protocol_settings_ref()->get_max_link_stab_ratio_inverse_threshold() ) ) )
						{
							events_flag = events_flag | ProtocolSettings::NEW_NB_BIDI;
						}
						else
						{
							events_flag = events_flag | ProtocolSettings::LOST_NB_BIDI;
						}
					}
					else
					{
						events_flag = events_flag | ProtocolSettings::LOST_NB_NO_BIDI;
						events_flag = events_flag | ProtocolSettings::LOST_NB_BIDI;
					}
				}
			}
#ifdef NB_DEBUG_RECEIVE
			debug().debug("NeighborDiscovery-receive %x - Exiting", radio().id() );
#endif

//			message_id_t msg_id = *_msg;
//			Message *message = (Message*) _msg;
//			protocol received_protocol;
//			received_protocol.de_serialize( message->payload() );

//			uint8_t found = 0;
//			neighbor* neighbor_from = received_protocol.get_neighbor_ref( _from );
//			if ( msg_id == NB_MESSAGE )
//			{
//				for ( protocol_vector_iterator pit = protocols.begin(); pit != protocols.end(); ++pit )
//				{
//					for ( neighbor_vector_iterator it1 = pit->get_neighborhood_ref()->begin(); it1 != pit->get_neighborhood_ref()->end(); ++it1 )
//					{
//						if ( neighbor_from->get_id() == it1->get_id() )
//						{
//							uint32_t dead_time = ( clock().seconds( current_time ) - clock().seconds( it1->get_last_beacon() ) ) * 1000 +	( clock().milliseconds( current_time ) - clock().milliseconds( it1->get_last_beacon() ) );
//							found = 1;
//							if ( neighbor_from->get_beacon_period() == it1->get_beacon_period() )
//							{
//								if (	( dead_time < neighbor_from->get_beacon_period() + NB_RELAX_MILLIS ) &&
//										( dead_time > neighbor_from->get_beacon_period() - NB_RELAX_MILLIS ) ) //care relax millis must be considerably lower than the beacon_rate millis... by definition should be tested but around 50ms...
//								{
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected();
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->inc_consecutive_beacons();
//									it1->set_consecutive_beacons_lost( 0 );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//								else
//								{
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected( dead_time / it1->get_beacon_period() );
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->set_consecutive_beacons( 0 );
//									it1->inc_consecutive_beacons_lost( dead_time / it1->get_beacon_period() );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//							}
//							else if ( neighbor_from->get_beacon_period() != it1->get_beacon_period() )
//							{
//								if (	( dead_time < neighbor_from->get_beacon_period() + NB_RELAX_MILLIS ) &&
//										( dead_time > neighbor_from->get_beacon_period() - NB_RELAX_MILLIS ) )
//								{
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected();
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->inc_consecutive_beacons();
//									it1->set_consecutive_beacons_lost( 0 );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//								else
//								{
//									uint32_t last_beacon_period_update = neighbor_from->get_beacon_period_update_counter() * neighbor_from->get_beacon_period();
//									millis_t approximate_beacon_period = ( neighbor_from->get_beacon_period() + it1->get_beacon_period() ) / 2;
//									uint32_t dead_time_messages_lost = ( dead_time - last_beacon_period_update ) * approximate_beacon_period;
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected( dead_time_messages_lost + neighbor_from->get_beacon_period_update_counter() );
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->set_consecutive_beacons( 0 );
//									it1->inc_consecutive_beacons_lost( dead_time_messages_lost + neighbor_from->get_beacon_period_update_counter() );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//							}
//							for ( neighbor_vector_iterator it2 = received_protocol.get_neighborhood_ref()->begin(); it2 != received_protocol.get_neighborhood_ref()->end(); ++it2 )
//							{
//								if ( it2->get_id() == radio().id() )
//								{
//									it1->set_avg_LQI_inverse( it2->get_avg_LQI() );
//									it1->set_link_stab_ratio_inverse( it2->get_link_stab_ratio() );
//								}
//							}
//							//TODO
//							//flag process
//							//notify_receiver();
//						}
//					}
//					if ( !found )
//					{
//						neighbor new_neighbor;
//						new_neighbor.set_id( _from );
//						new_neighbor.set_total_beacons( 1 );
//						new_neighbor.set_total_beacons_expected( 1 );
//						new_neighbor.set_avg_LQI( lqi );
//						new_neighbor.set_link_stab_ratio();
//						new_neighbor.set_consecutive_beacons( 1 );
//						new_neighbor.set_consecutive_beacons_lost( 0 );
//						new_neighbor.set_beacon_period( neighbor_from->get_beacon_period() );
//						new_neighbor.set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//						new_neighbor.set_last_beacon( clock().time() );
//						for ( neighbor_vector_iterator it2 = received_protocol.get_neighborhood_ref()->begin(); it2 != received_protocol.get_neighborhood_ref()->end(); ++it2 )
//						{
//							if ( it2->get_id() == radio().id() )
//							{
//								new_neighbor.set_avg_LQI_inverse( it2->get_avg_LQI() );
//								new_neighbor.set_link_stab_ratio_inverse( it2->get_link_stab_ratio() );
//							}
//						}
//						pit->get_neighborhood_ref()->push_back( new_neighbor );
//						//TODO
//						//flag process
//						//notify_receiver();
//					}
//				}
//			}
		}
		// --------------------------------------------------------------------
		void store_neighbor( Neighbor _n )
		{
			for ( Protocol_vector_iterator pit = protocols.begin(); pit != protocols.end(); ++pit )
			{

				//if
				//( pit->get_events_flag() | ProtocolSettings::NEW_NB )
				//( pit->get_events_flag | ProtocolSettings::NEW_NB_BIDI ) )
				//( pit->get_events_flag() | ProtocolSettings::LOST_BIDI )
				//( pit->get_events_flag() | ProtocolSettings::LOST_NB_BIDI )
				//if ( pit->get_events_flag() )
			}
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)(uint8_t, node_id_t, uint8_t, uint8_t*) >
		uint8_t register_protocol( uint8_t _pid, ProtocolSettings _psett, T *_obj_pnt )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			if ( ( _psett.get_payload_size() > protocol_max_payload_size ) && ( protocol_max_payload_size_strategy == FIXED_PAYLOAD_SIZE ) )
			{
				return PAYLOAD_SIZE_OUT_OF_BOUNDS;
			}
			uint8_t total_payload_size = 0;
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_protocol_settings().get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(message_id_t) + sizeof(size_t) + _psett.get_payload_size() > Radio::MAX_MSG_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _pid )
				{
					return PROT_NUM_IN_USE;
				}
			}
			Protocol p;
			p.set_protocol_id( _pid );
			p.set_protocol_settings( _psett );
			p.set_event_notifier_callback( event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt ) );
			protocols.push_back( p );
			return SUCCESS;
		}
		// --------------------------------------------------------------------
		uint8_t register_protocol( Protocol p )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			if ( ( p.get_protocol_settings().get_payload_size() > protocol_max_payload_size ) && ( protocol_max_payload_size_strategy == FIXED_PAYLOAD_SIZE ) )
			{
				return PAYLOAD_SIZE_OUT_OF_BOUNDS;
			}
			uint8_t total_payload_size = 0;
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_protocol_settings().get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(message_id_t) + sizeof(size_t) + p.get_protocol_settings().get_payload_size() > Radio::MAX_MESSAGE_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == p.get_protocol_id() )
				{
					return PROT_NUM_IN_USE;
				}
			}
			protocols.push_back( p );
			return SUCCESS;
		}
		// --------------------------------------------------------------------
		uint8_t unregister_protocol( uint8_t _pid )
		{
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _pid )
				{
					protocols.erase( it );
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
		Protocol* get_protocol_ref( uint8_t _pid )
		{
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _pid )
				{
					return &(*it);
				}
			}
			return NULL;
		}
		// --------------------------------------------------------------------
		int get_status()
		{
			return status;
		}
		// --------------------------------------------------------------------
		void set_status( int _st )
		{
			status = _st;
		}
		// --------------------------------------------------------------------
		millis_t get_beacon_period()
		{
			if ( beacon_period_strategy == FIXED_PERIOD )
			{
				return beacon_period;
			}
			else if ( beacon_period_strategy == LEAST_PERIOD )
			{
				millis_t least = 0xfff;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					if ( it->get_protocol_settings_ref()->get_proposed_beacon_period() < least )
					{
						least = it->get_protocol_settings_ref()->get_proposed_beacon_period();
					}
				}
				return least;
			}
			else if ( beacon_period_strategy == MEAN_PERIOD )
			{
				millis_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_beacon_period() + sum;
					num = num + 1;
				}
				millis_t mean = sum / num;
				return mean;
			}
			else if ( beacon_period_strategy == WEIGHTED_MEAN_PERIOD )
			{
				millis_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_beacon_period() * it->get_protocol_settings_ref()->get_proposed_beacon_period_weight() + sum;
					num = it->get_protocol_settings_ref()->get_proposed_beacon_period_weight() + num;
				}
				millis_t weighted_mean = sum / num;
				return weighted_mean;
			}
			return beacon_period;
		}
		// --------------------------------------------------------------------
		void set_beacon_period( millis_t _bp )
		{
			beacon_period =_bp;
		}
		// --------------------------------------------------------------------
		int8_t get_transmission_power_dB()
		{
			if ( transmission_power_dB_strategy == FIXED_TRANSM )
			{
				return transmission_power_dB;
			}
			else if ( transmission_power_dB_strategy == LEAST_TRANSM )
			{
				int8_t least = 128;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					if ( it->get_protocol_settings_ref()->get_proposed_transmission_power_dB() < least )
					{
						least = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB();
					}
				}
				return least;
			}
			else if ( transmission_power_dB_strategy == MEAN_TRANSM )
			{
				int8_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB() + sum;
					num = num + 1;
				}
				int8_t mean = sum / num;
				return mean;
			}
			else if ( transmission_power_dB_strategy == WEIGHTED_MEAN_TRANSM )
			{
				int8_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB() * it->get_protocol_settings_ref()->get_proposed_transmission_power_dB_weight() + sum;
					num = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB_weight() + num;
				}
				int8_t weighted_mean = sum / num;
				return weighted_mean;
			}
			return transmission_power_dB;
		}
		// --------------------------------------------------------------------
		void set_transmission_power_dB( int8_t _tp_dB )
		{
			 transmission_power_dB = _tp_dB;
		}
		// --------------------------------------------------------------------
		uint8_t get_channel()
		{
			return channel;
		}
		// --------------------------------------------------------------------
		void set_channel( uint8_t _ch )
		{
			channel =_ch;
		}
		// --------------------------------------------------------------------
		uint8_t get_protocol_max_payload_size()
		{
			return protocol_max_payload_size;
		}
		// --------------------------------------------------------------------
		void set_transmission_power_dB_strategy( uint8_t _tp_dB_s )
		{
			if ( _tp_dB_s > TP_DB_STRATEGY_NUM_VALUES )
			{
				transmission_power_dB_strategy = FIXED_TRANSM;
			}
			else
			{
				transmission_power_dB_strategy = _tp_dB_s;
			}
		}
		// --------------------------------------------------------------------
		uint8_t get_transmission_power_dB_strategy()
		{
			return transmission_power_dB_strategy;
		}
		// --------------------------------------------------------------------
		void set_beacon_period_strategy( uint8_t _bp_s )
		{
			if ( _bp_s > BP_STRATEGY_NUM_VALUES )
			{
				beacon_period_strategy = FIXED_TRANSM;
			}
			else
			{
				beacon_period_strategy = _bp_s;
			}
		}
		// --------------------------------------------------------------------
		uint8_t get_beacon_period_strategy()
		{
			return beacon_period_strategy;
		}
		// --------------------------------------------------------------------
		void set_protocol_max_payload_size_strategy( uint8_t _mpps_s )
		{
			if ( _mpps_s > PP_STRATEGY_NUM_VALUES )
			{
				protocol_max_payload_size_strategy = FIXED_PAYLOAD_SIZE;
			}
			else
			{
				protocol_max_payload_size_strategy = _mpps_s;
			}
		}
		// --------------------------------------------------------------------
		uint8_t get_protocol_max_payload_size_strategy()
		{
			return protocol_max_payload_size_strategy;
		}
		// --------------------------------------------------------------------
		millis_t get_relax_millis()
		{
			return relax_millis;
		}
		// --------------------------------------------------------------------
		void set_relax_millis( millis_t _lm )
		{
			relax_millis = _lm;
		}
		// --------------------------------------------------------------------
		void init( Radio& _radio, Timer& _timer, Debug& _debug, Clock& _clock )
		{
			radio_ = &_radio;
			timer_ = &_timer;
			debug_ = &_debug;
			clock_ = &_clock;
		}
		// --------------------------------------------------------------------
		Radio& radio()
		{
			return *radio_;
		}
		// --------------------------------------------------------------------
		Clock& clock()
		{
			return *clock_;
		}
		// --------------------------------------------------------------------
		Timer& timer()
		{
			return *timer_;
		}
		// --------------------------------------------------------------------
		Debug& debug()
		{
			return *debug_;
		}
		// --------------------------------------------------------------------
		enum error_codes
		{
			SUCCESS,
			PROT_NUM_IN_USE,
			PROT_LIST_FULL,
			INV_PROT_ID,
			NO_PAYLOAD_SPACE,
			PAYLOAD_SIZE_OUT_OF_BOUNDS,
			ERROR_CODES_NUM_VALUES
		};
		enum neighbor_discovery_status
		{
			ACTIVE_STATUS,
			WAITING_STATUS,
			NB_STATUS_NUM_VALUES
		};
		enum transmission_power_dB_strategies
		{
			FIXED_TRANSM,
			LEAST_TRANSM,
			MEAN_TRANSM,
			WEIGHTED_MEAN_TRANSM,
			TP_DB_STRATEGY_NUM_VALUES
		};
		enum beacon_period_strategies
		{
			FIXED_PERIOD,
			LEAST_PERIOD,
			MEAN_PERIOD,
			WEIGHTED_MEAN_PERIOD,
			BP_STRATEGY_NUM_VALUES
		};
		enum protocol_payload_strategies
		{
			FIXED_PAYLOAD_SIZE,
			DYNAMIC_PAYLOAD_SIZE,
			PP_STRATEGY_NUM_VALUES
		};
		enum message_ids
		{
			NB_MESSAGE = 12
		};
	private:
		uint32_t recv_callback_id_;
        uint8_t status;
        millis_t beacon_period;
        uint8_t channel;
        int8_t transmission_power_dB;
        Protocol_vector protocols;
        uint8_t protocol_max_payload_size;
        uint8_t transmission_power_dB_strategy;
        uint8_t protocol_max_payload_size_strategy;
        uint8_t beacon_period_strategy;
        millis_t relax_millis;
        Radio * radio_;
        Clock * clock_;
        Timer * timer_;
        Debug * debug_;
    };
}

#endif

//			neighbor Test cases:
//
//			Neighbor n1;
//			debug().debug( "neighbor n1:");
//			n1.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n1.get_last_beacon() ), clock().milliseconds( n1.get_last_beacon() ) );
//			Neighbor n2( 1, 2, 3 ,4 ,5 ,6 ,7 ,8 , 9, 10, 11, clock().time() );
//			debug().debug( "----------------------");
//			debug().debug( "Neighbor n2:");
//			n2.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n2.get_last_beacon() ), clock().milliseconds( n2.get_last_beacon() ) );
//			debug().debug( "----------------------");
//			n1.set_id( 11 );
//			n1.set_total_beacons( 22 );
//			n1.set_total_beacons_expected( 33 );
//			n1.set_avg_LQI( 44 );
//			n1.set_avg_LQI_inverse( 55 );
//			n1.set_link_stab_ratio( 66 );
//			n1.set_link_stab_ratio_inverse( 77 );
//			n1.set_consecutive_beacons( 88 );
//			n1.set_consecutive_beacons_lost( 99 );
//			n1.set_beacon_period( 111 );
//			n1.set_beacon_period_update_counter( 222 );
//			n1.set_last_beacon( clock().time() );
//			debug().debug( "Neighbor n1 with getters:");
//			debug().debug( "id: %i", n1.get_id() );
//			debug().debug( "total_beacons : %i", n1.get_total_beacons() );
//			debug().debug( "total_beacons_expected : %i", n1.get_total_beacons_expected() );
//			debug().debug( "avg_LQI : %i", n1.get_avg_LQI() );
//			debug().debug( "avg_LQI_inverse : %i", n1.get_avg_LQI_inverse() );
//			debug().debug( "link_stab_ratio : %i", n1.get_link_stab_ratio() );
//			debug().debug( "link_stab_ratio_inverse : %i", n1.get_link_stab_ratio_inverse() );
//			debug().debug( "consecutive_beacons : %i", n1.get_consecutive_beacons() );
//			debug().debug( "consecutive_beacons_lost : %i", n1.get_consecutive_beacons_lost() );
//			debug().debug( "beacon_period : %d", n1.get_beacon_period() );
//			debug().debug( "beacon_period_update_counter : %d", n1.get_beacon_period_update_counter() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n1.get_last_beacon() ), clock().milliseconds( n1.get_last_beacon() ) );
//			debug().debug( "----------------------");
//			n2 = n1;
//			debug().debug( "Neighbor n2 = n1:");
//			n2.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n2.get_last_beacon() ), clock().milliseconds( n2.get_last_beacon() ) );
//			debug().debug( "----------------------");
//			debug().debug( "n2 serial size: %i ", n2.serial_size() );
//			debug().debug( "----------------------");
//			debug().debug( "neighbor n3:");
//			Neighbor n3;
//			debug().debug( "last_beacon %d:%d", clock().seconds( n3.get_last_beacon() ), clock().milliseconds( n3.get_last_beacon() ) );
//			n3.print( debug() );
//			debug().debug( "----------------------");
//			block_data_t buff[100];
//			n3.de_serialize( n1.serialize( buff, 47 ), 47 );
//			debug().debug( "Neighbor n3 after serialize - de_serialize:");
//			n3.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n3.get_last_beacon() ), clock().milliseconds( n3.get_last_beacon() ) );
//			debug().debug( "----------------------");
//
//			n3.set_total_beacons( 0xfffffffe );
//			debug().debug( "total_beacons : %d", n3.get_total_beacons() );
//			n3.inc_total_beacons();
//			debug().debug( "total_beacons : %d", n3.get_total_beacons() );
//			n3.inc_total_beacons();
//			debug().debug( "total_beacons : %d", n3.get_total_beacons() );
//
//			n3.set_total_beacons_expected( 0xfffffffe );
//			debug().debug( "total_beacons_expected : %d", n3.get_total_beacons_expected() );
//			n3.inc_total_beacons_expected();
//			debug().debug( "total_beacons_expected : %d", n3.get_total_beacons_expected() );
//			n3.inc_total_beacons_expected();
//			debug().debug( "total_beacons_expected : %d", n3.get_total_beacons_expected() );
//
//			n3.set_avg_LQI( 80 );
//			debug().debug( "avg_LQI : %d", n3.get_avg_LQI() );
//			n3.set_total_beacons( 5 );
//			n3.update_avg_LQI( 50, 2 );
//			debug().debug( "avg_LQI : %d", n3.get_avg_LQI() );
//
//			n3.set_consecutive_beacons( 254 );
//			debug().debug( "consecutive_beacons : %d", n3.get_consecutive_beacons() );
//			n3.inc_consecutive_beacons();
//			debug().debug( "consecutive_beacons : %d", n3.get_consecutive_beacons() );
//			n3.inc_consecutive_beacons();
//			debug().debug( "consecutive_beacons : %d", n3.get_consecutive_beacons() );
//
//			n3.set_consecutive_beacons_lost( 254 );
//			debug().debug( "consecutive_beacons_lost : %d", n3.get_consecutive_beacons_lost() );
//			n3.inc_consecutive_beacons_lost();
//			debug().debug( "consecutive_beacons_lost : %d", n3.get_consecutive_beacons_lost() );
//			n3.inc_consecutive_beacons_lost();
//			debug().debug( "consecutive_beacons_lost : %d", n3.get_consecutive_beacons_lost() );
//
//			n3.set_beacon_period_update_counter( 0xfffffffe );
//			debug().debug( "beacon_period_update_counter : %d", n3.get_beacon_period_update_counter() );
//			n3.inc_beacon_period_update_counter();
//			debug().debug( "beacon_period_update_counter beacons_lost : %d", n3.get_beacon_period_update_counter() );
//			n3.inc_beacon_period_update_counter();
//			debug().debug( "beacon_period_update_counter beacons_lost : %d", n3.get_beacon_period_update_counter() );
//
//			ProtocolPayload Test cases:
//
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//			pp1.print( debug() );
//			debug().debug( "max_payload_size : %i", pp1.get_max_payload_size() );
//			debug().debug( "-------------------------------------");
//			ProtocolPayload pp2;
//			pp2.set_protocol_id( 2 );
//			pp2.set_payload_size( 10 );
//			pp2.set_payload_data( buff, 15 );
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			ProtocolPayload pp3 = pp2;
//			pp2 = pp1;
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			pp2.set_payload_size( pp2.get_max_payload_size() + 1 );
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			pp2.set_payload_size( 2 );
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			pp3.print( debug() );
//			debug().debug( "-------------------------------------");
//			block_data_t buff1[100];
//			pp3.de_serialize( pp2.serialize( buff1, 33 ), 33 );
//			pp3.print( debug() );
//			debug().debug( "-------------------------------------");
//			debug().debug( "pp1 serial_size %i", pp1.serial_size() );
//			debug().debug( "pp2 serial_size %i", pp2.serial_size() );
//			debug().debug( "pp3 serial_size %i", pp3.serial_size() );
//			debug().debug( "-------------------------------------");

//	ProtocolSettings Test cases:
//
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//
//
//			ProtocolSettings ps1;
//			debug().debug( " ProtocolSettings ps1 :");
//			ps1.print( debug() );
//			debug().debug( "------------------------------");
//
//
//			ProtocolSettings ps2( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, pp1 );
//			debug().debug( " Protocol_settings ps2 :");
//			ps2.print( debug() );
//			debug().debug( "------------------------------");
//			debug().debug( " ProtocolSettings ps1 = ps2 :");
//			ps1 = ps2;
//			ps1.print( debug() );
//			debug().debug( "------------------------------");
//			ps1.set_max_avg_LQI_threshold( 11 );
//			ps1.set_min_avg_LQI_threshold( 22 );
//			ps1.set_max_avg_LQI_inverse_threshold( 33 );
//			ps1.set_min_avg_LQI_inverse_threshold( 44 );
//			ps1.set_max_link_stab_ratio_threshold( 55 );
//			ps1.set_min_link_stab_ratio_threshold( 66 );
//			ps1.set_max_link_stab_ratio_inverse_threshold( 77 );
//			ps1.set_min_link_stab_ratio_inverse_threshold( 88 );
//			ps1.set_consecutive_beacons_threshold( 99 );
//			ps1.set_consecutive_beacons_lost_threshold( 111 );
//			ps1.set_events_flag( 222 );
//			ps1.set_proposed_transmission_power_dB( -6 );
//			ps1.set_proposed_transmission_power_dB_weight( 100 );
//			ps1.set_proposed_beacon_period( 2000 );
//			ps1.set_proposed_beacon_period_weight( 80 );
//			ps1.set_overflow_strategy( 255 );
//			ps1.set_ratio_divider( 0 );
//			ps1.set_protocol_payload( pp1 );
//
//			debug().debug( " protocol_settings ps1 setters:");
//			debug().debug( "max_avg_LQI_threshold : %i", ps1.get_max_avg_LQI_threshold() );
//			debug().debug( "min_avg_LQI_threshold : %i", ps1.get_min_avg_LQI_threshold() );
//			debug().debug( "max_avg_LQI_inverse_threshold : %i ", ps1.get_max_avg_LQI_inverse_threshold() );
//			debug().debug( "min_avg_LQI_inverse_threshold : %i ", ps1.get_min_avg_LQI_inverse_threshold() );
//			debug().debug( "max_link_stab_ratio_threshold : %i ", ps1.get_max_link_stab_ratio_threshold() );
//			debug().debug( "min_link_stab_ratio_threshold : %i ", ps1.get_min_link_stab_ratio_threshold() );
//			debug().debug( "max_link_stab_ratio_inverse_threshold : %i ", ps1.get_max_link_stab_ratio_inverse_threshold() );
//			debug().debug( "min_link_stab_ratio_inverse_threshold : %i ", ps1.get_min_link_stab_ratio_inverse_threshold() );
//			debug().debug( "consecutive_beacons_threshold : %i ", ps1.get_consecutive_beacons_threshold() );
//			debug().debug( "consecutive_beacons_lost_threshold : %i ", ps1.get_consecutive_beacons_lost_threshold() );
//			debug().debug( "events_flag : %i ", ps1.get_events_flag() );
//			debug().debug( "proposed_transmission_power_dB : %i", ps1.get_proposed_transmission_power_dB() );
//			debug().debug( "proposed_transmission_power_dB_weight : %i", ps1.get_proposed_transmission_power_dB_weight() );
//			debug().debug( "proposed_beacon_period : %i", ps1.get_proposed_beacon_period() );
//			debug().debug( "proposed_beacon_period_weight : %d", ps1.get_proposed_beacon_period_weight() );
//			debug().debug( "overflow_strategy : %i", ps1.get_overflow_strategy() );
//			debug().debug( "ratio_divider : %d", ps1.get_ratio_divider() );
//			ps1.get_protocol_payload().print( debug() );
//			debug().debug( "------------------------------");
//
// protocol Test Cases:
//
//			Protocol p;
//			p.print( debug() );
//			uint8_t event = 50;
//			node_id_t from = 0;
//			size_t len = 0;
//			block_data_t* data = NULL;
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			Neighbor n0( 10, 20, 30 ,40 ,50 ,60 ,70 ,80 , 90, 100, 110, clock().time() );
//			Neighbor n1( 11, 21, 31 ,41 ,51 ,61 ,71 ,81 , 91, 101, 111, clock().time() );
//			Neighbor n2( 12, 22, 32 ,42 ,52 ,62 ,72 ,82 , 92, 102, 112, clock().time() );
//			Neighbor n3( 13, 23, 33 ,43 ,53 ,63 ,73 ,83 , 93, 103, 113, clock().time() );
//			Neighbor n4( 14, 24, 34 ,44 ,54 ,64 ,74 ,84 , 94, 104, 114, clock().time() );
//			Neighbor n5( 15, 25, 35 ,45 ,55 ,65 ,75 ,85 , 95, 105, 115, clock().time() );
//			Neighbor_vector neighs;
//			neighs.push_back( n0 );
//			neighs.push_back( n1 );
//			neighs.push_back( n2 );
//			neighs.push_back( n3 );
//			neighs.push_back( n4 );
//			neighs.push_back( n5 );
//
//
//
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//			ProtocolSettings ps1( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, pp1 );
//
//			p.set_protocol_id ( 1 );
//			p.template set_event_notifier_callback< self_type, &self_type::sync_neighbors>( this );
//			p.set_protocol_settings( ps1 );
//			p.set_neighborhood( neighs );
//			p.print( debug() );
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			p.reset_event_notifier_callback();
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			p.template set_event_notifier_callback< self_type, &self_type::sync_neighbors>( this );
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			p.set_event_notifier_callback( p.get_event_notifier_callback() );
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "*****************%%%%%%*********************");
//			debug().debug( "protocol id : %i ", p.get_protocol_id() );
//			p.get_protocol_settings().print( debug() );
//			for ( Neighbor_vector_iterator it = p.get_neighborhood_ref()->begin(); it != p.get_neighborhood_ref()->end(); ++it )
//			{
//				it->print( debug() );
//			}
//			debug().debug( "******************************************");
//			p.get_neighbor_ref( 10 )->print( debug() );
//			debug().debug( "******************************************");
//			p.get_protocol_settings_ref()->set_proposed_transmission_power_dB( -9 );
//			p.get_protocol_settings().print( debug() );
//			debug().debug( "*****************%%%%%%*********************");
//			Protocol p2 = p;
//			p2.print( debug() );
//			p2.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "*****************%%%%%%*********************");
//
//			Neighbor n100 = *(p2.get_neighbor_ref( 10 ));
//			p2.get_neighbor_ref( 10 )->set_total_beacons( 0xffffffff );
//			p2.get_neighbor_ref( 10 )->print( debug() );
//			p2.get_protocol_settings_ref()->set_overflow_strategy( 	ProtocolSettings::RESET_TOTAL_BEACONS|
//																	ProtocolSettings::RESET_TOTAL_BEACONS_EXPECTED|
//																	ProtocolSettings::RESET_STAB|
//																	ProtocolSettings::RESET_STAB_INVERSE|
//																	ProtocolSettings::RESET_AVG_LQI|
//																	ProtocolSettings::RESET_AVG_LQI_INVERSE );
//			debug().debug(" the problem : %d", p2.get_protocol_settings_ref()->get_overflow_strategy() );
//			p2.resolve_overflow_strategy( 10 );
//			p2.get_neighbor_ref( 10 )->print( debug() );
//
//			Beacon Test cases
//			Beacon b1;
//			b1.print( debug() );
//			Neighbor n0( 10, 20, 30 ,40 ,50 ,60 ,70 ,80 , 90, 100, 110, clock().time() );
//			Neighbor n1( 11, 21, 31 ,41 ,51 ,61 ,71 ,81 , 91, 101, 111, clock().time() );
//			Neighbor n2( 12, 22, 32 ,42 ,52 ,62 ,72 ,82 , 92, 102, 112, clock().time() );
//			Neighbor n3( 13, 23, 33 ,43 ,53 ,63 ,73 ,83 , 93, 103, 113, clock().time() );
//			Neighbor n4( 14, 24, 34 ,44 ,54 ,64 ,74 ,84 , 94, 104, 114, clock().time() );
//			Neighbor n5( 15, 25, 35 ,45 ,55 ,65 ,75 ,85 , 95, 105, 115, clock().time() );
//			Neighbor_vector neighs;
//			neighs.push_back( n0 );
//			neighs.push_back( n1 );
//			neighs.push_back( n2 );
//			neighs.push_back( n3 );
//			neighs.push_back( n4 );
//			neighs.push_back( n5 );
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//			ProtocolPayload pp2( 2, 5, buff, 19 );
//			ProtocolPayload pp3( 3,	5, buff, 29 );
//			ProtocolPayload pp4( 4, 10, buff, 39 );
//			ProtocolPayload pp5( 5, 10, buff, 49 );
//			debug().debug( "********");
//			debug().debug( "pp1 serial_size : %i", pp1.serial_size() );
//			debug().debug( "pp2 serial_size : %i", pp2.serial_size() );
//			debug().debug( "pp3 serial_size : %i", pp3.serial_size() );
//			debug().debug( "pp4 serial_size : %i", pp4.serial_size() );
//			debug().debug( "pp5 serial_size : %i", pp5.serial_size() );
//			debug().debug( "********");
//			debug().debug( "n0 serial_size : %i", n0.serial_size() );
//			debug().debug( "n1 serial_size : %i", n1.serial_size() );
//			debug().debug( "n2 serial_size : %i", n2.serial_size() );
//			debug().debug( "n3 serial_size : %i", n3.serial_size() );
//			debug().debug( "n4 serial_size : %i", n4.serial_size() );
//			debug().debug( "n5 serial_size : %i", n5.serial_size() );
//			debug().debug( "********");
//			ProtocolPayload_vector payloads;
//			payloads.push_back( pp1 );
//			payloads.push_back( pp2 );
//			payloads.push_back( pp3 );
//			payloads.push_back( pp4 );
//			payloads.push_back( pp5 );
//			Beacon b2;
//			b2.set_neighborhood( neighs );
//			b2.set_protocol_payloads( payloads );
//			b2.set_beacon_period( 10000 );
//			b2.set_beacon_period_update_counter( 20000 );
//			b1 = b2;
//			b1.print( debug() );
//
//			debug().debug("***************************************");
//			for ( Neighbor_vector_iterator it = b1.get_neighborhood_ref()->begin(); it != b1.get_neighborhood_ref()->end(); ++it )
//			{
//				it->print( debug() );
//			}
//			debug().debug("***************************************");
//			for ( ProtocolPayload_vector_iterator it = b1.get_protocol_payloads_ref()->begin(); it != b1.get_protocol_payloads_ref()->end(); ++it )
//			{
//				it->print( debug() );
//			}
//			debug().debug("***************************************");
//			b2.print( debug() );
//			debug().debug("***************************************");
//			block_data_t bufff[100];
//			Beacon b3;
//			b3.de_serialize( b2.serialize( bufff, 4 ), 4 );
//			b3.print( debug() );
//			debug().debug("serial_size : %i", b3.serial_size() );
//			debug().debug("serial_size : %i", b2.serial_size() );
