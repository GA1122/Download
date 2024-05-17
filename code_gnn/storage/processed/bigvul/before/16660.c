ReadUserLogStateAccess::getFileOffset(
	unsigned long		&pos ) const
{
	int64_t	my_pos;
	if ( !m_state->getFileOffset(my_pos) ) {
		return false;
	}

	if ( my_pos > LONG_MAX ) {
		return false;
	}
	pos = (unsigned long) my_pos;
	return true;
}
