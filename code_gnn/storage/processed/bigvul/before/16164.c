int GahpClient::dcloud_action( const char *service_url,
							   const char *username,
							   const char *password,
							   const char *instance_id,
							   const char *action )
{
	static const char* command = "DELTACLOUD_VM_ACTION";

	if ( server->m_commands_supported->contains_anycase( command ) == FALSE ) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if ( !service_url ) service_url = NULLSTRING;
	if ( !username ) username = NULLSTRING;
	if ( !password ) password = NULLSTRING;
	if ( !instance_id ) instance_id = NULLSTRING;
	if ( !action ) action = NULLSTRING;

	MyString reqline;

	char* esc1 = strdup( escapeGahpString(service_url) );
	char* esc2 = strdup( escapeGahpString(username) );
	char* esc3 = strdup( escapeGahpString(password) );
	char* esc4 = strdup( escapeGahpString(instance_id) );
	char* esc5 = strdup( escapeGahpString(action) );

	bool x = reqline.sprintf("%s %s %s %s %s", esc1, esc2, esc3, esc4, esc5);

	free( esc1 );
	free( esc2 );
	free( esc3 );
	free( esc4 );
	free( esc5 );
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
		if ( result->argc != 2 ) {
			EXCEPT( "Bad %s result", command );
		} else {
			if ( strcmp( result->argv[1], NULLSTRING ) ) {
				error_string = result->argv[1];
				rc = 1;
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
