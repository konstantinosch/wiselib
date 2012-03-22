#include "external_interface/external_interface_testing.h"
#include "util/serialization/simple_types.h"
#include "util/pstl/vector_static.h"
#include "internal_interface/position/position_new.h"
#include "internal_interface/node/node_new.h"
#include "algorithms/tracking/PLTT_node.h"
#include "algorithms/tracking/PLTT_node_target.h"
#include "algorithms/tracking/PLTT_trace_revision.h"
#include "algorithms/tracking/PLTT_passive_nb_revision.h"
#include "algorithms/neighbor_discovery/neighbor_discovery.h"

#ifdef PLTT_SECURE
#include "algorithms/tracking/PLTT_secure_trace_revision.h"
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

typedef wiselib::OSMODEL Os;
typedef Os::TxRadio Radio;
typedef Radio::node_id_t node_id_t;
typedef uint16 CoordinatesNumber;
typedef uint8 IntensityNumber;
typedef Radio::ExtendedData ExtendedData;
typedef Os::Debug Debug;
typedef Os::Rand Rand;
typedef Os::Timer Timer;
typedef Os::Clock Clock;
typedef uint8 TimesNumber;
typedef uint8 SecondsNumber;
typedef uint32 AgentID;
typedef uint32 Integer;
typedef wiselib::NeighborDiscovery_Type<Os, Radio, Clock, Timer, Debug> NeighborDiscovery;
typedef wiselib::Position2DType<Os, Radio, CoordinatesNumber, Debug> Position;
typedef wiselib::NodeType<Os, Radio, node_id_t, Position, Debug> Node;
#ifdef PLTT_SECURE
typedef wiselib::PLTT_SecureTraceType<Os, Radio, TimesNumber, SecondsNumber, IntensityNumber, Node, node_id_t, Debug> PLTT_SecureTrace;
typedef wiselib::vector_static<Os, PLTT_SecureTrace, MAX_SECURE_TRACES_SUPPORTED> PLTT_SecureTraceList;
#endif
typedef wiselib::PLTT_TraceType<Os, Radio, TimesNumber, SecondsNumber, IntensityNumber, Node, node_id_t, Debug> PLTT_Trace;
typedef wiselib::vector_static<Os, PLTT_Trace, MAX_TARGETS_SUPPORTED> PLTT_TraceList;
typedef wiselib::PLTT_NodeTargetType<Os, Radio, node_id_t, IntensityNumber, Debug > PLTT_NodeTarget;
typedef wiselib::vector_static<Os, PLTT_NodeTarget, MAX_TARGETS_SUPPORTED> PLTT_NodeTargetList;
typedef wiselib::PLTT_NodeType<Os, Radio, Node, PLTT_NodeTarget, PLTT_NodeTargetList, PLTT_TraceList, Debug> PLTT_Node;
typedef wiselib::vector_static<Os, PLTT_Node, MAX_NEIGHBORS_SUPPORTED> PLTT_NodeList;
#ifdef PLTT_SECURE
typedef wiselib::PLTT_PassiveType<Os, Node, PLTT_Node, PLTT_NodeList, PLTT_Trace, PLTT_TraceList, PLTT_SecureTrace, PLTT_SecureTraceList, NeighborDiscovery, Timer, Radio, Rand, Clock, Debug> PLTT_Passive;
#else
typedef wiselib::PLTT_PassiveType<Os, Node, PLTT_Node, PLTT_NodeList, PLTT_Trace, PLTT_TraceList, NeighborDiscovery, Timer, Radio, Rand, Clock, Debug> PLTT_Passive;
#endif

NeighborDiscovery neighbor_discovery;
PLTT_Passive passive;

void application_main( Os::AppMainParameter& value )
{
	Radio *wiselib_radio_ = &wiselib::FacetProvider<Os, Radio>::get_facet( value );
	Timer *wiselib_timer_ = &wiselib::FacetProvider<Os, Timer>::get_facet( value );
	Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Debug>::get_facet( value );
	Rand *wiselib_rand_ = &wiselib::FacetProvider<Os, Rand>::get_facet( value );
	Clock *wiselib_clock_ = &wiselib::FacetProvider<Os, Clock>::get_facet( value );
	wiselib_rand_->srand( wiselib_radio_->id() );
	wiselib_radio_->set_channel(20);
	neighbor_discovery.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_clock_ );
	passive.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_rand_, *wiselib_clock_, neighbor_discovery );
	passive.set_self( PLTT_Node( Node( wiselib_radio_->id(), get_node_info<Position, Radio>( wiselib_radio_ ) ) ) );
	passive.set_intensity_detection_threshold( INTENSITY_DETECTION_THRESHOLD );
	passive.set_nb_convergence_time( NB_CONVERGENCE_TIME );
	passive.set_backoff_connectivity_weight( BACKOFF_CONNECTIVITY_WEIGHT );
	passive.set_backoff_lqi_weight( BACKOFF_LQI_WEIGHT );
	passive.set_backoff_random_weight( BACKOFF_RANDOM_WEIGHT );
	passive.set_backoff_candidate_list_weight( BACKOFF_CANDIDATE_LIST_WEIGHT );
	passive.set_transmission_power_dB( TRANSMISSION_POWER_DB );
	passive.set_random_enable_timer_range( RANDOM_ENABLE_TIMER_RANGE );
#ifdef PLTT_SECURE
	passive.set_decryption_request_timer( DECRYPTION_REQUEST_TIMER );
	passive.set_decryption_request_offset( DECRYPTION_REQUEST_OFFSET );
	passive.set_decryption_max_retries( DECRYPTION_MAX_RETRIES );
	passive.set_erase_daemon_timer( ERASE_DAEMON_TIMER );
#endif
#ifdef CONFIG_PROACTIVE_INHIBITION
	passive.set_proactive_inhibition_timer( PROACTIVE_INHIBITION_TIMER );
#endif
	passive.enable();
}
