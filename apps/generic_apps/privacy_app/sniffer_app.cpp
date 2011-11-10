#include "external_interface/external_interface_testing.h"
#include "internal_interface/position/position_new.h"
#include "algorithms/privacy/privacy_message.h"
#include "algorithms/privacy/sniffer.h"
#include "privacy_config.h"

typedef wiselib::OSMODEL Os;
typedef Os::Radio Radio;
typedef Os::Debug Debug;
typedef Radio::node_id_t node_id_t;
typedef wiselib::PrivacyMessageType<Os, Radio> PrivacyMessage;
typedef wiselib::SnifferType<Os, Radio, Debug, PrivacyMessage> Sniffer;

Sniffer sniffer;

void application_main( Os::AppMainParameter& value )
{
	Os::Radio *wiselib_radio_ = &(wiselib::FacetProvider<Os, Os::Radio>::get_facet(value));
	Os::Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet( value );
	sniffer.init( *wiselib_radio_, *wiselib_debug_ );
	sniffer.enable();
}
