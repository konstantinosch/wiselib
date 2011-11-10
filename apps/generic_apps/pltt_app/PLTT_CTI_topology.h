#define DB 0
template<typename Position, typename Radio>
Position get_node_info( Radio* radio )
{
	typedef typename Radio::TxPower TxPower;
	TxPower power;
	if ( radio->id() == 0x6699 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 158, 140, 8 );
	}
	else if ( radio->id() == 0x1bc4)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 158, 144, 8 );
	}
	else if ( radio->id() == 0xa41c )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 182, 44, 8 );
	}
	else if ( radio->id() == 0x6666)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 187, 47, 14 );
	}
	else if ( radio->id() == 0x7b99 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 152, 113, 8 );
	}
	else if ( radio->id() == 0x997b)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 141, 110, 8 );
	}
	else if ( radio->id() == 0x1b85)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 126, 6, 8 );
	}
	else if ( radio->id() == 0x1b8b )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 115, 2, 8 );
	}
	else if ( radio->id() == 0x1b77 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 138, 94, 8 );
	}
	else if ( radio->id() == 0x057b )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 143, 94, 8 );
	}
	else if ( radio->id() == 0xddba )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 38, 157, 8 );
	}
	else if ( radio->id() == 0x14e7 )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 39, 164, 8 );
	}
	else if ( radio->id() == 0x1b71)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 141, 66, 8 );
	}
	else if ( radio->id() == 0x153d )
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 136, 45, 8 );
	}
	else if ( radio->id() == 0x0c9b)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 188, 94, 8 );
	}
	else if ( radio->id() == 0x1cd0)
	{
		power.set_dB( DB );
		radio->set_power( power );
		return Position( 179, 94, 8 );
	}
	return Position( 0, 0, 0 );
}
