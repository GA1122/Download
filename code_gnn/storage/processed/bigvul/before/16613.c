void FileTransfer::setSecuritySession(char const *session_id) {
	free(m_sec_session_id);
	m_sec_session_id = NULL;
	m_sec_session_id = session_id ? strdup(session_id) : NULL;
}
