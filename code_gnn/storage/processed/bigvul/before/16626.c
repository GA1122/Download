ReadUserLogFileState::InitState( ReadUserLog::FileState &state )
{
	state.buf  = (void *) new ReadUserLogState::FileStatePub;
	state.size = sizeof( ReadUserLogState::FileStatePub );

	ReadUserLogFileState::FileState	*istate;
	if ( !convertState(state, istate)  ) {
		return false;
	}

	memset( istate, 0, sizeof(ReadUserLogState::FileStatePub) );
	istate->m_log_type = LOG_TYPE_UNKNOWN;

	strncpy( istate->m_signature,
			 FileStateSignature,
			 sizeof(istate->m_signature) );
	istate->m_signature[sizeof(istate->m_signature) - 1] = '\0';
	istate->m_version = FILESTATE_VERSION;

	return true;
}
