#ifndef RELIABLE_RADIO_H
#define	RELIABLE_RADIO_H

#include "util/pstl/vector_static.h"
#include "util/delegates/delegate.hpp"
#include "../../internal_interface/message/message.h"
#include "fragment.h"
#include "fragmenting_message.h"
#include "fragmenting_radio_source_config.h"
#include "fragmenting_radio_default_values_config.h"

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
		typedef typename Radio::ExtendedData ExtendedData;
		typedef typename Radio::ExtendedData ExData;
		typedef typename Radio::TxPower TxPower;
		typedef typename Timer::millis_t millis_t;
		typedef delegate4<void, node_id_t, size_t, uint8_t*, ExData const&> event_notifier_delegate_t;
		typedef vector_static<Os, event_notifier_delegate_t, FR_MAX_REGISTERED_PROTOCOLS> RegisteredCallbacks_vector;
		typedef typename RegisteredCallbacks_vector::iterator RegisteredCallbacks_vector_iterator;
		typedef FragmentingMessage_Type<Os, Radio, Debug> FragmentingMessage;
		typedef vector_static<Os, FragmentingMessage, FR_MAX_FRAGMENED_MESSAGES_BUFFERED> FragmentingMessage_vector;
		typedef typename FragmentingMessage_vector::iterator FragmentingMessage_vector_iterator;
		typedef typename FragmentingMessage::Fragment Fragment;
		typedef typename FragmentingMessage::Fragment_vector Fragment_vector;
		typedef typename FragmentingMessage::Fragment_vector_iterator Fragment_vector_iterator;
		typedef FragmentingRadio_Type<Os, Radio, Clock, Timer, Rand, Debug> self_t;
		typedef Message_Type<Os, self_t, Debug> Message;
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
		void disable_radio()
		{
			set_status( FR_WAITING_STATUS );
			radio().template unreg_recv_callback( recv_callback_id_ );
			radio().disable_radio();
		};
		// --------------------------------------------------------------------
		void send( node_id_t _dest, size_t _len, block_data_t* _data )
		{
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - send - Entering.\n" );
#endif
			if ( status == FR_ACTIVE_STATUS )
			{
				if ( Radio::MAX_MESSAGE_LENGTH >= _len )
				{
					radio().send( _dest, _len, _data );
				}
				else
				{
					if ( Radio::MAX_MESSAGE_LENGTH > reserved_bytes() )
					{
						FragmentingMessage fm;
						Message m;
						m.de_serialize( _data );
						size_t pure_payload = _len - radio().reserved_bytes();
						size_t pure_fragment_payload = Radio::MAX_MESSAGE_LENGTH - reserved_bytes();
						//fm.vectorize( _data, _len, pure_payload, pure_fragment_payload, ( rand()() % 0xffff ) );
						fm.print( debug(), radio() );
						debug().debug( " %d - %d - %d - %d - %d", m.get_payload_size(), pure_payload, pure_fragment_payload, Radio::MAX_MESSAGE_LENGTH, reserved_bytes() );
					}
					else
					{
#ifdef DEBUG_FRAGMENTING_RADIO_H
						debug().debug( "FragmentingRadio - send - Message headers exceed maximum payload!\n" );
#endif
					}
				}
			}
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - send - Exiting.\n" );
#endif
		}
		// --------------------------------------------------------------------
		void receive( node_id_t _from, size_t _len, block_data_t * _msg, ExData const &_ex )
		{
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - receive - Entering.\n"  );
#endif
			if ( status == FR_ACTIVE_STATUS )
			{
				if ( _from != radio().id() )
				{
					Message message;
					message.de_serialize( _msg );
					if ( message.get_message_id() == FR_MESSAGE )
					{
						if ( Radio::MAX_MESSAGE_LENGTH >= _len )
						{
							for ( RegisteredCallbacks_vector_iterator i = callbacks.begin(); i != callbacks.end(); ++i )
							{
								(*i)( _from, message.get_payload_size(), message.serialize(), _ex);
							}
						}
						else
						{
							//TODO de_vectorization management
							// -store
							// -check completion
								//-callback if yes
								//-wait if no
							// -purge on timeout
						}
					}
				}
			}
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - receive - Exiting.\n" );
#endif
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)( node_id_t, size_t, block_data_t*, ExData const& ) >
		uint32_t reg_recv_callback( T *_obj_pnt )
		{
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Entering.\n" );
#endif
			if ( status == FR_ACTIVE_STATUS )
			{
				if ( callbacks.max_size() == callbacks.size() )
				{
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Exiting FULL.\n" );
#endif
					return FR_PROT_LIST_FULL;
				}
				callbacks.push_back( event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt ) );
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Exiting SUCCESS.\n" );
#endif
				return FR_SUCCESS;
			}
			else
			{
#ifdef DEBUG_FRAGMENTING_RADIO_H
			debug().debug( "FragmentingRadio - reg_recv_callback - Exiting INACTIVE.\n" );
#endif
				return FR_INACTIVE;
			}
		}
        // --------------------------------------------------------------------
        size_t reserved_bytes()
        {
        	Fragment f;
        	debug().debug( "fragmenting_reserved_bytes - radio().reserved_bytes() : %d - f.serial_size() : %d", radio().reserved_bytes(), f.serial_size() );
        	f.print( debug(), radio() );
        	return ( radio().reserved_bytes() + f.serial_size() );
        };
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
		int set_channel( int _channel )
		{
			return radio().set_channel( _channel );
		}
		// --------------------------------------------------------------------
		int channel()
		{
			return radio().channel();
		}
		// --------------------------------------------------------------------
		int set_power( TxPower _p )
		{
			return radio().set_power( _p );
		}
		// --------------------------------------------------------------------
		TxPower power()
		{
			return radio().power();
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
		node_id_t id()
		{
			return radio().id();
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
            MAX_MESSAGE_LENGTH = 255
        };
        enum SpecialNodeIds
        {
        	BROADCAST_ADDRESS = Radio::BROADCAST_ADDRESS,
        	NULL_NODE_ID = Radio::NULL_NODE_ID
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
