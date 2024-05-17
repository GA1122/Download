ReadUserLogStateAccess::getState( const ReadUserLogFileState *&state ) const
{
	state = m_state;
	return true;
}
