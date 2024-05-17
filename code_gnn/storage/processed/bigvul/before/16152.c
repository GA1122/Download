GahpClient::cream_delegate(const char *delg_service, const char *delg_id)
{
	static const char* command = "CREAM_DELEGATE";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!delg_service) delg_service=NULLSTRING;
	if (!delg_id) delg_id=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(delg_service) );
	char *esc2 = strdup( escapeGahpString(delg_id) );
	int x = sprintf(reqline, "%s %s", esc2, esc1);
	free( esc1 );
	free( esc2 );
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,deleg_proxy,high_prio);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {

		if (result->argc != 2) {
			EXCEPT("Bad %s Result",command);
		}
		int rc;
		if (strcmp(result->argv[1], NULLSTRING) == 0) {
			rc = 0;
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
