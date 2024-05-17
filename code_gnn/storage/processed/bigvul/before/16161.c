GahpClient::cream_ping(const char * service)
{
	static const char* command = "CREAM_PING";
	
	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!service) service=NULLSTRING;
	std::string reqline;
	int x = sprintf(reqline,"%s",escapeGahpString(service));
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,normal_proxy,high_prio);
	}
	
	
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		int rc = 0;
		if (result->argc < 2 || result->argc > 3) {
			EXCEPT("Bad %s Result",command);
		}
		if (strcmp(result->argv[1], NULLSTRING) == 0) {
			if ( strcasecmp( result->argv[2], "true" ) ) {
				rc = 0;
			} else {
				rc = 1;
				error_string = "";
			}
		} else {
			rc = 1;
			error_string = result->argv[1];
		}
		delete result;
		return rc;
	}
	
	if ( check_pending_timeout(command,buf) ) {
		sprintf( error_string, "%s timed out", command );
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;
}
