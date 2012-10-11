#include "PLTT_app_config.h"
#include "external_interface/external_interface_testing.h"
#include "util/serialization/simple_types.h"
#include "internal_interface/position/position_new.h"
#include "internal_interface/node/node_new.h"
#include "algorithms/tracking/PLTT_trace.h"
#include "radio/reliable/reliable_radio.h"
#ifdef CONFIG_PLTT_TARGET
#include "algorithms/tracking/PLTT_target.h"
#endif
#ifdef CONFIG_PLTT_TRACKER
#include "algorithms/tracking/PLTT_agent.h"
#include "algorithms/tracking/PLTT_tracker.h"
#endif
#ifdef CONFIG_PLTT_PRIVACY
#include "algorithms/tracking/PLTT_privacy_trace.h"
#include "algorithms/privacy/privacy.h"
#include "algorithms/privacy/privacy_message.h"
#endif

typedef wiselib::OSMODEL Os;
typedef Os::TxRadio Radio;
typedef uint16 CoordinatesNumber;
typedef uint8 IntensityNumber;
typedef uint8 TimesNumber;
typedef uint8 SecondsNumber;
typedef Radio::node_id_t node_id_t;
typedef Radio::block_data_t block_data_t;
typedef Radio::size_t r_size_t;
typedef Os::Debug Debug;
typedef Os::Rand Rand;
typedef Os::Timer Timer;
typedef Os::Uart Uart;
typedef Os::Clock Clock;
typedef wiselib::ReliableRadio_Type<Os, Radio, Clock, Timer, Rand, Debug> ReliableRadio;
typedef wiselib::Position2DType<Os, Radio, CoordinatesNumber, Debug> Position;
typedef wiselib::NodeType<Os, Radio, node_id_t, Position, Debug> Node;
#ifdef CONFIG_PLTT_TRACKER
typedef uint32 AgentID;
typedef wiselib::PLTT_AgentType< Os, Radio, AgentID, IntensityNumber, Debug> PLTT_Agent;
typedef wiselib::PLTT_TrackerType<Os, PLTT_Agent, Node, Position, IntensityNumber, Timer, Radio, ReliableRadio, Rand, Clock, Debug> PLTT_Tracker;
#endif
typedef wiselib::PLTT_TraceType<Os, Radio, TimesNumber, SecondsNumber, IntensityNumber, Node, node_id_t, Debug> PLTT_Trace;
#ifdef CONFIG_PLTT_PRIVACY
typedef wiselib::PLTT_PrivacyTraceType<Os, Radio, TimesNumber, SecondsNumber, IntensityNumber, Node, node_id_t, Debug> PLTT_PrivacyTrace;
typedef wiselib::PrivacyMessageType<Os, Radio> PrivacyMessage;
typedef wiselib::vector_static<Os, PrivacyMessage, PLTT_MAX_PRIVACY_MESSAGES_SUPPORTED> PrivacyMessageList;
typedef wiselib::PrivacyType<Os, Radio, Timer, Uart, PrivacyMessage, PrivacyMessageList, Debug> Privacy;
typedef wiselib::PLTT_TargetType<Os, PLTT_PrivacyTrace, Node, Timer, Radio, PrivacyMessage, Clock, Debug> PLTT_Target;
#else
typedef wiselib::PLTT_TargetType<Os, PLTT_Trace, Node, Timer, Radio, Clock, Debug> PLTT_Target;
#endif
#ifdef CONFIG_PLTT_TARGET
#ifdef CONFIG_PLTT_PRIVACY
PLTT_Target target( PLTT_PrivacyTrace( PLTT_TRACE_DIMINISH_SECONDS, PLTT_TRACE_DIMINISH_AMOUNT, PLTT_TRACE_SPREAD_PENALTY, PLTT_TRACE_START_INTENSITY, 0 ), PLTT_TARGET_SPREAD_MILIS, PLTT_TARGET_TRANSMISSION_POWER );
#else
PLTT_Target target( PLTT_Trace( PLTT_TRACE_DIMINISH_SECONDS, PLTT_TRACE_DIMINISH_AMOUNT, PLTT_TRACE_SPREAD_PENALTY, PLTT_TRACE_START_INTENSITY, 0 ), PLTT_TARGET_SPREAD_MILIS, PLTT_TARGET_TRANSMISSION_POWER );
#endif
#endif
#ifdef CONFIG_PLTT_TRACKER
PLTT_Tracker tracker( PLTT_TARGET_ID, PLTT_TRACE_START_INTENSITY );
ReliableRadio reliable_radio;
#endif
#ifdef CONFIG_PLTT_PRIVACY
Privacy privacy;
#endif

void application_main( Os::AppMainParameter& value )
{
	Radio *wiselib_radio_ = &wiselib::FacetProvider<Os, Radio>::get_facet( value );
	Timer *wiselib_timer_ = &wiselib::FacetProvider<Os, Os::Timer>::get_facet( value );
	Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet( value );
	Rand *wiselib_rand_ = &wiselib::FacetProvider<Os, Os::Rand>::get_facet( value );
	Clock *wiselib_clock_ = &wiselib::FacetProvider<Os, Os::Clock>::get_facet( value );
	wiselib_radio_->set_channel( PLTT_CHANNEL );
	wiselib_rand_->srand( wiselib_radio_->id() );
#ifdef CONFIG_PLTT_TRACKER
	reliable_radio.init(  *wiselib_radio_, *wiselib_timer_, *wiselib_debug_, *wiselib_clock_, *wiselib_rand_ );
	tracker.init( *wiselib_radio_, reliable_radio, *wiselib_timer_, *wiselib_rand_, *wiselib_clock_, *wiselib_debug_ );
	tracker.set_self( Node( wiselib_radio_->id(), Position( 0, 0, 0 ) ) );
	tracker.enable();
#endif
#ifdef CONFIG_PLTT_TARGET
	target.init( *wiselib_radio_, *wiselib_timer_, *wiselib_clock_, *wiselib_debug_ );
	target.set_self( Node( wiselib_radio_->id(), Position( 0, 0, 0 ) ) );
#ifdef CONFIG_PLTT_PRIVACY
	target.set_request_id( PLTT_TARGET_PRIVACY_REQUEST_ID_1 );
	Os::Uart *wiselib_uart_ = &wiselib::FacetProvider<Os, Os::Uart>::get_facet( value );
	privacy.set_randomization();
	privacy.init( *wiselib_radio_, *wiselib_debug_, *wiselib_uart_, *wiselib_timer_ );
	target.reg_privacy_radio_callback<Privacy, &Privacy::radio_receive>( &privacy );
	privacy.reg_privacy_callback<PLTT_Target, &PLTT_Target::randomize_callback>( 999, &target );
	privacy.enable();
#endif
	target.enable();
#endif
}
