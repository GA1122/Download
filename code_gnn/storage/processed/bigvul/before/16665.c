ReadUserLogFileState::getSequenceNo( int &seqno ) const
{
	if ( NULL == m_ro_state ) {
		return false;
	}
	seqno = m_ro_state->internal.m_sequence;
	return true;
}
