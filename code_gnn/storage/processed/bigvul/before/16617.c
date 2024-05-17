ReadUserLogState::BasePath( const ReadUserLog::FileState &state ) const
{
	const ReadUserLogFileState::FileState *istate;
	if ( ( !convertState(state, istate) ) || ( !istate->m_version ) ) {
		return NULL;
	}
	return istate->m_base_path;
}
