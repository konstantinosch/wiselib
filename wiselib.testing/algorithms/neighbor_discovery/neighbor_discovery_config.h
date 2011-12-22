//default NB protocol settings
#define NB_MAX_NEIGHBORS 25
#define NB_MAX_REGISTERED_PROTOCOLS 2
#define NB_BEACON_PERIOD 4600
#define NB_CHANNEL 0
#define NB_TRANSMISSION_POWER_DB 0
#define NB_RELAX_MILLIS 200 //great care with this one...
#define NB_DAEMON_PERIOD 1000

//default registered protocol settings
#define NB_MAX_AVG_LQI_THRESHOLD 255
#define NB_MIN_AVG_LQI_THRESHOLD 0
#define NB_MAX_AVG_LQI_INVERSE_THRESHOLD 255
#define NB_MIN_AVG_LQI_INVERSE_THRESHOLD 0
#define NB_MAX_LINK_STAB_RATIO_THRESHOLD 100
#define NB_MIN_LINK_STABILITY_RATIO_THRESHOLD 0
#define NB_MAX_LINK_STAB_RATIO_INVERSE_THRESHOLD 100
#define NB_MIN_LINK_STAB_RATIO_INVERSE_THRESHOLD 0
#define NB_CONSECUTIVE_BEACONS_THRESHOLD 1
#define NB_CONSECUTIVE_BEACONS_LOST_THRESHOLD 1
#define NB_MAX_PROTOCOL_PAYLOAD_SIZE 10
#define NB_PROPOSED_TRANSMISSION_POWER_DB 0
#define NB_PROPOSED_TRANSMISSION_POWER_DB_WEIGHT 100
#define NB_PROPOSED_BEACON_PERIOD 1000
#define NB_PROPOSED_BEACON_PERIOD_WEIGHT 100
#define NB_PAYLOAD_OFFSET 0
#define NB_RATIO_DIVIDER 2
#define NB_OLD_DEAD_TIME_PERIOD_WEIGHT 100
#define NB_NEW_DEAD_TIME_PERIOD_WEIGHT 100
#define NB_BEACON_WEIGHT 1
#define NB_LOST_BEACON_WEIGHT 1


#define NB_DEBUG
#ifdef NB_DEBUG
//#define NB_DEBUG_BEACONS
//#define NB_DEBUG_RECEIVE
//#define NB_DEBUG_REGISTER_PROTOCOL
//#define NB_DEBUG_NB_DAEMON
//#define NB_DEBUG_NEIGHBOR_UPDATE_LINK_STAB_RATIO
#define NB_DEBUG_STATS
#endif

