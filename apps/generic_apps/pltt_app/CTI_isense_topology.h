#define DB 0
template<typename Position, typename Radio>
Position get_node_info( Radio* radio )
{
	typedef typename Radio::TxPower TxPower;
	TxPower power;
	if ( radio->id() == 0x296 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 27, 11, 1 );
	}
	else if ( radio->id() == 0x295 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 22, 18, 1 );
	}
	else if ( radio->id() == 0xca3)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 28, 18, 1 );
	}
	else if ( radio->id() == 0x1b77 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 24, 18, 1 );
	}
	else if ( radio->id() == 0x585 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 15, 18, 1 );
	}
	else if ( radio->id() == 0x786a)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 17, 18, 1 );
	}
	else if ( radio->id() == 0x1cde )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 28, 18, 1 );
	}
	else if ( radio->id() == 0x9979)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 11, 4, 1 );
	}
	else if ( radio->id() == 0x153d)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 11, 4, 1 );
	}
	else if ( radio->id() == 0X0180)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 20, 0, 0 );
	}
	else if ( radio->id() == 0X0180)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 12, 2, 0 );
	}
	return Position( 0, 0, 0 );
}
