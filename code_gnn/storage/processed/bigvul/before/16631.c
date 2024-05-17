ReadUserLogFileState::ReadUserLogFileState(
	ReadUserLog::FileState &state )
{
	convertState( state, m_rw_state );
	m_ro_state = m_rw_state;
}
