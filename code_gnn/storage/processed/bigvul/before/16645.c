ReadUserLogState::StatFile( void )
{
	int status = StatFile( CurPath(), m_stat_buf );
	if ( 0 == status ) {
		m_stat_time = time( NULL );
		m_stat_valid = true;
		Update();
	}
	return status;
}
