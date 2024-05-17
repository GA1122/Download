GahpServer::DeleteMe()
{
	m_deleteMeTid = TIMER_UNSET;

	if ( m_reference_count <= 0 ) {

		delete this;
	}
}
