ReadUserLogStateAccess::getEventNumberDiff(
	const ReadUserLogStateAccess	&other,
	long							&diff) const
{
	const	ReadUserLogFileState	*ostate;
	if ( !other.getState( ostate ) ) {
		return false;
	}
	int64_t	my_recno, other_recno;
	if ( !m_state->getLogRecordNo(my_recno) ||
		 ! ostate->getLogRecordNo(other_recno) ) {
		return false;
	}

	int64_t	idiff = my_recno - other_recno;
	diff = (long) idiff;
	return true;
}
