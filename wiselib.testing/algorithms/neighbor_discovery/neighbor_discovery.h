#ifndef NEIGHBOR_DISCOVERY_H
#define	NEIGHBOR_DISCOVERY_H

#include "util/delegates/delegate.hpp"
#include "util/pstl/vector_static.h"
#include "pgb_payloads_ids.h"
#include "neighbor_discovery_config.h"
#include "neighbor_discovery_message.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Timer_P,
				typename Debug_P>
	class NeighborDiscovery
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef typename Os_P::Clock Clock;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Clock::time_t time_t;
		typedef typename Radio::ExtendedData ExData;
		typedef typename Radio::TxPower TxPower;
		typedef typename Timer::millis_t millis_t;
		typedef delegate4<void, uint8_t, node_id_t, uint8_t, uint8_t*> event_notifier_delegate_t;
		typedef NeighborDiscovery<Os, Radio, Timer, Debug> self_t;
		typedef NeighborDiscovery_MessageType<Os, Radio> Message;

		class neighbor //fully tested
		{
		public:
			neighbor()	:
				id							( 0 ),
				total_beacons				( 0 ),
				total_beacons_expected		( 0 ),
				avg_LQI						( 0 ),
				avg_LQI_inverse				( 0 ),
				link_stab_ratio				( 0 ),
				link_stab_ratio_inverse		( 0 ),
				consecutive_beacons			( 0 ),
				consecutive_beacons_lost	( 0 )
			{}
			// --------------------------------------------------------------------
			neighbor(	node_id_t _id,
					uint32_t _tbeac,
					uint32_t _tbeac_exp,
					uint8_t _alqi,
					uint8_t _alqi_in,
					uint8_t _lsratio,
					uint8_t _lsratio_in,
					uint8_t _cb,
					uint8_t _cb_lost )
			{
				id = _id;
				total_beacons = _tbeac;
				total_beacons_expected = _tbeac_exp;
				avg_LQI = _alqi;
				avg_LQI_inverse = _alqi_in;
				link_stab_ratio = _lsratio;
				link_stab_ratio_inverse = _lsratio_in;
				consecutive_beacons = _cb;
				consecutive_beacons_lost =  _cb_lost;
			}
			// --------------------------------------------------------------------
			~neighbor()
			{}
			// --------------------------------------------------------------------
			node_id_t get_id()
			{
				return id;
			}
			// --------------------------------------------------------------------
			void set_id( node_id_t _id )
			{
				id = _id;
			}
			// --------------------------------------------------------------------
			uint32_t get_total_beacons()
			{
				return total_beacons;
			}
			// --------------------------------------------------------------------
			void set_total_beacons( uint32_t _tbeac )
			{
				total_beacons = _tbeac;
			}
			// --------------------------------------------------------------------
			uint32_t get_total_beacons_expected()
			{
				return total_beacons_expected;
			}
			// --------------------------------------------------------------------
			void set_total_beacons_expected( uint32_t _tbeac_exp )
			{
				total_beacons_expected = _tbeac_exp;
			}
			// --------------------------------------------------------------------
			uint8_t get_avg_LQI()
			{
				return avg_LQI;
			}
			// --------------------------------------------------------------------
			void set_avg_LQI( uint8_t _alqi )
			{
				avg_LQI = _alqi;
			}
			// --------------------------------------------------------------------
			uint8_t get_avg_LQI_inverse()
			{
				return avg_LQI_inverse;
			}
			// --------------------------------------------------------------------
			void set_avg_LQI_inverse( uint8_t _alqi_in )
			{
				avg_LQI_inverse = _alqi_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_link_stab_ratio()
			{
				return link_stab_ratio;
			}
			// --------------------------------------------------------------------
			void set_link_stab_ratio( uint8_t _lsratio )
			{
				link_stab_ratio = _lsratio;
			}
			// --------------------------------------------------------------------
			uint8_t get_link_stab_ratio_inverse()
			{
				return link_stab_ratio_inverse;
			}
			// --------------------------------------------------------------------
			void set_link_stab_ratio_inverse( uint8_t _lsratio_in )
			{
				link_stab_ratio_inverse = _lsratio_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons()
			{
				return consecutive_beacons;
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons( uint8_t _cb )
			{
				consecutive_beacons = _cb;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons_lost()
			{
				return consecutive_beacons_lost;
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons_lost( uint8_t _cb_lost )
			{
				consecutive_beacons_lost = _cb_lost;
			}
			// --------------------------------------------------------------------
			neighbor& operator=( const neighbor& _n )
			{
				id = _n.id;
				total_beacons = _n.total_beacons;
				total_beacons_expected = _n.total_beacons_expected;
				avg_LQI = _n.avg_LQI;
				avg_LQI_inverse = _n.avg_LQI_inverse;
				link_stab_ratio = _n.link_stab_ratio;
				link_stab_ratio_inverse = _n.link_stab_ratio_inverse;
				consecutive_beacons = _n.consecutive_beacons;
				consecutive_beacons_lost = _n.consecutive_beacons_lost;
				return *this;
			}
			// --------------------------------------------------------------------
			block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				uint8_t AVG_LQI_POS = 0;
				uint8_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATIO_POS + sizeof(uint8_t);
				write<Os, block_data_t, uint8_t>( _buff + AVG_LQI_POS + _offset, avg_LQI );
				write<Os, block_data_t, uint8_t>( _buff + AVG_LQI_IN_POS + _offset, avg_LQI_inverse );
				write<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_POS + _offset, link_stab_ratio );
				write<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_IN_POS + _offset, link_stab_ratio_inverse );
				return _buff;
			}
			// --------------------------------------------------------------------
			void de_serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				uint8_t AVG_LQI_POS = 0;
				uint8_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATIO_POS + sizeof(uint8_t);
				avg_LQI = read<Os, block_data_t, uint8_t>( _buff + AVG_LQI_POS + _offset );
				avg_LQI_inverse = read<Os, block_data_t, uint8_t>( _buff + AVG_LQI_IN_POS + _offset );
				link_stab_ratio = read<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_POS + _offset );
				link_stab_ratio_inverse = read<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_IN_POS + _offset );
			}
			// --------------------------------------------------------------------
			size_t serial_size()
			{
				uint8_t AVG_LQI_POS = 0;
				uint8_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATIO_POS + sizeof(uint8_t);
				return LINK_STAB_RATIO_IN_POS + sizeof(uint8_t);
			}
			// --------------------------------------------------------------------
			void print( Debug& debug )
			{
				debug.debug( "id : %i", id );
				debug.debug( "total_beacons : %i", total_beacons );
				debug.debug( "total_beacons_expected : %i", total_beacons_expected );
				debug.debug( "avg_LQI : %i", avg_LQI );
				debug.debug( "avg_LQI_inverse : %i", avg_LQI_inverse );
				debug.debug( "link_stab_ratio : %i", link_stab_ratio );
				debug.debug( "link_stab_ratio_inverse : %i", link_stab_ratio_inverse );
				debug.debug( "consecutive_beacons : %i", consecutive_beacons );
				debug.debug( "consecutive_beacons_lost : %i", consecutive_beacons_lost );
			}
			// --------------------------------------------------------------------
		private:
			node_id_t id;
			uint32_t total_beacons;
			uint32_t total_beacons_expected;
			uint8_t avg_LQI;
			uint8_t avg_LQI_inverse;
			uint8_t link_stab_ratio;
			uint8_t link_stab_ratio_inverse;
			uint8_t consecutive_beacons;
			uint8_t consecutive_beacons_lost;
		};
		typedef vector_static<Os, neighbor, MAX_NEIGHBORS> neighbor_vector;
		typedef typename neighbor_vector::iterator neighbor_vector_iterator;

		class protocol_settings //fully tested
		{
		public:
			protocol_settings()	:
				max_avg_LQI_threshold 					( 255 ),
				min_avg_LQI_threshold 					( 0 ),
				max_avg_LQI_inverse_threshold 			( 255 ),
				min_avg_LQI_inverse_threshold 			( 0 ),
				max_link_stab_ratio_threshold 			( 100 ),
				min_link_stab_ratio_threshold 			( 0 ),
				max_link_stab_ratio_inverse_threshold	( 100 ),
				min_link_stab_ratio_inverse_threshold	( 0 ),
				consecutive_beacons_threshold 			( 5 ),
				consecutive_beacons_lost_threshold		( 5 ),
				events_flag								( NEW_NB | NEW_NB_BIDI | NEW_PAYLOAD | NEW_PAYLOAD_BIDI | LOST_NB | LOST_NB_BIDI ),
				payload_size							( 0 ),
				payload_offset							( 0 )
			{}
			// --------------------------------------------------------------------
			protocol_settings(	uint8_t _maxLQI,
								uint8_t _minLQI,
								uint8_t _maxLQI_in,
								uint8_t _minLQI_in,
								uint8_t _maxlsr,
								uint8_t _minlsr,
								uint8_t _maxlsr_in,
								uint8_t _minlsr_in,
								uint8_t _cb,
								uint8_t _cblost,
								uint8_t _ef,
								uint8_t _psize,
								block_data_t* _pdata,
								uint8_t _offset = 0	)
			{
				max_avg_LQI_threshold = _maxLQI;
				min_avg_LQI_threshold = _minLQI;
				max_avg_LQI_inverse_threshold = _maxLQI_in;
				min_avg_LQI_inverse_threshold = _minLQI_in;
				max_link_stab_ratio_threshold = _maxlsr;
				min_link_stab_ratio_threshold = _minlsr;
				max_link_stab_ratio_inverse_threshold = _maxlsr_in;
				min_link_stab_ratio_inverse_threshold = _minlsr_in,
				consecutive_beacons_threshold = _cb;
				consecutive_beacons_lost_threshold = _cblost;
				events_flag = _ef;
				payload_size = _psize;
				payload_offset = _offset;
				for ( uint8_t i = 0 + _offset; i < payload_size + _offset; i++ )
				{
					payload_data[i] = _pdata[i];
				}
			}
			// --------------------------------------------------------------------
			~protocol_settings()
			{}
			// --------------------------------------------------------------------
			uint8_t get_max_avg_LQI_threshold()
			{
				return max_avg_LQI_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_avg_LQI_threshold( uint8_t _maxLQI )
			{
				max_avg_LQI_threshold = _maxLQI;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_avg_LQI_threshold()
			{
				return min_avg_LQI_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_avg_LQI_threshold( uint8_t _minLQI )
			{
				min_avg_LQI_threshold = _minLQI;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_avg_LQI_inverse_threshold()
			{
				return max_avg_LQI_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_avg_LQI_inverse_threshold( uint8_t _maxLQI_in )
			{
				max_avg_LQI_inverse_threshold = _maxLQI_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_avg_LQI_inverse_threshold()
			{
				return min_avg_LQI_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_avg_LQI_inverse_threshold( uint8_t _minLQI_in )
			{
				min_avg_LQI_inverse_threshold = _minLQI_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_link_stab_ratio_threshold()
			{
				return max_link_stab_ratio_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_link_stab_ratio_threshold( uint8_t _maxlsr )
			{
				max_link_stab_ratio_threshold = _maxlsr;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_link_stab_ratio_threshold()
			{
				return min_link_stab_ratio_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_link_stab_ratio_threshold( uint8_t _minlsr )
			{
				min_link_stab_ratio_threshold = _minlsr;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_link_stab_ratio_inverse_threshold()
			{
				return max_link_stab_ratio_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_link_stab_ratio_inverse_threshold( uint8_t _maxlsr_in )
			{
				max_link_stab_ratio_inverse_threshold = _maxlsr_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_link_stab_ratio_inverse_threshold()
			{
				return min_link_stab_ratio_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_link_stab_ratio_inverse_threshold( uint8_t _minlsr_in )
			{
				min_link_stab_ratio_inverse_threshold = _minlsr_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons_threshold()
			{
				return consecutive_beacons_threshold;
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons_threshold( uint8_t _cb )
			{
				consecutive_beacons_threshold = _cb;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons_lost_threshold()
			{
				return consecutive_beacons_lost_threshold;
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons_lost_threshold( uint8_t _cblost )
			{
				consecutive_beacons_lost_threshold = _cblost;
			}
			// --------------------------------------------------------------------
			uint8_t get_events_flag()
			{
				return events_flag;
			}
			// --------------------------------------------------------------------
			void set_events_flag( uint8_t _ef )
			{
				events_flag = _ef;
			}
			// --------------------------------------------------------------------
			uint8_t get_payload_size()
			{
				return payload_size;
			}
			// --------------------------------------------------------------------
			void set_payload_size( size_t _psize )
			{
				payload_size = _psize;
			}
			// --------------------------------------------------------------------
			block_data_t* get_payload_data()
			{
				return payload_data;
			}
			// --------------------------------------------------------------------
			void set_payload_data( block_data_t* _pdata, size_t _offset = 0 )
			{
				payload_offset = _offset;
				for ( uint8_t i = 0 + _offset; i < payload_size + _offset; i++ )
				{
					payload_data[i] = _pdata[i];
				}
			}
			// --------------------------------------------------------------------
			void set_payload( block_data_t* _pdata, size_t _psize, size_t _offset = 0 )
			{
				payload_size = _psize;
				payload_offset = _offset;
				for ( uint8_t i = 0 + _offset; i < payload_size + _offset; i++ )
				{
					payload_data[i] = _pdata[i];
				}
			}
			// --------------------------------------------------------------------
			protocol_settings& operator=( const protocol_settings& _psett )
			{
				max_avg_LQI_threshold = _psett.max_avg_LQI_threshold;
				min_avg_LQI_threshold = _psett.min_avg_LQI_threshold;
				max_avg_LQI_inverse_threshold = _psett.max_avg_LQI_inverse_threshold;
				min_avg_LQI_inverse_threshold = _psett.min_avg_LQI_inverse_threshold;
				max_link_stab_ratio_threshold = _psett.max_link_stab_ratio_threshold;
				min_link_stab_ratio_threshold = _psett.min_link_stab_ratio_threshold;
				max_link_stab_ratio_inverse_threshold = _psett.max_link_stab_ratio_inverse_threshold;
				min_link_stab_ratio_inverse_threshold = _psett.min_link_stab_ratio_inverse_threshold;
				consecutive_beacons_threshold = _psett.consecutive_beacons_threshold;
				consecutive_beacons_lost_threshold = _psett.consecutive_beacons_lost_threshold;
				events_flag = _psett.events_flag;
				payload_size = _psett.payload_size;
				payload_offset = _psett.payload_offset;
				for ( uint8_t i = 0 + payload_offset; i < payload_size + payload_offset; i++ )
				{
					payload_data[i] = _psett.payload_data[i];
				}
				return *this;
			}
			// --------------------------------------------------------------------
			void print( Debug& debug )
			{
				debug.debug( "protocol_settings:");
				debug.debug( "max_avg_LQI_threshold : %i", max_avg_LQI_threshold );
				debug.debug( "min_avg_LQI_threshold : %i", min_avg_LQI_threshold );
				debug.debug( "max_avg_LQI_inverse_threshold : %i", max_avg_LQI_inverse_threshold );
				debug.debug( "min_avg_LQI_inverse_threshold : %i", min_avg_LQI_inverse_threshold );
				debug.debug( "max_link_stab_ratio_threshold : %i", max_link_stab_ratio_threshold );
				debug.debug( "min_link_stab_ratio_threshold : %i", min_link_stab_ratio_threshold );
				debug.debug( "max_link_stab_ratio_inverse_threshold : %i", max_link_stab_ratio_inverse_threshold );
				debug.debug( "min_link_stab_ratio_inverse_threshold : %i", min_link_stab_ratio_inverse_threshold );
				debug.debug( "consecutive_beacons_threshold : %i", consecutive_beacons_threshold );
				debug.debug( "consecutive_beacons_lost_threshold : %i", consecutive_beacons_lost_threshold );
				debug.debug( "events_flag : %i ", events_flag );
				debug.debug( "payload_size : %i ", payload_size );
				for ( uint8_t i = 0 + payload_offset; i < payload_size + payload_offset; i++ )
				{
					debug.debug( "payload %i 'th byte : %i", i, payload_data[i] );
				}
			}
		private:
			enum event_codes
			{
				NEW_NB = 1,
				NEW_NB_BIDI = 2,
				NEW_PAYLOAD = 4,
				NEW_PAYLOAD_BIDI = 8,
				LOST_NB = 16,
				LOST_NB_BIDI = 32
			};
			uint8_t max_avg_LQI_threshold;
			uint8_t min_avg_LQI_threshold;
			uint8_t max_avg_LQI_inverse_threshold;
			uint8_t min_avg_LQI_inverse_threshold;
			uint8_t max_link_stab_ratio_threshold;
			uint8_t min_link_stab_ratio_threshold;
			uint8_t max_link_stab_ratio_inverse_threshold;
			uint8_t min_link_stab_ratio_inverse_threshold;
			uint8_t consecutive_beacons_threshold;
			uint8_t consecutive_beacons_lost_threshold;
			uint8_t events_flag;
			uint8_t payload_size;
			block_data_t payload_data[MAX_PROTOCOL_PAYLOAD];
			uint8_t payload_offset;
		};

		class protocol //fully tested
		{
		public:
			protocol() :
				prot_id					( 0 ),
				event_notifier_callback	( event_notifier_delegate_t::template from_method<protocol, &protocol::null_callback > ( this ) )
			{}
			// --------------------------------------------------------------------
			~protocol()
			{}
			// --------------------------------------------------------------------
			uint8_t get_protocol_id()
			{
				return prot_id;
			}
			// --------------------------------------------------------------------
			void set_protocol_id( uint8_t _pid )
			{
				prot_id = _pid;
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
			protocol_settings get_protocol_settings()
			{
				return settings;
			}
			// --------------------------------------------------------------------
			void set_protocol_settings( protocol_settings _ps )
			{
				settings = _ps;
			}
			// --------------------------------------------------------------------
			inline neighbor_vector* get_neighborhood()
			{
				return &neighborhood;
			}
			// --------------------------------------------------------------------
			void set_neighborhood( neighbor_vector _nv )
			{
				neighborhood = _nv;
			}
			// --------------------------------------------------------------------
			protocol& operator=( const protocol& _p )
			{
				prot_id = _p.prot_id;
				event_notifier_callback = _p.event_notifier_callback;
				settings = _p.settings;
				neighborhood = _p.neighborhood;
				return *this;
			}
			// --------------------------------------------------------------------
			block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				uint8_t NEIGH_SIZE_POS = 0;
				uint8_t NEIGH_VECTOR_POS = NEIGH_SIZE_POS + sizeof( size_t );
				size_t neigh_size = neighborhood.size();
				write<Os, block_data_t, size_t>( _buff + NEIGH_SIZE_POS + _offset, neigh_size );
				for ( size_t i = 0; i < neigh_size; i++ )
				{
					neighborhood.at( i ).serialize( _buff, NEIGH_VECTOR_POS + i * neighborhood.at( i ).serial_size() + _offset );
				}
				return _buff;
			}
			// --------------------------------------------------------------------
			void de_serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				uint8_t NEIGH_SIZE_POS = 0;
				uint8_t NEIGH_VECTOR_POS = NEIGH_SIZE_POS + sizeof( size_t );
				size_t neigh_size = read<Os, block_data_t, size_t>( _buff + NEIGH_SIZE_POS + _offset );
				neighborhood.clear();
				neighbor n;
				uint8_t NEIGH_ELEMS_POS = NEIGH_VECTOR_POS;
				for ( size_t i = 0; i < neigh_size; i++ )
				{
					n.de_serialize( _buff + NEIGH_ELEMS_POS + _offset );
					NEIGH_ELEMS_POS = NEIGH_ELEMS_POS + n.serial_size();
					neighborhood.push_back( n );
				}
			}
			// --------------------------------------------------------------------
			size_t serial_size()
			{
				uint8_t NEIGH_SIZE_POS = 0;
				uint8_t NEIGH_VECTOR_POS = NEIGH_SIZE_POS + sizeof( size_t );
				uint8_t NEIGH_ELEMS_POS = NEIGH_VECTOR_POS;
				for ( neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
				{
					NEIGH_ELEMS_POS = it->serial_size() + NEIGH_ELEMS_POS;
				}
				return NEIGH_ELEMS_POS;
			}
			// --------------------------------------------------------------------
			void print( Debug& debug )
			{
				debug.debug( "prot_id : %i", prot_id );
				debug.debug( "settings :");
				settings.print( debug );
				debug.debug( "neighborhood :");
				for ( neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
				{
					debug.debug("------------------");
					it->print( debug );
				}
				debug.debug("------------------");
			}
			// --------------------------------------------------------------------
			void null_callback( uint8_t null_event, node_id_t null_node_id, uint8_t null_len, uint8_t* null_data )
			{}
			// --------------------------------------------------------------------
		private:
			uint8_t prot_id;
			event_notifier_delegate_t event_notifier_callback;
			protocol_settings settings;
			neighbor_vector neighborhood;
		};
		typedef vector_static<Os, protocol, MAX_REGISTERED_PROTOCOLS> protocol_vector;
		typedef typename protocol_vector::iterator protocol_vector_iterator;

		// --------------------------------------------------------------------
		NeighborDiscovery()	:
			status	(WAITING)
		{};
		// --------------------------------------------------------------------
		~NeighborDiscovery()
		{};
		// --------------------------------------------------------------------
		void enable()
		{
			radio().enable_radio();
			recv_callback_id_ = radio().template reg_recv_callback<self_t, &self_t::receive>( this );
			set_status( ACTIVE );
			beacon( 0 );
		};
		// --------------------------------------------------------------------
		void disable()
		{
			set_status( WAITING );
			protocols.clear();
			radio().template unreg_recv_callback( recv_callback_id_ );
		};
		// --------------------------------------------------------------------
		void send( node_id_t _dest, size_t _len, block_data_t* _data, message_id_t _msg_id )
		{
			Message message;
			message.set_msg_id( _msg_id );
			message.set_payload( _len, _data );
			radio().send( _dest, message.buffer_size(), (uint8_t*) &message );
		}
		// --------------------------------------------------------------------
		void beacon( void* _data )
		{
			if ( get_status() == ACTIVE )
			{
				timer().template set_timer<self_t, &self_t::beacon> ( beacon_period, this, 0 );
			}
		}
		// --------------------------------------------------------------------
		void receive( node_id_t _from, size_t _len, block_data_t * _msg, ExData const &_ex )
		{
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)(uint8_t, node_id_t, uint8_t, uint8_t*) >
		uint8_t register_protocol( uint8_t _prot_id, protocol_settings _psett, T *_obj_pnt )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			if ( _psett.get_payload_size() > protocol_max_payload_size )
			{
				return PAYLOAD_SIZE_OUT_OF_BOUNDS;
			}
			uint8_t total_payload_size = 0;
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_protocol_settings().get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(message_id_t) + sizeof(size_t) + _psett.get_payload_size() > Radio::MAX_MSG_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _prot_id )
				{
					return PROT_NUM_IN_USE;
				}
			}
			protocol p;
			p.set_protocol_id( _prot_id );
			p.set_protocol_settings( _psett );
			p.set_event_notifier_callback( event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt ) );
			protocols.push_back( p );
			return SUCCESS;
		}
		// --------------------------------------------------------------------
		uint8_t register_protocol( protocol p )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			if ( p.get_protocol_settings().get_payload_size() > protocol_max_payload_size )
			{
				return PAYLOAD_SIZE_OUT_OF_BOUNDS;
			}
			uint8_t total_payload_size = 0;
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_protocol_settings().get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(message_id_t) + sizeof(size_t) + p.get_protocol_settings().get_payload_size() > Radio::MAX_MESSAGE_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
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
		uint8_t unregister_protocol( uint8_t _prot_id )
		{
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _prot_id )
				{
					protocols.erase( it );
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
		protocol get_protocol( uint8_t _prot_id )
		{
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _prot_id )
				{
					return *it;
				}
			}
		}
		// --------------------------------------------------------------------
		int get_status()
		{
			return status;
		};
		// --------------------------------------------------------------------
		void set_status( int _st )
		{
			status = _st;
		};
		// --------------------------------------------------------------------
		millis_t get_beacon_period()
		{
			return beacon_period;
		};
		// --------------------------------------------------------------------
		void set_beacon_period( millis_t _bp )
		{
			beacon_period =_bp;
		};
		// --------------------------------------------------------------------
		uint8_t get_transmission_power_dB()
		{
			return transmission_power_dB;
		};
		// --------------------------------------------------------------------
		void set_transmission_power_dB( uint8_t _tpdB )
		{
			transmission_power_dB =_tpdB;
		};
		// --------------------------------------------------------------------
		uint8_t get_channel()
		{
			return channel;
		};
		// --------------------------------------------------------------------
		void set_channel( uint8_t _ch )
		{
			channel =_ch;
		};
		// --------------------------------------------------------------------
		uint8_t get_protocol_max_payload_size()
		{
			return protocol_max_payload_size;
		}
		// --------------------------------------------------------------------
		void set_protocol_max_payload_size( uint8_t _pmps )
		{
			protocol_max_payload_size = _pmps;
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
			SUCCESS = 0,
			PROT_NUM_IN_USE = 1,
			PROT_LIST_FULL = 2,
			INV_PROT_ID = 3,
			NO_PAYLOAD_SPACE = 4,
			PAYLOAD_SIZE_OUT_OF_BOUNDS = 5
		};
		enum neighbor_discovery_status
		{
			ACTIVE = 1,
			WAITING = 0
		};
		uint32_t recv_callback_id_;
        uint8_t status;
        millis_t beacon_period;
        uint8_t channel;
        uint8_t transmission_power_dB;
        protocol_vector protocols;
        uint8_t protocol_max_payload_size;
        Radio * radio_;
        Clock * clock_;
        Timer * timer_;
        Debug * debug_;
    };
}

#endif
