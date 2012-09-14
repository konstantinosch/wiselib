#ifndef RELIABLE_RADIO_H
#define	RELIABLE_RADIO_H

#include "util/pstl/vector_static.h"
#include "util/delegates/delegate.hpp"
#include "../../internal_interface/message/message.h"
#include "fragmenting_radio_message.h"
#include "fragmenting_radio_source_config.h"
#include "fragmenging_radio_default_values_config.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Clock_P,
				typename Timer_P,
				typename Rand_P,
				typename Debug_P>
	class FragmentingRadio_Type
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
		typedef delegate4<void, node_id_t, size_t, uint8_t*, ExData const&> event_notifier_delegate_t;
		typedef vector_static<Os, event_notifier_delegate_t, FR_MAX_REGISTERED_PROTOCOLS> RegisteredCallbacks_vector;
		typedef typename RegisteredCallbacks_vector::iterator RegisteredCallbacks_vector_iterator;
		typedef Message_Type<Os, Radio, Debug> Message;
		typedef FragmentingRadioMessage_Type<Os, Radio, Debug> FragmentingRadioMessage;
		typedef vector_static<Os, FragmentingRadioMessage, FR_MAX_BUFFERED_MESSAGES> ReliableRadioMessage_vector;
		typedef typename ReliableRadioMessage_vector::iterator ReliableRadioMessage_vector_iterator;
		typedef ReliableRadio_Type<Os, Radio, Clock, Timer, Rand, Debug> self_t;
		// --------------------------------------------------------------------
		FragmentingRadio_Type()
		{};
		// --------------------------------------------------------------------
		~FragmentingRadio_Type()
		{};
		// --------------------------------------------------------------------
		void enable_radio()
		{
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - enable - Entering.\n" );
#endif
			radio().enable_radio();
			set_status( FR_ACTIVE_STATUS );
			recv_callback_id_ = radio().template reg_recv_callback<self_t, &self_t::receive>( this );
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - enable - Exiting.\n" );
#endif
		};
		// --------------------------------------------------------------------
		void disable()
		{
			set_status( FR_WAITING_STATUS );
			radio().template unreg_recv_callback( recv_callback_id_ );
		};
		// --------------------------------------------------------------------
		void send( node_id_t _dest, size_t _len, block_data_t* _data )
		{
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - send - Entering.\n" );
#endif
			if ( status == FR_ACTIVE_STATUS )
			{
			}
#ifdef DEBUG_RELIABLE_RADIO_H
			debug().debug( "FragmentingRadio - send - Exiting.\n" );
#endif
		}
		// --------------------------------------------------------------------
		void receive( node_id_t _from, size_t _len, block_data_t * _msg, ExData const &_ex )
		{
#ifdef DEBUG_RELIABLE_RADIO_H
			debug().debug( "FragmentingRadio - receive - Entering.\n"  );
#endif
			if ( status == FR_ACTIVE_STATUS )
			{
				if ( _from != radio().id() )
				{
				}
			}
#ifdef DEBUG_RELIABLE_RADIO_H
			debug().debug( "FragmentingRadio - receive - Exiting.\n" );
#endif
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)( node_id_t, size_t, block_data_t*, ExData const& ) >
		uint32_t reg_recv_callback( T *_obj_pnt )
		{
#ifdef DEBUG_RELIABLE_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Entering.\n" );
#endif
			if ( status == FR_ACTIVE_STATUS )
			{
				if ( callbacks.max_size() == callbacks.size() )
				{
#ifdef DEBUG_RELIABLE_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Exiting FULL.\n" );
#endif
					return FR_PROT_LIST_FULL;
				}
				callbacks.push_back( event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt ) );
#ifdef DEBUG_RELIABLE_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Exiting SUCCESS.\n" );
#endif
				return FR_SUCCESS;
			}
			else
			{
#ifdef DEBUG_RELIABLE_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Exiting INACTIVE.\n" );
#endif
				return FR_INACTIVE;
			}
		}
		// --------------------------------------------------------------------
		uint8_t get_status()
		{
			return status;
		}
		// --------------------------------------------------------------------
		void set_status( uint8_t _st )
		{
			status = _st;
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
			FR_ACTIVE_STATUS,
			FR_WAITING_STATUS,
			FR_STATUS_NUM_VALUES
		};
		enum reliable_radio_errors
		{
			FR_SUCCESS,
			FR_PROT_LIST_FULL,
			FR_INACTIVE,
			FR_ERROR_NUM_VALUES
		};
		enum reliable_radio_message_ids
		{
			FR_MESSAGE = 222,
			FR_REPLY = 223,
			FR_UNDELIVERED = 224
		};
        enum Restrictions
        {
            MAX_MESSAGE_LENGTH = 112
        };
	private:
		uint32_t recv_callback_id_;
        uint8_t status;
        RegisteredCallbacks_vector callbacks;
        Radio * radio_;
        Clock * clock_;
        Timer * timer_;
        Debug * debug_;
        Rand * rand_;
    };
}

#endif
