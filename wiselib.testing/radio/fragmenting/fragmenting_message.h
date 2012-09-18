#ifndef __FRAGMENTING_MESSAGE_H__
#define	__FRAGMENTING_MESSAGE_H__

#include "fragmenting_radio_source_config.h"
#include "fragmenting_radio_default_values_config.h"
#include "fragment.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Debug_P>
	class FragmentingMessage_Type
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Debug_P Debug;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef Fragment_Type<Os, Radio, Debug> Fragment;
		typedef vector_static<Os, Fragment, FR_MAX_FRAGMENTS> Fragment_vector;
		typedef typename Fragment_vector::iterator Fragment_vector_iterator;
		typedef FragmentingMessage_Type<Os, Radio, Debug> self_t;
		// --------------------------------------------------------------------
		FragmentingMessage_Type() :
			id		( 0 )
		{};
		// --------------------------------------------------------------------
		~FragmentingMessage_Type()
		{};
		// --------------------------------------------------------------------
		self_t& operator=( const self_t& _fm )
		{
			id = _fm.id;
			fragmenting_message = _fm.fragmenting_message;
			return *this;
		}
		// --------------------------------------------------------------------
		uint16_t get_id()
		{
			return id;
		}
		// --------------------------------------------------------------------
		void set_id( uint16_t _id )
		{
			id = _id;
		}
		// --------------------------------------------------------------------
		Fragment_vector* get_fragmenting_message_ref()
		{
			return &fragmenting_message;
		}
		// --------------------------------------------------------------------
		void vectorize( block_data_t* _buff, size_t _len, size_t _mr_len, Debug& _debug, size_t _offset = 0 )
		{
			size_t number_of_fragments = _len / _mr_len;
			size_t last_fragment_bytes = _len % _mr_len;
			if ( last_fragment_bytes != 0 )
			{
				number_of_fragments = number_of_fragments + 1;
			}
			size_t i;
			_debug.debug("%d - %d", number_of_fragments, last_fragment_bytes );
			for ( i = 0; i < number_of_fragments -1; i++ )
			{
				Fragment f;
				f.set_id( id );
				f.set_seq_fragment( i );
				f.set_total_fragments( number_of_fragments );
				f.set_payload( _mr_len, _buff + _offset + ( i *_mr_len ) );
				fragmenting_message.push_back( f );
			}
			if ( last_fragment_bytes != 0 )
			{
				Fragment f;
				f.set_id( id );
				f.set_seq_fragment( i );
				f.set_total_fragments( number_of_fragments );
				f.set_payload( last_fragment_bytes, _buff + _offset + ( i * _mr_len ) );
				fragmenting_message.push_back( f );
			}
		}
		// --------------------------------------------------------------------
		block_data_t* de_vectorize( block_data_t* _buff, size_t _offset = 0 )
		{
			size_t i = 0;
			for ( Fragment_vector_iterator it = fragmenting_message.begin(); it != fragmenting_message.end(); ++it )
			{
				memcpy( _buff + _offset + i, it->get_payload(), it->get_payload_size() );
				i = it->get_payload_size() + i;
			}
			return _buff;
		}
		// --------------------------------------------------------------------
		size_t serial_size()
		{
			size_t i = 0;
			for ( Fragment_vector_iterator it = fragmenting_message.begin(); it != fragmenting_message.end(); ++it )
			{
				i = it->get_payload_size() + i;
			}
			return i;
		}
		// --------------------------------------------------------------------
#ifdef DEBUG_FRAGMENTING_MESSAGE_H
		void print( Debug& _debug, Radio& _radio )
		{
			_debug.debug( "-------------------------------------------------------\n");
			_debug.debug( "FragmentingMessage : \n" );
			for ( Fragment_vector_iterator i = fragmenting_message.begin(); i != fragmenting_message.end(); ++i )
			{
				i->print( _debug, _radio );
			}
			_debug.debug( "-------------------------------------------------------\n");
		}
#endif
		// --------------------------------------------------------------------
	private:
		uint16_t id;
		Fragment_vector fragmenting_message;
    };
}
#endif
