ReadUserLogStateAccess::getFileEventNumDiff(
	const ReadUserLogStateAccess	&other,
	long							&diff ) const
{
	const	ReadUserLogFileState	*ostate;
	if ( !other.getState( ostate ) ) {
		return false;
	}
	int64_t	my_num, other_num;
	if ( !m_state->getFileEventNum(my_num) ||
		 ! ostate->getFileEventNum(other_num) ) {
		return false;
	}

	int64_t	idiff = my_num - other_num;
	diff = (long) idiff;
	return true;
}
