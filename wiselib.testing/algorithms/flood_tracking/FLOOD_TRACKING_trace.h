/***************************************************************************
** This file is part of the generic algorithm library Wiselib.           **
** Copyright (C) 2008,2009 by the Wisebed (www.wisebed.eu) project.      **
**									 									 **
** The Wiselib is free software: you can redistribute it and/or modify   **
** it under the terms of the GNU Lesser General Public License as        **
** published by the Free Software Foundation, either version 3 of the    **
** License, or (at your option) any later version.                       **
**                                                                       **
** The Wiselib is distributed in the hope that it will be useful,        **
** but WITHOUT ANY WARRANTY; without even the implied warranty of        **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
** GNU Lesser General Public License for more details.                   **
**                                                                       **
** You should have received a copy of the GNU Lesser General Public      **
** License along with the Wiselib.                                       **
** If not, see <http://www.gnu.org/licenses/>.                           **
***************************************************************************/
#ifndef __FLOOD_TRACKING_TRACE_H__
#define __FLOOD_TRACKING_TRACE_H__
namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename TimesNumber_P,
				typename Node_P,
				typename NodeID_P,
				typename Debug_P>
	class FLOOD_TRACKING_TraceType
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef TimesNumber_P TimesNumber;
		typedef Node_P Node;
		typedef NodeID_P NodeID;
		typedef Debug_P Debug;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::size_t size_t;
		typedef FLOOD_TRACKING_TraceType<Os, Radio, TimesNumber, Node, NodeID, Debug> self_type;

		inline FLOOD_TRACKING_TraceType():
		start_time ( 0 ),
		detection_LQI ( 0 )
		{}
		// --------------------------------------------------------------------
		inline FLOOD_TRACKING_TraceType( const self_type& _t)
		{
			*this = _t;
		}
		// --------------------------------------------------------------------
		inline FLOOD_TRACKING_TraceType( block_data_t* buff, size_t offset = 0 )
		{
			get_from_buffer( buff, offset );
		}
		// --------------------------------------------------------------------
		inline block_data_t* set_buffer_from( block_data_t* buff, size_t offset = 0 )
		{
			uint8_t CURRENT_NODE_POS = 0;
			uint8_t TARGET_ID_POS = current.get_buffer_size() + CURRENT_NODE_POS;
			uint8_t START_TIME_POS = sizeof( NodeID ) + TARGET_ID_POS;
			uint8_t DETECTION_LQI_POS = sizeof( TimesNumber) + START_TIME_POS;
			current.set_buffer_from( buff, CURRENT_NODE_POS + offset);
			write<Os, block_data_t, NodeID>( buff + TARGET_ID_POS + offset, target_id );
			write<Os, block_data_t, TimesNumber>( buff + START_TIME_POS + offset, start_time );
			write<Os, block_data_t, uint8_t>( buff + DETECTION_LQI_POS + offset, detection_LQI);
			return buff;
		}
		// --------------------------------------------------------------------
		inline void get_from_buffer(block_data_t* buff, size_t offset = 0)
		{
			uint8_t CURRENT_NODE_POS = 0;
			uint8_t TARGET_ID_POS = current.get_buffer_size() + CURRENT_NODE_POS;
			uint8_t START_TIME_POS = sizeof( NodeID ) + TARGET_ID_POS;
			uint8_t DETECTION_LQI_POS = sizeof( TimesNumber) + START_TIME_POS;
			current.get_from_buffer( buff, CURRENT_NODE_POS + offset);
			target_id = read<Os, block_data_t, NodeID>(buff + TARGET_ID_POS + offset );
			start_time = read<Os, block_data_t, TimesNumber>(buff + START_TIME_POS + offset );
			detection_LQI = read<Os, block_data_t, uint8_t>( buff + DETECTION_LQI_POS + offset);
		}
		// --------------------------------------------------------------------
		inline size_t get_buffer_size()
		{
			uint8_t CURRENT_NODE_POS = 0;
			uint8_t TARGET_ID_POS = current.get_buffer_size() + CURRENT_NODE_POS;
			uint8_t START_TIME_POS = sizeof( NodeID ) + TARGET_ID_POS;
			uint8_t DETECTION_LQI_POS = sizeof( TimesNumber) + START_TIME_POS;
			return DETECTION_LQI_POS + sizeof( uint8_t );
		}
		// --------------------------------------------------------------------
		inline self_type& operator=( const self_type& _t)
		{
			target_id = _t.target_id;
			current = _t.current;
			start_time = _t.start_time;
			detection_LQI = _t.detection_LQI;
			return *this;
		}
		// --------------------------------------------------------------------
		inline void update_start_time()	{ start_time = start_time + 1; }
		// --------------------------------------------------------------------
		inline NodeID get_target_id(){ return target_id; }
		// --------------------------------------------------------------------
		inline Node get_current(){ return current; }
		// --------------------------------------------------------------------
		inline TimesNumber get_start_time() { return start_time; }
		// --------------------------------------------------------------------
		inline void set_target_id( const NodeID& tarid ){ target_id = tarid; }
		// --------------------------------------------------------------------
		inline void set_start_time( const TimesNumber& t ) { start_time = t; }
		// --------------------------------------------------------------------
		inline void set_current( const Node& _c ){ current = _c; }
		// --------------------------------------------------------------------
		inline uint8_t get_detection_LQI()
		{
			return detection_LQI;
		}
		// --------------------------------------------------------------------
		inline void set_detection_LQI( uint8_t dt_lqi )
		{
			detection_LQI = dt_lqi;
		}
		inline void print( Debug& debug )
		{
			debug.debug(" Trace of: Target_id: %x, current_id %x [%i,%i], start_time %i, detection_LQI %i", target_id, current.get_id(), current.get_position().get_x(), current.get_position().get_y(), start_time, detection_LQI );
		}
		// --------------------------------------------------------------------
	private:
		NodeID target_id;
		Node current;
		TimesNumber start_time;
		uint8_t detection_LQI;
	};
}
#endif
