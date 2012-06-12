#ifndef RELIABLE_RADIO_PROTOCOL_SETTING_H
#define	RELIABLE_RADIO_PROTOCOL_SETTING_H

#include "reliable_radio_source_config.h"
#include "reliable_radio_default_values_config.h"
#include "../../internal_interface/message/message.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Timer_P,
				typename Debug_P>
	class ReliableRadioMessage_Type
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Timer::millis_t millis_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef Message_Type<Os, Radio> Message;
		typedef ReliableRadioMessage_Type<Os, Radio, Timer, Debug> self_t;
		// --------------------------------------------------------------------
		ReliableRadioMessage_Type() :
			reliable_message_id		( RELIABLE_MESSAGE ),
			message_id				( 0 ),
			period					( RR_PROTOCOL_SETTING_PERIOD ),
			alive_period			( RR_PROTOCOL_SETTING_ALIVE_PERIOD )
		{};
		// --------------------------------------------------------------------
		ReliableRadioMessage_Type( message_id_t _msg_id, millis_t _p, millis_t _ap )
		{
			reliable_message_id = RELIABLE_MESSAGE;
			message_id = _msg_id;
			period = _p;
			alive_period = _ap;
		}
		// --------------------------------------------------------------------
		~ReliableRadioProtocolSetting_Type()
		{};
		// --------------------------------------------------------------------
		ReliableRadioMessage_Type& operator=( const ReliableRadioMessage_Type& _rrps )
		{
			reliable_message_id = RELIABLE_MESSAGE;
			message_id = _rrps.message_id;
			period = _rrps.period;
			alive_period = _rrps.alive_period;
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
		message_id_t get_reliable_message_id()
		{
			return reliable_message_id;
		}
		// --------------------------------------------------------------------
		void set_period( millis_t _p )
		{
			period = _p;
		}
		// --------------------------------------------------------------------
		millis_t get_alive_period()
		{
			return alive_period;
		}
		// --------------------------------------------------------------------
		void set_alive_period( millis_t _ap )
		{
			alive_period = _ap;
		}
		// --------------------------------------------------------------------
		block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
		{
			size_t RELIABLE_MSG_ID_POS = 0;
			size_t MSG_ID_POS = RELIABLE_MSG_ID_POS + sizeof(message_id_t);
			size_t DATA_LEN_POS = MSG_ID_POS + sizeof(message_id_t);
			size_t DATA_POS = DATA_LEN_POS + sizeof(size_t);
			write<Os, block_data_t, message_id_t>( _buff + RELIABLE_MSG_ID_POS + _offset, RELIABLE_MESSAGE );
			write<Os, block_data_t, message_id_t>( _buff + MSG_ID_POS + _offset,  message_id );
			write<Os, block_data_t, size_t>( _buff + DATA_LEN_POS + _offset, message.buffer_size() );
			memcpy( _buff + DATA_POS + _offset, message.buffer(), message.buffer_size() );
			return _buff;
		}
		// --------------------------------------------------------------------
		void de_serialize( block_data_t* _buff, size_t _offset = 0 )
		{
			size_t RELIABLE_MSG_ID_POS = 0;
			size_t MSG_ID_POS = RELIABLE_MSG_ID_POS + sizeof(message_id_t);
			size_t DATA_LEN_POS = MSG_ID_POS + sizeof(message_id_t);
			size_t DATA_POS = DATA_LEN_POS + sizeof(size_t);
			reliable_message_id = read<Os, block_data_t, message_id_t>( _buff + RELIABLE_MSG_ID_POS + _offset );
			message_id = read<Os, block_data_t, message_id_t>( _buff + MSG_ID_POS + _offset );
			data_len = read<Os, block_data_t, size_t>( _buff + DATA_LEN_POS + _offset );
			memcpy( message.buffer(), _buff + DATA_POS + _offset, data_len );
		}
		// --------------------------------------------------------------------
		size_t serial_size()
		{
			size_t RELIABLE_MSG_ID_POS = 0;
			size_t MSG_ID_POS = RELIABLE_MSG_ID_POS + sizeof(message_id_t);
			size_t DATA_LEN_POS = MSG_ID_POS + sizeof(message_id_t);
			size_t DATA_POS = DATA_LEN_POS + sizeof(size_t);
			return DATA_LEN_POS + message.buffer_size();
		}
		// --------------------------------------------------------------------
		enum message_ids
		{
			RELIABLE_MESSAGE = 112
		};
		// --------------------------------------------------------------------
#ifdef RR_DEBUG
		void print( Debug& debug, Radio& radio )
		{
			debug.debug( "-------------------------------------------------------\n");
			debug.debug( "reliable_message_id : %d\n", message_id);
			debug.debug( "message_id : %d\n", message_id);
			debug.debug( "period : %d\n", period);
			debug.debug( "alive period : %d\n", alive_period);
			debug.debug( "serial_size : %d\n", serial_size() );
			debug.debug( "-------------------------------------------------------\n");
		}
#endif
		// --------------------------------------------------------------------
	private:
		message_id_t reliable_message_id;
		message_id_t message_id;
		millis_t period;
		millis_t alive_period;
		Message message;
    };
}
#endif
