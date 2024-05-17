ReadUserLogStateAccess::ReadUserLogStateAccess(
	const ReadUserLog::FileState &state)
{
	m_state = new ReadUserLogFileState(state);
}
