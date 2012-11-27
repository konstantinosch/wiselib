#define CONFIG_NEIGHBOR_DISCOVERY_H_RAND_STARTUP
#define CONFIG_NEIBHBOR_DISCOVERY_H_LQI_FILTERING
#define CONFIG_NEIBHBOR_DISCOVERY_H_RSSI_FILTERING

//#define DEBUG_NEIGHBOR_DISCOVERY
#ifdef DEBUG_NEIGHBOR_DISCOVERY
	#define DEBUG_NEIGHBOR_H
	//#define DEBUG_PROTOCOL_PAYLOAD_H
	//#define DEBUG_BEACON_H
	//#define DEBUG_PROTOCOL_SETTINGS_H
	#define DEBUG_PROTOCOL_H
	#define DEBUG_NEIGHBOR_DISCOVERY_STATS
	//#define DEBUG_NEIGHBOR_DISCOVERY_STATS_DAEMON
	//#define DEBUG_NEIGHBOR_DISCOVERY_H
	#ifdef DEBUG_NEIGHBOR_DISCOVERY_H
		#define DEBUG_NEIGHBOR_DISCOVERY_H_BEACONS
		#define DEBUG_NEIGHBOR_DISCOVERY_H_RECEIVE
		#define DEBUG_NEIGHBOR_DISCOVERY_H_REGISTER_PROTOCOL
		#define DEBUG_NEIGHBOR_DISCOVERY_H_ND_DAEMON
		#define DEBUG_NEIGHBOR_DISCOVERY_H_NEIGHBOR_UPDATE_LINK_STAB_RATIO
		#define DEBUG_NEIGHBOR_DISCOVERY_H_ND_METRICS_DAEMON
	#endif
#endif

//#define NEIGHBOR_DISCOVERY_RAND_STARTUP
//#define NEIGHBOR_DISCOVERY_COORD_SUPPORT
#ifdef NEIGHBOR_DISCOVERY_COORD_SUPPORT
#define NEIGHBOR_DISCOVERY_COORD_SUPPORT_2D
//#define NEIGHBOR_DISCOVERY_COORD_SUPPORT_3D
//#define NEIGHBOR_DISCOVERY_COORD_SUPPORT_SHAWN
#define NEIGHBOR_DISCOVERY_COORD_SUPPORT_ISENSE
#ifdef NEIGHBOR_DISCOVERY_COORD_SUPPORT_SHAWN
typedef double PositionNumber;
#endif
#ifdef NEIGHBOR_DISCOVERY_COORD_SUPPORT_ISENSE
typedef uint8_t PositionNumber;
#endif
#endif
