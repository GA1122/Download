ReadUserLogFileState::isValid( void ) const
{
	if ( !isInitialized() ) {
		return false;
	}
	if ( 0 == strlen(m_ro_state->internal.m_base_path) ) {
		return false;
	}
	return true;
}
