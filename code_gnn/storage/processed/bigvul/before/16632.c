ReadUserLogFileState::ReadUserLogFileState(
	const ReadUserLog::FileState &state )
{
	m_rw_state = NULL;
	convertState( state, m_ro_state );
}
