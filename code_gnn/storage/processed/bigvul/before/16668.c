ReadUserLogFileState::getUniqId( char *buf, int len ) const
{
	if ( NULL == m_ro_state ) {
		return false;
	}
	strncpy( buf, m_ro_state->internal.m_uniq_id, len );
	buf[len-1] = '\0';
	return true;
}
