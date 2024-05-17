GahpClient::condor_job_status_constrained(const char *schedd_name,
										  const char *constraint,
										  int *num_ads, ClassAd ***ads)
{
	static const char* command = "CONDOR_JOB_STATUS_CONSTRAINED";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!schedd_name) schedd_name=NULLSTRING;
	if (!constraint) constraint=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(schedd_name) );
	char *esc2 = strdup( escapeGahpString(constraint) );
	int x = sprintf( reqline, "%s %s", esc1, esc2 );
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
		if (result->argc < 4) {
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
		*num_ads = atoi(result->argv[3]);
		if (result->argc != 4 + *num_ads ) {
			EXCEPT("Bad %s Result",command);
		}
		if ( *num_ads > 0 ) {
			*ads = (ClassAd **)malloc( *num_ads * sizeof(ClassAd*) );
			int idst = 0;
			for ( int i = 0; i < *num_ads; i++,idst++ ) {
				if ( useXMLClassads ) {
					ClassAdXMLParser parser;
					(*ads)[idst] = parser.ParseClassAd( result->argv[4 + i] );
				} else {
					NewClassAdParser parser;
					(*ads)[idst] = parser.ParseClassAd( result->argv[4 + i] );
				}
				if( (*ads)[idst] == NULL) {
					dprintf(D_ALWAYS, "ERROR: Condor-C GAHP returned "
						"unparsable classad: (#%d) %s\n", i, result->argv[4+i]);
					idst--;
					(*num_ads)--;
				}
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
