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
			node_id_t id;
			uint32_t total_beacons;
			uint32_t total_beacons_expected;
			uint8_t avg_LQI;
			uint8_t avg_LQI_inverse;
			uint8_t link_stab_ratio;
			uint8_t link_stab_ratio_inverse;
			uint8_t consecutive_beacons;
			uint8_t consecutive_beacons_lost;
			bool active;
			bool bidi;
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
			protocol_settings(	uint8_t maxLQI,
								uint8_t minLQI,
								uint8_t maxLQI_in,
								uint8_t minLQI_in,
								uint8_t maxlsr,
								uint8_t minlsr,
								uint8_t maxlsr_in,
								uint8_t minlsr_in,
								uint8_t cb,
								uint8_t cblost	)
			{
				max_avg_LQI_threshold = maxLQI;
				min_avg_LQI_threshold = minLQI;
				max_avg_LQI_inverse_threshold = maxLQI_in;
				min_avg_LQI_inverse_threshold = minLQI_in;
				max_link_stab_ratio_threshold = maxlsr;
				min_link_stab_ratio_threshold = minlsr;
				max_link_stab_ratio_inverse_threshold = maxlsr_in;
				min_link_stab_ratio_inverse_threshold = minlsr_in,
				consecutive_beacons_threshold = cb;
				consecutive_beacons_lost_threshold = cblost;
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
			void set_max_avg_LQI_threshold( uint8_t maxLQI )
			{
				max_avg_LQI_threshold = maxLQI;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_avg_LQI_threshold()
			{
				return min_avg_LQI_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_avg_LQI_threshold( uint8_t minLQI )
			{
				min_avg_LQI_threshold = minLQI;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_avg_LQI_inverse_threshold()
			{
				return max_avg_LQI_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_avg_LQI_inverse_threshold( uint8_t maxLQI_in )
			{
				max_avg_LQI_inverse_threshold = maxLQI_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_avg_LQI_inverse_threshold()
			{
				return min_avg_LQI_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_avg_LQI_inverse_threshold( uint8_t minLQI_in )
			{
				min_avg_LQI_inverse_threshold = minLQI_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_link_stab_ratio_threshold()
			{
				return max_link_stab_ratio_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_link_stab_ratio_threshold( uint8_t maxlsr )
			{
				max_link_stab_ratio_threshold = maxlsr;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_link_stab_ratio_threshold()
			{
				return min_link_stab_ratio_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_link_stab_ratio_threshold( uint8_t minlsr )
			{
				min_link_stab_ratio_threshold = minlsr;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_link_stab_ratio_inverse_threshold()
			{
				return max_link_stab_ratio_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_link_stab_ratio_inverse_threshold( uint8_t maxlsr_in )
			{
				max_link_stab_ratio_inverse_threshold = maxlsr_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_link_stab_ratio_inverse_threshold()
			{
				return min_link_stab_ratio_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_link_stab_ratio_inverse_threshold( uint8_t minlsr_in )
			{
				min_link_stab_ratio_inverse_threshold = minlsr_in;
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
			protocol_settings& operator=( const protocol_settings& _t)
			{
				max_avg_LQI_threshold = _t.max_avg_LQI_threshold;
				min_avg_LQI_threshold = _t.min_avg_LQI_threshold;
				max_avg_LQI_inverse_threshold = _t.max_avg_LQI_inverse_threshold;
				min_avg_LQI_inverse_threshold = _t.min_avg_LQI_inverse_threshold;
				max_link_stab_ratio_threshold = _t.max_link_stab_ratio_threshold;
				min_link_stab_ratio_threshold = _t.min_link_stab_ratio_threshold;
				max_link_stab_ratio_inverse_threshold = _t.max_link_stab_ratio_inverse_threshold;
				min_link_stab_ratio_inverse_threshold = _t.min_link_stab_ratio_inverse_threshold;
				consecutive_beacons_threshold = _t.consecutive_beacons_threshold;
				consecutive_beacons_lost_threshold = _t.consecutive_beacons_lost_threshold;
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
			protocol()
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
		private:
			uint8_t prot_id;
			uint8_t paylod_data[MAX_PROTOCOL_PAYLOAD];
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
			recv_callback_id_ = radio().template reg_recv_callback<self_t, &self_t::receive>(this);
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
    	void send(node_id_t destination, size_t len, block_data_t *data, message_id_t msg_id)
    	{
    		Message message;
    		message.set_msg_id(msg_id);
    		message.set_payload(len, data);
    		radio().send( destination, message.buffer_size(), (uint8_t*) &message );
    	}
    	// --------------------------------------------------------------------
		void beacon( void* data )
		{
		}
		// --------------------------------------------------------------------
        void receive( node_id_t from, size_t len, block_data_t * msg, ExData const &ex )
        {
        }
        // --------------------------------------------------------------------
		uint8_t register_protocol( uint8_t _prot_id )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return RGD_LIST_FULL;
			}
			for ( size_t i = 0; i < protocols.size(); i++ )
			{
				if ( protocols.at(i).prot_id ==  _prot_id )
				{
					return RGD_NUM_INUSE;
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
		uint8_t unregister_protocol( uint8_t _prot_id )
		{
			for ( protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->prot_id == _prot_id )
				{
					protocols.erase( it );
					return SUCCESS;
				}
			}
			return INV_ALG_ID;
		}
		// --------------------------------------------------------------------
		uint8_t set_payload( uint8_t payload_id, uint8_t *data, uint8_t len )
		{
//			for ( reg_alg_iterator_t it = registered_apps.begin(); it != registered_apps.end(); it++ )
//			{
//				if ( it->alg_id == payload_id )
//				{
//					memcpy( it->data, data, len );
//					it->size = len;
//					return 0;
//				}
//			}
			return INV_ALG_ID;
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
			SUCCESS = 0, /*!< The method return with no errors */
			RGD_NUM_INUSE = 1, /*!< This app number is already registered */
			RGD_LIST_FULL = 2, /*!< The list with the registered apps is full*/
			INV_ALG_ID = 3 /*!< The alg id is invalid*/
		};

		enum event_codes
		{
			NEW_NB = 1, /*!< Event code for a newly added stable neighbor */
			NEW_NB_BIDI = 2, /*!< Event code for a newly added bidi neighbor */
			DROPPED_NB = 4, /*!< Event code for a neighbor removed from nb list */
			NEW_PAYLOAD = 8, /*!< Event code for a newly arrived pg payload */
			NEW_PAYLOAD_BIDI = 16, /*!< Event code for a newly arrived pg payload from a bidi neighbor */
			LOST_NB_BIDI = 32, /*!< Event code generated when we loose bidi comm with a nb */
			NB_READY = 64, /*!< Event code generated after the nb module has generated a stable nhd * Useful for starting other modules that must wait until the nb has * produced a stable neighborhood */
			DEFAULT = 5 /*!< Event code for NEW_NB + DROPED_NB*/
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
