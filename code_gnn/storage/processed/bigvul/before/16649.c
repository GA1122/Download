ReadUserLogFileState::UninitState( ReadUserLog::FileState &state )
{
	ReadUserLogState::FileStatePub	*istate =
		(ReadUserLogState::FileStatePub *) state.buf;
	
	delete istate;
	state.buf = NULL;
	state.size = 0;

	return true;
}
