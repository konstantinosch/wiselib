//#define PLTT_DEBUG
//#define PLTT_SECURE

//target configuration
//#define PLTT_TARGET_MINI_RUN

//passive configuration
#define CONFIG_SPREAD_RANDOM_RECEIVERS
#ifndef PLTT_SECURE
	//#define CONFIG_PROACTIVE_INHIBITION
#endif
#define CONFIG_NEIGHBORHOOD_DISCOVERY_STABILITY_FILTER
#define CONFIG_BACKOFF_LQI_WEIGHT
#define CONFIG_BACKOFF_CONNECTIVITY_WEIGHT
#define CONFIG_BACKOFF_RANDOM_WEIGHT
#define CONFIG_BACKOFF_CANDIDATE_LIST_WEIGHT

//debug
#ifdef PLTT_DEBUG
	#define PLTT_PASSIVE_DEBUG
	#define PLTT_TARGET_DEBUG
	//#define PLTT_TRACKER_DEBUG
	#define PLTT_SECURE_DEBUG
#endif

#ifdef PLTT_PASSIVE_DEBUG
	//#define PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY
	//#define PLTT_PASSIVE_DEBUG_NEIGHBORHOOD_DISCOVERY_ITER
	#define PLTT_PASSIVE_DEBUG_SPREAD
	//#define PLTT_PASSIVE_DEBUG_INHIBITION
	//#define PLTT_PASSIVE_DEBUG_TRACK_ECHO
	//#define PLTT_PASSIVE_DEBUG_TRACK_QUERY
	//#define PLTT_PASSIVE_DEBUG_TRACK_QUERY_REPORT
	#define PLTT_PASSIVE_DEBUG_MISC
	#ifdef PLTT_SECURE_DEBUG
		#define PLTT_PASSIVE_DEBUG_SECURE
	#endif
#endif
#ifdef PLTT_TARGET_DEBUG
	#define PLTT_TARGET_DEBUG_SEND
	#define PLTT_TARGET_DEBUG_MISC
	#ifdef PLTT_SECURE
		#define PLTT_TARGET_DEBUG_SECURE
	#endif
	#ifdef PLTT_METRICS_DEBUG
		#define PLTT_TARGET_METRICS_DEBUG
	#endif
#endif
#ifdef PLTT_TRACKER_DEBUG
	#define PLTT_TRACKER_DEBUG_REPORT
	#define PLTT_TRACKER_DEBUG_QUERY
	#define PLTT_TRACKER_DEBUG_MISC
	#ifdef PLTT_SECURE_DEBUG
		#define PLTT_TRACKER_DEBUG_SECURE
	#endif
#endif

