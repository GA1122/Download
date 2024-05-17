GahpClient::cream_job_register(const char *service, const char *delg_id, 
							   const char *jdl, const char *lease_id, char **job_id, char **upload_url, char **download_url)
{
	static const char* command = "CREAM_JOB_REGISTER";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!service) service=NULLSTRING;
	if (!delg_id) delg_id=NULLSTRING;
	if (!jdl) jdl = NULLSTRING;
	if (!lease_id) lease_id = "";

	std::string reqline;
	char *esc1 = strdup( escapeGahpString(service) );
	char *esc2 = strdup( escapeGahpString(delg_id) );
	char *esc3 = strdup( escapeGahpString(jdl) );
	char *esc4 = strdup( escapeGahpString(lease_id) );
	int x = sprintf( reqline, "%s %s %s %s", esc1, esc2, esc3, esc4 );
	free( esc1 );
	free( esc2 );
	free( esc3 );
	free( esc4 );
	ASSERT( x > 0 );
	const char *buf = reqline.c_str();
	
	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,deleg_proxy,low_prio);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);

	if ( result ) {
		int rc = 0;
		if ( result->argc == 2 ) {
			if ( !strcmp( result->argv[1], NULLSTRING ) ) {
				EXCEPT( "Bad %s result", command );
			}
			error_string = result->argv[1];
			rc = 1;
		} else if ( result->argc == 5 ) {
			if ( strcmp( result->argv[1], NULLSTRING ) ) {
				EXCEPT( "Bad %s result", command );
			}

			if ( strcasecmp(result->argv[2], NULLSTRING) ) {
				*job_id = strdup(result->argv[2]);
			}
			if ( strcasecmp(result->argv[3], NULLSTRING) ) {
				*upload_url = strdup(result->argv[3]);
			}
			if ( strcasecmp(result->argv[4], NULLSTRING) ) {
				*download_url = strdup(result->argv[4]);
			}
			rc = 0;
		} else {
			EXCEPT( "Bad %s result", command );
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
