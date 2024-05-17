ReadUserLogState::ReadUserLogState(
	const ReadUserLog::FileState	&state,
	int								 recent_thresh )
	: ReadUserLogFileState( state )
{
	Reset( RESET_INIT );
	m_recent_thresh = recent_thresh;

	if ( !SetState( state ) ) {
		dprintf( D_FULLDEBUG, "::ReadUserLogState: failed to set state from buffer\n" );
		m_init_error = true;
	}
}
