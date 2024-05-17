GahpServer::err_pipe_ready(int   )
{
	int count = 0;

	char buff[5001];
	buff[0] = '\0';

	while (((count = (daemonCore->Read_Pipe(m_gahp_errorfd, &buff, 5000))))>0) {

		char *prev_line = buff;
		char *newline = buff - 1;
		buff[count]='\0';

		while ( (newline = strchr( newline + 1, '\n' ) ) != NULL ) {

			*newline = '\0';
			dprintf( D_FULLDEBUG, "GAHP[%d] (stderr) -> %s%s\n", m_gahp_pid,
					 m_gahp_error_buffer.c_str(), prev_line );
			prev_line = newline + 1;
			m_gahp_error_buffer = "";

		}

		m_gahp_error_buffer += prev_line;
	}

	return TRUE;
}
