//configuration
#define CONFIG_PLTT_SECURE
#define CONFIG_PLTT_PASSIVE_H_RANDOM_BOOT
#define CONFIG_PLTT_PASSIVE_H_TRACE_RANDOM_RECEIVERS
#define CONFIG_PLTT_TARGET_H_MINI_RUN

//debug
#define DEBUG_PLTT
#ifdef DEBUG_PLTT
	#define DEBUG_PLTT_NODE_H
	#define DEBUG_PLTT_NODE_TARGET_H
	#define DEBUG_PLTT_AGENT_H
	#define DEBUG_PLTT_TRACE_H
	#define DEBUG_PLTT_SECURE_TRACE_H
	//#define DEBUG_PLTT_PASSIVE_H
	#define DEBUG_PLTT_TARGET_H
	#define DEBUG_PLTT_TRACKER_H
	#ifdef DEBUG_PLTT_PASSIVE_H
		#define DEBUG_PLTT_PASSIVE_H_ENABLE
		#define DEBUB_PLTT_PASSIVE_H_NEIGHBOR_DISCOVERY_ENABLE_TASK
		#define DEBUG_PLTT_PASSIVE_H_NEIGHBOR_DISCOVERY_DISABLE_TASK
		#define DEBUG_PLTT_PASSIVE_H_RECEIVE
		#define DEBUG_PLTT_PASSIVE_H_PROCCESS_QUERY
		#define DEBUG_PLTT_PASSIVE_H_PROCCESS_REPORT
		#define DEBUG_PLTT_PASSIVE_H_SYNC_NEIGHBORS
		#define DEBUG_PLTT_PASSIVE_H_STORE_INHIBIT_TRACE
		#define DEBUG_PLTT_PASSIVE_H_UPDATE_TRACES
		#define DEBUG_PLTT_PASSIVE_H_PREPARE_SPREAD_TRACE
		#define DEBUG_PLTT_PASSIVE_H_SPREAD_TRACE
		#define DEBUG_PLTT_PASSIVE_H_STORE_INHIBIT_SECURE_TRACE
		#define DEBUG_PLTT_PASSIVE_H_PREPARE_SPREAD_SECURE_TRACE
		#define DEBUG_PLTT_PASSIVE_H_SPREAD_SECURE_TRACE
		#define DEBUG_PLTT_PASSIVE_H_DECRYPTION_REQUEST_DAEMON
		#define DEBUB_PLTT_PASSIVE_H_ERASE_SECURE_TRACE
	#endif
	#ifdef DEBUG_PLTT_TARGET_H
		#define DEBUG_PLTT_TARGET_H_ENABLE
		#define DEBUG_PLTT_TARGET_H_RECEIVE
		#define DEBUG_PLTT_TARGET_H_ENCRYPTION_REQUEST_DAEMON
		#define DEBUG_PLTT_TARGET_H_RANDOMIZE_CALLBACK
		#define DEBUG_PLTT_TARGET_H_TIMED_PRIVACY_CALLBACK
		#define DEBUG_PLTT_TARGET_H_SEND_TRACE
	#endif
	#ifdef DEBUG_PLTT_TRACKER_H
		#define DEBUG_PLTT_TRACKER_H_ENABLE
		#define DEBUG_PLTT_TRACKER_H_SEND_QUERY
		#define DEBUG_PLTT_TRACKER_H_SEND_ECHO
		#define DEBUG_PLTT_TRACKER_H_RECEIVE
	#endif
#endif
