GahpClient::cream_job_status_all(const char *service, 
	GahpClient::CreamJobStatusMap & results)
{
	static const char* command = "CREAM_JOB_STATUS";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	std::string reqline;

	char *esc1 = strdup( escapeGahpString(service) );
	int job_number = 0;  
	int x = sprintf(reqline, "%s %d", esc1, job_number);
	ASSERT( x > 0 );
	free( esc1 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,normal_proxy,high_prio);
	}

	const int FIRST_RESULT = 3;  
	const int RECORD_FIELDS = 4;  

	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		int num_results = 0;
		if (result->argc > 2) {
			num_results = atoi(result->argv[2]);
			if( result->argc != FIRST_RESULT + num_results * RECORD_FIELDS) {
				EXCEPT("Bad %s Result",command);
			}
		}
		else if (result->argc != 2) {
			EXCEPT("Bad %s Result",command);
		}
		
		int rc;
		if (strcmp(result->argv[1], NULLSTRING) == 0) {
			rc = 0;
		} else {
			rc = 1;
			error_string = result->argv[1];
		}
		
		if ( rc == 0 ) {
			for(int i = 0; i < num_results; i++) {
				CreamJobStatus status;
				int offset = FIRST_RESULT + i * RECORD_FIELDS;
				status.job_id = result->argv[offset + 0];
				status.job_status = result->argv[offset + 1];
				status.exit_code = atoi(result->argv[offset + 2]);
				if ( strcasecmp(result->argv[offset + 3], NULLSTRING) ) {
					status.failure_reason = result->argv[offset + 3];
				}

				results[status.job_id] = status;
			}
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
