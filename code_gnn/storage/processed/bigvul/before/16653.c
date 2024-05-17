ReadUserLogFileState::convertState(
	ReadUserLog::FileState			&state,
	ReadUserLogFileState::FileState	*&internal )
{
	ReadUserLogFileState::FileStatePub	*pub;
	convertState(state, pub);
	internal = &(pub->internal);
	return true;
}
