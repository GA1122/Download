ReadUserLogStateAccess::getLogPositionDiff(
	const ReadUserLogStateAccess	&other,
	long							&diff ) const
{
	const	ReadUserLogFileState	*ostate;
	if ( !other.getState( ostate ) ) {
		return false;
	}
	int64_t	my_pos, other_pos;
	if ( !m_state->getLogPosition(my_pos) ||
		 ! ostate->getLogPosition(other_pos) ) {
		return false;
	}

	int64_t	idiff = my_pos - other_pos;
	diff = (long) idiff;
	return true;
}
