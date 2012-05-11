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
		typedef ReliableRadioProtocol_Type<Os, Radio, Timer, Debug> self_t;
		typedef ReliableRadioProtocolSetting_Type<Os, Radio, Timer, Debug> ReliableRadioProtocolSetting;
		typedef vector_static<Os, ReliableRadioProtocolSetting, RR_MAX_PROTOCOLS_REGISTERED> ReliableRadioProtocolSetting_vector;
		typedef typename ReliableRadioProtocolSetting_vector::iterator ReliableRadioProtocolSetting_vector_iterator;
		typedef delegate4<void, uint8_t, node_id_t, size_t, uint8_t*> event_notifier_delegate_t;
		// --------------------------------------------------------------------
		ReliableRadioProtocol_Type()
		{};
		// --------------------------------------------------------------------
		~ReliableRadioProtocol_Type()
		{};
		// --------------------------------------------------------------------
		uint16_t get_reliable_radio_protocol_id()
		{
			return protocol_id;
		}
		// --------------------------------------------------------------------
		void set_reliable_radio_protocol_id( uint16_t _prot_id )
		{
			protocol_id = _prot_id;
		}
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting* get_protocol_setting_ref( message_id_t _msg_id )
		{
			return &( *get_protocol_setting_it( _msg_id ) );
		}
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting get_protocol_setting( message_id_t _msg_id )
		{
			return *get_protocol_setting_it( _msg_id );
		}
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting_vector_iterator get_protocol_setting_it( message_id_t _msg_id )
		{
			for ( ReliableRadioProtocolSetting_vector_iterator i = protocol_settings.begin(); i != protocol_settings.end(); ++i )
			{
				if ( _msg_id == i->get_message_id() )
				{
					return i;
				}
			}
			return NULL;
		}
		// --------------------------------------------------------------------
		void add_protocol_setting( const ReliableRadioProtocolSetting& _prot_set )
		{
			if ( get_protocol_setting_it( _prot_set.get_message_id() ) )
			{
				protocol_settings.push_back( _prot_set );
			}
		}
		// --------------------------------------------------------------------
		void remove_protocol_setting( const ReliableRadioProtocolSetting& _prot_set )
		{
			ReliableRadioProtocolSetting_vector_iterator i = get_protocol_setting_it( _prot_set.get_message_id() );
			if ( i != NULL )
			{
				protocol_settings.remove( i );
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
		ReliableRadioProtocol_Type& operator=( const ReliableRadioProtocol_Type& _rrp )
		{
			protocol_id = _rrp.protocol_id;
			protocol_settings = _rrp.protocol_settings;
			return *this;
		}
	private:
		uint16_t protocol_id;
		ReliableRadioProtocolSetting_vector protocol_settings;
		event_notifier_delegate_t event_notifier_callback;
    };
}
#endif
