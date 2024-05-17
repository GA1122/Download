int GahpClient::dcloud_start_auto( const char *service_url,
								   const char *username,
								   const char *password,
								   bool *autostart )
{
	static const char* command = "DELTACLOUD_START_AUTO";

	if ( server->m_commands_supported->contains_anycase( command ) == FALSE ) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if ( !service_url ) service_url = NULLSTRING;
	if ( !username ) username = NULLSTRING;
	if ( !password ) password = NULLSTRING;

	MyString reqline;

	char* esc1 = strdup( escapeGahpString(service_url) );
	char* esc2 = strdup( escapeGahpString(username) );
	char* esc3 = strdup( escapeGahpString(password) );

	bool x = reqline.sprintf("%s %s %s", esc1, esc2, esc3);

	free( esc1 );
	free( esc2 );
	free( esc3 );
	ASSERT( x == true );

	const char *buf = reqline.Value();
	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command, buf, deleg_proxy);
	}


	Gahp_Args* result = get_pending_result(command, buf);


	if ( result ) {
		int rc = 0;
		if ( result->argc < 2 || result->argc > 3 ) {
			EXCEPT( "Bad %s result", command );
		} else if ( result->argc == 2 ) {
			if ( !strcmp( result->argv[1], NULLSTRING ) ) {
				EXCEPT( "Bad %s result", command );
			}
			error_string = result->argv[1];
			rc = 1;
		} else {
			if ( strcmp( result->argv[1], NULLSTRING ) ) {
				EXCEPT( "Bad %s result", command );
			}
			if ( strcmp( result->argv[2], "TRUE" ) == 0 ) {
				*autostart = TRUE;
			} else if ( strcmp( result->argv[2], "FALSE" ) == 0 ) {
				*autostart = FALSE;
			} else {
				EXCEPT( "Bad %s result", command );
			}
		}

		delete result;
		return rc;
	}

	if ( check_pending_timeout(command, buf) )
	{
		sprintf( error_string, "%s timed out", command );
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;
}
