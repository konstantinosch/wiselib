#include "external_interface/external_interface_testing.h"
#include "util/serialization/simple_types.h"
#include "internal_interface/position/position_new.h"
#include "algorithms/neighbor_discovery/neighbor_discovery.h"
#include "nd_app_config.h"
#ifdef FRAGMENTING_RADIO
#include "radio/fragmenting/fragmenting_radio.h"
#endif
#ifdef UNIGE_TESTBED
#include "../topologies/UNIGE_ISENSE_topology.h"
#endif
#ifdef CTI_TESTBED
#include "../topologies/CTI_ISENSE_topology.h"
#endif

typedef wiselib::OSMODEL Os;
typedef Os::TxRadio Radio;
typedef Os::Clock Clock;
typedef Os::Debug Debug;
typedef Os::Timer Timer;
typedef Os::Rand Rand;
#ifdef FRAGMENTING_RADIO
typedef wiselib::FragmentingRadio_Type<Os, Radio, Clock, Timer, Rand, Debug> FragmentingRadio;
typedef wiselib::NeighborDiscovery_Type<Os, FragmentingRadio, Clock, Timer, Rand, Debug> NeighborDiscovery;
typedef wiselib::Position2DType<Os, FragmentingRadio, uint8_t, Debug> Position;
#else
typedef wiselib::NeighborDiscovery_Type<Os, Radio, Clock, Timer, Rand, Debug> NeighborDiscovery;
typedef wiselib::Position2DType<Os, Radio, uint8_t, Debug> Position;
#endif

#ifdef FRAGMENTING_RADIO
FragmentingRadio fragmenting_radio;
#endif
NeighborDiscovery neighbor_discovery;

void application_main( Os::AppMainParameter& value )
{
	Radio *wiselib_radio_ = &wiselib::FacetProvider<Os, Radio>::get_facet( value );
	Timer *wiselib_timer_ = &wiselib::FacetProvider<Os, Timer>::get_facet( value );
	Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Debug>::get_facet( value );
	Clock *wiselib_clock_ = &wiselib::FacetProvider<Os, Clock>::get_facet( value );
	Rand *wiselib_rand_ = &wiselib::FacetProvider<Os, Rand>::get_facet( value );
	wiselib_rand_->srand( wiselib_radio_->id() );
#ifdef FRAGMENTING_RADIO
	fragmenting_radio.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_clock_, *wiselib_rand_ );
	neighbor_discovery.init( fragmenting_radio, *wiselib_timer_, *wiselib_debug_, *wiselib_clock_, *wiselib_rand_ );
	neighbor_discovery.set_coords( get_node_info<Position, FragmentingRadio>( &fragmenting_radio ).get_x(), get_node_info<Position, FragmentingRadio>( &fragmenting_radio ).get_y(), get_node_info<Position, FragmentingRadio>( &fragmenting_radio ).get_z() );
#else
	neighbor_discovery.init( *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_clock_, *wiselib_rand_ );
	neighbor_discovery.set_coords( get_node_info<Position, Radio>( wiselib_radio_ ).get_x(), get_node_info<Position, Radio>( wiselib_radio_ ).get_y(), get_node_info<Position, Radio>( wiselib_radio_ ).get_z() );
#endif
	neighbor_discovery.enable();
}
