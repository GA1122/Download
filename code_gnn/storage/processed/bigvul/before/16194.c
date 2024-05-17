GahpClient::globus_gram_client_job_request(
	const char * resource_manager_contact,
	const char * description,
	const int limited_deleg,
	const char * callback_contact,
	char ** job_contact)
{

	static const char* command = "GRAM_JOB_REQUEST";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!resource_manager_contact) resource_manager_contact=NULLSTRING;
	if (!description) description=NULLSTRING;
	if (!callback_contact) callback_contact=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(resource_manager_contact) );
	char *esc2 = strdup( escapeGahpString(callback_contact) );
	char *esc3 = strdup( escapeGahpString(description) );
	int x = sprintf(reqline,"%s %s %d %s", esc1, esc2, limited_deleg, esc3 );
	free( esc1 );
	free( esc2 );
	free( esc3 );
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
		if (result->argc != 3) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi(result->argv[1]);
		if ( strcasecmp(result->argv[2], NULLSTRING) ) {
			*job_contact = strdup(result->argv[2]);
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
