#ifndef RELIABLE_RADIO_PROTOCOL_H
#define	RELIABLE_RADIO_PROTOCOL_H

#include "util/pstl/vector_static.h"
#include "reliable_radio_source_config.h"
#include "reliable_radio_protocol_setting.h"
#include "reliable_radio_default_values_config.h"


namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Timer_P,
				typename Debug_P>
	class ReliableRadioProtocol_Type
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Radio::ExtendedData ExData;
		typedef ReliableRadioProtocol_Type<Os, Radio, Timer, Debug> self_t;
		typedef ReliableRadioProtocolSetting_Type<Os, Radio, Timer, Debug> ReliableRadioProtocolSetting;
		typedef vector_static<Os, ReliableRadioProtocolSetting, RR_MAX_MESSAGES_PER_PROTOCOL> ReliableRadioProtocolSetting_vector;
		typedef typename ReliableRadioProtocolSetting_vector::iterator ReliableRadioProtocolSetting_vector_iterator;
		typedef delegate4<void, node_id_t, size_t, uint8_t*, ExData const&> event_notifier_delegate_t;
		// --------------------------------------------------------------------
		ReliableRadioProtocol_Type() :
			protocol_id		( 0 ),
			event_notifier_callback		( event_notifier_delegate_t::template from_method<ReliableRadioProtocol_Type, &ReliableRadioProtocol_Type::null_callback > ( this ) )
		{};
		// --------------------------------------------------------------------
		~ReliableRadioProtocol_Type()
		{};
		// --------------------------------------------------------------------
		uint8_t get_protocol_id()
		{
			return protocol_id;
		}
		// --------------------------------------------------------------------
		void set_protocol_id( uint8_t _prot_id )
		{
			protocol_id = _prot_id;
		}
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting* get_protocol_setting_ref( message_id_t _msg_id )
		{
			for ( ReliableRadioProtocolSetting_vector_iterator i = protocol_settings.begin(); i != protocol_settings.end(); ++i )
			{
				if ( _msg_id == i->get_message_id() )
				{
					return &(*i);
				}
			}
			return NULL;
		}
		// --------------------------------------------------------------------
		void add_protocol_setting( ReliableRadioProtocolSetting& _prot_set )
		{
			if ( !get_protocol_setting_ref( _prot_set.get_message_id() ) )
			{
				protocol_settings.push_back( _prot_set );
			}
		}
		// --------------------------------------------------------------------
		void remove_protocol_setting( message_id_t _msg_id )
		{
			for ( ReliableRadioProtocolSetting_vector_iterator i = protocol_settings.begin(); i != protocol_settings.end(); ++i )
			{
				if ( _msg_id == i->get_message_id() )
				{
					protocol_settings.erase( i );
					return;
				}
			}
		}
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting_vector* get_protocol_settings_ref()
		{
			return &protocol_settings;
		}
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting_vector get_protocol_settings()
		{
			return protocol_settings;
		}
		// --------------------------------------------------------------------
		void set_protocol_settings( ReliableRadioProtocolSetting_vector& _rrps_v )
		{
			protocol_settings = _rrps_v;
		}
		// --------------------------------------------------------------------
		event_notifier_delegate_t get_event_notifier_callback()
		{
			return event_notifier_callback;
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)(node_id_t, size_t, uint8_t*, ExData const&) >
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
			event_notifier_callback	= event_notifier_delegate_t::template from_method<ReliableRadioProtocol_Type, &ReliableRadioProtocol_Type::null_callback > ( this );
		}
		// --------------------------------------------------------------------
		void null_callback( node_id_t null_node_id, size_t null_len, uint8_t* null_data, ExData const& null_metrics )
		{
		}
		// --------------------------------------------------------------------
		ReliableRadioProtocol_Type& operator=( const ReliableRadioProtocol_Type& _rrp )
		{
			protocol_id = _rrp.protocol_id;
			protocol_settings = _rrp.protocol_settings;
			event_notifier_callback = _rrp.event_notifier_callback;
			return *this;
		}
		// --------------------------------------------------------------------
#ifdef RR_DEBUG
		void print( Debug& debug, Radio& radio )
		{
			debug.debug( "-------------------------------------------------------\n");
			debug.debug( "protocol :\n");
			debug.debug( "protocol_id : %d\n", protocol_id );
			debug.debug( "settings :\n");
			for ( ReliableRadioProtocolSetting_vector_iterator it = protocol_settings.begin(); it != protocol_settings.end(); ++it )
			{

				it->print( debug, radio );
			}
			debug.debug( "-------------------------------------------------------\n");
		}
#endif
	private:
		uint8_t protocol_id;
		ReliableRadioProtocolSetting_vector protocol_settings;
		event_notifier_delegate_t event_notifier_callback;
    };
}
#endif
