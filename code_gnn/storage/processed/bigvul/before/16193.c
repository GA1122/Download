GahpClient::globus_gram_client_job_refresh_credentials(const char *job_contact,
													   int limited_deleg)
{
	static const char* command = "GRAM_JOB_REFRESH_PROXY";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!job_contact) job_contact=NULLSTRING;
	std::string reqline;
	int x = sprintf(reqline,"%s %d",escapeGahpString(job_contact),limited_deleg);
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,deleg_proxy);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		if (result->argc != 2) {
			EXCEPT("Bad %s Result",command);
		}
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