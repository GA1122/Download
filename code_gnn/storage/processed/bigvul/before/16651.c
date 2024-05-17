ReadUserLogFileState::convertState(
	ReadUserLog::FileState				&state,
	ReadUserLogFileState::FileStatePub	*&pub )
{
	pub = (ReadUserLogFileState::FileStatePub *) state.buf;
	return true;
}
