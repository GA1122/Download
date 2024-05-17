ReadUserLogState::EventNum( const ReadUserLog::FileState &state ) const
{
	const ReadUserLogFileState::FileState *istate;
	if ( ( !convertState(state, istate) ) || ( !istate->m_version ) ) {
		return -1;
	}
	return (filesize_t) istate->m_event_num.asint;
}
