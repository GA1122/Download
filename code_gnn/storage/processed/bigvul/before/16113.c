GahpServer::AddGahpClient()
{
	m_reference_count++;

	if ( m_deleteMeTid != TIMER_UNSET ) {
		daemonCore->Cancel_Timer( m_deleteMeTid );
		m_deleteMeTid = TIMER_UNSET;
	}
}
