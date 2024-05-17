CStarter::RemoveRecoveryFile()
{
	if ( m_recoveryFile.Length() > 0 ) {
		unlink( m_recoveryFile.Value() );
		m_recoveryFile = "";
	}
}
