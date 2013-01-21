//configuration
//general behaviour configs
#define CONFIG_PLTT_PRIVACY
#define CONFIG_PLTT_PASSIVE_H_RANDOM_BOOT
#define CONFIG_PLTT_PASSIVE_H_DISABLE_NEIGHBOR_DISCOVERY
//#define CONFIG_PLTT_PASSIVE_H_DYNAMIC_NEIGHBOR_DISCOVERY
#define CONFIG_PLTT_PASSIVE_H_ND_DISABLE
#define CONFIG_PLTT_PASSIVE_H_MEMORYLESS_STATISTICS
#define CONFIG_PLTT_PASSIVE_H_ND_INTER_TASK
#define CONFIG_PLTT_PASSIVE_H_TREE
#define CONFIG_PLTT_TARGET_H_MINI_RUN
#define CONFIG_PLTT_TRACKER_H_MINI_RUN

//performance-experimentation configs
//#define CONFIG_PLTT_PASSIVE_H_TRACE_RANDOM_RECEIVERS
#define CONFIG_PLTT_PASSIVE_H_INHIBITION_LIMIT_ON_DECRYPTION_REQUEST
#define CONFIG_PLTT_PASSIVE_H_TRACKING_INTENSITY
//#define CONFIG_PLTT_PASSIVE_H_TRACKING_INC_DETECTIONS
//#define CONFIG_PLTT_PASSIVE_H_TREE_RECONFIGURATION

//debug
#define DEBUG_PLTT
#ifdef DEBUG_PLTT
	//#define DEBUG_PLTT_NODE_H
	//#define DEBUG_PLTT_NODE_TARGET_H
	//#define DEBUG_PLTT_AGENT_H
	//#define DEBUG_PLTT_TRACE_H
	//#define DEBUG_PLTT_PRIVACY_TRACE_H
	#define DEBUG_PLTT_PASSIVE_H
	//#define DEBUG_PLTT_TARGET_H
	//#define DEBUG_PLTT_TRACKER_H
	#define DEBUG_PLTT_STATS
	#ifdef DEBUG_PLTT_PASSIVE_H
		//#define DEBUG_PLTT_PASSIVE_H_ENABLE
		//#define DEBUB_PLTT_PASSIVE_H_NEIGHBOR_DISCOVERY_ENABLE_TASK
		//#define DEBUG_PLTT_PASSIVE_H_NEIGHBOR_DISCOVERY_DISABLE_TASK
		//#define DEBUG_PASSIVE_H_SEND_DECTRYPTION_REQUEST
		#define DEBUG_PLTT_PASSIVE_H_RECEIVE
		#define DEBUG_PLTT_PASSIVE_H_PROCCESS_QUERY_REPORT
		//#define DEBUG_PLTT_PASSIVE_H_SYNC_NEIGHBORS
		//#define DEBUG_PLTT_PASSIVE_H_STORE_INHIBIT_TRACE
		//#define DEBUG_PLTT_PASSIVE_H_UPDATE_TRACES
		//#define DEBUG_PLTT_PASSIVE_H_PREPARE_SPREAD_TRACE
		//#define DEBUG_PLTT_PASSIVE_H_SPREAD_TRACE
		//#define DEBUG_PLTT_PASSIVE_H_STORE_INHIBIT_PRIVACY_TRACE
		//#define DEBUG_PLTT_PASSIVE_H_PREPARE_SPREAD_PRIVACY_TRACE
		//#define DEBUG_PLTT_PASSIVE_H_SPREAD_PRIVACY_TRACE
		//#define DEBUG_PLTT_PASSIVE_H_DECRYPTION_REQUEST_DAEMON
		//#define DEBUB_PLTT_PASSIVE_H_ERASE_PRIVACY_TRACE
		//#define DEBUG_PLTT_PASSIVE_H_STATUS
		//#define DEBUG_PLTT_PASSIVE_H_NEIGHBOR_DISCOVERY_STATS
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
		#define DEBUG_PLTT_TRACKER_H_INSERT_AGENT
	#endif
#endif
