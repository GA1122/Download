GahpClient::condor_job_update_constrained(const char *schedd_name,
										  const char *constraint,
										  ClassAd *update_ad)
{
	static const char* command = "CONDOR_JOB_UPDATE_CONSTRAINED";

	MyString ad_string;

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!schedd_name) schedd_name=NULLSTRING;
	if (!constraint) constraint=NULLSTRING;
	if (!update_ad) {
		ad_string=NULLSTRING;
	} else {
		if ( useXMLClassads ) {
			ClassAdXMLUnparser unparser;
			unparser.SetUseCompactSpacing( true );
			unparser.SetOutputType( false );
			unparser.SetOutputTargetType( false );
			unparser.Unparse( update_ad, ad_string );
		} else {
			NewClassAdUnparser unparser;
			unparser.SetUseCompactSpacing( true );
			unparser.SetOutputType( false );
			unparser.SetOutputTargetType( false );
			unparser.Unparse( update_ad, ad_string );
		}
	}
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(schedd_name) );
	char *esc2 = strdup( escapeGahpString(constraint) );
	char *esc3 = strdup( escapeGahpString(ad_string.Value()) );
	int x = sprintf( reqline, "%s %s %s", esc1, esc2, esc3 );
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
		int rc = 1;
		if ( result->argv[1][0] == 'S' ) {
			rc = 0;
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