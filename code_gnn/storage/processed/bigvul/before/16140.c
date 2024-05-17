GahpServer::command_version()
{
	int i,j,result;
	bool ret_val = false;

	j = sizeof(m_gahp_version);
	i = 0;
	while ( i < j ) {
		result = buffered_read(m_gahp_readfd, &(m_gahp_version[i]), 1 );
		 
		if ( result < 0 ) {		 
			continue;
		}
		if ( result == 0 ) {	 
			return false;
		}
		if ( i==0 && m_gahp_version[0] != '$' ) {
			continue;
		}
		if ( m_gahp_version[i] == '\\' ) {
			continue;
		}
		if ( m_gahp_version[i] == '\n' ) {
			ret_val = true;
			m_gahp_version[i] = '\0';
			break;
		}
		i++;
	}

	return ret_val;
}
