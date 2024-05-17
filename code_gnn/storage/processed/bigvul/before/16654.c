ReadUserLogStateAccess::getEventNumber(
	unsigned long		&event_no ) const
{
	int64_t	my_event_no;
	if ( !m_state->getLogRecordNo(my_event_no) ) {
		return false;
	}

	if ( (unsigned long)my_event_no > ULONG_MAX ) {
		return false;
	}
	event_no = (unsigned long) my_event_no;
	return true;
}
