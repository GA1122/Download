ReadUserLogState::Rotation( const ReadUserLog::FileState &state ) const
{
	const ReadUserLogFileState::FileState *istate;
	if ( ( !convertState(state, istate) ) || ( !istate->m_version ) ) {
		return -1;
	}
	return istate->m_rotation;
}
