int GahpClient::dcloud_submit( const char *service_url,
							   const char *username,
							   const char *password,
							   const char *image_id,
							   const char *instance_name,
							   const char *realm_id,
							   const char *hwp_id,
							   const char *hwp_memory,
							   const char *hwp_cpu,
							   const char *hwp_storage,
							   const char *keyname,
							   const char *userdata,
							   StringList &attrs )
{
	static const char* command = "DELTACLOUD_VM_SUBMIT";

	if ( server->m_commands_supported->contains_anycase( command ) == FALSE ) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if ( !service_url ) service_url = NULLSTRING;
	if ( !username ) username = NULLSTRING;
	if ( !password ) password = NULLSTRING;
	if ( !image_id ) image_id = NULLSTRING;
	if ( !instance_name ) instance_name = NULLSTRING;
	if ( !realm_id ) realm_id = NULLSTRING;
	if ( !hwp_id ) hwp_id = NULLSTRING;
	if ( !hwp_memory ) hwp_memory = NULLSTRING;
	if ( !hwp_cpu ) hwp_cpu = NULLSTRING;
	if ( !hwp_storage ) hwp_storage = NULLSTRING;
	if ( !keyname ) keyname = NULLSTRING;
	if ( !userdata ) userdata = NULLSTRING;

	MyString reqline;

	char* esc1 = strdup( escapeGahpString(service_url) );
	char* esc2 = strdup( escapeGahpString(username) );
	char* esc3 = strdup( escapeGahpString(password) );
	char* esc4 = strdup( escapeGahpString(image_id) );
	char* esc5 = strdup( escapeGahpString(instance_name) );
	char* esc6 = strdup( escapeGahpString(realm_id) );
	char* esc7 = strdup( escapeGahpString(hwp_id) );
	char* esc8 = strdup( escapeGahpString(hwp_memory) );
	char* esc9 = strdup( escapeGahpString(hwp_cpu) );
	char* esc10 = strdup( escapeGahpString(hwp_storage) );
	char* esc11 = strdup( escapeGahpString(keyname) );
	char* esc12 = strdup( escapeGahpString(userdata) );

	bool x = reqline.sprintf("%s %s %s %s %s %s %s %s %s %s %s %s", esc1, esc2, esc3,
                                 esc4, esc5, esc6, esc7, esc8, esc9, esc10, esc11, esc12);

	free( esc1 );
	free( esc2 );
	free( esc3 );
	free( esc4 );
	free( esc5 );
	free( esc6 );
	free( esc7 );
	free( esc8 );
	free( esc9 );
	free( esc10 );
	free( esc11 );
	free( esc12 );
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
		if ( result->argc < 2 ) {
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
			attrs.clearAll();
			for ( int i = 2; i < result->argc; i++ ) {
				attrs.append( result->argv[i] );
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
