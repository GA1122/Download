GahpClient::nordugrid_stage_out(const char *hostname, const char *job_id,
								StringList &files)
{
	static const char* command = "NORDUGRID_STAGE_OUT";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!hostname) hostname=NULLSTRING;
	if (!job_id) job_id=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(hostname) );
	char *esc2 = strdup( escapeGahpString(job_id) );
	int x = sprintf(reqline,"%s %s %d", esc1, esc2, files.number() );
	free( esc1 );
	free( esc2 );
	ASSERT( x > 0 );
	int cnt = 0;
	const char *filename;
	files.rewind();
	while ( (filename = files.next()) ) {
		sprintf_cat(reqline," %s", filename);
		cnt++;
	}
	ASSERT( cnt == files.number() );
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
