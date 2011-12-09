#ifndef NEIGHBOR_DISCOVERY_H
#define	NEIGHBOR_DISCOVERY_H

#include "util/delegates/delegate.hpp"
#include "util/pstl/vector_static.h"
#include "pgb_payloads_ids.h"
#include "neighbor_discovery_config.h"
#include "neighbor_discovery_message.h"

namespace wiselib
{
	template<	typename Os_P,
				typename Radio_P,
				typename Timer_P,
				typename Debug_P>
	class NeighborDiscovery
	{
	public:
		typedef Os_P Os;
		typedef Radio_P Radio;
		typedef Timer_P Timer;
		typedef Debug_P Debug;
		typedef typename Os_P::Clock Clock;
		typedef typename Radio::node_id_t node_id_t;
		typedef typename Radio::size_t size_t;
		typedef typename Radio::block_data_t block_data_t;
		typedef typename Radio::message_id_t message_id_t;
		typedef typename Clock::time_t time_t;
		typedef typename Radio::ExtendedData ExData;
		typedef typename Radio::TxPower TxPower;
		typedef typename Timer::millis_t millis_t;
		typedef delegate4<void, uint8_t, node_id_t, uint8_t, uint8_t*> event_notifier_delegate_t;
		typedef NeighborDiscovery<Os, Radio, Timer, Debug> self_t;
		typedef NeighborDiscovery_MessageType<Os, Radio> Message;

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		class Neighbor
		{
		public:
			Neighbor()	:
				id								( 0 ),
				total_beacons					( 0 ),
				total_beacons_expected			( 0 ),
				avg_LQI							( 0 ),
				avg_LQI_inverse					( 0 ),
				link_stab_ratio					( 0 ),
				link_stab_ratio_inverse			( 0 ),
				consecutive_beacons				( 0 ),
				consecutive_beacons_lost		( 0 ),
				beacon_period					( 0 ),
				beacon_period_update_counter	( 0 )
			{}
			// --------------------------------------------------------------------
			Neighbor(	node_id_t _id,
						uint32_t _tbeac,
						uint32_t _tbeac_exp,
						uint8_t _alqi,
						uint8_t _alqi_in,
						uint8_t _lsratio,
						uint8_t _lsratio_in,
						uint8_t _cb,
						uint8_t _cb_lost,
						millis_t _bp,
						uint32_t _bpuc,
						time_t _lb )
			{
				id = _id;
				total_beacons = _tbeac;
				total_beacons_expected = _tbeac_exp;
				avg_LQI = _alqi;
				avg_LQI_inverse = _alqi_in;
				link_stab_ratio = _lsratio;
				link_stab_ratio_inverse = _lsratio_in;
				consecutive_beacons = _cb;
				consecutive_beacons_lost =  _cb_lost;
				beacon_period = _bp;
				beacon_period_update_counter = _bpuc;
				last_beacon = _lb;
			}
			// --------------------------------------------------------------------
			~Neighbor()
			{}
			// --------------------------------------------------------------------
			node_id_t get_id()
			{
				return id;
			}
			// --------------------------------------------------------------------
			void set_id( node_id_t _id )
			{
				id = _id;
			}
			// --------------------------------------------------------------------
			uint32_t get_total_beacons()
			{
				return total_beacons;
			}
			// --------------------------------------------------------------------
			void inc_total_beacons( uint32_t _tbeac = 1 )
			{
				total_beacons = total_beacons + _tbeac;
			}
			// --------------------------------------------------------------------
			void set_total_beacons( uint32_t _tbeac )
			{
				total_beacons = _tbeac;
			}
			// --------------------------------------------------------------------
			uint32_t get_total_beacons_expected()
			{
				return total_beacons_expected;
			}
			// --------------------------------------------------------------------
			void inc_total_beacons_expected( uint32_t _tbeac_exp = 1 )
			{
				total_beacons_expected = total_beacons_expected + _tbeac_exp;
			}
			// --------------------------------------------------------------------
			void set_total_beacons_expected( uint32_t _tbeac_exp )
			{
				total_beacons_expected = _tbeac_exp;
			}
			// --------------------------------------------------------------------
			uint8_t get_avg_LQI()
			{
				return avg_LQI;
			}
			// --------------------------------------------------------------------
			void set_avg_LQI( uint8_t _alqi )
			{
				avg_LQI = _alqi;
			}
			// --------------------------------------------------------------------
			void update_avg_LQI( uint8_t _lqi, uint32_t _w = 1 )
			{
				avg_LQI = ( ( avg_LQI * total_beacons ) + _lqi * _w ) / ( total_beacons + _w );
			}
			// --------------------------------------------------------------------
			uint8_t get_avg_LQI_inverse()
			{
				return avg_LQI_inverse;
			}
			// --------------------------------------------------------------------
			void set_avg_LQI_inverse( uint8_t _alqi_in )
			{
				avg_LQI_inverse = _alqi_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_link_stab_ratio()
			{
				return link_stab_ratio;
			}
			// --------------------------------------------------------------------
			void set_link_stab_ratio()
			{
				link_stab_ratio = total_beacons / total_beacons_expected;
			}
			// --------------------------------------------------------------------
			void set_link_stab_ratio( uint8_t _lsratio )
			{
				link_stab_ratio = _lsratio;
			}
			// --------------------------------------------------------------------
			uint8_t get_link_stab_ratio_inverse()
			{
				return link_stab_ratio_inverse;
			}
			// --------------------------------------------------------------------
			void set_link_stab_ratio_inverse( uint8_t _lsratio_in )
			{
				link_stab_ratio_inverse = _lsratio_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons()
			{
				return consecutive_beacons;
			}
			// --------------------------------------------------------------------
			void inc_consecutive_beacons( uint8_t _cb = 1 )
			{
				if ( consecutive_beacons != 0xff )
				{
					consecutive_beacons = consecutive_beacons + _cb;
				}
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons( uint8_t _cb )
			{
				consecutive_beacons = _cb;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons_lost()
			{
				return consecutive_beacons_lost;
			}
			// --------------------------------------------------------------------
			void inc_consecutive_beacons_lost( uint8_t _cb_lost = 1 )
			{
				if ( consecutive_beacons_lost != 0xff )
				{
					consecutive_beacons_lost = consecutive_beacons_lost + _cb_lost;
				}
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons_lost( uint8_t _cb_lost )
			{
				consecutive_beacons_lost = _cb_lost;
			}
			// --------------------------------------------------------------------
			millis_t get_beacon_period()
			{
				return beacon_period;
			}
			// --------------------------------------------------------------------
			void set_beacon_period( millis_t _bp )
			{
				beacon_period = _bp;
			}
			// --------------------------------------------------------------------
			time_t get_last_beacon()
			{
				return last_beacon;
			}
			// --------------------------------------------------------------------
			void set_last_beacon( time_t _lb )
			{
				last_beacon = _lb;
			}
			// --------------------------------------------------------------------
			uint32_t get_beacon_period_update_counter()
			{
				return beacon_period_update_counter;
			}
			// --------------------------------------------------------------------
			void set_beacon_period_update_counter( uint32_t _bpuc )
			{
				if ( beacon_period_update_counter != 0xffffffff )
				{
					beacon_period_update_counter = _bpuc;
				}
			}
			// --------------------------------------------------------------------
			void inc_beacon_period_update_counter( uint32_t _bpuc = 1)
			{
				if ( beacon_period_update_counter != 0xffffffff )
				{
					beacon_period_update_counter = beacon_period_update_counter + _bpuc;
				}
			}
			// --------------------------------------------------------------------
			Neighbor& operator=( const Neighbor& _n )
			{
				id = _n.id;
				total_beacons = _n.total_beacons;
				total_beacons_expected = _n.total_beacons_expected;
				avg_LQI = _n.avg_LQI;
				avg_LQI_inverse = _n.avg_LQI_inverse;
				link_stab_ratio = _n.link_stab_ratio;
				link_stab_ratio_inverse = _n.link_stab_ratio_inverse;
				consecutive_beacons = _n.consecutive_beacons;
				consecutive_beacons_lost = _n.consecutive_beacons_lost;
				beacon_period = _n.beacon_period;
				beacon_period_update_counter = _n.beacon_period_update_counter;
				last_beacon = _n.last_beacon;
				return *this;
			}
			// --------------------------------------------------------------------
			block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				size_t AVG_LQI_POS = 0;
				size_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				size_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				size_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATIO_POS + sizeof(uint8_t);
				write<Os, block_data_t, uint8_t>( _buff + AVG_LQI_POS + _offset, avg_LQI );
				write<Os, block_data_t, uint8_t>( _buff + AVG_LQI_IN_POS + _offset, avg_LQI_inverse );
				write<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_POS + _offset, link_stab_ratio );
				write<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_IN_POS + _offset, link_stab_ratio_inverse );
				return _buff;
			}
			// --------------------------------------------------------------------
			void de_serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				size_t AVG_LQI_POS = 0;
				size_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				size_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				size_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATIO_POS + sizeof(uint8_t);
				avg_LQI = read<Os, block_data_t, uint8_t>( _buff + AVG_LQI_POS + _offset );
				avg_LQI_inverse = read<Os, block_data_t, uint8_t>( _buff + AVG_LQI_IN_POS + _offset );
				link_stab_ratio = read<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_POS + _offset );
				link_stab_ratio_inverse = read<Os, block_data_t, uint8_t>( _buff + LINK_STAB_RATIO_IN_POS + _offset );
			}
			// --------------------------------------------------------------------
			size_t serial_size()
			{
				size_t AVG_LQI_POS = 0;
				size_t AVG_LQI_IN_POS = AVG_LQI_POS + sizeof(uint8_t);
				size_t LINK_STAB_RATIO_POS = AVG_LQI_IN_POS + sizeof(uint8_t);
				size_t LINK_STAB_RATIO_IN_POS = LINK_STAB_RATIO_POS + sizeof(uint8_t);
				return LINK_STAB_RATIO_IN_POS + sizeof( uint8_t );
			}
			// --------------------------------------------------------------------
			void print( Debug& debug )
			{
				debug.debug( "id : %i", id );
				debug.debug( "total_beacons : %i", total_beacons );
				debug.debug( "total_beacons_expected : %i", total_beacons_expected );
				debug.debug( "avg_LQI : %i", avg_LQI );
				debug.debug( "avg_LQI_inverse : %i", avg_LQI_inverse );
				debug.debug( "link_stab_ratio : %i", link_stab_ratio );
				debug.debug( "link_stab_ratio_inverse : %i", link_stab_ratio_inverse );
				debug.debug( "consecutive_beacons : %i", consecutive_beacons );
				debug.debug( "consecutive_beacons_lost : %i", consecutive_beacons_lost );
				debug.debug( "beacon_period : %d", beacon_period );
				debug.debug( "beacon_period_update_counter : %d", beacon_period_update_counter );
			}
			// --------------------------------------------------------------------
		private:
			node_id_t id;
			uint32_t total_beacons;
			uint32_t total_beacons_expected;
			uint8_t avg_LQI;
			uint8_t avg_LQI_inverse;
			uint8_t link_stab_ratio;
			uint8_t link_stab_ratio_inverse;
			uint8_t consecutive_beacons;
			uint8_t consecutive_beacons_lost;
			millis_t beacon_period;
			uint32_t beacon_period_update_counter;
			time_t last_beacon;
		};
		typedef vector_static<Os, Neighbor, NB_MAX_NEIGHBORS> Neighbor_vector;
		typedef typename Neighbor_vector::iterator Neighbor_vector_iterator;

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		class ProtocolPayload
		{
		public:
			ProtocolPayload() :
				payload_size		( NB_MAX_PROTOCOL_PAYLOAD_SIZE )
			{}
			// --------------------------------------------------------------------
			ProtocolPayload( uint8_t _pid, size_t _ps, block_data_t* _pd, size_t _offset = 0 )
			{
				protocol_id = _pid;
				if ( _ps <= NB_MAX_PROTOCOL_PAYLOAD_SIZE )
				{
					payload_size = _ps;
					for ( size_t i = 0; i < payload_size; i++ )
					{
						payload_data[i] = _pd[i + _offset];
					}
				}
			}
			// --------------------------------------------------------------------
			~ProtocolPayload()
			{}
			// --------------------------------------------------------------------
			uint8_t get_protocol_id()
			{
				return protocol_id;
			}
			// --------------------------------------------------------------------
			void set_protocol_id( uint8_t _pid )
			{
				protocol_id = _pid;
			}
			// --------------------------------------------------------------------
			size_t get_payload_size()
			{
				return payload_size;
			}
			// --------------------------------------------------------------------
			size_t get_max_payload_size()
			{
				return NB_MAX_PROTOCOL_PAYLOAD_SIZE;
			}
			// --------------------------------------------------------------------
			void set_payload_size( size_t _ps )
			{
				if ( _ps <= NB_MAX_PROTOCOL_PAYLOAD_SIZE )
				{
					payload_size = _ps;
				}
				else
				{
					payload_size = NB_MAX_PROTOCOL_PAYLOAD_SIZE;
				}
			}
			// --------------------------------------------------------------------
			block_data_t* get_payload_data()
			{
				return payload_data;
			}
			// --------------------------------------------------------------------
			void set_payload_data( block_data_t* _pd, size_t _offset = 0 )
			{
				for ( size_t i = 0; i < payload_size; i++ )
				{
					payload_data[i] = _pd[i + _offset];
				}
			}
			// --------------------------------------------------------------------
			void set_payload( block_data_t* _pd, size_t _ps, size_t _offset = 0 )
			{
				if ( _ps <= NB_MAX_PROTOCOL_PAYLOAD_SIZE )
				{
					payload_size = _ps;
					for ( size_t i = 0; i < payload_size; i++ )
					{
						payload_data[i] = _pd[i + _offset];
					}
				}
			}
			// --------------------------------------------------------------------
			ProtocolPayload& operator=( const ProtocolPayload& _pp )
			{
				protocol_id = _pp.protocol_id;
				payload_size = _pp.payload_size;
				for ( size_t i = 0 ; i < payload_size; i++ )
				{
					payload_data[i] = _pp.payload_data[i];
				}
				return *this;
			}
			// --------------------------------------------------------------------
			void print( Debug& debug )
			{
				debug.debug( "protocol_id : %i ", protocol_id );
				debug.debug( "max_payload_size : %i", NB_MAX_PROTOCOL_PAYLOAD_SIZE );
				debug.debug( "payload_size : %i ", payload_size );
				for ( size_t i = 0; i < payload_size; i++ )
				{
					debug.debug( "payload %i 'th byte : %i", i, payload_data[i] );
				}
			}
			// --------------------------------------------------------------------
			block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				size_t PROTOCOL_ID_POS = 0;
				size_t PAYLOAD_SIZE_POS = PROTOCOL_ID_POS + sizeof(uint8_t);
				size_t PAYLOAD_DATA_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
				write<Os, block_data_t, uint8_t>( _buff + PROTOCOL_ID_POS + _offset, protocol_id );
				write<Os, block_data_t, size_t>( _buff + PAYLOAD_SIZE_POS + _offset, payload_size );
				for ( size_t i = 0 ; i < payload_size; i++ )
				{
					_buff[PAYLOAD_DATA_POS + i + _offset] = payload_data[i];
				}
				return _buff;
			}
			// --------------------------------------------------------------------
			void de_serialize( block_data_t* _buff, size_t _offset = 0 )
			{
				size_t PROTOCOL_ID_POS = 0;
				size_t PAYLOAD_SIZE_POS = PROTOCOL_ID_POS + sizeof(uint8_t);
				size_t PAYLOAD_DATA_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
				protocol_id = read<Os, block_data_t, uint8_t>( _buff + PROTOCOL_ID_POS + _offset );
				payload_size = read<Os, block_data_t, size_t>( _buff + PAYLOAD_SIZE_POS + _offset );
				for ( size_t i = 0 ; i < payload_size; i++ )
				{
					 payload_data[i] = _buff[PAYLOAD_DATA_POS + i + _offset];
				}
			}
			// --------------------------------------------------------------------
			size_t serial_size()
			{
				size_t PROTOCOL_ID_POS = 0;
				size_t PAYLOAD_SIZE_POS = PROTOCOL_ID_POS + sizeof(uint8_t);
				size_t PAYLOAD_DATA_POS = PAYLOAD_SIZE_POS + sizeof(size_t);
				return PAYLOAD_DATA_POS + sizeof( block_data_t) * payload_size;
			}
			// --------------------------------------------------------------------
		private:
			uint8_t protocol_id;
			size_t payload_size;
			block_data_t payload_data[NB_MAX_PROTOCOL_PAYLOAD_SIZE];
		};
		typedef vector_static<Os, ProtocolPayload, NB_MAX_REGISTERED_PROTOCOLS> ProtocolPayload_vector;
		typedef typename ProtocolPayload_vector::iterator ProtocolPayload_vector_iterator;

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		class ProtocolSettings
		{
		public:
			ProtocolSettings()	:
				max_avg_LQI_threshold 					( NB_MAX_AVG_LQI_THRESHOLD ),
				min_avg_LQI_threshold 					( NB_MIN_AVG_LQI_THRESHOLD ),
				max_avg_LQI_inverse_threshold 			( NB_MAX_AVG_LQI_INVERSE_THRESHOLD ),
				min_avg_LQI_inverse_threshold 			( NB_MIN_AVG_LQI_INVERSE_THRESHOLD ),
				max_link_stab_ratio_threshold 			( NB_MAX_LINK_STAB_RATIO_THRESHOLD ),
				min_link_stab_ratio_threshold 			( NB_MIN_LINK_STABILITY_RATIO_THRESHOLD ),
				max_link_stab_ratio_inverse_threshold	( NB_MAX_LINK_STAB_RATIO_INVERSE_THRESHOLD ),
				min_link_stab_ratio_inverse_threshold	( NB_MIN_LINK_STAB_RATIO_INVERSE_THRESHOLD ),
				consecutive_beacons_threshold 			( NB_CONSECUTIVE_BEACONS_THRESHOLD ),
				consecutive_beacons_lost_threshold		( NB_CONSECUTIVE_BEACONS_LOST_THRESHOLD ),
				events_flag								(	NEW_NB	|	NEW_NB_BIDI		|	NEW_PAYLOAD		|	NEW_PAYLOAD_BIDI	|
															LOST_NB	|	LOST_NB_BIDI	|	TRANS_DB_UPDATE	|	BEACON_PERIOD_UPDATE	),
				proposed_transmission_power_dB			( NB_PROPOSED_TRANSMISSION_POWER_DB ),
				proposed_transmission_power_dB_weight	( NB_PROPOSED_TRANSMISSION_POWER_DB_WEIGHT ),
				proposed_beacon_period					( NB_PROPOSED_BEACON_PERIOD ),
				proposed_beacon_period_weight			( NB_PROPOSED_BEACON_PERIOD_WEIGHT ),
				overflow_strategy						( RATIO_DIVIDER ),
				ratio_divider							( NB_RATIO_DIVIDER )
			{}
			// --------------------------------------------------------------------
			ProtocolSettings(	uint8_t _maxLQI,
								uint8_t _minLQI,
								uint8_t _maxLQI_in,
								uint8_t _minLQI_in,
								uint8_t _maxlsr,
								uint8_t _minlsr,
								uint8_t _maxlsr_in,
								uint8_t _minlsr_in,
								uint8_t _cb,
								uint8_t _cblost,
								uint8_t _ef,
								int8_t _tp_dB,
								uint8_t _tp_dB_w,
								millis_t _pb,
								uint8_t _pb_w,
								uint8_t _ofs,
								uint32_t _rd,
								ProtocolPayload _pp )
			{
				max_avg_LQI_threshold = _maxLQI;
				min_avg_LQI_threshold = _minLQI;
				max_avg_LQI_inverse_threshold = _maxLQI_in;
				min_avg_LQI_inverse_threshold = _minLQI_in;
				max_link_stab_ratio_threshold = _maxlsr;
				min_link_stab_ratio_threshold = _minlsr;
				max_link_stab_ratio_inverse_threshold = _maxlsr_in;
				min_link_stab_ratio_inverse_threshold = _minlsr_in,
				consecutive_beacons_threshold = _cb;
				consecutive_beacons_lost_threshold = _cblost;
				events_flag = _ef;
				proposed_transmission_power_dB = _tp_dB;
				proposed_transmission_power_dB_weight = _tp_dB_w;
				proposed_beacon_period = _pb;
				proposed_beacon_period_weight = _pb_w;
				overflow_strategy = _ofs;
				ratio_divider = _rd;
				protocol_payload = _pp;
			}
			// --------------------------------------------------------------------
			~ProtocolSettings()
			{}
			// --------------------------------------------------------------------
			uint8_t get_max_avg_LQI_threshold()
			{
				return max_avg_LQI_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_avg_LQI_threshold( uint8_t _maxLQI )
			{
				max_avg_LQI_threshold = _maxLQI;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_avg_LQI_threshold()
			{
				return min_avg_LQI_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_avg_LQI_threshold( uint8_t _minLQI )
			{
				min_avg_LQI_threshold = _minLQI;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_avg_LQI_inverse_threshold()
			{
				return max_avg_LQI_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_avg_LQI_inverse_threshold( uint8_t _maxLQI_in )
			{
				max_avg_LQI_inverse_threshold = _maxLQI_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_avg_LQI_inverse_threshold()
			{
				return min_avg_LQI_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_avg_LQI_inverse_threshold( uint8_t _minLQI_in )
			{
				min_avg_LQI_inverse_threshold = _minLQI_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_link_stab_ratio_threshold()
			{
				return max_link_stab_ratio_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_link_stab_ratio_threshold( uint8_t _maxlsr )
			{
				max_link_stab_ratio_threshold = _maxlsr;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_link_stab_ratio_threshold()
			{
				return min_link_stab_ratio_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_link_stab_ratio_threshold( uint8_t _minlsr )
			{
				min_link_stab_ratio_threshold = _minlsr;
			}
			// --------------------------------------------------------------------
			uint8_t get_max_link_stab_ratio_inverse_threshold()
			{
				return max_link_stab_ratio_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_max_link_stab_ratio_inverse_threshold( uint8_t _maxlsr_in )
			{
				max_link_stab_ratio_inverse_threshold = _maxlsr_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_min_link_stab_ratio_inverse_threshold()
			{
				return min_link_stab_ratio_inverse_threshold;
			}
			// --------------------------------------------------------------------
			void set_min_link_stab_ratio_inverse_threshold( uint8_t _minlsr_in )
			{
				min_link_stab_ratio_inverse_threshold = _minlsr_in;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons_threshold()
			{
				return consecutive_beacons_threshold;
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons_threshold( uint8_t _cb )
			{
				consecutive_beacons_threshold = _cb;
			}
			// --------------------------------------------------------------------
			uint8_t get_consecutive_beacons_lost_threshold()
			{
				return consecutive_beacons_lost_threshold;
			}
			// --------------------------------------------------------------------
			void set_consecutive_beacons_lost_threshold( uint8_t _cblost )
			{
				consecutive_beacons_lost_threshold = _cblost;
			}
			// --------------------------------------------------------------------
			uint8_t get_events_flag()
			{
				return events_flag;
			}
			// --------------------------------------------------------------------
			void set_events_flag( uint8_t _ef )
			{
				events_flag = _ef;
			}
			// --------------------------------------------------------------------
			int8_t get_proposed_transmission_power_dB()
			{
				return proposed_transmission_power_dB;
			}
			// --------------------------------------------------------------------
			void set_proposed_transmission_power_dB( int8_t _tp_dB )
			{
				proposed_transmission_power_dB = _tp_dB;
			}
			// --------------------------------------------------------------------
			uint8_t get_proposed_transmission_power_dB_weight()
			{
				return proposed_transmission_power_dB_weight;
			}
			// --------------------------------------------------------------------
			void set_proposed_transmission_power_dB_weight( uint8_t _tp_dB_w )
			{
				proposed_transmission_power_dB_weight = _tp_dB_w;
			}
			// --------------------------------------------------------------------
			millis_t get_proposed_beacon_period()
			{
				return proposed_beacon_period;
			}
			// --------------------------------------------------------------------
			void set_proposed_beacon_period( millis_t _pbp )
			{
				proposed_beacon_period = _pbp;
			}
			// --------------------------------------------------------------------
			uint8_t get_proposed_beacon_period_weight()
			{
				return proposed_beacon_period_weight;
			}
			// --------------------------------------------------------------------
			void set_proposed_beacon_period_weight( uint8_t _pbp_w )
			{
				proposed_beacon_period_weight = _pbp_w;
			}
			// --------------------------------------------------------------------
			uint8_t get_overflow_strategy()
			{
				return overflow_strategy;
			}
			// --------------------------------------------------------------------
			void set_overflow_strategy( uint8_t _ofs )
			{
				overflow_strategy = _ofs;
			}
			// --------------------------------------------------------------------
			uint32_t get_ratio_divider()
			{
				return ratio_divider;
			}
			// --------------------------------------------------------------------
			void set_ratio_divider( uint32_t _rd )
			{
				if ( _rd == 0 )
				{
					_rd = 1;
				}
				ratio_divider = _rd;
			}
			// --------------------------------------------------------------------
			ProtocolPayload get_protocol_payload()
			{
				return protocol_payload;
			}
			// --------------------------------------------------------------------
			ProtocolPayload* get_protocol_payload_ref()
			{
				return &protocol_payload;
			}
			// --------------------------------------------------------------------
			void set_protocol_payload( ProtocolPayload _pp )
			{
				protocol_payload = _pp;
			}
			// --------------------------------------------------------------------
			ProtocolSettings& operator=( const ProtocolSettings& _psett )
			{
				max_avg_LQI_threshold = _psett.max_avg_LQI_threshold;
				min_avg_LQI_threshold = _psett.min_avg_LQI_threshold;
				max_avg_LQI_inverse_threshold = _psett.max_avg_LQI_inverse_threshold;
				min_avg_LQI_inverse_threshold = _psett.min_avg_LQI_inverse_threshold;
				max_link_stab_ratio_threshold = _psett.max_link_stab_ratio_threshold;
				min_link_stab_ratio_threshold = _psett.min_link_stab_ratio_threshold;
				max_link_stab_ratio_inverse_threshold = _psett.max_link_stab_ratio_inverse_threshold;
				min_link_stab_ratio_inverse_threshold = _psett.min_link_stab_ratio_inverse_threshold;
				consecutive_beacons_threshold = _psett.consecutive_beacons_threshold;
				consecutive_beacons_lost_threshold = _psett.consecutive_beacons_lost_threshold;
				events_flag = _psett.events_flag;
				proposed_transmission_power_dB = _psett.proposed_transmission_power_dB;
				proposed_transmission_power_dB_weight = _psett.proposed_transmission_power_dB_weight;
				proposed_beacon_period = _psett.proposed_beacon_period;
				proposed_beacon_period_weight = _psett.proposed_beacon_period_weight;
				overflow_strategy = _psett.overflow_strategy;
				ratio_divider = _psett.ratio_divider;
				protocol_payload = _psett.protocol_payload;
				return *this;
			}
			// --------------------------------------------------------------------
			void print( Debug& debug )
			{
				debug.debug( "protocol_settings:");
				debug.debug( "max_avg_LQI_threshold : %i", max_avg_LQI_threshold );
				debug.debug( "min_avg_LQI_threshold : %i", min_avg_LQI_threshold );
				debug.debug( "max_avg_LQI_inverse_threshold : %i", max_avg_LQI_inverse_threshold );
				debug.debug( "min_avg_LQI_inverse_threshold : %i", min_avg_LQI_inverse_threshold );
				debug.debug( "max_link_stab_ratio_threshold : %i", max_link_stab_ratio_threshold );
				debug.debug( "min_link_stab_ratio_threshold : %i", min_link_stab_ratio_threshold );
				debug.debug( "max_link_stab_ratio_inverse_threshold : %i", max_link_stab_ratio_inverse_threshold );
				debug.debug( "min_link_stab_ratio_inverse_threshold : %i", min_link_stab_ratio_inverse_threshold );
				debug.debug( "consecutive_beacons_threshold : %i", consecutive_beacons_threshold );
				debug.debug( "consecutive_beacons_lost_threshold : %i", consecutive_beacons_lost_threshold );
				debug.debug( "events_flag : %i ", events_flag );
				debug.debug( "transmission_power_dB : %i ", proposed_transmission_power_dB );
				debug.debug( "transmission_power_dB_weight : %i ", proposed_transmission_power_dB_weight );
				debug.debug( "proposed_beacon_period : %d ", proposed_beacon_period );
				debug.debug( "proposed_beacon_period_weight : %i", proposed_beacon_period_weight );
				debug.debug( "overflow_strategy : %i", overflow_strategy );
				debug.debug( "ratio_divider : %d", ratio_divider );
				protocol_payload.print( debug );
			}
			// --------------------------------------------------------------------
			enum overflow_strategy
			{
				RESET_TOTAL_BEACONS = 1,
				RESET_TOTAL_BEACONS_EXPECTED = 2,
				RESET_STAB = 4,
				RESET_STAB_INVERSE = 8,
				RESET_AVG_LQI = 16,
				RESET_AVG_LQI_INVERSE = 32,
				RATIO_DIVIDER = 64
			};
			// --------------------------------------------------------------------
			enum event_codes
			{
				NEW_NB = 1,
				NEW_NB_BIDI = 2,
				NEW_PAYLOAD = 4,
				NEW_PAYLOAD_BIDI = 8,
				LOST_NB = 16,
				LOST_NB_BIDI = 32,
				TRANS_DB_UPDATE = 64,
				BEACON_PERIOD_UPDATE = 128
			};
			// --------------------------------------------------------------------
		private:
			uint8_t max_avg_LQI_threshold;
			uint8_t min_avg_LQI_threshold;
			uint8_t max_avg_LQI_inverse_threshold;
			uint8_t min_avg_LQI_inverse_threshold;
			uint8_t max_link_stab_ratio_threshold;
			uint8_t min_link_stab_ratio_threshold;
			uint8_t max_link_stab_ratio_inverse_threshold;
			uint8_t min_link_stab_ratio_inverse_threshold;
			uint8_t consecutive_beacons_threshold;
			uint8_t consecutive_beacons_lost_threshold;
			uint8_t events_flag;
			int8_t proposed_transmission_power_dB;
			uint8_t proposed_transmission_power_dB_weight;
			millis_t proposed_beacon_period;
			uint8_t proposed_beacon_period_weight;
			uint8_t overflow_strategy;
			uint32_t ratio_divider;
			ProtocolPayload protocol_payload;
		};

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		class Protocol
		{
		public:
			Protocol() :
				protocol_id					( 0 ),
				event_notifier_callback		( event_notifier_delegate_t::template from_method<Protocol, &Protocol::null_callback > ( this ) )
			{}
			// --------------------------------------------------------------------
			~Protocol()
			{}
			// --------------------------------------------------------------------
			void set_protocol_id( uint8_t _pid )
			{
				protocol_id = _pid;
				settings.get_protocol_payload_ref()->set_protocol_id( _pid );
			}
			// --------------------------------------------------------------------
			uint8_t get_protocol_id()
			{
				return protocol_id;
			}
			// --------------------------------------------------------------------
			event_notifier_delegate_t get_event_notifier_callback()
			{
				return event_notifier_callback;
			}
			// --------------------------------------------------------------------
			template<class T, void(T::*TMethod)(uint8_t, node_id_t, uint8_t, uint8_t*) >
			void set_event_notifier_callback( T *_obj_pnt )
			{
				event_notifier_callback = event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt );
			}
			// --------------------------------------------------------------------
			void set_event_notifier_callback( event_notifier_delegate_t _enc )
			{
				event_notifier_callback = _enc;
			}
			// --------------------------------------------------------------------
			void reset_event_notifier_callback()
			{
				event_notifier_callback	= event_notifier_delegate_t::template from_method<Protocol, &Protocol::null_callback > ( this );
			}
			// --------------------------------------------------------------------
			ProtocolSettings get_protocol_settings()
			{
				return settings;
			}
			// --------------------------------------------------------------------
			ProtocolSettings* get_protocol_settings_ref()
			{
				return &settings;
			}
			// --------------------------------------------------------------------
			void set_protocol_settings( ProtocolSettings _ps )
			{
				settings = _ps;
			}
			// --------------------------------------------------------------------
			Neighbor_vector* get_neighborhood_ref()
			{
				return &neighborhood;
			}
			// --------------------------------------------------------------------
			Neighbor_vector get_neighborhood()
			{
				return neighborhood;
			}
			// --------------------------------------------------------------------
			void set_neighborhood( Neighbor_vector _nv )
			{
				neighborhood = _nv;
			}
			// --------------------------------------------------------------------
			Neighbor* get_neighbor_ref( node_id_t _nid )
			{
				for ( Neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
				{
					if (it->get_id() == _nid )
					{
						return &(*it);
					}
				}
				return NULL;
			}
			// --------------------------------------------------------------------
			void resolve_overflow_strategy( node_id_t _nid )
			{
				Neighbor* n_ref = get_neighbor_ref( _nid );
				ProtocolSettings* p_ref = get_protocol_settings_ref();
				if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_TOTAL_BEACONS )
				{
					n_ref->set_total_beacons( 0 );
				}
				if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_TOTAL_BEACONS_EXPECTED )
				{
					n_ref->set_total_beacons_expected( 0 );
				}
				if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_AVG_LQI )
				{
					n_ref->set_avg_LQI( 0 );
				}
				if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_AVG_LQI_INVERSE )
				{
					n_ref->set_avg_LQI_inverse( 0 );
				}
				if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_STAB )
				{
					n_ref->set_link_stab_ratio( 0 );
				}
				if ( p_ref->get_overflow_strategy() & ProtocolSettings::RESET_STAB_INVERSE )
				{
					n_ref->set_link_stab_ratio_inverse( 0 );
				}
				if ( p_ref->get_overflow_strategy() == ProtocolSettings::RATIO_DIVIDER )
				{
					n_ref->set_total_beacons( n_ref->get_total_beacons() / p_ref->get_ratio_divider() );
					n_ref->set_total_beacons_expected( n_ref->get_total_beacons_expected() / p_ref->get_ratio_divider() );
				}
				else
				{
					n_ref->set_total_beacons( n_ref->get_total_beacons() / p_ref->get_ratio_divider() );
					n_ref->set_total_beacons_expected( n_ref->get_total_beacons_expected() / p_ref->get_ratio_divider() );
				}
			}
			// --------------------------------------------------------------------
			Protocol& operator=( const Protocol& _p )
			{
				protocol_id = _p.protocol_id;
				event_notifier_callback = _p.event_notifier_callback;
				settings = _p.settings;
				neighborhood = _p.neighborhood;
				return *this;
			}
			// --------------------------------------------------------------------
			void print( Debug& debug )
			{
				debug.debug( "protocol_id : %i", protocol_id );
				debug.debug( "settings :");
				settings.print( debug );
				debug.debug( "neighborhood :");
				for ( Neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
				{
					debug.debug("------------------");
					it->print( debug );
				}
				debug.debug("------------------");
			}
			// --------------------------------------------------------------------
			void null_callback( uint8_t null_event, node_id_t null_node_id, uint8_t null_len, uint8_t* null_data )
			{}
			// --------------------------------------------------------------------
		private:
			uint8_t protocol_id;
			event_notifier_delegate_t event_notifier_callback;
			ProtocolSettings settings;
			Neighbor_vector neighborhood;
			ProtocolPayload protocol_payload;
		};
		typedef vector_static<Os, Protocol, NB_MAX_REGISTERED_PROTOCOLS> Protocol_vector;
		typedef typename Protocol_vector::iterator Protocol_vector_iterator;

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		class Beacon
		{
			public:
				Beacon() :
					beacon_period					( 0 ),
					beacon_period_update_counter	( 0 )
				{}
				// --------------------------------------------------------------------
				~Beacon()
				{}
				// --------------------------------------------------------------------
				Neighbor_vector get_neighborhood()
				{
					return neighborhood;
				}
				// --------------------------------------------------------------------
				Neighbor_vector* get_neighborhood_ref()
				{
					return &neighborhood;
				}
				// --------------------------------------------------------------------
				void set_neighborhood( Neighbor_vector _nv )
				{
					neighborhood = _nv;
				}
				// --------------------------------------------------------------------
				millis_t get_beacon_period()
				{
					return beacon_period;
				}
				// --------------------------------------------------------------------
				void set_beacon_period( millis_t _bp )
				{
					beacon_period = _bp;
				}
				// --------------------------------------------------------------------
				uint32_t get_beacon_period_update_counter()
				{
					return beacon_period_update_counter;
				}
				// --------------------------------------------------------------------
				void set_beacon_period_update_counter( uint32_t _bpuc )
				{
					beacon_period_update_counter = _bpuc;
				}
				// --------------------------------------------------------------------
				ProtocolPayload_vector get_protocol_payloads()
				{
					return protocol_payloads;
				}
				// --------------------------------------------------------------------
				ProtocolPayload_vector* get_protocol_payloads_ref()
				{
					return &protocol_payloads;
				}
				// --------------------------------------------------------------------
				void set_protocol_payloads( ProtocolPayload_vector _ppv )
				{
					protocol_payloads = _ppv;
				}
				// --------------------------------------------------------------------
				void set_protocol_payloads( Protocol_vector _pv )
				{
					for ( Protocol_vector_iterator it = _pv.begin(); it != _pv.end(); ++it )
					{
						if ( it->get_protocol_settings_ref()->get_protocol_payload_ref()->get_payload_size() > 0 )
						{
							protocol_payloads.push_back( it->get_protocol_settings_ref()->get_protocol_payload() );
						}
					}
				}
				// --------------------------------------------------------------------
				void print( Debug& debug )
				{
					for ( ProtocolPayload_vector_iterator it = protocol_payloads.begin(); it != protocol_payloads.end(); ++it )
					{
						it->print( debug );
					}
					for ( Neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
					{
						it->print( debug );
					}
					debug.debug( "beacon_period : %d", beacon_period );
					debug.debug( "beacon_period_update_counter : %d", beacon_period_update_counter );
				}
				// --------------------------------------------------------------------
				block_data_t* serialize( block_data_t* _buff, size_t _offset = 0 )
				{
					size_t PROTOCOL_PAYLOADS_SIZE_POS = 0;
					size_t PROTOCOL_PAYLOADS_POS = PROTOCOL_PAYLOADS_SIZE_POS + sizeof(size_t);
					size_t pps_size = protocol_payloads.size();
					write<Os, block_data_t, size_t>( _buff + PROTOCOL_PAYLOADS_SIZE_POS + _offset, pps_size );
					size_t pp_size = 0;
					for ( ProtocolPayload_vector_iterator it = protocol_payloads.begin(); it != protocol_payloads.end(); ++it )
					{
						it->serialize( _buff + PROTOCOL_PAYLOADS_POS + pp_size, _offset );
						pp_size = it->serial_size() + pp_size;
					}
					size_t NEIGHBORHOOD_SIZE_POS = PROTOCOL_PAYLOADS_POS + pp_size;
					size_t NEIGHBORHOOD_POS = NEIGHBORHOOD_SIZE_POS + sizeof(size_t);
					size_t ns_size = neighborhood.size();
					write<Os, block_data_t, size_t>( _buff + NEIGHBORHOOD_SIZE_POS + _offset, ns_size );
					size_t n_size = 0;
					for ( Neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
					{
						it->serialize( _buff + NEIGHBORHOOD_POS + n_size, _offset );
						n_size = it->serial_size() + n_size;
					}
					size_t BEACON_PERIOD_POS = NEIGHBORHOOD_POS + n_size;
					size_t BEACON_PERIOD_UPDATE_COUNTER_POS = BEACON_PERIOD_POS + sizeof(millis_t);
					write<Os, block_data_t, millis_t>( _buff + BEACON_PERIOD_POS + _offset, beacon_period );
					write<Os, block_data_t, uint32_t>( _buff + BEACON_PERIOD_UPDATE_COUNTER_POS + _offset, beacon_period_update_counter );
					return _buff;
				}
				// --------------------------------------------------------------------
				void de_serialize( block_data_t* _buff, size_t _offset = 0 )
				{
					size_t PROTOCOL_PAYLOADS_SIZE_POS = 0;
					size_t PROTOCOL_PAYLOADS_POS = PROTOCOL_PAYLOADS_SIZE_POS + sizeof(size_t);
					size_t pps_size = read<Os, block_data_t, size_t>( _buff + PROTOCOL_PAYLOADS_SIZE_POS + _offset );
					protocol_payloads.clear();
					for ( size_t i = 0; i < pps_size; i++ )
					{
						ProtocolPayload pp;
						pp.de_serialize( _buff + PROTOCOL_PAYLOADS_POS, _offset );
						protocol_payloads.push_back( pp );
						PROTOCOL_PAYLOADS_POS = pp.serial_size() + PROTOCOL_PAYLOADS_POS;
					}
					size_t NEIGHBORHOOD_SIZE_POS = PROTOCOL_PAYLOADS_POS;
					size_t NEIGHBORHOOD_POS = NEIGHBORHOOD_SIZE_POS + sizeof(size_t);
					size_t ns_size = read<Os, block_data_t, size_t>( _buff + NEIGHBORHOOD_SIZE_POS + _offset );
					neighborhood.clear();
					for ( size_t i = 0; i < ns_size; i++ )
					{
						Neighbor n;
						n.de_serialize( _buff + NEIGHBORHOOD_POS , _offset );
						neighborhood.push_back( n );
						NEIGHBORHOOD_POS = n.serial_size() + NEIGHBORHOOD_POS;
					}
					size_t BEACON_PERIOD_POS = NEIGHBORHOOD_POS;
					size_t BEACON_PERIOD_UPDATE_COUNTER_POS = BEACON_PERIOD_POS + sizeof(millis_t);
					beacon_period = read<Os, block_data_t, millis_t>( _buff + BEACON_PERIOD_POS + _offset );
					beacon_period_update_counter = read<Os, block_data_t, uint32_t>( _buff + BEACON_PERIOD_UPDATE_COUNTER_POS + _offset );
				}
				// --------------------------------------------------------------------
				size_t serial_size()
				{
					uint8_t pp_size = 0;
					for ( ProtocolPayload_vector_iterator it = protocol_payloads.begin(); it != protocol_payloads.end(); ++it )
					{
						pp_size = it->serial_size() + pp_size;
					}
					uint8_t n_size = 0;
					for ( Neighbor_vector_iterator it = neighborhood.begin(); it != neighborhood.end(); ++it )
					{
						n_size = it->serial_size() + n_size;
					}
					size_t PROTOCOL_PAYLOADS_SIZE_POS = 0;
					size_t PROTOCOL_PAYLOADS_POS = PROTOCOL_PAYLOADS_SIZE_POS + sizeof(size_t);
					size_t NEIGHBORHOOD_SIZE_POS = PROTOCOL_PAYLOADS_POS + pp_size;
					size_t NEIGHBORHOOD_POS = NEIGHBORHOOD_SIZE_POS + sizeof(size_t);
					size_t BEACON_PERIOD_POS = NEIGHBORHOOD_POS + n_size;
					size_t BEACON_PERIOD_UPDATE_COUNTER_POS = BEACON_PERIOD_POS + sizeof(millis_t);
					return BEACON_PERIOD_UPDATE_COUNTER_POS + sizeof(uint32_t);
				}
				// --------------------------------------------------------------------
			private:
				ProtocolPayload_vector protocol_payloads;
				Neighbor_vector neighborhood;
				millis_t beacon_period;
				uint32_t beacon_period_update_counter;
		};

		// --------------------------------------------------------------------
		// --------------------------------------------------------------------
		// --------------------------------------------------------------------

		NeighborDiscovery()	:
			status								( WAITING_STATUS ),
			beacon_period						( NB_BEACON_PERIOD ),
			channel								( NB_CHANNEL ),
			transmission_power_dB				( NB_TRANSMISSION_POWER_DB ),
			protocol_max_payload_size			( NB_MAX_PROTOCOL_PAYLOAD_SIZE ),
			transmission_power_dB_strategy		( FIXED_PERIOD ),
			protocol_max_payload_size_strategy	( FIXED_PAYLOAD_SIZE ),
			beacon_period_strategy				( FIXED_TRANSM ),
			relax_millis						( NB_RELAX_MILLIS )
		{};
		// --------------------------------------------------------------------
		~NeighborDiscovery()
		{};
		// --------------------------------------------------------------------
		void enable()
		{
			Protocol p;
			Neighbor n;
			n.set_id( radio().id() );
			Neighbor_vector neighbors;
			neighbors.push_back( n );
			ProtocolPayload pp;
			pp.set_protocol_id( NB_PROTOCOL_ID );
			pp.set_payload_size( 0 );
			uint8_t ef = 			ProtocolSettings::NEW_NB|ProtocolSettings::NEW_NB_BIDI|ProtocolSettings::NEW_PAYLOAD|ProtocolSettings::NEW_PAYLOAD_BIDI|
									ProtocolSettings::LOST_NB|ProtocolSettings::LOST_NB_BIDI|ProtocolSettings::TRANS_DB_UPDATE|ProtocolSettings::BEACON_PERIOD_UPDATE;
			ProtocolSettings ps( 255, 0, 255, 0, 100, 0, 100, 0, 5, 5, ef, -6, 100, 3000, 100, ProtocolSettings::RATIO_DIVIDER, 2, pp );
			p.set_protocol_id( NB_PROTOCOL_ID );
			p.set_neighborhood( neighbors );
			p.set_protocol_settings( ps );
			protocols.push_back( p );
			set_status( ACTIVE_STATUS );
			radio().enable_radio();
			recv_callback_id_ = radio().template reg_recv_callback<self_t, &self_t::receive>( this );
			beacon();
		};
		// --------------------------------------------------------------------
		void disable()
		{
			set_status( WAITING_STATUS );
			protocols.clear();
			radio().template unreg_recv_callback( recv_callback_id_ );
		};
		// --------------------------------------------------------------------
		void send( node_id_t _dest, size_t _len, block_data_t* _data, message_id_t _msg_id )
		{
			Message message;
			message.set_msg_id( _msg_id );
			message.set_payload( _len, _data );
			TxPower power;
			power.set_dB( get_transmission_power_dB() );
			radio().set_channel( get_channel() );
			radio().set_power( power );
			radio().send( _dest, message.buffer_size(), (uint8_t*) &message );
		}
		// --------------------------------------------------------------------
		void beacon( void* _data = NULL )
		{
			if ( get_status() == ACTIVE_STATUS )
			{
				//Protocol* p_ptr = NULL;
				//Protocol** p_ptrptr = &p_ptr;
				Protocol* p_ptr = get_protocol_ref( NB_PROTOCOL_ID );
				debug().debug( "b4 check:");
				if ( p_ptr != NULL )
				{
					//debug().debug( "before" );
					Neighbor_vector nv = p_ptr->get_neighborhood();
					for ( Neighbor_vector_iterator it = nv.begin(); it != nv.end(); ++it )
					{
						it->print( debug() );
					}
					debug().debug( "after check" );

					for ( Neighbor_vector_iterator it = p_ptr->get_neighborhood_ref()->begin(); it != p_ptr->get_neighborhood_ref()->end(); ++it )
					{
						it->print( debug() );
					}
					debug().debug( "after check" );

					//p_ptr->get_protocol_settings_ref()->print( debug() );
					debug().debug( "check ps");
					p_ptr->print( debug() );
					ProtocolSettings ps = p_ptr->get_protocol_settings();
					ps.print( debug() );
					//p_ptr->print( debug() );
					//Neighbor* n = p_ptr->get_neighbor_ref( radio().id() );
					//n->set_beacon_period( get_beacon_period() );
					//n->inc_beacon_period_update_counter();
					//debug().debug("***********1**************");
					//n->print( debug() );

					//Beacon b;
					//b.print( debug() );
					//b.set_neighborhood( nv );
					//b.set_protocol_payloads( protocols );
					//debug().debug( "after ");
					//b.print( debug() );
				//	b.print( debug() );
				//	debug().debug("************2*************");
				//	//p_ptr->get_neighborhood().at( 0 ).print( debug() );
				//	//b.set_neighborhood( *nv );
				//	for ( Neighbor_vector_iterator it = p_ptr->get_neighborhood_ref()->begin(); it != p_ptr->get_neighborhood_ref()->end(); ++it )
				//	{
				//		it->print( debug() );
				//	}
				//	debug().debug("************2*************");
					//b.set_protocol_payloads( protocols );
					//b.set_beacon_period( get_beacon_period() );
					//b.set_beacon_period_update_counter( n->get_beacon_period_update_counter() );

					debug().debug("**********3**************");
				//	debug().debug("**********3**************");
				//
					//b.print( debug() );

					//block_data_t buff[Radio::MAX_MESSAGE_LENGTH];
					//b.serialize( buff );
					//size_t len = b.serial_size();
					//send( Radio::BROADCAST_ADDRESS, len, buff, NB_MESSAGE );
				//	debug().debug("*************4************");
				//	p_ptr->print( debug() );
				//	debug().debug("*************4************");
					//b.print( debug() );
					//timer().template set_timer<self_t, &self_t::beacon> ( get_beacon_period(), this, 0 );
				}
			}
		}
		// --------------------------------------------------------------------
		void receive( node_id_t _from, size_t _len, block_data_t * _msg, ExData const &_ex )
		{
//			time_t current_time = clock().time();
//			message_id_t msg_id = *_msg;
//			Message *message = (Message*) _msg;
//			protocol received_protocol;
//			received_protocol.de_serialize( message->payload() );
//			uint8_t lqi = _ex.link_metric();
//			uint8_t found = 0;
//			neighbor* neighbor_from = received_protocol.get_neighbor_ref( _from );
//			if ( msg_id == NB_MESSAGE )
//			{
//				for ( protocol_vector_iterator pit = protocols.begin(); pit != protocols.end(); ++pit )
//				{
//					for ( neighbor_vector_iterator it1 = pit->get_neighborhood_ref()->begin(); it1 != pit->get_neighborhood_ref()->end(); ++it1 )
//					{
//						if ( neighbor_from->get_id() == it1->get_id() )
//						{
//							uint32_t dead_time = ( clock().seconds( current_time ) - clock().seconds( it1->get_last_beacon() ) ) * 1000 +	( clock().milliseconds( current_time ) - clock().milliseconds( it1->get_last_beacon() ) );
//							found = 1;
//							if ( neighbor_from->get_beacon_period() == it1->get_beacon_period() )
//							{
//								if (	( dead_time < neighbor_from->get_beacon_period() + NB_RELAX_MILLIS ) &&
//										( dead_time > neighbor_from->get_beacon_period() - NB_RELAX_MILLIS ) ) //care relax millis must be considerably lower than the beacon_rate millis... by definition should be tested but around 50ms...
//								{
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected();
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->inc_consecutive_beacons();
//									it1->set_consecutive_beacons_lost( 0 );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//								else
//								{
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected( dead_time / it1->get_beacon_period() );
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->set_consecutive_beacons( 0 );
//									it1->inc_consecutive_beacons_lost( dead_time / it1->get_beacon_period() );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//							}
//							else if ( neighbor_from->get_beacon_period() != it1->get_beacon_period() )
//							{
//								if (	( dead_time < neighbor_from->get_beacon_period() + NB_RELAX_MILLIS ) &&
//										( dead_time > neighbor_from->get_beacon_period() - NB_RELAX_MILLIS ) )
//								{
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected();
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->inc_consecutive_beacons();
//									it1->set_consecutive_beacons_lost( 0 );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//								else
//								{
//									uint32_t last_beacon_period_update = neighbor_from->get_beacon_period_update_counter() * neighbor_from->get_beacon_period();
//									millis_t approximate_beacon_period = ( neighbor_from->get_beacon_period() + it1->get_beacon_period() ) / 2;
//									uint32_t dead_time_messages_lost = ( dead_time - last_beacon_period_update ) * approximate_beacon_period;
//									it1->inc_total_beacons();
//									it1->inc_total_beacons_expected( dead_time_messages_lost + neighbor_from->get_beacon_period_update_counter() );
//									it1->set_avg_LQI( lqi );
//									it1->set_link_stab_ratio();
//									it1->set_consecutive_beacons( 0 );
//									it1->inc_consecutive_beacons_lost( dead_time_messages_lost + neighbor_from->get_beacon_period_update_counter() );
//									it1->set_beacon_period( neighbor_from->get_beacon_period() );
//									it1->set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//									it1->set_last_beacon( neighbor_from->get_last_beacon() );
//								}
//							}
//							for ( neighbor_vector_iterator it2 = received_protocol.get_neighborhood_ref()->begin(); it2 != received_protocol.get_neighborhood_ref()->end(); ++it2 )
//							{
//								if ( it2->get_id() == radio().id() )
//								{
//									it1->set_avg_LQI_inverse( it2->get_avg_LQI() );
//									it1->set_link_stab_ratio_inverse( it2->get_link_stab_ratio() );
//								}
//							}
//							//TODO
//							//flag process
//							//notify_receiver();
//						}
//					}
//					if ( !found )
//					{
//						neighbor new_neighbor;
//						new_neighbor.set_id( _from );
//						new_neighbor.set_total_beacons( 1 );
//						new_neighbor.set_total_beacons_expected( 1 );
//						new_neighbor.set_avg_LQI( lqi );
//						new_neighbor.set_link_stab_ratio();
//						new_neighbor.set_consecutive_beacons( 1 );
//						new_neighbor.set_consecutive_beacons_lost( 0 );
//						new_neighbor.set_beacon_period( neighbor_from->get_beacon_period() );
//						new_neighbor.set_beacon_period_update_counter( neighbor_from->get_beacon_period_update_counter() );
//						new_neighbor.set_last_beacon( clock().time() );
//						for ( neighbor_vector_iterator it2 = received_protocol.get_neighborhood_ref()->begin(); it2 != received_protocol.get_neighborhood_ref()->end(); ++it2 )
//						{
//							if ( it2->get_id() == radio().id() )
//							{
//								new_neighbor.set_avg_LQI_inverse( it2->get_avg_LQI() );
//								new_neighbor.set_link_stab_ratio_inverse( it2->get_link_stab_ratio() );
//							}
//						}
//						pit->get_neighborhood_ref()->push_back( new_neighbor );
//						//TODO
//						//flag process
//						//notify_receiver();
//					}
//				}
//			}
		}
		// --------------------------------------------------------------------
		template<class T, void(T::*TMethod)(uint8_t, node_id_t, uint8_t, uint8_t*) >
		uint8_t register_protocol( uint8_t _pid, ProtocolSettings _psett, T *_obj_pnt )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			if ( ( _psett.get_payload_size() > protocol_max_payload_size ) && ( protocol_max_payload_size_strategy == FIXED_PAYLOAD_SIZE ) )
			{
				return PAYLOAD_SIZE_OUT_OF_BOUNDS;
			}
			uint8_t total_payload_size = 0;
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_protocol_settings().get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(message_id_t) + sizeof(size_t) + _psett.get_payload_size() > Radio::MAX_MSG_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _pid )
				{
					return PROT_NUM_IN_USE;
				}
			}
			Protocol p;
			p.set_protocol_id( _pid );
			p.set_protocol_settings( _psett );
			p.set_event_notifier_callback( event_notifier_delegate_t::template from_method<T, TMethod > ( _obj_pnt ) );
			protocols.push_back( p );
			return SUCCESS;
		}
		// --------------------------------------------------------------------
		uint8_t register_protocol( Protocol p )
		{
			if ( protocols.max_size() == protocols.size() )
			{
				return PROT_LIST_FULL;
			}
			if ( ( p.get_protocol_settings().get_payload_size() > protocol_max_payload_size ) && ( protocol_max_payload_size_strategy == FIXED_PAYLOAD_SIZE ) )
			{
				return PAYLOAD_SIZE_OUT_OF_BOUNDS;
			}
			uint8_t total_payload_size = 0;
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				total_payload_size = it->get_protocol_settings().get_payload_size() + total_payload_size;
			}
			if ( total_payload_size + sizeof(message_id_t) + sizeof(size_t) + p.get_protocol_settings().get_payload_size() > Radio::MAX_MESSAGE_LENGTH )
			{
				return NO_PAYLOAD_SPACE;
			}
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == p.get_protocol_id() )
				{
					return PROT_NUM_IN_USE;
				}
			}
			protocols.push_back( p );
			return SUCCESS;
		}
		// --------------------------------------------------------------------
		uint8_t unregister_protocol( uint8_t _pid )
		{
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _pid )
				{
					protocols.erase( it );
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
		Protocol* get_protocol_ref( uint8_t _pid )
		{
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _pid )
				{
					return &(*it);
				}
			}
			return NULL;
		}
		// --------------------------------------------------------------------
		uint8_t set_protocol( uint8_t _pid, Protocol _pptr )
		{
			for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
			{
				if ( it->get_protocol_id() == _pid )
				{
					_pptr = *it;
					return SUCCESS;
				}
			}
			return INV_PROT_ID;
		}
		// --------------------------------------------------------------------
		int get_status()
		{
			return status;
		}
		// --------------------------------------------------------------------
		void set_status( int _st )
		{
			status = _st;
		}
		// --------------------------------------------------------------------
		millis_t get_beacon_period()
		{
			if ( beacon_period_strategy == FIXED_PERIOD )
			{
				return beacon_period;
			}
			else if ( beacon_period_strategy == LEAST_PERIOD )
			{
				millis_t least = 0xfff;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					if ( it->get_protocol_settings_ref()->get_proposed_beacon_period() < least )
					{
						least = it->get_protocol_settings_ref()->get_proposed_beacon_period();
					}
				}
				return least;
			}
			else if ( beacon_period_strategy == MEAN_PERIOD )
			{
				millis_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_beacon_period() + sum;
					num = num + 1;
				}
				millis_t mean = sum / num;
				return mean;
			}
			else if ( beacon_period_strategy == WEIGHTED_MEAN_PERIOD )
			{
				millis_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_beacon_period() * it->get_protocol_settings_ref()->get_proposed_beacon_period_weight() + sum;
					num = it->get_protocol_settings_ref()->get_proposed_beacon_period_weight() + num;
				}
				millis_t weighted_mean = sum / num;
				return weighted_mean;
			}
			return beacon_period;
		}
		// --------------------------------------------------------------------
		void set_beacon_period( millis_t _bp )
		{
			beacon_period =_bp;
		}
		// --------------------------------------------------------------------
		int8_t get_transmission_power_dB()
		{
			if ( transmission_power_dB_strategy == FIXED_TRANSM )
			{
				return transmission_power_dB;
			}
			else if ( transmission_power_dB_strategy == LEAST_TRANSM )
			{
				int8_t least = 128;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					if ( it->get_protocol_settings_ref()->get_proposed_transmission_power_dB() < least )
					{
						least = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB();
					}
				}
				return least;
			}
			else if ( transmission_power_dB_strategy == MEAN_TRANSM )
			{
				int8_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB() + sum;
					num = num + 1;
				}
				int8_t mean = sum / num;
				return mean;
			}
			else if ( transmission_power_dB_strategy == WEIGHTED_MEAN_TRANSM )
			{
				int8_t sum, num;
				sum = num = 0;
				for ( Protocol_vector_iterator it = protocols.begin(); it != protocols.end(); ++it )
				{
					sum = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB() * it->get_protocol_settings_ref()->get_proposed_transmission_power_dB_weight() + sum;
					num = it->get_protocol_settings_ref()->get_proposed_transmission_power_dB_weight() + num;
				}
				int8_t weighted_mean = sum / num;
				return weighted_mean;
			}
			return transmission_power_dB;
		}
		// --------------------------------------------------------------------
		void set_transmission_power_dB( int8_t _tp_dB )
		{
			 transmission_power_dB = _tp_dB;
		}
		// --------------------------------------------------------------------
		uint8_t get_channel()
		{
			return channel;
		}
		// --------------------------------------------------------------------
		void set_channel( uint8_t _ch )
		{
			channel =_ch;
		}
		// --------------------------------------------------------------------
		uint8_t get_protocol_max_payload_size()
		{
			return protocol_max_payload_size;
		}
		// --------------------------------------------------------------------
		void set_transmission_power_dB_strategy( uint8_t _tp_dB_s )
		{
			if ( _tp_dB_s > TP_DB_STRATEGY_NUM_VALUES )
			{
				transmission_power_dB_strategy = FIXED_TRANSM;
			}
			else
			{
				transmission_power_dB_strategy = _tp_dB_s;
			}
		}
		// --------------------------------------------------------------------
		uint8_t get_transmission_power_dB_strategy()
		{
			return transmission_power_dB_strategy;
		}
		// --------------------------------------------------------------------
		void set_beacon_period_strategy( uint8_t _bp_s )
		{
			if ( _bp_s > BP_STRATEGY_NUM_VALUES )
			{
				beacon_period_strategy = FIXED_TRANSM;
			}
			else
			{
				beacon_period_strategy = _bp_s;
			}
		}
		// --------------------------------------------------------------------
		uint8_t get_beacon_period_strategy()
		{
			return beacon_period_strategy;
		}
		// --------------------------------------------------------------------
		void set_protocol_max_payload_size_strategy( uint8_t _mpps_s )
		{
			if ( _mpps_s > PP_STRATEGY_NUM_VALUES )
			{
				protocol_max_payload_size_strategy = FIXED_PAYLOAD_SIZE;
			}
			else
			{
				protocol_max_payload_size_strategy = _mpps_s;
			}
		}
		// --------------------------------------------------------------------
		uint8_t get_protocol_max_payload_size_strategy()
		{
			return protocol_max_payload_size_strategy;
		}
		// --------------------------------------------------------------------
		millis_t get_relax_millis()
		{
			return relax_millis;
		}
		// --------------------------------------------------------------------
		void set_relax_millis( millis_t _lm )
		{
			relax_millis = _lm;
		}
		// --------------------------------------------------------------------
		void init( Radio& _radio, Timer& _timer, Debug& _debug, Clock& _clock )
		{
			radio_ = &_radio;
			timer_ = &_timer;
			debug_ = &_debug;
			clock_ = &_clock;
		}
		// --------------------------------------------------------------------
		Radio& radio()
		{
			return *radio_;
		}
		// --------------------------------------------------------------------
		Clock& clock()
		{
			return *clock_;
		}
		// --------------------------------------------------------------------
		Timer& timer()
		{
			return *timer_;
		}
		// --------------------------------------------------------------------
		Debug& debug()
		{
			return *debug_;
		}
		// --------------------------------------------------------------------
		enum error_codes
		{
			SUCCESS,
			PROT_NUM_IN_USE,
			PROT_LIST_FULL,
			INV_PROT_ID,
			NO_PAYLOAD_SPACE,
			PAYLOAD_SIZE_OUT_OF_BOUNDS,
			ERROR_CODES_NUM_VALUES
		};
		enum neighbor_discovery_status
		{
			ACTIVE_STATUS,
			WAITING_STATUS,
			NB_STATUS_NUM_VALUES
		};
		enum transmission_power_dB_strategies
		{
			FIXED_TRANSM,
			LEAST_TRANSM,
			MEAN_TRANSM,
			WEIGHTED_MEAN_TRANSM,
			TP_DB_STRATEGY_NUM_VALUES
		};
		enum beacon_period_strategies
		{
			FIXED_PERIOD,
			LEAST_PERIOD,
			MEAN_PERIOD,
			WEIGHTED_MEAN_PERIOD,
			BP_STRATEGY_NUM_VALUES
		};
		enum protocol_payload_strategies
		{
			FIXED_PAYLOAD_SIZE,
			DYNAMIC_PAYLOAD_SIZE,
			PP_STRATEGY_NUM_VALUES
		};
		enum message_ids
		{
			NB_MESSAGE = 12
		};
	private:
		uint32_t recv_callback_id_;
        uint8_t status;
        millis_t beacon_period;
        uint8_t channel;
        int8_t transmission_power_dB;
        Protocol_vector protocols;
        uint8_t protocol_max_payload_size;
        uint8_t transmission_power_dB_strategy;
        uint8_t protocol_max_payload_size_strategy;
        uint8_t beacon_period_strategy;
        millis_t relax_millis;
        Radio * radio_;
        Clock * clock_;
        Timer * timer_;
        Debug * debug_;
    };
}

#endif

//			neighbor Test cases:
//
//			Neighbor n1;
//			debug().debug( "neighbor n1:");
//			n1.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n1.get_last_beacon() ), clock().milliseconds( n1.get_last_beacon() ) );
//			Neighbor n2( 1, 2, 3 ,4 ,5 ,6 ,7 ,8 , 9, 10, 11, clock().time() );
//			debug().debug( "----------------------");
//			debug().debug( "Neighbor n2:");
//			n2.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n2.get_last_beacon() ), clock().milliseconds( n2.get_last_beacon() ) );
//			debug().debug( "----------------------");
//			n1.set_id( 11 );
//			n1.set_total_beacons( 22 );
//			n1.set_total_beacons_expected( 33 );
//			n1.set_avg_LQI( 44 );
//			n1.set_avg_LQI_inverse( 55 );
//			n1.set_link_stab_ratio( 66 );
//			n1.set_link_stab_ratio_inverse( 77 );
//			n1.set_consecutive_beacons( 88 );
//			n1.set_consecutive_beacons_lost( 99 );
//			n1.set_beacon_period( 111 );
//			n1.set_beacon_period_update_counter( 222 );
//			n1.set_last_beacon( clock().time() );
//			debug().debug( "Neighbor n1 with getters:");
//			debug().debug( "id: %i", n1.get_id() );
//			debug().debug( "total_beacons : %i", n1.get_total_beacons() );
//			debug().debug( "total_beacons_expected : %i", n1.get_total_beacons_expected() );
//			debug().debug( "avg_LQI : %i", n1.get_avg_LQI() );
//			debug().debug( "avg_LQI_inverse : %i", n1.get_avg_LQI_inverse() );
//			debug().debug( "link_stab_ratio : %i", n1.get_link_stab_ratio() );
//			debug().debug( "link_stab_ratio_inverse : %i", n1.get_link_stab_ratio_inverse() );
//			debug().debug( "consecutive_beacons : %i", n1.get_consecutive_beacons() );
//			debug().debug( "consecutive_beacons_lost : %i", n1.get_consecutive_beacons_lost() );
//			debug().debug( "beacon_period : %d", n1.get_beacon_period() );
//			debug().debug( "beacon_period_update_counter : %d", n1.get_beacon_period_update_counter() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n1.get_last_beacon() ), clock().milliseconds( n1.get_last_beacon() ) );
//			debug().debug( "----------------------");
//			n2 = n1;
//			debug().debug( "Neighbor n2 = n1:");
//			n2.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n2.get_last_beacon() ), clock().milliseconds( n2.get_last_beacon() ) );
//			debug().debug( "----------------------");
//			debug().debug( "n2 serial size: %i ", n2.serial_size() );
//			debug().debug( "----------------------");
//			debug().debug( "neighbor n3:");
//			Neighbor n3;
//			debug().debug( "last_beacon %d:%d", clock().seconds( n3.get_last_beacon() ), clock().milliseconds( n3.get_last_beacon() ) );
//			n3.print( debug() );
//			debug().debug( "----------------------");
//			block_data_t buff[100];
//			n3.de_serialize( n1.serialize( buff, 47 ), 47 );
//			debug().debug( "Neighbor n3 after serialize - de_serialize:");
//			n3.print( debug() );
//			debug().debug( "last_beacon %d:%d", clock().seconds( n3.get_last_beacon() ), clock().milliseconds( n3.get_last_beacon() ) );
//			debug().debug( "----------------------");
//
//			n3.set_total_beacons( 0xfffffffe );
//			debug().debug( "total_beacons : %d", n3.get_total_beacons() );
//			n3.inc_total_beacons();
//			debug().debug( "total_beacons : %d", n3.get_total_beacons() );
//			n3.inc_total_beacons();
//			debug().debug( "total_beacons : %d", n3.get_total_beacons() );
//
//			n3.set_total_beacons_expected( 0xfffffffe );
//			debug().debug( "total_beacons_expected : %d", n3.get_total_beacons_expected() );
//			n3.inc_total_beacons_expected();
//			debug().debug( "total_beacons_expected : %d", n3.get_total_beacons_expected() );
//			n3.inc_total_beacons_expected();
//			debug().debug( "total_beacons_expected : %d", n3.get_total_beacons_expected() );
//
//			n3.set_avg_LQI( 80 );
//			debug().debug( "avg_LQI : %d", n3.get_avg_LQI() );
//			n3.set_total_beacons( 5 );
//			n3.update_avg_LQI( 50, 2 );
//			debug().debug( "avg_LQI : %d", n3.get_avg_LQI() );
//
//			n3.set_consecutive_beacons( 254 );
//			debug().debug( "consecutive_beacons : %d", n3.get_consecutive_beacons() );
//			n3.inc_consecutive_beacons();
//			debug().debug( "consecutive_beacons : %d", n3.get_consecutive_beacons() );
//			n3.inc_consecutive_beacons();
//			debug().debug( "consecutive_beacons : %d", n3.get_consecutive_beacons() );
//
//			n3.set_consecutive_beacons_lost( 254 );
//			debug().debug( "consecutive_beacons_lost : %d", n3.get_consecutive_beacons_lost() );
//			n3.inc_consecutive_beacons_lost();
//			debug().debug( "consecutive_beacons_lost : %d", n3.get_consecutive_beacons_lost() );
//			n3.inc_consecutive_beacons_lost();
//			debug().debug( "consecutive_beacons_lost : %d", n3.get_consecutive_beacons_lost() );
//
//			n3.set_beacon_period_update_counter( 0xfffffffe );
//			debug().debug( "beacon_period_update_counter : %d", n3.get_beacon_period_update_counter() );
//			n3.inc_beacon_period_update_counter();
//			debug().debug( "beacon_period_update_counter beacons_lost : %d", n3.get_beacon_period_update_counter() );
//			n3.inc_beacon_period_update_counter();
//			debug().debug( "beacon_period_update_counter beacons_lost : %d", n3.get_beacon_period_update_counter() );
//
//			ProtocolPayload Test cases:
//
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//			pp1.print( debug() );
//			debug().debug( "max_payload_size : %i", pp1.get_max_payload_size() );
//			debug().debug( "-------------------------------------");
//			ProtocolPayload pp2;
//			pp2.set_protocol_id( 2 );
//			pp2.set_payload_size( 10 );
//			pp2.set_payload_data( buff, 15 );
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			ProtocolPayload pp3 = pp2;
//			pp2 = pp1;
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			pp2.set_payload_size( pp2.get_max_payload_size() + 1 );
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			pp2.set_payload_size( 2 );
//			pp2.print( debug() );
//			debug().debug( "-------------------------------------");
//			pp3.print( debug() );
//			debug().debug( "-------------------------------------");
//			block_data_t buff1[100];
//			pp3.de_serialize( pp2.serialize( buff1, 33 ), 33 );
//			pp3.print( debug() );
//			debug().debug( "-------------------------------------");
//			debug().debug( "pp1 serial_size %i", pp1.serial_size() );
//			debug().debug( "pp2 serial_size %i", pp2.serial_size() );
//			debug().debug( "pp3 serial_size %i", pp3.serial_size() );
//			debug().debug( "-------------------------------------");

//	ProtocolSettings Test cases:
//
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//
//
//			ProtocolSettings ps1;
//			debug().debug( " ProtocolSettings ps1 :");
//			ps1.print( debug() );
//			debug().debug( "------------------------------");
//
//
//			ProtocolSettings ps2( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, pp1 );
//			debug().debug( " Protocol_settings ps2 :");
//			ps2.print( debug() );
//			debug().debug( "------------------------------");
//			debug().debug( " ProtocolSettings ps1 = ps2 :");
//			ps1 = ps2;
//			ps1.print( debug() );
//			debug().debug( "------------------------------");
//			ps1.set_max_avg_LQI_threshold( 11 );
//			ps1.set_min_avg_LQI_threshold( 22 );
//			ps1.set_max_avg_LQI_inverse_threshold( 33 );
//			ps1.set_min_avg_LQI_inverse_threshold( 44 );
//			ps1.set_max_link_stab_ratio_threshold( 55 );
//			ps1.set_min_link_stab_ratio_threshold( 66 );
//			ps1.set_max_link_stab_ratio_inverse_threshold( 77 );
//			ps1.set_min_link_stab_ratio_inverse_threshold( 88 );
//			ps1.set_consecutive_beacons_threshold( 99 );
//			ps1.set_consecutive_beacons_lost_threshold( 111 );
//			ps1.set_events_flag( 222 );
//			ps1.set_proposed_transmission_power_dB( -6 );
//			ps1.set_proposed_transmission_power_dB_weight( 100 );
//			ps1.set_proposed_beacon_period( 2000 );
//			ps1.set_proposed_beacon_period_weight( 80 );
//			ps1.set_overflow_strategy( 255 );
//			ps1.set_ratio_divider( 0 );
//			ps1.set_protocol_payload( pp1 );
//
//			debug().debug( " protocol_settings ps1 setters:");
//			debug().debug( "max_avg_LQI_threshold : %i", ps1.get_max_avg_LQI_threshold() );
//			debug().debug( "min_avg_LQI_threshold : %i", ps1.get_min_avg_LQI_threshold() );
//			debug().debug( "max_avg_LQI_inverse_threshold : %i ", ps1.get_max_avg_LQI_inverse_threshold() );
//			debug().debug( "min_avg_LQI_inverse_threshold : %i ", ps1.get_min_avg_LQI_inverse_threshold() );
//			debug().debug( "max_link_stab_ratio_threshold : %i ", ps1.get_max_link_stab_ratio_threshold() );
//			debug().debug( "min_link_stab_ratio_threshold : %i ", ps1.get_min_link_stab_ratio_threshold() );
//			debug().debug( "max_link_stab_ratio_inverse_threshold : %i ", ps1.get_max_link_stab_ratio_inverse_threshold() );
//			debug().debug( "min_link_stab_ratio_inverse_threshold : %i ", ps1.get_min_link_stab_ratio_inverse_threshold() );
//			debug().debug( "consecutive_beacons_threshold : %i ", ps1.get_consecutive_beacons_threshold() );
//			debug().debug( "consecutive_beacons_lost_threshold : %i ", ps1.get_consecutive_beacons_lost_threshold() );
//			debug().debug( "events_flag : %i ", ps1.get_events_flag() );
//			debug().debug( "proposed_transmission_power_dB : %i", ps1.get_proposed_transmission_power_dB() );
//			debug().debug( "proposed_transmission_power_dB_weight : %i", ps1.get_proposed_transmission_power_dB_weight() );
//			debug().debug( "proposed_beacon_period : %i", ps1.get_proposed_beacon_period() );
//			debug().debug( "proposed_beacon_period_weight : %d", ps1.get_proposed_beacon_period_weight() );
//			debug().debug( "overflow_strategy : %i", ps1.get_overflow_strategy() );
//			debug().debug( "ratio_divider : %d", ps1.get_ratio_divider() );
//			ps1.get_protocol_payload().print( debug() );
//			debug().debug( "------------------------------");
//
// protocol Test Cases:
//
//			Protocol p;
//			p.print( debug() );
//			uint8_t event = 50;
//			node_id_t from = 0;
//			size_t len = 0;
//			block_data_t* data = NULL;
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			Neighbor n0( 10, 20, 30 ,40 ,50 ,60 ,70 ,80 , 90, 100, 110, clock().time() );
//			Neighbor n1( 11, 21, 31 ,41 ,51 ,61 ,71 ,81 , 91, 101, 111, clock().time() );
//			Neighbor n2( 12, 22, 32 ,42 ,52 ,62 ,72 ,82 , 92, 102, 112, clock().time() );
//			Neighbor n3( 13, 23, 33 ,43 ,53 ,63 ,73 ,83 , 93, 103, 113, clock().time() );
//			Neighbor n4( 14, 24, 34 ,44 ,54 ,64 ,74 ,84 , 94, 104, 114, clock().time() );
//			Neighbor n5( 15, 25, 35 ,45 ,55 ,65 ,75 ,85 , 95, 105, 115, clock().time() );
//			Neighbor_vector neighs;
//			neighs.push_back( n0 );
//			neighs.push_back( n1 );
//			neighs.push_back( n2 );
//			neighs.push_back( n3 );
//			neighs.push_back( n4 );
//			neighs.push_back( n5 );
//
//
//
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//			ProtocolSettings ps1( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, pp1 );
//
//			p.set_protocol_id ( 1 );
//			p.template set_event_notifier_callback< self_type, &self_type::sync_neighbors>( this );
//			p.set_protocol_settings( ps1 );
//			p.set_neighborhood( neighs );
//			p.print( debug() );
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			p.reset_event_notifier_callback();
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			p.template set_event_notifier_callback< self_type, &self_type::sync_neighbors>( this );
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "******************************************");
//			p.set_event_notifier_callback( p.get_event_notifier_callback() );
//			p.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "*****************%%%%%%*********************");
//			debug().debug( "protocol id : %i ", p.get_protocol_id() );
//			p.get_protocol_settings().print( debug() );
//			for ( Neighbor_vector_iterator it = p.get_neighborhood_ref()->begin(); it != p.get_neighborhood_ref()->end(); ++it )
//			{
//				it->print( debug() );
//			}
//			debug().debug( "******************************************");
//			p.get_neighbor_ref( 10 )->print( debug() );
//			debug().debug( "******************************************");
//			p.get_protocol_settings_ref()->set_proposed_transmission_power_dB( -9 );
//			p.get_protocol_settings().print( debug() );
//			debug().debug( "*****************%%%%%%*********************");
//			Protocol p2 = p;
//			p2.print( debug() );
//			p2.get_event_notifier_callback()( event, from, len, data );
//			debug().debug( "*****************%%%%%%*********************");
//
//			Neighbor n100 = *(p2.get_neighbor_ref( 10 ));
//			p2.get_neighbor_ref( 10 )->set_total_beacons( 0xffffffff );
//			p2.get_neighbor_ref( 10 )->print( debug() );
//			p2.get_protocol_settings_ref()->set_overflow_strategy( 	ProtocolSettings::RESET_TOTAL_BEACONS|
//																	ProtocolSettings::RESET_TOTAL_BEACONS_EXPECTED|
//																	ProtocolSettings::RESET_STAB|
//																	ProtocolSettings::RESET_STAB_INVERSE|
//																	ProtocolSettings::RESET_AVG_LQI|
//																	ProtocolSettings::RESET_AVG_LQI_INVERSE );
//			debug().debug(" the problem : %d", p2.get_protocol_settings_ref()->get_overflow_strategy() );
//			p2.resolve_overflow_strategy( 10 );
//			p2.get_neighbor_ref( 10 )->print( debug() );
//
//			Beacon Test cases
//			Beacon b1;
//			b1.print( debug() );
//			Neighbor n0( 10, 20, 30 ,40 ,50 ,60 ,70 ,80 , 90, 100, 110, clock().time() );
//			Neighbor n1( 11, 21, 31 ,41 ,51 ,61 ,71 ,81 , 91, 101, 111, clock().time() );
//			Neighbor n2( 12, 22, 32 ,42 ,52 ,62 ,72 ,82 , 92, 102, 112, clock().time() );
//			Neighbor n3( 13, 23, 33 ,43 ,53 ,63 ,73 ,83 , 93, 103, 113, clock().time() );
//			Neighbor n4( 14, 24, 34 ,44 ,54 ,64 ,74 ,84 , 94, 104, 114, clock().time() );
//			Neighbor n5( 15, 25, 35 ,45 ,55 ,65 ,75 ,85 , 95, 105, 115, clock().time() );
//			Neighbor_vector neighs;
//			neighs.push_back( n0 );
//			neighs.push_back( n1 );
//			neighs.push_back( n2 );
//			neighs.push_back( n3 );
//			neighs.push_back( n4 );
//			neighs.push_back( n5 );
//			block_data_t buff[100];
//			for ( size_t i = 0; i < 100; ++i )
//			{
//				buff[i]=i;
//			}
//			ProtocolPayload pp1( 1, 5, buff, 9 );
//			ProtocolPayload pp2( 2, 5, buff, 19 );
//			ProtocolPayload pp3( 3,	5, buff, 29 );
//			ProtocolPayload pp4( 4, 10, buff, 39 );
//			ProtocolPayload pp5( 5, 10, buff, 49 );
//			debug().debug( "********");
//			debug().debug( "pp1 serial_size : %i", pp1.serial_size() );
//			debug().debug( "pp2 serial_size : %i", pp2.serial_size() );
//			debug().debug( "pp3 serial_size : %i", pp3.serial_size() );
//			debug().debug( "pp4 serial_size : %i", pp4.serial_size() );
//			debug().debug( "pp5 serial_size : %i", pp5.serial_size() );
//			debug().debug( "********");
//			debug().debug( "n0 serial_size : %i", n0.serial_size() );
//			debug().debug( "n1 serial_size : %i", n1.serial_size() );
//			debug().debug( "n2 serial_size : %i", n2.serial_size() );
//			debug().debug( "n3 serial_size : %i", n3.serial_size() );
//			debug().debug( "n4 serial_size : %i", n4.serial_size() );
//			debug().debug( "n5 serial_size : %i", n5.serial_size() );
//			debug().debug( "********");
//			ProtocolPayload_vector payloads;
//			payloads.push_back( pp1 );
//			payloads.push_back( pp2 );
//			payloads.push_back( pp3 );
//			payloads.push_back( pp4 );
//			payloads.push_back( pp5 );
//			Beacon b2;
//			b2.set_neighborhood( neighs );
//			b2.set_protocol_payloads( payloads );
//			b2.set_beacon_period( 10000 );
//			b2.set_beacon_period_update_counter( 20000 );
//			b1 = b2;
//			b1.print( debug() );
//
//			debug().debug("***************************************");
//			for ( Neighbor_vector_iterator it = b1.get_neighborhood_ref()->begin(); it != b1.get_neighborhood_ref()->end(); ++it )
//			{
//				it->print( debug() );
//			}
//			debug().debug("***************************************");
//			for ( ProtocolPayload_vector_iterator it = b1.get_protocol_payloads_ref()->begin(); it != b1.get_protocol_payloads_ref()->end(); ++it )
//			{
//				it->print( debug() );
//			}
//			debug().debug("***************************************");
//			b2.print( debug() );
//			debug().debug("***************************************");
//			block_data_t bufff[100];
//			Beacon b3;
//			b3.de_serialize( b2.serialize( bufff, 4 ), 4 );
//			b3.print( debug() );
//			debug().debug("serial_size : %i", b3.serial_size() );
//			debug().debug("serial_size : %i", b2.serial_size() );
