#ifndef RELIABLE_RADIO_PROTOCOL_SETTING_H
#define	RELIABLE_RADIO_PROTOCOL_SETTING_H

#include "reliable_radio_source_config.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Timer_P,
				typename Debug_P>
	class ReliableRadioProtocolSetting_Type
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Timer::millis_t millis_t;
		typedef ReliableRadioProtocolSetting_Type<Os, Radio, Timer, Debug> self_t;
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting_Type() :
			message_id		( 0 ),
			period			( 0 )
		{};
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting_Type( message_id_t _msg_id, millis_t _p )
		{
			message_id = _msg_id;
			period = _p;
		}
		// --------------------------------------------------------------------
		~ReliableRadioProtocolSetting_Type()
		{};
		// --------------------------------------------------------------------
		ReliableRadioProtocolSetting_Type& operator=( const ReliableRadioProtocolSetting_Type& _rrps )
		{
			message_id = _rrps.message_id;
			period = _rrps.period;
			return *this;
		}
		// --------------------------------------------------------------------
		message_id_t get_message_id()
		{
			return message_id;
		}
		// --------------------------------------------------------------------
		void set_message_id( message_id_t _msg_id )
		{
			message_id = _msg_id;
		}
		// --------------------------------------------------------------------
		millis_t get_period()
		{
			return period;
		}
		// --------------------------------------------------------------------
		void set_period( millis_t _p )
		{
			period = _p;
		}
		// --------------------------------------------------------------------
#ifdef RR_DEBUG
		void print( Debug& debug, Radio& radio )
		{
			debug.debug( "-------------------------------------------------------\n");
			debug.debug( "message_id : %d\n", message_id);
			debug.debug( "period : %d\n", period);
			debug.debug( "-------------------------------------------------------\n");
		}
#endif
	private:
		message_id_t message_id;
		millis_t period;
    };
}
#endif
