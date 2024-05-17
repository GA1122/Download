GahpClient::unicore_job_create(
	const char * description,
	char ** job_contact)
{

	static const char* command = "UNICORE_JOB_CREATE";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!description) description=NULLSTRING;
char *desc = strdup(description);
int i = strlen( desc );
if ( i > 0 && desc[i-1] == '\n' ) {
desc[i-1] = '\0';
}
description = desc;
	std::string reqline;
	int x = sprintf( reqline, "%s", escapeGahpString(description) );
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();
free(desc);

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,deleg_proxy);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		if (result->argc != 4) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = 1;
		if ( result->argv[1][0] == 'S' ) {
			rc = 0;
		}
		if ( result->argv[2] && strcasecmp(result->argv[2], NULLSTRING) ) {
			*job_contact = strdup(result->argv[2]);
		}
		delete result;
		return rc;
	}

	if ( check_pending_timeout(command,buf) ) {
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;
}
