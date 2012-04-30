#define NB_DEBUG
#ifdef NB_DEBUG
//#define NB_DEBUG_BEACONS
//#define NB_DEBUG_RECEIVE
//#define NB_DEBUG_REGISTER_PROTOCOL
//#define NB_DEBUG_NB_DAEMON
//#define NB_DEBUG_NEIGHBOR_UPDATE_LINK_STAB_RATIO
//#define NB_DEBUG_NB_METRICS_DAEMON
#endif
#define NB_DEBUG_STATS
#define NB_COORD_SUPPORT

#ifdef NB_COORD_SUPPORT
#define NB_COORD_SUPPORT_2D
//#define NB_COORD_SUPPORT_3D
//#define NB_COORD_SUPPORT_SHAWN
#define NB_COORD_SUPPORT_ISENSE

#ifdef NB_COORD_SUPPORT_SHAWN
typedef double PositionNumber;
#endif
#ifdef NB_COORD_SUPPORT_ISENSE
typedef uint8_t PositionNumber;
#endif
#endif
