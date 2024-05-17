GahpClient::cream_job_status(const char *service, const char *job_id, 
									char **job_status, int *exit_code, char **failure_reason)
{
	static const char* command = "CREAM_JOB_STATUS";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!service) service=NULLSTRING;
	if (!job_id) job_id=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(service) );
	char *esc2 = strdup( escapeGahpString(job_id) );
	int job_number = 1;   
	int x = sprintf(reqline, "%s %d %s", esc1, job_number, esc2);
	free( esc1 );
	free( esc2 );
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,normal_proxy,medium_prio);
	}


	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		if (result->argc > 2) {
			if( result->argc != 3 + atoi(result->argv[2]) * 4){
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
			*job_status = strdup(result->argv[4]);
			*exit_code = atoi(result->argv[5]);
			if ( strcasecmp(result->argv[6], NULLSTRING) ) {
				*failure_reason = strdup(result->argv[6]);
			} else {
				*failure_reason = NULL;
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
