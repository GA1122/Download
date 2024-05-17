GahpServer::buffered_read( int fd, void *buf, int count )
{
	ASSERT(fd == m_gahp_readfd);
	ASSERT(count == 1);

	if ( m_buffer_pos >= m_buffer_end ) {
		int dummy_pipe = -1;
		err_pipe_ready(dummy_pipe);
		int rc = daemonCore->Read_Pipe(fd, m_buffer, m_buffer_size );
		m_buffer_pos = 0;
		if ( rc <= 0 ) {
			m_buffer_end = 0;
			return rc;
		} else {
			m_buffer_end = rc;
		}
	}

	((char *)buf)[0] = ((char *)m_buffer)[m_buffer_pos];
	m_buffer_pos++;
	return 1;
}
