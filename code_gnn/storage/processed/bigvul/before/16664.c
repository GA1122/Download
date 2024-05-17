ReadUserLogFileState::getLogRecordNo( int64_t &recno ) const
{
	if ( NULL == m_ro_state ) {
		return false;
	}
	recno = m_ro_state->internal.m_log_record.asint;
	return true;
}
