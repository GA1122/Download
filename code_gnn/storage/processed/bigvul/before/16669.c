ReadUserLogFileState::isInitialized( void ) const
{
	if ( NULL == m_ro_state ) {
		return false;
	}
	if ( strcmp( m_ro_state->internal.m_signature, FileStateSignature ) ) {
		return false;
	}
	return true;
}
