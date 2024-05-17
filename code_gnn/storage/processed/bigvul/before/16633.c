ReadUserLogState::ReadUserLogState(
	const char		*path,
	int				 max_rotations,
	int				 recent_thresh )
	: ReadUserLogFileState( )
{
	Reset( RESET_INIT );
	m_max_rotations = max_rotations;
	m_recent_thresh = recent_thresh;
	if ( path ) {
		m_base_path = path;
	}
	m_initialized = true;
	m_update_time = 0;
}
