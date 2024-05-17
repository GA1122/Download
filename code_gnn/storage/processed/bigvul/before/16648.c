ReadUserLogState::StatFile( int fd )
{
	StatWrapper	statwrap;
	if ( statwrap.Stat( fd )  ) {
		dprintf( D_FULLDEBUG, "StatFile: errno = %d\n", statwrap.GetErrno() );
		return statwrap.GetRc( );
	}

	statwrap.GetBuf( m_stat_buf );
	m_stat_time = time( NULL );
	m_stat_valid = true;
	Update();

	return 0;
}
