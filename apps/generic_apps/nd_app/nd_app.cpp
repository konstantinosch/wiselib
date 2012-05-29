#include "external_interface/external_interface_testing.h"
#include "util/serialization/simple_types.h"
#include "internal_interface/position/position_new.h"
#include "algorithms/neighbor_discovery/neighbor_discovery.h"
#include "radio/reliable/reliable_radio.h"

#define UNIGE_TESTBED
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
typedef Os::Clock Clock;
typedef Os::Debug Debug;
typedef Os::Timer Timer;
typedef Os::Rand Rand;
typedef wiselib::Position2DType<Os, Radio, uint8, Debug> Position;
typedef wiselib::NeighborDiscovery_Type<Os, Radio, Clock, Timer, Rand, Debug> NeighborDiscovery;
typedef wiselib::ReliableRadio_Type<Os, Radio, Clock, Timer, Rand, Debug> ReliableRadio;

NeighborDiscovery neighbor_discovery;
ReliableRadio reliable_radio;

void application_main( Os::AppMainParameter& value )
{
	Radio *wiselib_radio_ = &wiselib::FacetProvider<Os, Radio>::get_facet( value );
	Timer *wiselib_timer_ = &wiselib::FacetProvider<Os, Timer>::get_facet( value );
	Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Debug>::get_facet( value );
	Clock *wiselib_clock_ = &wiselib::FacetProvider<Os, Clock>::get_facet( value );
	Rand *wiselib_rand_ = &wiselib::FacetProvider<Os, Rand>::get_facet( value );
	neighbor_discovery.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_clock_, *wiselib_rand_ );
	reliable_radio.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_clock_, *wiselib_rand_ );
	neighbor_discovery.set_coords( get_node_info<Position, Radio>( wiselib_radio_ ).get_x(), get_node_info<Position, Radio>( wiselib_radio_ ).get_y(), get_node_info<Position, Radio>( wiselib_radio_ ).get_z() );
	neighbor_discovery.set_RR( reliable_radio );
	neighbor_discovery.enable();
}
