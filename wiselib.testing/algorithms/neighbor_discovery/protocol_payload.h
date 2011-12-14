#ifndef PROTOCOL_PAYLOAD_H
#define	PROTOCOL_PAYLOAD_H

#include "neighbor_discovery_config.h"

namespace wiselib
{
	template< 	typename Os_P,
				typename Radio_P,
				typename Debug_P>
	class ProtocolPayload
	{
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Debug_P Debug;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef ProtocolPayload< Os, Radio, Debug> self_type;
	public:
		ProtocolPayload() :
			payload_size		( NB_MAX_PROTOCOL_PAYLOAD_SIZE )
		{}
		// --------------------------------------------------------------------
		ProtocolPayload( uint8_t _pid, size_t _ps, block_data_t* _pd, size_t _offset = 0 )
		{
			protocol_id = _pid;
			if ( _ps <= NB_MAX_PROTOCOL_PAYLOAD_SIZE )
			{
				payload_size = _ps;
			}
			else
			{
				payload_size = NB_MAX_PROTOCOL_PAYLOAD_SIZE;
			}
			for ( size_t i = 0; i < payload_size; i++ )
			{
				payload_data[i] = _pd[i + _offset];
			}
		}
		// --------------------------------------------------------------------
		~ProtocolPayload()
		{}
		// --------------------------------------------------------------------
		uint8_t get_protocol_id()
		{
			return protocol_id;
		}
		// --------------------------------------------------------------------
		void set_protocol_id( uint8_t _pid )
		{
			protocol_id = _pid;
		}
		// --------------------------------------------------------------------
		size_t get_payload_size()
		{
			return payload_size;
		}
		// --------------------------------------------------------------------
		size_t get_max_payload_size()
		{
			return NB_MAX_PROTOCOL_PAYLOAD_SIZE;
		}
		// --------------------------------------------------------------------
		void set_payload_size( size_t _ps )
		{
			if ( _ps <= NB_MAX_PROTOCOL_PAYLOAD_SIZE )
			{
				payload_size = _ps;
			}
			else
			{
				payload_size = NB_MAX_PROTOCOL_PAYLOAD_SIZE;
			}
		}
		// --------------------------------------------------------------------
		block_data_t* get_payload_data()
		{
			return payload_data;
		}
		// --------------------------------------------------------------------
		void set_payload_data( block_data_t* _pd, size_t _offset = 0 )
		{
			for ( size_t i = 0; i < payload_size; i++ )
			{
				payload_data[i] = _pd[i + _offset];
			}
		}
		// --------------------------------------------------------------------
		void set_payload( block_data_t* _pd, size_t _ps, size_t _offset = 0 )
		{
			if ( _ps <= NB_MAX_PROTOCOL_PAYLOAD_SIZE )
			{
				payload_size = _ps;
				for ( size_t i = 0; i < payload_size; i++ )
				{
					payload_data[i] = _pd[i + _offset];
				}
			}
		}
		// --------------------------------------------------------------------
		ProtocolPayload& operator=( const ProtocolPayload& _pp )
		{
			protocol_id = _pp.protocol_id;
			payload_size = _pp.payload_size;
			for ( size_t i = 0 ; i < payload_size; i++ )
			{
				payload_data[i] = _pp.payload_data[i];
			}
			return *this;
		}
		// --------------------------------------------------------------------
		void print( Debug& debug )
		{
			debug.debug( "-------------------------------------------------------");
			debug.debug( "protocol_payload :");
			debug.debug( "protocol_id : %d ", protocol_id );
			debug.debug( "max_payload_size : %d", NB_MAX_PROTOCOL_PAYLOAD_SIZE );
			debug.debug( "payload_size : %d ", payload_size );
			for ( size_t i = 0; i < payload_size; i++ )
			{
				debug.debug( "payload %d 'th byte : %d", i, payload_data[i] );
			}
			debug.debug( "-------------------------------------------------------");
		}
		// --------------------------------------------------------------------
		block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
		{
			size_t PROTOCOL_ID_POS = 0;
			size_t PAYLOAD_SIZE_POS = PROTOCOL_ID_POS + sizeof(uint8_t);
			size_t PAYLOAD_DATA_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
			write<Os, block_data_t, uint8_t>( _buff + PROTOCOL_ID_POS + _offset, protocol_id );
			write<Os, block_data_t, size_t>( _buff + PAYLOAD_SIZE_POS + _offset, payload_size );
			for ( size_t i = 0 ; i < payload_size; i++ )
			{
				_buff[PAYLOAD_DATA_POS + i + _offset] = payload_data[i];
			}
			return _buff;
		}
		// --------------------------------------------------------------------
		void de_serialize( block_data_t* _buff, size_t _offset = 0 )
		{
			size_t PROTOCOL_ID_POS = 0;
			size_t PAYLOAD_SIZE_POS = PROTOCOL_ID_POS + sizeof(uint8_t);
			size_t PAYLOAD_DATA_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
			protocol_id = read<Os, block_data_t, uint8_t>( _buff + PROTOCOL_ID_POS + _offset );
			payload_size = read<Os, block_data_t, size_t>( _buff + PAYLOAD_SIZE_POS + _offset );
			for ( size_t i = 0 ; i < payload_size; i++ )
			{
				 payload_data[i] = _buff[PAYLOAD_DATA_POS + i + _offset];
			}
		}
		// --------------------------------------------------------------------
		size_t serial_size()
		{
			size_t PROTOCOL_ID_POS = 0;
			size_t PAYLOAD_SIZE_POS = PROTOCOL_ID_POS + sizeof(uint8_t);
			size_t PAYLOAD_DATA_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
			return PAYLOAD_DATA_POS + sizeof( block_data_t) * payload_size;
		}
		// --------------------------------------------------------------------
	private:
		uint8_t protocol_id;
		size_t payload_size;
		block_data_t payload_data[NB_MAX_PROTOCOL_PAYLOAD_SIZE];
	};
}
#endif
