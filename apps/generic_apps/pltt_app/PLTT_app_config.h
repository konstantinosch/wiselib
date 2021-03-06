#define CONFIG_PLTT_TRACKER
#define CONFIG_PLTT_TARGET
//#define CONFIG_PLTT_PRIVACY
#define UNIGE_TESTBED
//#define CTI_TESTBED

//#define PLTT_MAX_NEIGHBORS_SUPPORTED 6 //setting for really heavy tracking... worked!
#define PLTT_MAX_NEIGHBORS_SUPPORTED 25
#define PLTT_MAX_TARGETS_SUPPORTED 2
#define PLTT_MAX_TRACKERS_SUPPORTED 2
#define PLTT_CHANNEL 20
#define PLTT_MAX_PRIVACY_TRACES_SUPPORTED 25
#define PLTT_MAX_PRIVACY_MESSAGES_SUPPORTED 25
#define CONFIG_PLTT_TARGET_H_1
//#define PLTT_TARGET_1_ID 0x1cd0
//#define CONFIG_PLTT_TARGET_H_2
//#define PLTT_TARGET_1_ID 0x1c99
//#define CONFIG_PLTT_TARGET_H_3
#define PLTT_TARGET_1_ID 0x1b84
#ifdef CONFIG_PLTT_TARGET_H_1
	#define PLTT_TARGET_ID PLTT_TARGET_1_ID
	#define PLTT_TRACE_DIMINISH_SECONDS 1
	#define PLTT_TRACE_DIMINISH_AMOUNT 8
	#define PLTT_TRACE_SPREAD_PENALTY 16
	#define PLTT_TRACE_START_INTENSITY 255
	#define PLTT_TARGET_SPREAD_MILIS 2000
	#define PLTT_TARGET_INIT_SPREAD_MILIS 5000
	#define PLTT_TARGET_TRANSMISSION_POWER -30
	#define PLTT_TARGET_PRIVACY_REQUEST_ID_1 0x1111
#endif
#ifdef CONFIG_PLTT_TARGET_H_2
	#define PLTT_TARGET_ID PLTT_TARGET_1_ID
	#define PLTT_TRACE_DIMINISH_SECONDS 2
	#define PLTT_TRACE_DIMINISH_AMOUNT 16
	#define PLTT_TRACE_SPREAD_PENALTY 32
	#define PLTT_TRACE_START_INTENSITY 255
	#define PLTT_TARGET_SPREAD_MILIS 5000
	#define PLTT_TARGET_INIT_SPREAD_MILIS 110000
	#define PLTT_TARGET_TRANSMISSION_POWER -30
	#define PLTT_TARGET_PRIVACY_REQUEST_ID_1 0x2222
#endif
#ifdef CONFIG_PLTT_TARGET_H_3
	#define PLTT_TARGET_ID PLTT_TARGET_1_ID
	#define PLTT_TRACE_DIMINISH_SECONDS 1
	#define PLTT_TRACE_DIMINISH_AMOUNT 12
	#define PLTT_TRACE_SPREAD_PENALTY 24
	#define PLTT_TRACE_START_INTENSITY 255
	#define PLTT_TARGET_SPREAD_MILIS 3000
	#define PLTT_TARGET_INIT_SPREAD_MILIS 110000
	#define PLTT_TARGET_TRANSMISSION_POWER -30
	#define PLTT_TARGET_PRIVACY_REQUEST_ID_1 0x3333
#endif
