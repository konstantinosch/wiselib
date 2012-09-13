#include "external_interface/external_interface_testing.h"
#include "internal_interface/position/position_new.h"
#include "algorithms/privacy/privacy_message.h"
#include "algorithms/privacy/privacy.h"
#include "privacy_app_config.h"

typedef wiselib::OSMODEL Os;
typedef Os::Radio Radio;
typedef Os::Debug Debug;
typedef Os::Uart Uart;
typedef Os::Timer Timer;
typedef Radio::node_id_t node_id_t;
typedef wiselib::PrivacyMessageType<Os, Radio> PrivacyMessage;
typedef wiselib::vector_static<Os, PrivacyMessage, MAX_PRIVACY_BUFFERED_MESSAGES> PrivacyMessageList;
typedef wiselib::PrivacyType<Os, Radio, Timer, Uart, PrivacyMessage, PrivacyMessageList, Debug> Privacy;

Privacy privacy;

void application_main( Os::AppMainParameter& value )
{
	Os::Radio *wiselib_radio_ = &(wiselib::FacetProvider<Os, Os::Radio>::get_facet(value));
	Os::Debug *wiselib_debug_ = &wiselib::FacetProvider<Os, Os::Debug>::get_facet( value );
	Os::Uart *wiselib_uart_ = &wiselib::FacetProvider<Os, Os::Uart>::get_facet( value );
	Os::Timer *wiselib_timer_ = &(wiselib::FacetProvider<Os, Os::Timer>::get_facet(value));
	wiselib_radio_->set_channel(20);
#ifdef CA
	privacy.set_encryption();
#endif
#ifdef HELPER
	privacy.set_randomization();
#endif
	privacy.init( *wiselib_radio_, *wiselib_debug_, *wiselib_uart_, *wiselib_timer_ );
	privacy.enable();
}
