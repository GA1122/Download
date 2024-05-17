GahpClient::globus_gram_client_job_signal(const char * job_contact,
	globus_gram_protocol_job_signal_t signal,
	const char * signal_arg,
	int * job_status,
	int * failure_code)
{
	static const char* command = "GRAM_JOB_SIGNAL";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!job_contact) job_contact=NULLSTRING;
	if (!signal_arg) signal_arg=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(job_contact) );
	char *esc2 = strdup( escapeGahpString(signal_arg) );
	int x = sprintf(reqline,"%s %d %s",esc1,signal,esc2);
	free( esc1 );
	free( esc2 );
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,normal_proxy);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		if (result->argc != 4) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi(result->argv[1]);
		*failure_code = atoi(result->argv[2]);
		if ( rc == 0 ) {
			*job_status = atoi(result->argv[3]);
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
