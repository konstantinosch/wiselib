#include "external_interface/external_interface_testing.h"
#include "util/serialization/simple_types.h"
#include "util/pstl/vector_static.h"
#include "internal_interface/position/position_new.h"
#include "internal_interface/node/node_new.h"
#include "algorithms/tracking/PLTT_node.h"
#include "algorithms/tracking/PLTT_node_target.h"
#include "algorithms/tracking/PLTT_trace.h"
#include "algorithms/tracking/PLTT_agent.h"
#include "algorithms/tracking/PLTT_reliable_agent.h"
#include "algorithms/tracking/PLTT_passive_nb.h"
#include "algorithms/neighbor_discovery/echo.h"
#include "algorithms/tracking/PLTT_spread_metrics.h"
#include "algorithms/tracking/PLTT_tracking_metrics.h"
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

typedef wiselib::OSMODEL Os;
typedef Os::TxRadio Radio;
typedef Radio::node_id_t node_id_t;
typedef uint16 CoordinatesNumber;
typedef int8 IntensityNumber;
typedef Radio::ExtendedData ExtendedData;
typedef Os::Debug Debug;
typedef Os::Rand Rand;
typedef Os::Timer Timer;
typedef Os::Clock Clock;
typedef uint8 TimesNumber;
typedef uint8 SecondsNumber;
typedef uint32 AgentID;
typedef uint32 Integer;
typedef wiselib::Echo<Os, Radio, Timer, Debug> NeighborDiscovery;
typedef wiselib::Position2DType<Os, Radio, CoordinatesNumber, Debug> Position;
typedef wiselib::NodeType<Os, Radio, node_id_t, Position, Debug> Node;
typedef wiselib::PLTT_AgentType<Os, Radio, AgentID, Node, IntensityNumber, Clock, Debug> PLTT_Agent;
typedef wiselib::vector_static<Os, PLTT_Agent, MAX_TRACKERS_SUPPORTED> PLTT_AgentList;
typedef wiselib::PLTT_ReliableAgentType<Os, Radio, PLTT_Agent, Timer, Debug> PLTT_ReliableAgent;
typedef wiselib::vector_static<Os, PLTT_ReliableAgent, MAX_RELIABLE_AGENTS_SUPPORTED> PLTT_ReliableAgentList;
#ifdef PLTT_SECURE
typedef wiselib::PLTT_Secure_TraceType<Os, Radio, TimesNumber, SecondsNumber, IntensityNumber, Node, node_id_t, Debug> PLTT_SecureTrace;
typedef wiselib::vector_static<Os, PLTT_SecureTrace, MAX_TARGETS_SUPPORTED> PLTT_SecureTraceList;
#endif
typedef wiselib::PLTT_TraceType<Os, Radio, TimesNumber, SecondsNumber, IntensityNumber, Node, node_id_t, Debug> PLTT_Trace;
typedef wiselib::vector_static<Os, PLTT_Trace, MAX_TARGETS_SUPPORTED> PLTT_TraceList;
typedef wiselib::PLTT_NodeTargetType<Os, Radio, node_id_t, IntensityNumber, Debug > PLTT_NodeTarget;
typedef wiselib::vector_static<Os, PLTT_NodeTarget, MAX_TARGETS_SUPPORTED> PLTT_NodeTargetList;
typedef wiselib::PLTT_NodeType<Os, Radio, Node, PLTT_NodeTarget, PLTT_NodeTargetList, PLTT_TraceList, Debug> PLTT_Node;
typedef wiselib::vector_static<Os, PLTT_Node, MAX_NEIGHBORS_SUPPORTED> PLTT_NodeList;
typedef wiselib::PLTT_PassiveSpreadMetricType<node_id_t, Integer> PLTT_PassiveSpreadMetric;
typedef wiselib::vector_static<Os, PLTT_PassiveSpreadMetric, MAX_TARGETS_SUPPORTED> PLTT_PassiveSpreadMetricList;
typedef wiselib::PLTT_PassiveSpreadMetricsType<PLTT_PassiveSpreadMetric, PLTT_PassiveSpreadMetricList> PLTT_PassiveSpreadMetrics;
typedef wiselib::PLTT_PassiveTrackingMetricType<node_id_t, Integer> PLTT_PassiveTrackingMetric;
typedef wiselib::vector_static<Os, PLTT_PassiveTrackingMetric, MAX_TRACKERS_SUPPORTED> PLTT_PassiveTrackingMetricList;
typedef wiselib::PLTT_PassiveTrackingMetricsType<PLTT_PassiveTrackingMetric, PLTT_PassiveTrackingMetricList> PLTT_PassiveTrackingMetrics;
#ifdef PLTT_SECURE
typedef wiselib::PLTT_PassiveType<Os, Node, PLTT_Node, PLTT_NodeList, PLTT_Trace, PLTT_TraceList, PLTT_SecureTrace, PLTT_SecureTraceList, PLTT_Agent, PLTT_AgentList, PLTT_ReliableAgent, PLTT_ReliableAgentList, NeighborDiscovery, Timer, Radio, Rand, Clock, PLTT_PassiveSpreadMetrics, PLTT_PassiveTrackingMetrics, Debug> PLTT_Passive;
#else
typedef wiselib::PLTT_PassiveType<Os, Node, PLTT_Node, PLTT_NodeList, PLTT_Trace, PLTT_TraceList, PLTT_Agent, PLTT_AgentList, PLTT_ReliableAgent, PLTT_ReliableAgentList, NeighborDiscovery, Timer, Radio, Rand, Clock, PLTT_PassiveSpreadMetrics, PLTT_PassiveTrackingMetrics, Debug> PLTT_Passive;
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
#ifdef LUEBECK_DEMO
	//if ( ( wiselib_radio_->id() == 0x1b6b ) || ( wiselib_radio_->id() == 0xcc3a ) ) { return; }
#endif
	wiselib_radio_->set_channel(20);
	wiselib_rand_->srand( wiselib_radio_->id() );
	passive.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_rand_, *wiselib_clock_, neighbor_discovery );
	passive.set_self( PLTT_Node( Node( wiselib_radio_->id(), get_node_info<Position, Radio>( wiselib_radio_ ) ) ) );
	passive.set_intensity_detection_threshold( INTENSITY_DETECTION_THRESHOLD );
#ifdef OPT_RELIABLE_TRACKING
	passive.set_reliable_agent_exp_time( RELIABLE_TRACKING_EXPIRATION_TIME );
	passive.set_reliable_agent_rec_time( RELIABLE_TRACKING_RECURRING_TIME );
	passive.set_reliable_millis_counter( RELIABLE_TRACKING_DAEMON_TIME );
#endif
	#ifdef PLTT_METRICS
	passive.set_metrics_timeout( METRICS_TIMEOUT );
#endif
	passive.enable();
}
