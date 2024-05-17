ReadUserLogFileState::getFileOffset( int64_t &pos ) const
{
	if ( NULL == m_ro_state ) {
		return false;
	}
	pos = m_ro_state->internal.m_offset.asint;
	return true;
}
