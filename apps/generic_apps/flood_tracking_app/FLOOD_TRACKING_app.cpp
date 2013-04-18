#include "external_interface/external_interface_testing.h"
#include "util/serialization/simple_types.h"
#include "util/pstl/vector_static.h"
#include "internal_interface/position/position_new.h"
#include "internal_interface/node/node_new.h"
#include "algorithms/flood_tracking/FLOOD_TRACKING_trace.h"
#include "algorithms/flood_tracking/FLOOD_TRACKING_passive.h"
#include "algorithms/flood_tracking/FLOOD_TRACKING_target.h"
#include "FLOOD_TRACKING_app_config.h"
#ifdef UNIGE_TESTBED
#include "UNIGE_ISENSE_topology.h"
#endif

typedef wiselib::OSMODEL Os;
typedef Os::TxRadio Radio;
typedef Radio::node_id_t node_id_t;
typedef uint16 CoordinatesNumber;
typedef Radio::ExtendedData ExtendedData;
typedef Os::Debug Debug;
typedef Os::Rand Rand;
typedef Os::Timer Timer;
typedef Os::Clock Clock;
typedef uint8 TimesNumber;
typedef uint32 Integer;
typedef Radio::TxPower TxPower;
typedef wiselib::Position2DType<Os, Radio, CoordinatesNumber, Debug> Position;
typedef wiselib::NodeType<Os, Radio, node_id_t, Position, Debug> Node;
typedef wiselib::FLOOD_TRACKING_TraceType<Os, Radio, TimesNumber, Node, node_id_t, Debug> FLOOD_TRACKING_Trace;
typedef wiselib::vector_static<Os, FLOOD_TRACKING_Trace, MAX_DETECTIONS> FLOOD_TRACKING_TraceList;
typedef wiselib::FLOOD_TRACKING_PassiveType<Os, Node, FLOOD_TRACKING_Trace, FLOOD_TRACKING_TraceList, Timer, Radio, Rand, Clock, Debug> FLOOD_TRACKING_Passive;
typedef wiselib::FLOOD_TRACKING_TargetType<Os, FLOOD_TRACKING_Trace, Node, Timer, Radio, Clock, Debug> FLOOD_TRACKING_Target;

#ifdef PASSIVE_TRACKER_NODE
FLOOD_TRACKING_Passive passive( PASSIVE_TRACKER_TRANSMISSION_DB );
#endif
#ifdef TARGET_NODE
FLOOD_TRACKING_Target target( FLOOD_TRACKING_Trace(), TARGET_SPREAD_MILIS, TARGET_TRANSMISSION_DB );
#endif

void application_main( Os::AppMainParameter& value )
{
	Radio *wiselib_radio_ = &wiselib::FacetProvider<Os, Radio>::get_facet( value );
	Timer *wiselib_timer_ = &wiselib::FacetProvider<Os, Timer>::get_facet( value );
	Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Debug>::get_facet( value );
	Rand *wiselib_rand_ = &wiselib::FacetProvider<Os, Rand>::get_facet( value );
	Clock *wiselib_clock_ = &wiselib::FacetProvider<Os, Clock>::get_facet( value );
	wiselib_rand_->srand( wiselib_radio_->id() );

#ifdef PASSIVE_TRACKER_NODE
	passive.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_rand_, *wiselib_clock_ );
	passive.set_self( Node( wiselib_radio_->id(), get_node_info<Position, Radio>( wiselib_radio_ ) ) );
	passive.enable();
#endif
#ifdef TARGET_NODE
	target.init( *wiselib_radio_, *wiselib_timer_, *wiselib_clock_, *wiselib_debug_ );
	target.set_self( Node( wiselib_radio_->id(), Position( 0, 0, 0 ) ) );
	target.enable();
#endif
}
