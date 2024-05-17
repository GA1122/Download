ReadUserLogState::LogRecordNo( const ReadUserLog::FileState &state ) const
{
	const ReadUserLogFileState::FileState *istate;
	if ( ( !convertState(state, istate) ) || ( !istate->m_version ) ) {
		return -1;
	}
	return (filesize_t) istate->m_log_record.asint;
}
