#ifndef RELIABLE_RADIO_H
#define	RELIABLE_RADIO_H

#include "util/pstl/vector_static.h"
#include "util/delegates/delegate.hpp"
#include "../../internal_interface/message/message.h"
#include "reliable_radio_protocol.h"
#include "reliable_radio_protocol_setting.h"
#include "reliable_radio_source_config.h"
#include "reliable_radio_default_values_config.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Clock_P,
				typename Timer_P,
				typename Rand_P,
				typename Debug_P>
	class ReliableRadio_Type
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef Clock_P Clock;
		typedef Rand_P Rand;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Clock::time_t time_t;
		typedef typename Radio::ExtendedData ExData;
		typedef typename Radio::TxPower TxPower;
		typedef typename Timer::millis_t millis_t;
		typedef ReliableRadioProtocol_Type<Os, Radio, Timer, Debug> ReliableRadioProtocol;
		typedef vector_static<Os, ReliableRadioProtocol, RR_MAX_PROTOCOLS_REGISTERED > ReliableRadioProtocol_vector;
		typedef typename ReliableRadioProtocol_vector::iterator ReliableRadioProtocol_vector_iterator;
		typedef typename ReliableRadioProtocol::event_notifier_delegate_t ReliableRadioProtocol_event_notifier_delegate_t;
		typedef ReliableRadioProtocolSetting_Type<Os, Radio, Timer, Debug> ReliableRadioProtocolSetting;
		typedef ReliableRadio_Type<Os, Radio,	Clock, Timer, Rand, Debug> self_t;
		typedef Message_Type<Os, Radio> Message;
		// --------------------------------------------------------------------
		ReliableRadio_Type()
		{};
		// --------------------------------------------------------------------
		~ReliableRadio_Type()
		{};
		// --------------------------------------------------------------------
		void enable()
		{
			reliable_radio_daemon();
		};
		// --------------------------------------------------------------------
		void disable()
		{
			set_status( WAITING_STATUS );
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
		void receive( node_id_t _from, size_t _len, block_data_t * _msg, ExData const &_ex )
		{
			if ( _from != radio().id() )
			{
			}
		}
		// --------------------------------------------------------------------
		void reliable_radio_daemon( void* user_data = NULL )
		{
			if ( status == ACTIVE_STATUS )
			{
				timer().template set_timer<self_t, &self_t::reliable_radio_daemon> ( reliable_radio_period, this, 0 );
			}
		}
		// --------------------------------------------------------------------
		uint8_t register_protocol( ReliableRadioProtocol& _p )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			for ( ReliableRadioProtocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _p.get_protocol_id() )
				{
					return PROT_NUM_IN_USE;
				}
			}
			protocols.push_back( _p );
			return SUCCESS;
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
		millis_t get_reliable_radio_period()
		{
			return reliable_radio_period;
		}
		// --------------------------------------------------------------------
		void set_reliable_radio_period( millis_t _rrp )
		{
			reliable_radio_period =_rrp;
		}
		// --------------------------------------------------------------------
		void init( Radio& _radio, Timer& _timer, Debug& _debug, Clock& _clock, Rand& _rand )
		{
			radio_ = &_radio;
			timer_ = &_timer;
			debug_ = &_debug;
			clock_ = &_clock;
			rand_ = &_rand;
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
		Rand& rand()
		{
			return *rand_;
		}
		// --------------------------------------------------------------------
		enum reliable_radio_status
		{
			ACTIVE_STATUS,
			WAITING_STATUS,
			RR_STATUS_NUM_VALUES
		};
		enum reliable_radio_errors
		{
			SUCCESS,
			PROT_LIST_FULL,
			PROT_NUM_IN_USE,
			RR_ERROR_NUM_VALUES
		};
	private:
		uint32_t recv_callback_id_;
        uint8_t status;
        millis_t reliable_radio_period;
        ReliableRadioProtocol_vector protocols;
        Radio * radio_;
        Clock * clock_;
        Timer * timer_;
        Debug * debug_;
        Rand * rand_;
    };
}

#endif
