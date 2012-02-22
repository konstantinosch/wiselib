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

#ifndef __PLTT_PASSIVE_H__
#define __PLTT_PASSIVE_H__
#include "PLTT_config_revision.h"

#include "PLTT_message.h"
#ifdef PLTT_SECURE
#include "../privacy/privacy_message.h"
#endif
namespace wiselib
{

template<typename Os_P, typename Node_P, typename PLTT_Node_P,
		typename PLTT_NodeList_P, typename PLTT_Trace_P,
		typename PLTT_TraceList_P,
#ifdef PLTT_SECURE
		typename PLTT_SecureTrace_P,
		typename PLTT_SecureTraceList_P,
#endif
		typename NeighborDiscovery_P, typename Timer_P, typename Radio_P,
		typename Rand_P, typename Clock_P, typename Debug_P>
class PLTT_PassiveType
{
public:
	typedef Os_P Os;
	typedef Radio_P Radio;
	typedef Rand_P Rand;
	typedef typename Rand::rand_t rand_t;
	typedef Debug_P Debug;
	typedef Node_P Node;
	typedef PLTT_Node_P PLTT_Node;
	typedef PLTT_NodeList_P PLTT_NodeList;
	typedef typename PLTT_NodeList::iterator PLTT_NodeListIterator;
	typedef PLTT_Trace_P PLTT_Trace;
	typedef PLTT_TraceList_P PLTT_TraceList;
	typedef NeighborDiscovery_P NeighborDiscovery;
	typedef typename PLTT_TraceList::iterator PLTT_TraceListIterator;
#ifdef PLTT_SECURE
	typedef PLTT_SecureTrace_P PLTT_SecureTrace;
	typedef PLTT_SecureTraceList_P PLTT_SecureTraceList;
	typedef typename PLTT_SecureTraceList::iterator PLTT_SecureTraceListIterator;
#endif
	typedef typename Node::Position Position;
	typedef typename Node::Position::CoordinatesNumber CoordinatesNumber;
	typedef typename PLTT_Node::PLTT_NodeTarget PLTT_NodeTarget;
	typedef typename PLTT_NodeTarget::IntensityNumber IntensityNumber;
	typedef typename PLTT_Node::PLTT_NodeTargetList PLTT_NodeTargetList;
	typedef typename PLTT_Node::PLTT_NodeTargetListIterator PLTT_NodeTargetListIterator;
	typedef Timer_P Timer;
	typedef Clock_P Clock;
	typedef typename Radio::node_id_t node_id_t;
	typedef typename Radio::size_t size_t;
	typedef typename Radio::block_data_t block_data_t;
	typedef typename Radio::message_id_t message_id_t;
	typedef typename Radio::ExtendedData ExtendedData;
	typedef typename Timer::millis_t millis_t;
	typedef typename Radio::TxPower TxPower;
	typedef typename Clock::time_t time_t;
	typedef wiselib::vector_static<Os, Node, 100> NodeList;
	typedef typename NodeList::iterator NodeList_Iterator;
	typedef PLTT_MessageType<Os, Radio> Message;
#ifdef PLTT_SECURE
	typedef PLTT_PassiveType<Os, Node, PLTT_Node, PLTT_NodeList, PLTT_Trace, PLTT_TraceList, PLTT_SecureTrace, PLTT_SecureTraceList, NeighborDiscovery, Timer, Radio, Rand, Clock, Debug> self_type;
	typedef PrivacyMessageType<Os, Radio> PrivacyMessage;
#else
	typedef PLTT_PassiveType<Os, Node, PLTT_Node, PLTT_NodeList, PLTT_Trace, PLTT_TraceList, NeighborDiscovery, Timer, Radio, Rand, Clock, Debug> self_type;
#endif
	typedef typename NeighborDiscovery::ProtocolSettings ProtocolSettings;
	typedef typename NeighborDiscovery::Neighbor Neighbor;
	typedef typename NeighborDiscovery::ProtocolPayload ProtocolPayload;
	typedef typename NeighborDiscovery::Protocol Protocol;
	typedef typename NeighborDiscovery::Beacon Beacon;
	typedef typename NeighborDiscovery::Neighbor_vector Neighbor_vector;
	typedef typename NeighborDiscovery::Neighbor_vector_iterator Neighbor_vector_iterator;
	typedef typename NeighborDiscovery::ProtocolPayload_vector ProtocolPayload_vector;
	typedef typename NeighborDiscovery::ProtocolPayload_vector_iterator ProtocolPayload_vector_iterator;
	// -----------------------------------------------------------------------
	PLTT_PassiveType() :
		radio_callback_id_(0), seconds_counter(1)
	{
	}
	// -----------------------------------------------------------------------
	~PLTT_PassiveType()
	{
	}
	// -----------------------------------------------------------------------
	void enable(void)
	{
//#ifdef PLTT_PASSIVE_DEBUG_MISC
		debug().debug( "PLTT_Passive %x: Boot \n", self.get_node().get_id() );
//#endif
		radio().enable_radio();
		TxPower power;
		power.set_dB( transmission_power_dB);
		radio().set_power( power );
		//millis_t r = rand()() % random_enable_timer_range;
		timer().template set_timer<self_type, &self_type::neighbor_discovery_enable_task> (1, this, 0);
	}
	// -----------------------------------------------------------------------
	void neighbor_discovery_enable_task(void* userdata = NULL)
	{
//#ifdef PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY
		debug().debug( "PLTT_Passive %x: Neighbor discovery enable task \n", self.get_node().get_id() );
//#endif
		//block_data_t buff[100];
		//ProtocolPayload pp( NeighborDiscovery::TRACKING_PROTOCOL_ID, self.get_node().get_position().get_buffer_size(), self.get_node().get_position().set_buffer_from( buff ) );
		//uint8_t ef = ProtocolSettings::NEW_PAYLOAD|ProtocolSettings::LOST_NB|ProtocolSettings::NEIGHBOR_REMOVED|ProtocolSettings::NEW_PAYLOAD;
		//ProtocolSettings ps( 255, 0, 255, 0, 100, 75, 100, 75, ef, -6, 100, 3000, 100, ProtocolSettings::RATIO_DIVIDER, 2, ProtocolSettings::MEAN_DEAD_TIME_PERIOD, 100, 100, ProtocolSettings::R_NR_WEIGHTED, 10, 10, pp );
		//uint8_t result = 0;
		//result = neighbor_discovery(). template register_protocol<self_type, &self_type::sync_neighbors>( NeighborDiscovery::TRACKING_PROTOCOL_ID, ps, this  );
		//Protocol* prot_ref = neighbor_discovery().get_protocol_ref( NeighborDiscovery::TRACKING_PROTOCOL_ID );
		//if ( prot_ref != NULL )
		//{
		//	neighbor_discovery().enable();
			nb_convergence_time = 1;
			timer().template set_timer<self_type, &self_type::neighbor_discovery_unregister_task> ( nb_convergence_time, this, 0 );
		//}
	}
	// -----------------------------------------------------------------------
	void neighbor_discovery_unregister_task( void* userdata = NULL )
	{
//#ifdef PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY
		debug().debug( "PLTT_Passive %x: Neighbor discovery unregister task \n", self.get_node().get_id() );
//#endif
		//Protocol* prot_ref = NULL;
		//neighbor_discovery().disable();
		radio_callback_id_ = radio().template reg_recv_callback<self_type, &self_type::receive> (this);
		//update_traces();
//#ifdef PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY
			//debug().debug( "PLTT_Passive %x (%i, %i): NB READY! - Neighbor discovery unregister - size of neighbor list %i vs nb size %i ", self.get_node().get_id(), self.get_node().get_position().get_x(), self.get_node().get_position().get_y(), neighbors.size(), neighbor_discovery().neighborhood.size() );
			//print_neighbors();
//#endif
#ifdef PLTT_SECURE
		decryption_request_daemon();
#endif
#ifdef CONFIG_PROACTIVE_INHIBITION
		proactive_inhibition_daemon();
#endif
	}
	// -----------------------------------------------------------------------
	void disable(void)
	{
//#ifdef PLTT_PASSIVE_DEBUG_MISC
		debug().debug( "PLTT_Passive %x: Disable \n", self.get_node().id );
//#endif
		radio().unreg_recv_callback( radio_callback_id_ );
		radio().disable();
	}
	// -----------------------------------------------------------------------
	void send(node_id_t destination, size_t len, block_data_t *data, message_id_t msg_id)
	{
		Message message;
		message.set_msg_id(msg_id);
		message.set_payload(len, data);
		radio().send( destination, message.buffer_size(), (uint8_t*) &message );
	}
	// -----------------------------------------------------------------------
	void receive( node_id_t from, size_t len, block_data_t *data, const ExtendedData& exdata )
	{
		debug().debug("Reception! %d\n", len);

		message_id_t msg_id = *data;
		Message *message = (Message*) data;
#ifndef PLTT_SECURE
		if ( msg_id == PLTT_SPREAD_ID )
		{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Received spread message from %x of link metric %i and size %i \n", self.get_node().get_id(), from, exdata.link_metric(), len );
#endif
			PLTT_Trace trace = PLTT_Trace( message->payload() );
			if ( ( trace.get_recipient_1_id() == self.get_node().get_id() ) || ( trace.get_recipient_2_id() == self.get_node().get_id() ) || (  ( trace.get_recipient_1_id() == 0 ) && (  trace.get_recipient_2_id() == 0 ) ) )
			{
				prepare_spread_trace( store_inhibit_trace( trace ) , exdata );
			}
			else if ( ( trace.get_recipient_1_id() != self.get_node().get_id() ) && ( trace.get_recipient_2_id() != self.get_node().get_id() ) && ( trace.get_recipient_1_id() != 0 ) && ( trace.get_recipient_2_id() != 0 ) )
			{
				store_inhibit_trace( trace, 1 );
			}
		}
#else
		if ( msg_id == PLTT_SECURE_SPREAD_ID )
		{
#ifdef PLTT_PASSIVE_DEBUG_SECURE
			debug().debug( "PLTT_Passive %x: Received encrypted trace from unknown target of size %i \n", self.get_node().get_id(), message->payload_size() );
#endif
			PLTT_SecureTrace secure_trace = PLTT_SecureTrace( message->payload() );
			uint16_t request_id = rand()() % 0xffff;
			secure_trace.set_request_id( request_id );
			if ( ( secure_trace.get_recipient_1_id() == self.get_node().get_id() ) || ( secure_trace.get_recipient_2_id() == self.get_node().get_id() ) || ( secure_trace.get_intensity() == secure_trace.get_max_intensity() ) )
			{
//#ifdef PLTT_PASSIVE_DEBUG_SECURE
				debug().debug( "PLTT_Passive %x: Received encrypted trace from unknown target %x of size %i - Encrypted trace is detection or direct spread - inhibition: 0\n", self.get_node().get_id(), from, message->payload_size(), secure_trace.get_intensity() );
//#endif
				PLTT_SecureTrace* secure_trace_ptr = store_inhibit_secure_trace( secure_trace );
				if ( secure_trace_ptr != NULL )
				{
//#ifdef PLTT_PASSIVE_DEBUG_SECURE
				debug().debug( "PLTT_Passive %x: Received encrypted trace from unknown target of size %i - Encrypted trace is valid for propagation and decryption\n", self.get_node().get_id(), message->payload_size() );
//#endif
					prepare_spread_secure_trace( secure_trace_ptr, exdata );
					PrivacyMessage pm;
					pm.set_request_id( secure_trace.get_request_id() );
					pm.set_payload( secure_trace.get_target_id_size(), secure_trace.get_target_id() );
					pm.set_msg_id( PRIVACY_DECRYPTION_REQUEST_ID );
					radio().send( Radio::BROADCAST_ADDRESS, pm.buffer_size(), pm.buffer() );
				}
			}
			else
			{
//#ifdef PLTT_PASSIVE_DEBUG_SECURE
				debug().debug( "PLTT_Passive %x: Received encrypted trace from unknown target of size %i - Encrypted trace is indirect spread - inhibition: 1\n", self.get_node().get_id(), message->payload_size() );
//#endif
				store_inhibit_secure_trace( secure_trace, 1 );
			}
		}
		else if ( msg_id == PRIVACY_DECRYPTION_REPLY_ID )
		{
			PrivacyMessage* pm = ( PrivacyMessage* ) data;
//#ifdef PLTT_PASSIVE_DEBUG_SECURE
			debug().debug( "PLTT_Passive %x: Received decryption reply from helper %x of size %i\n", self.get_node().get_id(), from, pm->buffer_size() );
//#endif
			for ( PLTT_SecureTraceListIterator i = secure_traces.begin(); i != secure_traces.end(); ++i )
			{
				if ( ( pm->request_id() == i->get_request_id() ) && ( i->get_decryption_retries() < decryption_max_retries ) )
				{
					node_id_t id = read<Os, block_data_t, node_id_t>( pm->payload() );
					PLTT_Trace t;
					t.set_target_id( id );
//#ifdef PLTT_PASSIVE_DEBUG_SECURE
					debug().debug( "PLTT_Passive %x: Received decryption reply from helper %x\n", self.get_node().get_id(), from );
//#endif
					t.set_start_time( i->get_start_time() );
					t.set_inhibited( i->get_inhibited() );
					t.set_diminish_seconds( i->get_diminish_seconds() );
					t.set_diminish_amount( i->get_diminish_amount() );
					t.set_spread_penalty( i->get_spread_penalty() );
					t.set_intensity( i->get_intensity() );
					t.set_current( i->get_current() );
					t.set_parent( i->get_parent() );
					t.set_grandparent( i->get_grandparent() );
					t.set_recipient_1_id( i->get_recipient_1_id() );
					t.set_recipient_2_id( i->get_recipient_2_id() );
					store_inhibit_trace( t );
					i->set_decryption_retries( decryption_max_retries );
					if ( ( i->get_decryption_retries() >= decryption_max_retries ) && ( i->get_inhibited() !=0 ) )
					{
						secure_traces.erase( i );
					}
					return;
				}
			}
		}
#endif
#ifdef CONFIG_PROACTIVE_INHIBITION
		else if (msg_id == PLTT_INHIBITION_MESSAGE_ID)
		{
#ifdef PLTT_PASSIVE_DEBUG_INHIBITION
			debug().debug( "PLTT_Passive %x: Received inhibition message from %x of size %i and payload size of %i \n", self.get_node().get_id(), from, len, message->payload_size() );
#endif
			inhibit_traces( PLTT_Node( message->payload() ) );
		}
#endif
	}
	// -----------------------------------------------------------------------
	void sync_neighbors( uint8_t event, node_id_t from, size_t len, uint8_t* data )
	{
#ifdef PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY
		debug().debug( "PLTT_Passive %x: Sync neighbors\n", self.get_node().get_id() );
#endif
		if ( event & ProtocolSettings::NEW_PAYLOAD )
		{
			PLTT_NodeListIterator i = neighbors.begin();
			while ( i != neighbors.end() )
			{
				if ( i->get_node().get_id() == from )
				{
					Position p;
					p.get_from_buffer( data );
					i->get_node().set_position( p );
					return;
				}
				++i;
			}
			Position p;
			p.get_from_buffer( data );
			Node n = Node( from , p );
			neighbors.push_back( PLTT_Node( n ) );
		}
		else if ( event & ProtocolSettings::LOST_NB )
		{
			PLTT_NodeListIterator i = neighbors.begin();
			while ( i != neighbors.end() )
			{
				if ( i->get_node().get_id() == from )
				{
#ifdef PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY
					debug().debug( "PLTT_Passive %x: Erased neighbor %x due to protocol settings requirements", self.get_node().get_id(), from );
#endif
					neighbors.erase( i );
					return;
				}
				++i;
			}
		}
		else if ( event & ProtocolSettings::NEIGHBOR_REMOVED )
		{
			PLTT_NodeListIterator i = neighbors.begin();
			while ( i != neighbors.end() )
			{
				if ( i->get_node().get_id() == from )
				{
#ifdef PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY
					debug().debug( "PLTT_Passive %x: Erased neighbor %x due to memory limitations", self.get_node().get_id(), from );
#endif
					neighbors.erase( i );
					return;
				}
				++i;
			}
		}
	}
	// -----------------------------------------------------------------------
	PLTT_Trace* store_inhibit_trace( PLTT_Trace trace, uint8_t inhibition_flag = 0 )
	{
//#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Store inhibit trace\n", self.get_node().get_id() );
//#endif
		PLTT_TraceListIterator traces_iterator = traces.begin();
		while ( traces_iterator != traces.end())
		{
			if ( traces_iterator->get_target_id() == trace.get_target_id() )
			{
//#ifdef PLTT_PASSIVE_DEBUG_SPREAD
				debug().debug( "PLTT_Passive %x: Store inhibit trace - new trace intensity and start time (%i, %i ) vs current (%i, %i, %i) ", self.get_node().get_id(), trace.get_intensity(), trace.get_start_time(), traces_iterator->get_intensity(), traces_iterator->get_start_time(), traces_iterator->get_inhibited() );
//#endif
				if ( ( (trace.get_start_time() == traces_iterator->get_start_time() ) && (traces_iterator->get_intensity() < trace.get_intensity() ) ) ||
					 ( trace.get_start_time() > traces_iterator->get_start_time() ) )
				{
					*traces_iterator = trace;
					traces_iterator->update_path(self.get_node());
					if ( inhibition_flag )
					{
						traces_iterator->set_inhibited();
					}
					return &(*traces_iterator);
				}
				else
				{
					return NULL;
				}
			}
			++traces_iterator;
		}
		trace.update_path( self.get_node() );
		traces.push_back( trace );
		if ( inhibition_flag )
		{
			traces_iterator->set_inhibited();
		}
		traces_iterator = traces.end() - 1;
		return &(*traces_iterator);
	}
	// -----------------------------------------------------------------------
	void update_traces(void* userdata = NULL)
	{
#ifdef PLTT_PASSIVE_DEBUG_MISC
		debug().debug( "PLTT_Passive %x: Update Traces : tracelist size: %i ", self.get_node().get_id(), traces.size() );
#endif
		for ( PLTT_TraceListIterator traces_iterator = traces.begin(); traces_iterator != traces.end(); ++traces_iterator )
		{
			if ( ( seconds_counter % traces_iterator->get_diminish_seconds() == 0 ) && ( traces_iterator->get_inhibited() != 0 ) )
			{
				traces_iterator->update_intensity_diminish();
				if (traces_iterator->get_intensity() == 0)
				{
					traces_iterator->set_inhibited();
				}
			}
		}
		seconds_counter++;
		timer().template set_timer<self_type, &self_type::update_traces> ( 1000, this, 0 );
	}
	// -----------------------------------------------------------------------
	void print_traces(void* userdata = NULL)
	{
		debug().debug( "PLTT_Passive %x: Traces start print-out\n", self.get_node().id );
		for ( PLTT_TraceListIterator traces_iterator = traces.begin(); traces_iterator != traces.end(); ++traces_iterator )
		{
			traces_iterator->print_trace( debug() );
			debug().debug("-----------------------------\n");
		}
		debug().debug( "PLTT_Passive %x: Traces end print-out \n", self.get_node().get_id() );
		timer().template set_timer<self_type, &self_type::print_traces> ( 11000, this, 0 );
	}
	// -----------------------------------------------------------------------
#ifndef PLTT_SECURE
	void prepare_spread_trace( PLTT_Trace* t, const ExtendedData& exdata )
	{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Prepare Spread Trace\n", self.get_node().get_id() );
#endif
		if ( ( t != NULL ) && ( (*t).get_inhibited() == 0 ) )
		{
			NodeList recipient_candidates;
			Node rep_point = (*t).get_repulsion_point();
			for (PLTT_NodeListIterator neighbors_iterator = neighbors.begin(); neighbors_iterator != neighbors.end(); ++neighbors_iterator )
			{
				if ( rep_point.get_id() != 0 )
				{
					if ( rep_point.get_position().distsq( self.get_node().get_position() ) <= rep_point.get_position().distsq( neighbors_iterator->get_node().get_position() ) )
					{
						recipient_candidates.push_back( neighbors_iterator->get_node() );
					}
				}
			}
			millis_t r = 0;
			if ( !recipient_candidates.size() )
			{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
				debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace - Exited due to 0 element candidate list\n", self.get_node().get_id() );
#endif
				t->set_inhibited();
				return;
			}
#ifdef CONFIG_BACKOFF_CANDIDATE_LIST_WEIGHT
			else if ( recipient_candidates.size() == 1 )
			{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
				debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace - Candidate list of size 1 - Imposing 1000ms delay\n", self.get_node().get_id() );
#endif
				r = r + backoff_candidate_list_weight;
			}
#endif
#ifdef CONFIG_BACKOFF_RANDOM_WEIGHT
			if ( backoff_random_weight )
			{
				r = rand()() % backoff_random_weight + r;
			}
#endif
#ifdef CONFIG_BACKOFF_LQI_WEIGHT
			if ( exdata.link_metric() )
			{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
				debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace - Has lqi of %i\n", self.get_node().get_id(), exdata.link_metric() );
#endif
				r = backoff_lqi_weight * exdata.link_metric() + r;
			}
#endif
#ifdef CONFIG_BACKOFF_CONNECTIVITY_WEIGHT
			if ( neighbors.size() )
			{
				r = backoff_connectivity_weight / neighbors.size() + r;
			}
#endif
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Prepare Spread - Scheduled inhibition and spread in %i millis \n", self.get_node().get_id(), r );
#endif

#ifdef PLTT_PROACTIVE_INHIBITION
			timer().template set_timer<self_type, &self_type::send_inhibition> ( r, this, ( void* )t );
			r = r + 100;
#else
			timer().template set_timer<self_type, &self_type::spread_trace> (r, this, (void*) t );
#endif
		}
		else
		{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Prepared Spread - Exited due to ignore from store or inhibition\n", self.get_node().get_id() );
#endif
		}
	}
	// -----------------------------------------------------------------------
	void spread_trace(void* userdata)
	{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Spread Trace\n", self.get_node().get_id() );
#endif
		PLTT_Trace* t = (PLTT_Trace*) userdata;
		if ( (*t).get_inhibited() == 0 )
		{
			NodeList recipient_candidates;
			NodeList_Iterator recipient_candidates_iterator;
#ifndef CONFIG_SPREAD_RANDOM_RECEIVERS
			NodeList_Iterator recipient_candidates_iterator_buff;
			CoordinatesNumber d = 0;
			uint8_t found = 0;
#endif
			Node rep_point = (*t).get_repulsion_point();
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Spread Trace - Neighbor list of size %i \n", self.get_node().get_id(), neighbors.size() );
#endif
			for (PLTT_NodeListIterator neighbors_iterator = neighbors.begin(); neighbors_iterator != neighbors.end(); ++neighbors_iterator )
			{
				if (rep_point.get_id() != 0)
				{
					if (rep_point.get_position().distsq( self.get_node().get_position() ) <= rep_point.get_position().distsq( neighbors_iterator->get_node().get_position() ) )
					{
						recipient_candidates.push_back(	neighbors_iterator->get_node() );
					}
				}
			}
			uint8_t send_flag = 0;
#ifdef CONFIG_SPREAD_RANDOM_RECEIVERS
			if (recipient_candidates.size() != 0)
			{
				(*t).update_intensity_penalize();
				rand_t rand_elem = rand()() % recipient_candidates.size();
				(*t).set_recipient_1_id( recipient_candidates.at( rand_elem ).get_id() );
				recipient_candidates.erase(recipient_candidates.begin() + rand_elem );
				send_flag = 1;
			}
#else
			for ( recipient_candidates_iterator = recipient_candidates.begin(); recipient_candidates_iterator != recipient_candidates.end(); ++recipient_candidates_iterator )
			{
				CoordinatesNumber cand_d = rep_point.get_position().distsq( recipient_candidates_iterator->get_position() );
				if (cand_d > d)
				{
					d = cand_d;
					recipient_candidates_iterator_buff = recipient_candidates_iterator;
					found = 1;
				}
			}
			if ( found == 1 )
			{
				(*t).update_intensity_penalize();
				(*t).set_recipient_1_id( recipient_candidates_iterator_buff->get_id() );
				recipient_candidates.erase( recipient_candidates_iterator_buff );
				send_flag = 1;
			}
#endif
			if (recipient_candidates.size() != 0)
			{
				(*t).set_recipient_2_id( recipient_candidates.at( rand()() % recipient_candidates.size() ).get_id() );
				send_flag = 1;
			}
			if ( send_flag ==1 )
			{
				size_t len = (*t).get_buffer_size();
				block_data_t buf[Radio::MAX_MESSAGE_LENGTH];
				block_data_t* buff = buf;
				buff = (*t).set_buffer_from(buff);
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
				debug().debug( "PLTT_Passive %x: Spread Trace - Trace was spread", self.get_node().get_id() );
#endif
				send( Radio::BROADCAST_ADDRESS, len, (uint8_t*) buff, PLTT_SPREAD_ID );
			}
			(*t).set_inhibited();
		}
		else
		{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Spread Trace - Exited due to inhibition\n", self.get_node().get_id() );
#endif
		}
	}
#endif
	// -----------------------------------------------------------------------
#ifdef PLTT_SECURE
	PLTT_SecureTrace* store_inhibit_secure_trace( PLTT_SecureTrace secure_trace, uint8_t inhibition_flag = 0 )
	{
//#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Store inhibit secure trace\n", self.get_node().get_id() );
//#endif
		PLTT_SecureTraceListIterator secure_traces_iterator = secure_traces.begin();
		while ( secure_traces_iterator != secure_traces.end() )
		{
			if ( secure_traces_iterator->compare_target_id( secure_trace.get_target_id() ) )
			{
				if ( ( secure_trace.get_start_time() == secure_traces_iterator->get_start_time() ) && ( secure_traces_iterator->get_intensity() < secure_trace.get_intensity() ) )
				{
					*secure_traces_iterator = secure_trace;
					secure_traces_iterator->update_path(self.get_node() );
					if (inhibition_flag)
					{
						secure_traces_iterator->set_inhibited();
					}
//#ifdef PLTT_PASSIVE_DEBUG_SPREAD
					debug().debug( "PLTT_Passive %x: Store inhibit secure trace - Secure Trace updated\n", self.get_node().get_id() );
//#endif
					return &(*secure_traces_iterator);
				}
				else
				{
//#ifdef PLTT_PASSIVE_DEBUG_SPREAD
					debug().debug( "PLTT_Passive %x: Store inhibit secure trace - Secure Trace not updated\n", self.get_node().get_id() );
//#endif
					return NULL;

				}
			}
			++secure_traces_iterator;
		}

		secure_trace.update_path( self.get_node() );
		secure_traces.push_back( secure_trace );
		secure_traces_iterator = secure_traces.end() - 1;
//#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Store inhibit secure trace - Secure Trace stored with secure traces list size %d\n", self.get_node().get_id(), secure_traces.size() );
//#endif
		if (inhibition_flag)
		{
			secure_traces_iterator->set_inhibited();
		}
		return &( *secure_traces_iterator );
	}
	// -----------------------------------------------------------------------
	void prepare_spread_secure_trace( PLTT_SecureTrace* t, const ExtendedData& exdata )
	{
		if ( ( t != NULL ) && ( (*t).get_inhibited() == 0 ) )
		{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace\n", self.get_node().get_id() );
#endif
			NodeList recipient_candidates;
			Node rep_point = (*t).get_repulsion_point();
			for ( PLTT_NodeListIterator neighbors_iterator = neighbors.begin(); neighbors_iterator != neighbors.end(); ++neighbors_iterator )
			{
				if (rep_point.get_id() != 0)
				{
					if ( rep_point.get_position().distsq( self.get_node().get_position() ) <= rep_point.get_position().distsq( neighbors_iterator->get_node().get_position() ) )
					{
						recipient_candidates.push_back(	neighbors_iterator->get_node() );
					}
				}
			}
			millis_t r = 0;
			if ( !recipient_candidates.size() )
			{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace - Exited due to 0 element candidate list\n", self.get_node().get_id() );
#endif
				t->set_inhibited();
				return;
			}
#ifdef CONFIG_BACKOFF_RANDOM_WEIGHT
			if ( backoff_random_weight )
			{
				r = rand()() % backoff_random_weight + r;
			}
#endif
#ifdef CONFIG_BACKOFF_LQI_WEIGHT
			if ( exdata.link_metric() )
			{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace - Has lqi of %i\n", self.get_node().get_id(), exdata.link_metric() );
#endif
				r = backoff_lqi_weight / exdata.link_metric() + r;
			}
#endif
#ifdef CONFIG_BACKOFF_CONNECTIVITY_WEIGHT
			if ( neighbors.size() )
			{
				r = backoff_connectivity_weight / neighbors.size() + r;
			}
#endif
#ifdef CONFIG_BACKOFF_CANDIDATE_LIST_WEIGHT
			else if ( recipient_candidates.size() == 1 )
			{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace - Candidate list of size 1 - Imposing 1000ms delay\n", self.get_node().get_id() );
#endif
				r = r + backoff_candidate_list_weight;
			}
#endif
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Prepare Spread Secure Trace - Scheduled inhibition and spread in %i millis \n", self.get_node().get_id(), r );
#endif
			timer().template set_timer<self_type, &self_type::spread_secure_trace> ( r, this, (void*) t );
		}
	}
	// -----------------------------------------------------------------------
	void spread_secure_trace( void* userdata )
	{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
		debug().debug( "PLTT_Passive %x: Spread Trace\n", self.get_node().get_id() );
#endif
		PLTT_SecureTrace* t = (PLTT_SecureTrace*) userdata;
		if ( (*t).get_inhibited() == 0 )
		{
			NodeList recipient_candidates;
			NodeList_Iterator recipient_candidates_iterator;
#ifndef CONFIG_SPREAD_RANDOM_RECEIVERS
			NodeList_Iterator recipient_candidates_iterator_buff;
			CoordinatesNumber d = 0;
			uint8_t found = 0;
#endif
			Node rep_point = (*t).get_repulsion_point();
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Spread Trace - Neighbor list of size %i \n", self.get_node().get_id(), neighbors.size() );
#endif
			for (PLTT_NodeListIterator neighbors_iterator = neighbors.begin(); neighbors_iterator != neighbors.end(); ++neighbors_iterator)
			{
				if (rep_point.get_id() != 0)
				{
					if (rep_point.get_position().distsq( self.get_node().get_position() ) <= rep_point.get_position().distsq( neighbors_iterator->get_node().get_position() ) )
					{
						recipient_candidates.push_back(	neighbors_iterator->get_node() );
					}
				}
			}
			uint8_t send_flag = 0;
#ifdef CONFIG_SPREAD_RANDOM_RECEIVERS
			if (recipient_candidates.size() != 0)
			{
				(*t).update_intensity_penalize();
				rand_t rand_elem = rand()() % recipient_candidates.size();
				node_id_t rand_id = recipient_candidates.at(rand_elem).get_id();
				(*t).set_recipient_1_id(rand_id);
				recipient_candidates.erase(recipient_candidates.begin()	+ rand_elem );
				send_flag = 1;
			}
#else
			for ( recipient_candidates_iterator = recipient_candidates.begin(); recipient_candidates_iterator != recipient_candidates.end(); ++recipient_candidates_iterator )
			{
				CoordinatesNumber cand_d = rep_point.get_position().distsq( recipient_candidates_iterator->get_position() );
				if (cand_d > d)
				{
					d = cand_d;
					recipient_candidates_iterator_buff = recipient_candidates_iterator;
					found = 1;
				}
			}
			if ( found == 1 )
			{
				( *t ).update_intensity_penalize();
				( *t ).set_recipient_1_id( recipient_candidates_iterator_buff->get_id() );
				recipient_candidates.erase( recipient_candidates_iterator_buff );
				send_flag = 1;
			}
#endif
			if (recipient_candidates.size() != 0)
			{
				(*t).set_recipient_2_id(recipient_candidates.at(rand()() % recipient_candidates.size()).get_id() );
				send_flag = 1;
			}
			if ( send_flag == 1 )
			{
				size_t len = (*t).get_buffer_size();
				block_data_t buf[Radio::MAX_MESSAGE_LENGTH];
				block_data_t* buff = buf;
				buff = (*t).set_buffer_from(buff);
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
				debug().debug( "PLTT_Passive %x: Spread Trace - Trace was spread\n", self.get_node().get_id() );
#endif
				send( Radio::BROADCAST_ADDRESS, len, (uint8_t*) buff, PLTT_SPREAD_ID );
			}
			erase_secure_trace( *t );
		}
		else
		{
#ifdef PLTT_PASSIVE_DEBUG_SPREAD
			debug().debug( "PLTT_Passive %x: Spread Trace - Exited due to inhibition\n", self.get_node().get_id() );
#endif
		}
	}
	// -----------------------------------------------------------------------
	void decryption_request_daemon( void* userdata = NULL )
	{
#ifdef PLTT_PASSIVE_DEBUG_SECURE
		debug().debug( "PLTT_Passive %x: decryption request daemon with secure_trace list of size: %i\n", self.get_node().get_id(), secure_traces.size() );
#endif
		PLTT_SecureTraceListIterator i = secure_traces.begin();
		while ( i != secure_traces.end() )
		{
			if ( ( i->get_decryption_retries() >= decryption_max_retries ) && ( i->get_inhibited() !=0 ) )
			{
				secure_traces.erase( i );
			}
			else if ( i->get_decryption_retries() < decryption_max_retries )
			{
				PrivacyMessage pm;
				pm.set_request_id( i->get_request_id() );
				pm.set_payload( i->get_target_id_size(), i->get_target_id() );
				pm.set_msg_id( PRIVACY_DECRYPTION_REQUEST_ID );
//#ifdef PLTT_PASSIVE_DEBUG_SECURE
				debug().debug( "PLTT_Passive %x: Encryption request daemon - sending request with id: %x\n", self.get_node().get_id(), i->get_request_id() );
				i->print( debug() );
				debug().debug(" PLTT_Passive %x: Encryption request daemon - buffer size of : %i vs %i\n",self.get_node().get_id(), pm.payload_size(), i->get_target_id_size() );
//#endif
				i->set_decryption_retries();
				radio().send( Radio::BROADCAST_ADDRESS, pm.buffer_size(), pm.buffer() );
				++i;
			}
		}
		timer().template set_timer<self_type, &self_type::decryption_request_daemon>( decryption_request_timer, this, 0 );
	}
	// -----------------------------------------------------------------------
	void erase_secure_trace( PLTT_SecureTrace st )
	{
#ifdef PLTT_PASSIVE_DEBUG_MISC
		debug().debug( "PLTT_Passive %x: Erase secure trace\n", self.get_node().get_id() );
#endif
		for ( PLTT_SecureTraceListIterator i = secure_traces.begin(); i != secure_traces.end(); ++i )
		{
#ifdef PLTT_PASSIVE_DEBUG_MISC
			debug().debug( "PLTT_Passive %x: Erase secure trace - trace of %x with [c : %x] [p: %x] [g %x]\n", self.get_node().get_id(), i->get_target_id(), i->get_current().get_id(), i->get_parent().get_id(), i->get_grandparent().get_id() );
#endif
			if ( ( i->compare_target_id( st.get_target_id() ) ) && ( i->get_decryption_retries() >= decryption_max_retries ) && ( i->get_inhibited() !=0 ) )
			{
				secure_traces.erase( i );
				return;
			}
		}
		return;
	}
#endif
	// -----------------------------------------------------------------------
#ifdef CONFIG_PROACTIVE_INHIBITION
	void inhibit_traces( PLTT_Node n )
	{
#ifdef PLTT_PASSIVE_DEBUG_INHIBITION
		debug().debug( "PLTT_Passive %x: Inhibit traces\n", self.get_node().get_id() );
#endif
		for ( PLTT_TraceListIterator i = traces.begin(); i != traces.end(); ++i )
		{
			for ( PLTT_NodeTargetListIterator j = n.get_node_target_list()->begin(); j != n.get_node_target_list()->end(); ++j )
			{
#ifdef PLTT_PASSIVE_DEBUG_INHIBITION
				debug().debug(" PLTT_Passive %x: Inhbit traces - Has trace of %i intensity vs %i \n", self.get_node().get_id(), i->get_intensity(), j->get_intensity() );
#endif
				if ( ( i->get_inhibited() == 0 ) && ( j->get_target_id() == i->get_target_id() && ( j->get_intensity() >=  i->get_intensity() ) ) )
				{
#ifdef PLTT_PASSIVE_DEBUG_INHIBITION
					debug().debug(" PLTT_Passive %x: Inhibit traces - Has trace of %i inhibited\n", self.get_node().get_id(), i->get_target_id() );
#endif
					i->set_inhibited();
					i->set_parent( n.get_node() );
					i->set_grandparent( n.get_node() );
				}
			}
		}
	}
	// -----------------------------------------------------------------------
	void proactive_inhibition_daemon( void* userdata = NULL )
	{
#ifdef PLTT_PASSIVE_DEBUG_PROACTIVE_INHIBITION
		debug().debug( "PLTT_Passive %x: Proactive inhibition daemon %i\n" );
#endif
		self.set_node_target_list( traces );
		if ( self.get_node_target_list()->size() > 0 )
		{
			block_data_t buff[Radio::MAX_MESSAGE_LENGTH];
			self.set_buffer_from( buff );
			radio().send( Radio::BROADCAST_ADDRESS, self.get_buffer_size(), buff );
		}
		timer().template set_timer<self_type, &self_type::proactive_inhibition_daemon>( proactive_inhibition_timer, this, 0 );
	}
#endif
	// -----------------------------------------------------------------------
	void print_neighbors( void* userdata = NULL )
	{
		//debug().debug( "PLTT_Passive %x: Begin neighbors printout\n", self.get_node().get_id() );
		//self.print( debug() );
		for ( PLTT_NodeListIterator i = neighbors.begin(); i != neighbors.end(); ++i )
		{
			//i->print( debug() );
			debug().debug("NB_STATS:%x:%x:(%f,%f)\n", self.get_node().get_id(), i->get_node().get_id(), i->get_node().get_position().get_x(), i->get_node().get_position().get_y() );
		}
		//debug().debug( "\nPLTT_Passive %x: End neighbors printout\n", self.get_node().get_id() );
	}
	// -----------------------------------------------------------------------
	void init(Radio& radio, Timer& timer, Debug& debug, Rand& rand,
			Clock& clock, NeighborDiscovery& neighbor_discovery)
	{
		radio_ = &radio;
		timer_ = &timer;
		debug_ = &debug;
		rand_ = &rand;
		clock_ = &clock;
		neighbor_discovery_ = &neighbor_discovery;
	}
	// -----------------------------------------------------------------------
	PLTT_NodeList* get_neighbors()
	{
		return &neighbors;
	}
	// -----------------------------------------------------------------------
	PLTT_TraceList* get_traces()
	{
		return &traces;
	}
	// -----------------------------------------------------------------------
	PLTT_Node* get_self()
	{
		return &self;
	}
	// -----------------------------------------------------------------------
	void set_self(PLTT_Node _n)
	{
		self = _n;
	}
	// -----------------------------------------------------------------------
	void set_intensity_detection_threshold( IntensityNumber value )
	{
		intensity_detection_threshold = value;
	}
	// -----------------------------------------------------------------------
	void set_backoff_connectivity_weight( millis_t c )
	{
		backoff_connectivity_weight = c;
	}
	// -----------------------------------------------------------------------
	void set_backoff_lqi_weight( millis_t l )
	{
		backoff_lqi_weight = l;
	}
	// -----------------------------------------------------------------------
	void set_backoff_random_weight( millis_t r )
	{
		backoff_random_weight = r;
	}
	// -----------------------------------------------------------------------
	void set_backoff_candidate_list_weight( millis_t p )
	{
		backoff_candidate_list_weight = p;
	}
	// -----------------------------------------------------------------------
	void set_nb_convergence_time( millis_t nb )
	{
		nb_convergence_time = nb;
	}
	// -----------------------------------------------------------------------
	millis_t get_backoff_connectivity_weight()
	{
		return backoff_connectivity_weight;
	}
	// -----------------------------------------------------------------------
	millis_t get_backoff_lqi_weight()
	{
		return backoff_lqi_weight;
	}
	// -----------------------------------------------------------------------
	millis_t get_backoff_random_weight()
	{
		return backoff_random_weight;
	}
	// -----------------------------------------------------------------------
	millis_t get_backoff_penalty_list_weight()
	{
		return backoff_candidate_list_weight;
	}
	// -----------------------------------------------------------------------
	millis_t get_nb_convergence_time()
	{
		return nb_convergence_time;
	}
	// -----------------------------------------------------------------------
	void set_transmission_power_dB( uint8_t _tpdb )
	{
		transmission_power_dB = _tpdb;
	}
	// -----------------------------------------------------------------------
	uint8_t get_transmission_power_dB()
	{
		return transmission_power_dB;
	}
	// -----------------------------------------------------------------------
	void set_random_enable_timer_range( uint32_t _retr )
	{
		random_enable_timer_range = _retr;
	}
	// -----------------------------------------------------------------------
	uint32_t get_random_enable_timer_range()
	{
		return random_enable_timer_range;
	}
	// -----------------------------------------------------------------------
#ifdef CONFIG_PROACTIVE_INHIBITION
	uint32_t get_proactive_inhibition_timer()
	{
		return proactive_inhibition_timer;
	}
	// -----------------------------------------------------------------------
	void set_proactive_inhibition_timer( uint32_t pit )
	{
		proactive_inhibition_timer = pit;
	}
#endif
	// -----------------------------------------------------------------------
#ifdef PLTT_SECURE
	// -----------------------------------------------------------------------
	void set_decryption_request_timer( millis_t drt )
	{
		decryption_request_timer = drt;
	}
	// -----------------------------------------------------------------------
	millis_t get_decryption_request_timer( )
	{
		return decryption_request_timer;
	}
	// -----------------------------------------------------------------------
	void set_decryption_max_retries( uint8_t _dmr )
	{
		decryption_max_retries = _dmr;
	}
	// -----------------------------------------------------------------------
	uint8_t get_decryption_max_retries()
	{
		return decryption_max_retries;
	}
	// -----------------------------------------------------------------------

#endif
private:
	Radio& radio()
	{
		return *radio_;
	}
	// -----------------------------------------------------------------------
	Timer& timer()
	{
		return *timer_;
	}
	// -----------------------------------------------------------------------
	Debug& debug()
	{
		return *debug_;
	}
	// -----------------------------------------------------------------------
	Rand& rand()
	{
		return *rand_;
	}
	// -----------------------------------------------------------------------
	Clock& clock()
	{
		return *clock_;
	}
	// -----------------------------------------------------------------------
	NeighborDiscovery& neighbor_discovery()
	{
		return *neighbor_discovery_;
	}
	// -----------------------------------------------------------------------
	Radio * radio_;
	Timer * timer_;
	Debug * debug_;
	Rand * rand_;
	Clock * clock_;
	NeighborDiscovery * neighbor_discovery_;
	enum MessageIds
	{
		PLTT_SPREAD_ID = 11
#ifdef CONFIG_PROACTIVE_INHIBITION
		,PLTT_INHIBITION_MESSAGE_ID = 21
#endif
#ifdef PLTT_SECURE
		,PLTT_SECURE_SPREAD_ID = 91
		,PLTT_SECURE_HELPER_REPLY_ID = 101
		,PRIVACY_DECRYPTION_REQUEST_ID = 100
		,PRIVACY_DECRYPTION_REPLY_ID = 130
#endif
	};
	uint32_t radio_callback_id_;
	uint32_t seconds_counter;
	PLTT_NodeList neighbors;
	PLTT_TraceList traces;
	millis_t nb_convergence_time;
	uint8_t transmission_power_dB;
	uint32_t backoff_connectivity_weight;
	uint32_t backoff_random_weight;
	uint32_t backoff_lqi_weight;
	uint32_t backoff_candidate_list_weight;
	uint32_t random_enable_timer_range;
#ifdef PLTT_SECURE
	PLTT_SecureTraceList secure_traces;
	millis_t decryption_request_timer;
	uint8_t decryption_max_retries;
#endif
#ifdef CONFIG_PROACTIVE_INHIBITION
	uint32_t proactive_inhibition_timer;
#endif
	PLTT_Node self;
	IntensityNumber intensity_detection_threshold;
};

}
#endif
