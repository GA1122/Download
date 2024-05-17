GahpClient::nordugrid_exit_info(const char *hostname, const char *job_id,
								bool &normal_exit, int &exit_code,
								float &wallclock, float &sys_cpu,
								float &user_cpu )
{
	static const char* command = "NORDUGRID_EXIT_INFO";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!hostname) hostname=NULLSTRING;
	if (!job_id) job_id=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(hostname) );
	char *esc2 = strdup( escapeGahpString(job_id) );
	int x = sprintf(reqline, "%s %s", esc1, esc2 );
	free( esc1 );
	free( esc2 );
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
		if (result->argc != 8) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi(result->argv[1]);
		if ( atoi( result->argv[2] ) ) {
			normal_exit = true;
		} else {
			normal_exit = false;
		}
		exit_code = atoi( result->argv[3] );
		wallclock = atof( result->argv[4] );
		sys_cpu = atof( result->argv[5] );
		user_cpu = atof( result->argv[6] );
		if ( strcasecmp(result->argv[7], NULLSTRING) ) {
			error_string = result->argv[7];
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
