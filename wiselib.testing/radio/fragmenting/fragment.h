#ifndef __FRAGMENT_H__
#define	__FRAGMENT_H__

#include "fragmenting_radio_source_config.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Debug_P>
	class Fragment_Type
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Debug_P Debug;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::node_id_t node_id_t;
		typedef Fragment_Type<Os, Radio, Debug> self_t;
		// --------------------------------------------------------------------
		Fragment_Type() :
			message_id				( 0 ),
			seq_fragment			( 0 ),
			total_fragments			( 0 ),
			payload_size			( 0 )
		{};
		// --------------------------------------------------------------------
		~Fragment_Type()
		{};
		// --------------------------------------------------------------------
		self_t& operator=( const self_t& _frm )
		{
			message_id = _frm.message_id;
			seq_fragment = _frm.seq_fragment;
			payload_size = _frm.payload_size;
			total_fragments = _frm.total_fragments;
			memcpy( payload, _frm.payload, payload_size );
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
		void set_payload( size_t _len, block_data_t* _buff )
		{
			payload_size = _len;
			memcpy( payload, _buff, _len );
		}
		// --------------------------------------------------------------------
		block_data_t* get_payload()
		{
			return payload;
		}
		// --------------------------------------------------------------------
		size_t get_payload_size()
		{
			return payload_size;
		}
		// --------------------------------------------------------------------
		void set_total_fragments( size_t _tf )
		{
			total_fragments = _tf;
		}
		// --------------------------------------------------------------------
		size_t get_total_fragments()
		{
			return total_fragments;
		}
		// --------------------------------------------------------------------
		size_t get_seq_fragment()
		{
			return seq_fragment;
		}
		// --------------------------------------------------------------------
		void set_seq_fragment( size_t _sf )
		{
			seq_fragment = _sf;
		}
		// --------------------------------------------------------------------
		block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
		{
			size_t MESSAGE_ID_POS = 0;
			size_t SEQ_FRAGMENT_POS = MESSAGE_ID_POS + sizeof(message_id_t);
			size_t TOTAL_FRAGMENTS_POS = SEQ_FRAGMENT_POS + sizeof(size_t);
			size_t PAYLOAD_SIZE_POS = TOTAL_FRAGMENTS_POS + sizeof(size_t);
			size_t PAYLOAD_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
			write<Os, block_data_t, message_id_t>( _buff + MESSAGE_ID_POS + _offset, message_id );
			write<Os, block_data_t, uint16_t>( _buff + SEQ_FRAGMENT_POS + _offset, seq_fragment );
			write<Os, block_data_t, uint16_t>( _buff + TOTAL_FRAGMENTS_POS + _offset, total_fragments );
			write<Os, block_data_t, size_t>( _buff + PAYLOAD_SIZE_POS + _offset, payload_size );
			memcpy( _buff + PAYLOAD_POS + _offset, payload, payload_size );
			return _buff;
		}
		// --------------------------------------------------------------------
		void de_serialize( block_data_t* _buff, size_t _offset = 0 )
		{
			size_t MESSAGE_ID_POS = 0;
			size_t SEQ_FRAGMENT_POS = MESSAGE_ID_POS + sizeof(message_id_t);
			size_t TOTAL_FRAGMENTS_POS = SEQ_FRAGMENT_POS + sizeof(size_t);
			size_t PAYLOAD_SIZE_POS = TOTAL_FRAGMENTS_POS + sizeof(size_t);
			size_t PAYLOAD_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
			message_id = read<Os, block_data_t, message_id_t>( _buff + MESSAGE_ID_POS + _offset );
			seq_fragment = read<Os, block_data_t, uint16_t>( _buff + SEQ_FRAGMENT_POS + _offset );
			total_fragments = read<Os, block_data_t, uint16_t>( _buff + TOTAL_FRAGMENTS_POS + _offset );
			payload_size = read<Os, block_data_t, size_t>( _buff + PAYLOAD_SIZE_POS + _offset );
			memcpy( payload, _buff + PAYLOAD_POS + _offset, payload_size );
		}
		// --------------------------------------------------------------------
		size_t serial_size()
		{
			size_t MESSAGE_ID_POS = 0;
			size_t SEQ_FRAGMENT_POS = MESSAGE_ID_POS + sizeof(message_id_t);
			size_t TOTAL_FRAGMENTS_POS = SEQ_FRAGMENT_POS + sizeof(size_t);
			size_t PAYLOAD_SIZE_POS = TOTAL_FRAGMENTS_POS + sizeof(size_t);
			size_t PAYLOAD_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
			return PAYLOAD_POS + payload_size;
		}
		// --------------------------------------------------------------------
#ifdef DEBUG_FRAGMENT_H
		void print( Debug& _debug, Radio& _radio )
		{
			_debug.debug( "-------------------------------------------------------\n");
			_debug.debug( "Fragment : \n" );
			_debug.debug( "message_id (size %i) : %d\n", sizeof(message_id_t), message_id );
			_debug.debug( "seq_fragment (size %i) : %d\n", sizeof(size_t), seq_fragment );
			_debug.debug( "total_fragments (size %i) : %d\n", sizeof(size_t), total_fragments );
			_debug.debug( "payload_size (size %i) : %d\n", sizeof(size_t), payload_size );
			_debug.debug( "payload: \n");
			for ( size_t i = 0; i < payload_size; i++ )
			{
				_debug.debug("%d", payload[i] );
			}
			_debug.debug( "-------------------------------------------------------\n");
		}
#endif
		// --------------------------------------------------------------------
	private:
		message_id_t message_id;
		size_t seq_fragment;
		size_t total_fragments;
		block_data_t payload[Radio::MAX_MESSAGE_LENGTH];
		size_t payload_size;
    };
}
#endif
