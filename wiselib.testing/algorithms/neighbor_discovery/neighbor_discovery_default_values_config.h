//default link information
#define ND_ENABLE_LQI
#define ND_ENABLE_RSSI
#define ND_ENABLE_LINK_STAB_RATIO

//default ND protocol settings
#define ND_MAX_NEIGHBORS 15
#define ND_MAX_REGISTERED_PROTOCOLS 2
#define ND_BEACON_PERIOD 5000
#define ND_TRANSMISSION_POWER_DB 0
#define ND_RELAX_MILLIS 2500 //great care with this one when tinkering with very minute period predictions/calculations. Could be set to a tiny analogous value of the period like 10%.
#define ND_DAEMON_PERIOD 2500

//benchmark settings
#define ND_STATS_DURATION 120000

//default registered protocol settings
#define ISENSE_MAX_LQI 255
#define ISENSE_MIN_LQI 0
#define ISENSE_MAX_RSSI 255
#define ISENSE_MIN_RSSI 0
#define ND_MAX_AVG_LQI_THRESHOLD ISENSE_MAX_LQI
#define ND_MIN_AVG_LQI_THRESHOLD ISENSE_MIN_LQI
#define ND_MAX_AVG_LQI_INVERSE_THRESHOLD ISENSE_MAX_LQI
#define ND_MIN_AVG_LQI_INVERSE_THRESHOLD ISENSE_MIN_LQI
#define ND_MAX_AVG_RSSI_THRESHOLD ISENSE_MAX_RSSI
#define ND_MIN_AVG_RSSI_THRESHOLD ISENSE_MIN_RSSI
#define ND_MAX_AVG_RSSI_INVERSE_THRESHOLD ISENSE_MAX_RSSI
#define ND_MIN_AVG_RSSI_INVERSE_THRESHOLD ISENSE_MIN_RSSI
#define ND_MAX_LINK_STAB_RATIO_THRESHOLD 100
#define ND_MIN_LINK_STABILITY_RATIO_THRESHOLD 0
#define ND_MAX_LINK_STAB_RATIO_INVERSE_THRESHOLD 100
#define ND_MIN_LINK_STAB_RATIO_INVERSE_THRESHOLD 0
//#define ND_MAX_PROTOCOL_PAYLOAD_SIZE 768
#define ND_MAX_PROTOCOL_PAYLOAD_SIZE 100
#define ND_PROPOSED_TRANSMISSION_POWER_DB 0
#define ND_PROPOSED_TRANSMISSION_POWER_DB_WEIGHT 100
#define ND_PROPOSED_BEACON_PERIOD 1000
#define ND_PROPOSED_BEACON_PERIOD_WEIGHT 100
#define ND_PAYLOAD_OFFSET 0
#define ND_RATIO_DIVIDER 2
#define ND_OLD_DEAD_TIME_PERIOD_WEIGHT 100
#define ND_NEW_DEAD_TIME_PERIOD_WEIGHT 100
#define ND_BEACON_WEIGHT 1
#define ND_LOST_BEACON_WEIGHT 1
