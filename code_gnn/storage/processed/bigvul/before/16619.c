ReadUserLogState::CompareUniqId( const MyString &id ) const
{
	if ( ( m_uniq_id == "" ) || ( id == "" ) ) {
		return 0;
	}
	else if ( m_uniq_id == id ) {
		return 1;
	}
	else {
		return -1;
	}
}
