int GahpClient::ec2_ping(const char *service_url, const char * publickeyfile, const char * privatekeyfile)
{
	static const char* command = "EC2_VM_STATUS_ALL";
	
	char* esc1 = strdup( escapeGahpString(service_url) );
	char* esc2 = strdup( escapeGahpString(publickeyfile) );
	char* esc3 = strdup( escapeGahpString(privatekeyfile) );
	
	std::string reqline;
	sprintf(reqline, "%s %s %s", esc1, esc2, esc3 );
	const char *buf = reqline.c_str();
	
	free( esc1 );
	free( esc2 );
	free( esc3 );
		
	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command, buf, deleg_proxy);
	}
	

	Gahp_Args* result = get_pending_result(command, buf);
	
	if ( result ) {
		int rc = atoi(result->argv[1]);
		delete result;
		return rc;
	}
	
	if ( check_pending_timeout(command,buf) ) {
		sprintf( error_string, "%s timed out", command );
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;
}
