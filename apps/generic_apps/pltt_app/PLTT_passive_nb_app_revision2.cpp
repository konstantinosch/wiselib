#include "external_interface/external_interface_testing.h"
#include "util/serialization/simple_types.h"
#include "util/pstl/vector_static.h"
//#include "internal_interface/position/position_new.h"
#include "internal_interface/serializables/serializables.h"
//#include "internal_interface/node/node_new.h"
//#include "algorithms/tracking/PLTT_node.h"
//#include "algorithms/tracking/PLTT_node_target.h"
#include "algorithms/tracking/PLTT_trace.h"
#include "algorithms/tracking/PLTT_passive_nb_revision.h"
#include "algorithms/neighbor_discovery/echo.h"
#ifdef PLTT_SECURE
#include "algorithms/tracking/PLTT_secure_trace.h"
#endif
#include "PLTT_app_config.h"
#ifdef UNIGE_TESTBED
#include "PLTT_UNIGE_topology.h"
#endif
#ifdef CTI_TESTBED
#include "PLTT_CTI_topology.h"
#endif
#ifdef UZL_TESTBED
#include "PLTT_UZL_topology.h"
#endif
#ifdef PLTT_DEMO_topology
#include "PLTT_DEMO_topology.h"
#endif


//Type parameters
//----------------------------------------------------------------------------------------------//
#define MAX_RECIPIENT_LIST_SIZE 2
#define MAX_REPULSION_LIST_SIZE 3
#define MAX_TARGETS 3
#define MAX_CYPHER_TEXT_SIZE 80
#define CHANNEL 10
//----------------------------------------------------------------------------------------------//


//OS types
//----------------------------------------------------------------------------------------------//
typedef wiselib::OSMODEL Os;
typedef Os::TxRadio Radio;
typedef Radio::node_id_t node_id_t;
typedef Radio::ExtendedData ExtendedData;
typedef Os::Debug Debug;
typedef Os::Rand Rand;
typedef Os::Timer Timer;
typedef Os::Clock Clock;
//----------------------------------------------------------------------------------------------//


//primitive types
//----------------------------------------------------------------------------------------------//
typedef uint8 TimesNumber;
typedef uint8 SecondsNumber;
typedef uint16 CoordinatesNumber;
typedef int8 IntensityNumber;
typedef uint32 AgentID;
typedef uint8 CypherTextByte;
typedef uint8 InhibitionFlag;
//----------------------------------------------------------------------------------------------//


//serializable primitive types
//----------------------------------------------------------------------------------------------//
typedef wiselib::SerializableDataType<Os, Radio, TimesNumber, Debug> S_TimesNumber;
typedef wiselib::SerializableDataType<Os, Radio, SecondsNumber, Debug> S_SecondsNumber;
typedef wiselib::SerializableDataType<Os, Radio, CoordinatesNumber, Debug> S_CoordinatesNumber;
typedef wiselib::SerializableDataType<Os, Radio, IntensityNumber, Debug> S_IntensityNumber;
typedef wiselib::SerializableDataType<Os, Radio, AgentID, Debug> S_AgentID;
typedef wiselib::SerializableDataType<Os, node_id_t, Debug> S_NodeID;
//----------------------------------------------------------------------------------------------//


//serializable complex types
//----------------------------------------------------------------------------------------------//
typedef wiselib::SerializableDataSetType2<Os, Radio, S_CoordinatesNumber, S_CoordinatesNumber, Debug> S_Position;
typedef wiselib::SerializableDataSetType2<Os, Radio, S_NodeID, S_Position, Debug> S_Node;
typedef wiselib::SerializableDataSetType2<Os, Radio, S_NodeID, S_IntensityNumber> S_TargetNode;
typedef wiselib::SerializableDataListType<Os, Radio, S_TargetNode, MAX_TARGETS, Debug> S_TargetNodeList;
typedef wiselib::SerializableDataSetType2<Os, Radio, S_Node, S_TargetNodeList, Debug> S_PassiveNodeType;
typedef wiselib::vector_static<Os, S_PassiveNodeType, MAX_NEIGHBORS_SUPPORTED> PassiveNodeList;
typedef wiselib::SerializableDataListType<Os, Radio, S_NodeID, MAX_RECIPIENT_LIST_SIZE, Debug> S_RecipientList;
typedef wiselib::SerializableDataListType<Os, Radio, S_Node, MAX_REPULSION_LIST_SIZE, Debug> S_RepulsionList;
//----------------------------------------------------------------------------------------------//


//secure trace types
//----------------------------------------------------------------------------------------------//
#ifdef PLTT_SECURE
typedef wiselib::SerializableDataType< Os, Radio, CypherTextByte, Debug> S_CypherTextByte;
typedef wiselib::SerializableDataListType< Os, Radio, S_CypherTextByte, MAX_CYPHER_TEXT_SIZE, Debug> S_CypherText;
typedef wiselib::SerializableDataSetType8<Os, Radio, S_RepulsionList, S_NodeID, S_TimesNumber, S_SecondsNumber, S_IntensityNumber, S_IntensityNumber, S_IntensityNumber, S_CypherText, S_RecipientList> S_SecureTraceData;
typedef wiselib::Secure_TraceType<Os, Radio, SecureTraceData, IntensityNumber, InhibitionFlag, Debug> SecureTrace;
typedef wiselib::vector_static<Os, PLTT_SecureTrace, MAX_TARGETS> SecureTraceList;
#endif
//----------------------------------------------------------------------------------------------//


//trace types
//----------------------------------------------------------------------------------------------//
typedef wiselib::SerializableDataSetType8<Os, Radio, S_RepulsionList, S_NodeID, S_TimesNumber, S_SecondsNumber, S_IntensityNumber, S_IntensityNumber, S_IntensityNumber, S_RecipientList> S_TraceData;
typedef wiselib::TraceType<Os, Radio, S_TraceData, IntensityNumber, InhibitionFlag, Debug> Trace;
typedef wiselib::vector_static<Os, PLTT_Trace, MAX_TARGETS> TraceList;
//----------------------------------------------------------------------------------------------//


//protocol entity types
//----------------------------------------------------------------------------------------------//
typedef wiselib::Echo<Os, Radio, Timer, Debug> NeighborDiscovery;
#ifdef PLTT_SECURE
typedef wiselib::PLTT_PassiveType<Os, S_Node, S_PassiveNode, PassiveNodeList, Trace, TraceList, SecureTrace, SecureTraceList, NeighborDiscovery, Timer, Radio, Rand, Clock, Debug> Passive;
#else
typedef wiselib::PLTT_PassiveType<Os, S_Node, S_PassiveNode, PassiveNodeList, Trace, TraceList, NeighborDiscovery, Timer, Radio, Rand, Clock, Debug> Passive;
#endif
//----------------------------------------------------------------------------------------------//




NeighborDiscovery neighbor_discovery;
PLTT_Passive passive;

void application_main( Os::AppMainParameter& value )
{
	Radio *wiselib_radio_ = &wiselib::FacetProvider<Os, Radio>::get_facet( value );
	Timer *wiselib_timer_ = &wiselib::FacetProvider<Os, Timer>::get_facet( value );
	Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Debug>::get_facet( value );
	Rand *wiselib_rand_ = &wiselib::FacetProvider<Os, Rand>::get_facet( value );
	Clock *wiselib_clock_ = &wiselib::FacetProvider<Os, Clock>::get_facet( value );
	wiselib_radio_->set_channel( CHANNEL );
	wiselib_rand_->srand( wiselib_radio_->id() );
	passive.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_rand_, *wiselib_clock_, neighbor_discovery );
	//passive.set_self( PLTT_Node( Node( wiselib_radio_->id(), get_node_info<Position, Radio>( wiselib_radio_ ) ) ) );
	//passive.set_intensity_detection_threshold( INTENSITY_DETECTION_THRESHOLD );
	//passive.set_nb_convergence_time( NB_CONVERGENCE_TIME );
	//passive.set_backoff_connectivity_weight( BACKOFF_CONNECTIVITY_WEIGHT );
	//passive.set_backoff_lqi_weight( BACKOFF_LQI_WEIGHT );
	//passive.set_backoff_random_weight( BACKOFF_RANDOM_WEIGHT );
	//passive.set_backoff_candidate_list_weight( BACKOFF_CANDIDATE_LIST_WEIGHT );
#ifdef PLTT_SECURE
	//passive.set_decryption_request_timer( DECRYPTION_REQUEST_TIMER );
#endif
#ifdef CONFIG_PROACTIVE_INHIBITION
	//passive.set_proactive_inhibition_timer( PROACTIVE_INHIBITION_TIMER );
#endif
	//passive.enable();
}
