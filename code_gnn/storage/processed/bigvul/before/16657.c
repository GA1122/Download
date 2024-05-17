ReadUserLogStateAccess::getFileEventNum(
	unsigned long		&num ) const
{
	int64_t	my_num;
	if ( !m_state->getFileEventNum(my_num) ) {
		return false;
	}

	if ( (unsigned long)my_num > ULONG_MAX ) {
		return false;
	}
	num = (unsigned long) my_num;
	return true;
}
