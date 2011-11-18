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

		class neighbor
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
						uint8_t _lsration_in,
						uint8_t _cb,
						uint8_t _cb_lost )
			{
				id = _id;
				total_beacons = _tbeac;
				total_beacons_expected = _tbeac_exp;
				avg_LQI = _alqi;
				avg_LQI_inverse = _alqi_in;
				link_stab_ratio = _lsratio;
				link_stab_ratio_inverse = _lsration_in;
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
			void set_avg( uint8_t _alqi )
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
			block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				uint8_t AVG_LQI_POS = 0;
				uint8_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATION_POS + sizeof(uint8_t);
				write<Os, block_data_t, uint8_t>( _buff + AVG_LQI_POS + _offset, avg_LQI );
				write<Os, block_data_t, uint8_t>( _buff + AVG_LQI_IN_POS + _offset, avg_LQI_inverse );
				write<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_POS + _offset, link_stab_ratio );
				write<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_IN_POS + _offset, link_stab_ratio_inverse );
				return buff;
			}
			// --------------------------------------------------------------------
			void de_serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				uint8_t AVG_LQI_POS = 0;
				uint8_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATION_POS + sizeof(uint8_t);
				avg_LQI = read<Os, block_data_t, uint8_t>( _buff + AVG_LQI_POS + _offset );
				avg_LQI_inverse = read<Os, block_data_t, uint8_t>( _buff + AVG_LQI_IN_POS + _offset );
				link_stab_ratio = read<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_POS + _offset );
				link_stab_ratio_inverse = read<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_IN_POS + _offset );
			}
			// --------------------------------------------------------------------
			uint8_t serial_size()
			{
				uint8_t AVG_LQI_POS = 0;
				uint8_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				uint8_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATION_POS + sizeof(uint8_t);
				return LINK_STAB_RATIO_IN_POS + sizeof(uint8_t);
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

		class protocol_settings
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
				consecutive_beacons_lost_threshold		( 5 )
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
								uint8_t _cblost	)
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
			void set_consecutive_beacons_threshold()
			{
				consecutive_beacons_threshold = cb;
			}
			// --------------------------------------------------------------------
			void get_consecutive_beacons_lost_threshold()
			{
				return consecutive_beacons_lost_threshold;
			}
			// --------------------------------------------------------------------
			uint8_t set_consecutive_beacons_lost_threshold()
			{
				consecutive_beacons_lost_threshold = cblost;
			}
			// --------------------------------------------------------------------
			protocol_settings& operator=( const protocol_settings& _psett)
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
				return *this;
			}
			// --------------------------------------------------------------------
		private:
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
		};

		class protocol
		{
		public:
			protocol() :
				prot_id			( 0 ),
				events_flag		( NEW_NB | NEW_NB_BIDI | NEW_PAYLOAD | NEW_PAYLOAD_BIDI | LOST_NB | LOST_NB_BIDI ),
				payload_size	( 0 )
			{}
			// --------------------------------------------------------------------
			~protocol()
			{}
			// --------------------------------------------------------------------
			uint8_t get_prot_id()
			{
				return prot_id;
			}
			// --------------------------------------------------------------------
			void set_prot_id( uint8_t _pid )
			{
				prot_id = _pid;
			}
			// --------------------------------------------------------------------
			size_t get_payload_size()
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
			void set_payload_data( block_data_t* _pdata )
			{
				write<Os, block_data_t, payload_size>( _pdata, payload_data );
			}
			// --------------------------------------------------------------------
			void set_payload( block_data_t* _pdata, size_t _psize )
			{
				payload_size = _psize;
				write<Os, block_data_t, payload_size>( _pdata, payload_data );
			}
			// --------------------------------------------------------------------
			block_data_t* serialize( block_data_t* _buff, _offset = 0 )
			{
				uint8_t NEIGH_SIZE_POS = 0;
				uint8_t NEIGH_VECTOR_POS = NEIGH_SIZE_POS + sizeof( size_t );
				size_t neigh_size = neighborhood.size();
				write<Os, block_data_t, size_t>( _buff + NEIGH_SIZE_POS + _offset, neigh_size );
				for ( size_t i = 0; i < neigh_size; i++ )
				{
					neighborhood.at( i ).serialize( _buff, NEIGH_VECTOR_POS + i * neighborhood.at( i ).serial_size() + _offset );
				}
				return buff;
			}
			// --------------------------------------------------------------------
			void de_serialize( block_data_t* _buff, _offset )
			{
				uint8_t NEIGH_SIZE_POS = 0;
				uint8_t NEIGH_VECTOR_POS = NEIGH_SIZE_POS + sizeof( size_t );
				size_t neigh_size = read<Os, block_data_t, size_t>( buff + NEIGH_SIZE_POS + _offset );
				neighborhood.clear();
				neighbor n;
				uint8_t NEIGH_ELEMS_POS = NEIGH_VECTOR_POS;
				for ( size_t i = 0; i < neigh_size; i++ )
				{
					n.de_serialize( buff + NEIGH_ELEMS_POS );
					NEIGH_ELEMS_POS = NEIGH_ELEMS_POS + n.serial_size();
					neighborhood.push_back( n );
				}
			}
			// --------------------------------------------------------------------
			void serial_size()
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
			protocol& operator=( const protocol& _p )
			{
				prot_id = _p.prot_id;
				write<Os, block_data_t, payload_size>( _p.payload_data, payload_data );
				payload_size = _p.payload_size;
				event_notifier_callback = _p.event_notifier_callback;
				events_flag = _p.events_flag;
				settings = _p.settings;
				neighborhood.clear();
				for ( neighbor_vector_iterator it; it < _p.neighborhood.size(); ++it )
				{
					neighborhood.push_back( *it );
				}
				return *this;
			}
			// --------------------------------------------------------------------
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

			uint8_t prot_id;
			block_data_t* payload_data[MAX_PROTOCOL_PAYLOAD];
			uint8_t payload_size;
			event_notifier_delegate_t event_notifier_callback;
			uint8_t events_flag;
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
			set_status( SEARCHING );
			beacon( 0 );
		};
		// --------------------------------------------------------------------
        void disable()
        {
            set_status( WAITING );
            //TODO
            //clear the lists
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
		}
		// --------------------------------------------------------------------
        void receive( node_id_t _from, size_t _len, block_data_t * _msg, ExData const &_ex )
        {
        }
        // --------------------------------------------------------------------
		uint8_t register_protocol_id( uint8_t _prot_id )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if (it->prot_id == _prot_id )
				{
					return PROT_NUM_IN_USE;
				}
			}
			protocol p;
			p.prot_id = _prot_id;
			p.payload_size  = 0;
			p.events_flag = 0;
			p.event_notifier_callback = event_notifier_delegate_t();
			protocols.push_back( p );
			return SUCCESS;
		}
		// --------------------------------------------------------------------
		uint8_t unregister_protocol_id( uint8_t _prot_id )
		{
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->prot_id == _prot_id )
				{
					protocols.erase( it );
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
		uint8_t register_protocol_settings( protocol_settings _psett )
		{
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->prot_id == _prot_id )
				{
					it->settings = _psett;
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
		uint8_t register_protocol_payload( uint8_t _prot_id, block_data_t* _pdata, size_t _psize )
		{
			total_payload_size = 0;
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(messsage_id_t) + sizeof(size_t) + _psize > Radio::MAX_MSG_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->prot_id == _prot_id )
				{
					it->set_payload( _pdata, _psize );
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
		uint8_t remaining_protocol_payload()
		{
			total_payload_size = 0;
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_payload_size() + total_payload_size;
			}
			return Radio::MAX_MSG_LENGTH - ( total_payload_size + sizeof(messsage_id_t) + sizeof(size_t) );
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)(uint8_t, node_id_t, uint8_t, uint8_t*) >
		uint8_t reg_protocol_event_callback( uint8_t _prot_id, uint8_t _events_flag, T *_obj_pnt )
		{
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->prot_id == _prot_id )
				{
					it->event_notifier_callback = event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt );
					it->events_flag = _events_flag;
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
        void unreg_protocol_event_callback( uint8_t _prot_id )
		{
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->prot_id == _prot_id )
				{
					it->event_notifier_callback = event_notifier_delegate_t();
					it->events_flag = 	protocol::NEW_NB | protocol::NEW_NB_BIDI |
										protocol::NEW_PAYLOAD | protocol::NEW_PAYLOAD_BIDI |
										protocol::LOST_NB | protocol::LOST_NB_BIDI;
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
        template<class T, void(T::*TMethod)(uint8_t, node_id_t, uint8_t, uint8_t*) >
        uint8_t register_protocol( uint8_t _prot_id, uint8_t _psett, block_data_t* _pdata, size_t _psize, uint8_t _events_flag, T *_obj_pnt )
		{
        	if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			total_payload_size = 0;
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(messsage_id_t) + sizeof(size_t) + _psize > Radio::MAX_MSG_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->prot_id == _prot_id )
				{
					return PROT_NUM_IN_USE;
				}
			}
			protocol p;
			p.prot_id = _prot_id;
			p.settings = _psett;
			p.set_payload( _pdata, _psize );
			p.event_notifier_callback = event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt );
			p.events_flag = _events_flag;
			protocols.push_back( p );
			return SUCCESS;
		}
		// --------------------------------------------------------------------
		void set_status( int _st )
		{
			status = _st;
		};
		// --------------------------------------------------------------------
		int get_status()
		{
			return status;
		};
		// --------------------------------------------------------------------
		void set_beacon_period( millis_t _bp )
		{
			beacon_period =_bp;
		};
		// --------------------------------------------------------------------
		millis_t get_beacon_period()
		{
			return beacon_period;
		};
		// --------------------------------------------------------------------
		void set_transmission_power_dB( uint8_t _tpdB )
		{
			transmission_power_dB =_tpdB;
		};
		// --------------------------------------------------------------------
		uint8_t get_transmission_power_dB()
		{
			return transmission_power_dB;
		};
		// --------------------------------------------------------------------
		void set_channel( uint8_t _ch )
		{
			channel =_ch;
		};
		// --------------------------------------------------------------------
		uint8_t get_channel()
		{
			return channel;
		};
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
			NO_PAYLOAD_SPACE = 4
		};

		enum neighbor_discovery_status
		{
			SEARCHING = 1,
			WAITING = 0
		};

		uint32_t recv_callback_id_;
        uint8_t status;
        millis_t beacon_period;
        uint8_t channel;
        uint8_t transmission_power_dB;
        protocol_vector protocols;
        Radio * radio_;
        Clock * clock_;
        Timer * timer_;
        Debug * debug_;
    };
}

#endif
