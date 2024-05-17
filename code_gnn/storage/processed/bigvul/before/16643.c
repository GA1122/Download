ReadUserLogState::SecondsSinceStat( void ) const
{
	time_t	now = time( NULL );
	return (int) (now - m_stat_time);
}
