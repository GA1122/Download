GahpClient::gt4_gram_client_job_callback_register(const char * job_contact,
	const char * callback_contact)
{
	static const char* command = "GT4_GRAM_JOB_CALLBACK_REGISTER";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!job_contact) job_contact=NULLSTRING;
	if (!callback_contact) callback_contact=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(job_contact) );
	char *esc2 = strdup( escapeGahpString(callback_contact) );
	int x = sprintf(reqline,"%s %s",esc1,esc2);
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
		if (result->argc != 3) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi(result->argv[1]);
		if ( strcasecmp(result->argv[2], NULLSTRING) ) {
			error_string = result->argv[2];
		} else {
			error_string = "";
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
