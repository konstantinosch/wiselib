/***************************************************************************
** This file is part of the generic algorithm library Wiselib.           **
** Copyright (C) 2008,2009 by the Wisebed (www.wisebed.eu) project.      **
**                                                                       **
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
#ifndef __PLTT_AGENT_H__
#define __PLTT_AGENT_H__
namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename AgentID_P,
				typename Node_P,
				typename IntensitiyNumber_P,
				typename Debug_P>
	class PLTT_AgentType
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef AgentID_P AgentID;
		typedef IntensitiyNumber_P IntensityNumber;
		typedef Debug_P Debug;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::node_id_t node_id_t;
		typedef PLTT_AgentType<Os, Radio, AgentID, Node, IntensityNumber, Clock, Debug> self_type;
		PLTT_AgentType()
		{}
		PLTT_AgentType( const AgentID& _aid, const node_id_t& _tid, const node_id_t& _trid, const IntensityNumber& _mi )
		{
			agent_id = _aid;
			target_id = _tid;
			tracker_id = _trid;
			max_intensity = _mi;
		}
		block_data_t* serialize( block_data_t* buff, size_t offset = 0 )
		{
			size_t AGENT_ID_POS = 0;
			size_t TARGET_ID_POS = AGENT_ID_POS + sizeof(AgentID);
			size_t TRACKER_ID_POS = TARGET_ID_POS + sizeof(node_id_t);
			size_t MAX_INTEN_POS = TRACKER_ID_POS + sizeof(node_id_t);
			write<Os, block_data_t, AgentID> (buff + AGENT_ID_POS + offset, agent_id);
			write<Os, block_data_t, node_id_t> (buff + TARGET_ID_POS + offset, target_id);
			write<Os, block_data_t, node_id_t> (buff + TRACKER_ID_POS + offset, tracker_id);
			write<Os, block_data_t, IntensityNumber> (buff + MAX_INTEN_POS + offset, max_intensity);
			return buff;
		}
		void de_serialize( block_data_t* buff, size_t offset = 0 )
		{
			size_t AGENT_ID_POS = 0;
			size_t TARGET_ID_POS = AGENT_ID_POS + sizeof(AgentID);
			size_t TRACKER_ID_POS = TARGET_ID_POS + sizeof(node_id_t);
			size_t MAX_INTEN_POS = TRACKER_ID_POS + sizeof(node_id_t);
			agent_id = read<Os, block_data_t, AgentID> (buff + AGENT_ID_POS + offset );
			target_id = read<Os, block_data_t, node_id_t> (buff + TARGET_ID_POS + offset );
			tracker_id = read<Os, block_data_t, node_id_t> (buff + TRACKER_ID_POS + offset );
			max_intensity = read<Os, block_data_t, IntensityNumber> (buff + MAX_INTEN_POS + offset );
		}
		size_t get_buffer_size()
		{
			size_t AGENT_ID_POS = 0;
			size_t TARGET_ID_POS = AGENT_ID_POS + sizeof(AgentID);
			size_t TRACKER_ID_POS = TARGET_ID_POS + sizeof(node_id_t);
			size_t MAX_INTEN_POS = TRACKER_ID_POS + sizeof(node_id_t);
			return MAX_INTEN_POS + sizeof( max_intensity );
		}
		inline self_type& operator=( const self_type& _a)
		{
			agent_id = _a.agent_id;
			target_id = _a.target_id;
			tracker_id = _a.tracker_id;
			max_intensity = _a.max_intensity;
			return *this;
		}
		inline AgentID get_agent_id()
		{
			return agent_id;
		}
		inline void set_agent_id( AgentID _aid )
		{
			agent_id = _aid;
		}
		inline node_id_t get_target_id()
		{
			return target_id;
		}
		inline void set_target_id( node_id_t _tid )
		{
			target_id = _tid;
		}
		inline node_id_t get_tracker_id()
		{
			return tracker_id;
		}
		inline void set_tracker_id( node_id_t _trid )
		{
			tracker_id = _trid;
		}
		inline void set_max_intensity( IntensityNumber _mi )
		{
			max_intensity = _mi;
		}
		inline IntensityNumber get_max_intensity()
		{
			return max_intensity;
		}
		void print( Debug& debug )
		{
			debug.debug( "agent_id (size %i), %x\n", sizeof( agent_id ), agent_id );
			debug.debug( "max_intensity (size %i), %i\n", sizeof( max_intensity ), max_intensity );
		}
	private:
		AgentID agent_id;
		node_id_t target_id;
		node_id_t tracker_id;
		IntensityNumber max_intensity;
	};
}
#endif


