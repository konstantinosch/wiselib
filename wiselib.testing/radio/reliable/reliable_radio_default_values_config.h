#define RR_RESEND_DAEMON_PERIOD 100
#define RR_MAX_REGISTERED_PROTOCOLS 2
//#define RR_MAX_BUFFERED_MESSAGES 200 shawn setting
#define RR_MAX_BUFFERED_MESSAGES 20 //new concept to ensure throughput only depended on one vector size
#define RR_MAX_BUFFERED_REPLIES 60  //reply buffer has to be x[retransmission_times-1] bigger than message buffer
#define RR_MAX_RETRIES 3			//documented in evernote for now
