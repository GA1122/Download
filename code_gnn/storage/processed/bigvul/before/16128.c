GahpClient::blah_job_submit(ClassAd *job_ad, char **job_id)
{
	static const char* command = "BLAH_JOB_SUBMIT";

	MyString ad_string;

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!job_ad) {
		ad_string=NULLSTRING;
	} else {
		NewClassAdUnparser unparser;
		unparser.SetUseCompactSpacing( true );
		unparser.SetOutputType( false );
		unparser.SetOutputTargetType( false );
		unparser.Unparse( job_ad, ad_string );
	}
	std::string reqline;
	int x = sprintf( reqline, "%s", escapeGahpString(ad_string.Value()) );
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
		if (result->argc != 4) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi( result->argv[1] );
		if ( strcasecmp(result->argv[3], NULLSTRING) ) {
			*job_id = strdup(result->argv[3]);
		}
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
